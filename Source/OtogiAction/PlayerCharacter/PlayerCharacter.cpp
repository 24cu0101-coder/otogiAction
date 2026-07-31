//-----------------------------
//プレイヤーの処理
//-----------------------------

#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerComponent/Move/MoveComponent.h"
#include "PlayerComponent/Camera/MoveCameraComponent.h"
#include "PlayerComponent/PlayerDodgeComponent.h"
#include "PlayerComponent/SkillComponent.h"
#include "PlayerComponent/NormalAttack/NormalComboAttackComponent.h"
#include "PlayerComponent/StrongAttack/StrongAttackComponent.h"
#include "PlayerComponent/StrongAttack/IaiAttackComponent.h"
#include "../PlayerCharacter/PlayerComponent/PlayerTargetComponent.h"
#include "../Component/Collision/SphereCollisionComponent.h"
#include "PlayerStatusComponent.h"
#include "../Orb/OrbActor.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerComponent/SkillGaugeComponent.h"
#include "PlayerComponent/WeaponComponent.h"
#include "../HitStopComponent.h"
#include "HitReactionComponent.h"
#include "../Component/UHitReactionBaseComponent.h"
#include "OtogiAction/UI/PlayerHPWidget.h"
#include "OtogiAction/UI/SkillGaugeWidget.h"
#include "OtogiAction/UI/SkillCircle.h"
#include"PlayerDeathComponent.h"

//コンストラクタ
APlayerCharacter::APlayerCharacter()
{
 	//毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ
	//オフにする
	PrimaryActorTick.bCanEverTick = false;

	// デフォルトプレイヤーとして設定
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//スプリングアームとカメラの生成
	m_SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	m_CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	if (m_SpringArmComp)
	{
		//スプリングアームのアタッチ
		m_SpringArmComp->SetupAttachment(RootComponent);

		//スプリングアームの距離
		m_SpringArmComp->TargetArmLength = 400.f;

		//カメラの子コリジョンテストを行うか設定
		m_SpringArmComp->bDoCollisionTest = false;
		//カメラ追従ラグを使うか設定
		m_SpringArmComp->bEnableCameraLag = true;
		//カメラ追従ラグの速度を設定
		m_SpringArmComp->CameraLagSpeed = 1000.0f;
		//カメラ回転ラグを使うかを設定
		m_SpringArmComp->bEnableCameraRotationLag = true;
		//カメラ回転ラグの速度を設定
		m_SpringArmComp->CameraLagSpeed = 10.0f;
	}

	if ((m_CameraComp) && (m_SpringArmComp))
	{
		//カメラをスプリングアームにアタッチさせる
		m_CameraComp->SetupAttachment(m_SpringArmComp, USpringArmComponent::SocketName);
	}

	//AbilitySystem
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

	//キャラクター移動コンポーネント生成
	MovementCharaComp = CreateDefaultSubobject<UMoveComponent>(TEXT("MoveComp"));

	//カメラ操作コンポーネント生成
	MovementCameraComp = CreateDefaultSubobject<UMoveCameraComponent>(TEXT("CameraComp"));

	//回避コンポーネント生成
	PlayerDodgeComp = CreateDefaultSubobject<UPlayerDodgeComponent>(TEXT("DodgeComp"));


	//通常攻撃コンポーネント生成
	NormalCombo = CreateDefaultSubobject<UNormalComboAttackComponent>(TEXT("NormalAtComp"));

	//強攻撃コンポーネント生成
	StrongAttackComp = CreateDefaultSubobject<UStrongAttackComponent>(TEXT("SAttackComp"));

	IaiAttackComp = CreateDefaultSubobject<UIaiAttackComponent>(TEXT("IAttackComp"));

	//ターゲットコンポーネント
	TargetComp = CreateDefaultSubobject<UPlayerTargetComponent>(TEXT("TargetComp"));

	//スキルコンポーネントの生成
	SkillComp = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComp"));

	//AttackCollisionComponentの生成
	CollisionComp = CreateDefaultSubobject<USphereCollisionComponent>(TEXT("CollisionComp"));

	//Statusコンポーネント生成
	StatusComp = CreateDefaultSubobject<UPlayerStatusComponent>(TEXT("StatusComp"));

	//スキルゲージコンポーネント
	GaugeComp = CreateDefaultSubobject<USkillGaugeComponent>(TEXT("GaugeComp"));

	//武器コンポーネント
	WeaponComp = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComp"));

	//ヒットストップコンポーネント
	HitStopComp = CreateDefaultSubobject<UHitStopComponent>(TEXT("HitStopComp"));

	//ターゲットコンポーネント
	//TargetComp = CreateDefaultSubobject<UPlayerTargetComponent>(TEXT("TargetComp"));

	//ヒットリアクションコンポーネント
	HitReactionComp = CreateDefaultSubobject<UHitReactionComponent>(TEXT("HitReactionComp"));
	//Audioコンポーネント
	CharacterAudioComponent =CreateDefaultSubobject<UCharacterAudioComponent>(TEXT("CharacterAudioComponent"));
	
	//デッドコンポーネント
	DeathComp = CreateDefaultSubobject<UPlayerDeathComponent>(TEXT("DeathComp"));
}

//ゲームが始まったときに生成
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Enhanced Input のマッピングコンテキストを追加
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//SkillComponentに自分のASCを渡す
	if (SkillComp && GetAbilitySystemComponent())
	{
		SkillComp->RegisterAbilities(GetAbilitySystemComponent());
	}

	//ステータスコンポーネントからの通知によってハンドル関数につなげる
	if (StatusComp)
	{
		StatusComp->OnDamaged.AddDynamic(this, &APlayerCharacter::HandleDamaged);
	}

	//statusコンポーネントから死亡通知を受け取る
	if (StatusComp)
	{
		StatusComp->OnDead.AddDynamic(this, &APlayerCharacter::OnDeath);
	}

	// 現在のレベル名取得
	FString LevelName = GetWorld()->GetMapName();

	// OtogiKantrikyokuではUIを出さない
	bool bShowUI = !LevelName.Contains(TEXT("OtogiKantrikyoku"));


	//PlayerのHP表示
	if (bShowUI && PlayerHPWidgetClass)
	{
		PlayerHPWidget = CreateWidget<UPlayerHPWidget>(
			GetWorld(),
			PlayerHPWidgetClass);

		if (PlayerHPWidget)
		{
			PlayerHPWidget->AddToViewport();

			PlayerHPWidget->SetHPPercent(
				StatusComp->GetCurrentHP() /
				StatusComp->GetMaxHP());
		}
	}


	//SkillGauge表示
	if (bShowUI && SkillGaugeWidgetClass)
	{
		SkillGaugeWidget = CreateWidget<USkillGaugeWidget>(
			GetWorld(),
			SkillGaugeWidgetClass);

		if (SkillGaugeWidget)
		{
			SkillGaugeWidget->AddToViewport();

			SkillGaugeWidget->SetGaugePercent(
				GaugeComp->GetGaugeRatio());
		}
	}


	//SkillCircle表示
	if (bShowUI && SkillCircleClass)
	{
		SkillCircle = CreateWidget<USkillCircle>(
			GetWorld(),
			SkillCircleClass);

		if (SkillCircle)
		{
			SkillCircle->AddToViewport();
		}
	}	
	if (StatusComp)
	{
		StatusComp->OnDamaged.AddDynamic(this, &APlayerCharacter::OnPlayerDamaged);
		PreviousHP = StatusComp->GetMaxHP();
	}
}

//毎フレーム処理
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//入力インプット
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 入力アクションをバインドする
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//キャラとカメラの移動
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::OnCharacterMovement);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::OnCameraMovement);;
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &APlayerCharacter::OnPlayerDodge);
		//通常攻撃
		EnhancedInputComponent->BindAction(NormalAttackAction, ETriggerEvent::Started, this, &APlayerCharacter::OnNormalCombo);
		//強攻撃
		EnhancedInputComponent->BindAction(SAttackAction, ETriggerEvent::Started, this, &APlayerCharacter::OnStrongAttack);
		//居合攻撃	
		EnhancedInputComponent->BindAction(IaiAttackAction, ETriggerEvent::Started, this, &APlayerCharacter::OnIaiAttack);

		//スキルの切り替え
		EnhancedInputComponent->BindAction(SwitchSkillGroup, ETriggerEvent::Started, this, &APlayerCharacter::OnSwitchSkillGroup);
		//スキルの発動四つ
		EnhancedInputComponent->BindAction(ExcuteSkill1, ETriggerEvent::Started, this, &APlayerCharacter::OnSkill1Pressed);

		//オーブ吸う
		EnhancedInputComponent->BindAction(AbsorbAction, ETriggerEvent::Started, this, &APlayerCharacter::OnAbsorb);

		//ターゲット
		EnhancedInputComponent->BindAction(TargetInput, ETriggerEvent::Started, this, &APlayerCharacter::OnTarget);

	}

}

UAbilitySystemComponent* APlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

//キャラクター移動
void APlayerCharacter::OnCharacterMovement(const FInputActionValue& Value)
{
	//ダウン中なら移動入力を完全に無視する
	if (HitReactionComp && HitReactionComp->IsStunned())
	{
		return;
	}

	//スティックの傾きの軸を取得
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (MovementCharaComp)
	{
		//傾きの角度を取得
		float CalculatedRatio = MovementVector.Size();

		//UpdateMovementSpeedに入力量を伝えて移動させる
		MovementCharaComp->UpdateMovementSpeed(CalculatedRatio);

		//移動処理を呼び出す
		MovementCharaComp->MoveForword(MovementVector.Y);
		MovementCharaComp->MoveRight(MovementVector.X);
	}
}

//カメラ操作
void APlayerCharacter::OnCameraMovement(const FInputActionValue& Value)
{
	//スティックの傾きの軸を取得
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (MovementCameraComp)
	{
		//移動関数を呼び出す
		MovementCameraComp->CameraMove(LookAxisVector);
	}
}

//回避(髙山)
void APlayerCharacter::OnPlayerDodge(const FInputActionValue& Value)
{
	//ダウン中にボタンで起き上がる
	if (HitReactionComp && HitReactionComp->IsStunned())
	{
		if (UHitReactionComponent* PlayerHitComp = Cast<UHitReactionComponent>(HitReactionComp))
		{
			PlayerHitComp->RequestGetUp();
		}
		return;
	}

	
	//回避コンポーネントがあったら
	if (PlayerDodgeComp)
	{
		//コンポーネントの処理実行
		PlayerDodgeComp->ExecuteAbility();
	}

}       

//
void APlayerCharacter::OnNormalCombo(const FInputActionValue& Value)
{
	//ダウン中なら移動入力を完全に無視する
	if (HitReactionComp && HitReactionComp->IsStunned())
	{
		return;
	}

	//通常攻撃コンポーネントがあったら
	if (NormalCombo)
	{
		//コンポーネントの処理実行
		NormalCombo->ExecuteNormalAttackAbility();

	}
}

void APlayerCharacter::OnStrongAttack()
{
	//ダウン中にボタンで起き上がる
	if (HitReactionComp && HitReactionComp->IsStunned())
	{
		return;
	}

	//強攻撃コンポーネントがあったら
	if (StrongAttackComp)
	{
		StrongAttackComp->ExecuteStrongAttackAbility();
	}
}

void APlayerCharacter::OnIaiAttack()
{
	//ダウン中にボタンで起き上がる
	if (HitReactionComp && HitReactionComp->IsStunned())
	{
		return;
	}

	//居合攻撃コンポーネントがあったら
	if (IaiAttackComp)
	{

		UE_LOG(LogTemp, Warning, TEXT("yfdsday"));

		IaiAttackComp->ExecuteIaiAttackAbility();
	}

}

//スキル群の切り替え
void APlayerCharacter::OnSwitchSkillGroup(const FInputActionValue& Value)
{
	if (SkillComp)
	{
		float AxsisValue = Value.Get<float>();

		int32 Direction = AxsisValue > 0.f ? 1 : -1;

		//スキル選択を変更
		SkillComp->SwitchSkillGroup(Direction);


		//CIrcleを回す
		if (SkillCircle)
		{
			SkillCircle->UpdateSkillCircle(
				SkillComp->GetCurrentGroupPointer()
			);
		}
	}
}
//スキルの発動
void APlayerCharacter::OnSkill1Pressed()
{
	UE_LOG(LogTemp, Warning, TEXT("★ PlayerCharacter: OnSkill1Pressed 実行!"));

	if (SkillComp)
	{
		SkillComp->RequestSkillTrigger(0);
	}
}

void APlayerCharacter::OnAbsorb()
{
	UE_LOG(LogTemp, Warning, TEXT("Absorb Button Pressed"));

	// レベル内にあるOrbを格納する配列
	TArray<AActor*> Orbs;

	// レベル内に存在する全てのOrbActorを取得
	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		AOrbActor::StaticClass(),
		Orbs);

	// 取得したOrbを1つずつ処理する
	for (AActor* Actor : Orbs)
	{
		// AActorをAOrbActorへ変換
		AOrbActor* Orb = Cast<AOrbActor>(Actor);

		// Orbでなければ次へ
		if (!Orb)
		{
			continue;
		}

		// PlayerとOrbとの距離を計算
		float Distance = FVector::Dist(
			GetActorLocation(),
			Orb->GetActorLocation());

		// 吸い込み範囲(1000)以内なら
		if (Distance <= 1000.f)
		{
			// OrbにPlayerを目標として設定し、吸い込み状態にする
			Orb->StartAbsorb(this);
		}
	}
}

void APlayerCharacter::OnTarget()
{
	TargetComp->ToggleTargetLock();
}

//ステータスコンポーネントからのヒット通知で呼ばれるリアクション関数
void APlayerCharacter::HandleDamaged(float NewHP)
{
	//// 死亡している場合はリアクションを処理しない
	//if (StatusComp && StatusComp->IsDead()) return;

	//if (HitReactionComp && StatusComp)
	//{
	//	//ダメージ量
	//	float DamageAmount = PreviousHP - NewHP;

	//	//最新HPを前回のHPとして保存
	//	PreviousHP = NewHP;

	//	if (DamageAmount > 0.f)
	//	{
	//		HitReactionComp->PlayHitReaction(DamageAmount);
	//	}
	//}
}
//げんざいHPをUIに表示
void APlayerCharacter::OnPlayerDamaged(float CurrentHP)
{
	if (PlayerHPWidget && StatusComp)
	{
		PlayerHPWidget->SetHPPercent(
			CurrentHP / StatusComp->GetMaxHP());
	}
}
//現在のSkillGaugeをUIに表示
void APlayerCharacter::UpdateSkillGaugeUI()
{
	if (SkillGaugeWidget && GaugeComp)
	{
		SkillGaugeWidget->SetGaugePercent(
			GaugeComp->GetGaugeRatio());
	}
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//死んでいたらリターン
	if (bIsDead)return 0.f;

	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage <= 0.f || !StatusComp) return 0.f;

	// StatusComp の TakeDamage 内で HP <= 0 になると、自動的に OnDead.Broadcast() ? OnDeath() が呼ばれる！
	const float AppliedDamage = StatusComp->TakeDamage(ActualDamage);

	//死亡していない（生きている）場合のみ、被弾リアクションを再生する
		// ※ StatusComp->TakeDamage() の中で死亡した場合、すでに OnDeath() が走って bIsDead = true になっているため、ここを回避できます！
	if (!bIsDead && AppliedDamage > 0.f)
	{
		if (HitReactionComp)
		{
			HitReactionComp->PlayHitReaction(AppliedDamage);
		}
	}

	//戻り値を忘れずに返す
	return AppliedDamage;
}

void APlayerCharacter::OnDeath()
{
	if (bIsDead) return;
	bIsDead = true;

	// 入力を止める
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC);
	}

	//// 移動を止める
	//if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	//{
	//	MoveComp->StopMovementImmediately();
	//	MoveComp->DisableMovement();
	//}

	// 3. 死亡コンポーネントの処理を開始
	if (DeathComp)
	{
		DeathComp->Dead();
	}
}

