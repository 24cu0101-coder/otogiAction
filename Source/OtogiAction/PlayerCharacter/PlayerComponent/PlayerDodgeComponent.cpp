
//--------------------------------------------------------------------------------------------------------
//回避コンポーネント
//-----------------------------------------------------------------------------------------------------
#include "PlayerDodgeComponent.h"
#include "GameplayTagContainer.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"
#include "OtogiAction/GameFlow/Actor/TutorialActor.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/InputBuffer/InputBufferComponent.h"



UPlayerDodgeComponent::UPlayerDodgeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;	

	DodgeInputBufferComp = CreateDefaultSubobject<UInputBufferComponent>(TEXT("DodgeInputBufferComp"));


}


void UPlayerDodgeComponent::BeginPlay()
{
	Super::BeginPlay();

	//
	AActor* OwnerActor = GetOwner();
	PlayerActor = Cast<APlayerCharacter>(OwnerActor);

	if (PlayerActor)
	{

		AbilitySystemComponent = PlayerActor->GetAbilitySystemComponent();
		UE_LOG(LogTemp, Warning, TEXT("【PlayerDodgeComponent.cpp】:PlayerActor"));

	}

	// ジャスト回避タグを判別
	m_justDodgeTag = FGameplayTag::RequestGameplayTag(FName("IsDodge.JustDodge"));

	// プレイヤーがいれば
	if (GetOwner()->HasAuthority() && DodgeAbility)
	{
		//プレイヤーにDodgeAbilityを付与する
		DodgeHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DodgeAbility, 1));

		JustDodgeHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(JustDodgeAbility, 1));

	}


	if (AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("【PlayerDodgeComponent.cpp】:AbilitySystemComponent"));

		AbilitySystemComponent->RegisterGameplayTagEvent(m_justDodgeTag, EGameplayTagEventType::NewOrRemoved)
			.AddUObject(this, &UPlayerDodgeComponent::JustDodgeWindow);
	}
	
}


void UPlayerDodgeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UPlayerDodgeComponent::DodgeAbilitySet(UAbilitySystemComponent* DodgeASC)
{
	////ヌルチェック
	//if (!DodgeASC || !DodgeAbility)
	//{
	//	return;
	//}
	//if (GetOwner()->HasAuthority())
	//{
	//	//プレイヤーにDodgeAbilityを付与する
	//	DodgeHandle = DodgeASC->GiveAbility(FGameplayAbilitySpec(DodgeAbility, 1));
	//}
}

void UPlayerDodgeComponent::ExecuteAbility()
{

	if (DodgeInputBufferComp)
	{
		DodgeInputBufferComp->KeepOrExeFunction([this]()
			{
				ExecuteDodgeAbility();
			});
	}

}

void UPlayerDodgeComponent::ExecuteDodgeAbility()
{
	//FGameplayTag DodgeTag = FGameplayTag::RequestGameplayTag(FName("IsDodge.NormalDodge"));

	//FGameplayTag NAttackTag = FGameplayTag::RequestGameplayTag(FName("PlayerNotify.CantAttack"));

	FGameplayTag DontDogeTag = FGameplayTag::RequestGameplayTag(FName("IsDodge.DontDodge"));


	////アビリティシステムコンポーネントがあり、回避と攻撃が実装中じゃなければ
	//if (AbilitySystemComponent->HasMatchingGameplayTag(DodgeTag) || AbilitySystemComponent->HasMatchingGameplayTag(NAttackTag))
	//{
	//	return;
	//}



	if (!AbilitySystemComponent) return;

	if (AbilitySystemComponent->HasMatchingGameplayTag(DontDogeTag))
	{
		return;
	}

	else
	{

		//回避アビリティがあるなら
		if (DodgeAbility)
		{
			//AbilitySystemComponent->RemoveLooseGameplayTag(NAttackTag);

			//アビリティ実行
			AbilitySystemComponent->TryActivateAbilityByClass(DodgeAbility);
		}
	}
}

void UPlayerDodgeComponent::JustDodgeWindow(const FGameplayTag Tag, int32 NewCount)
{



	if (!PlayerActor)return;

	if (NewCount > 0)
	{
		PlayerActor->OnTakeAnyDamage.RemoveDynamic(this, &UPlayerDodgeComponent::OnPlayerTakeDamage); 
		PlayerActor->OnTakeAnyDamage.AddDynamic(this, &UPlayerDodgeComponent::OnPlayerTakeDamage);
	}
	else
	{
		PlayerActor->OnTakeAnyDamage.RemoveDynamic(this, &UPlayerDodgeComponent::OnPlayerTakeDamage);
	}
}



// 被ダメージ処理の変更
void UPlayerDodgeComponent::OnPlayerTakeDamage(AActor* DamagedActor,float Damage,const UDamageType* DamageType,AController* InstigatedBy,AActor* DamageCauser)
{


	if (!AbilitySystemComponent) return;


	if (PlayerActor)
	{
		PlayerActor->OnTakeAnyDamage.RemoveDynamic(this, &UPlayerDodgeComponent::OnPlayerTakeDamage);
	}

	// ジャスト回避abilityがあるなら
	if (JustDodgeAbility)
	{
		EnemyRote = GetEnemyAngleVector(DamageCauser);
		ExecuteJustDodgeAbility();
	}

}

FVector UPlayerDodgeComponent::GetEnemyAngleVector(const AActor* EnemyActor)
{

	FVector AngleInDegrees = FVector::ZeroVector;

	// プレイヤーかエネミーがないなら0を返す
	if (!PlayerActor || !EnemyActor) return AngleInDegrees;

	// プレイヤーから敵への方向ベクトル
	FVector DirToEnemy = (EnemyActor->GetActorLocation() - PlayerActor->GetActorLocation());
	DirToEnemy.Z = 0.0f;
	DirToEnemy.Normalize();

	AngleInDegrees = DirToEnemy;

	return AngleInDegrees;
}


void UPlayerDodgeComponent::ExecuteJustDodgeAbility()
{



	// 回避不可能時のタグ
	FGameplayTag DontDogeTag = FGameplayTag::RequestGameplayTag(FName("IsDodge.DontDodge"));


	//回避アビリティがあるなら
	if (JustDodgeAbility)
	{

		if (AbilitySystemComponent && DodgeHandle.IsValid())
		{
			AbilitySystemComponent->CancelAbilityHandle(DodgeHandle);
		}

		//AbilitySystemComponent->RemoveLooseGameplayTag(NAttackTag);

		//アビリティ実行
		AbilitySystemComponent->TryActivateAbilityByClass(JustDodgeAbility);

		// チュートリアルを取得
		ATutorialActor* TutorialManager =
			Cast<ATutorialActor>(
				UGameplayStatics::GetActorOfClass(
					GetWorld(),
					ATutorialActor::StaticClass()
				)
			);

		// ジャスト回避のチュートリアルをクリア
		if (TutorialManager)
		{
			TutorialManager->CompleteStep(7);
		}
	}
	
}

// ジャスト回避GAが参照するエネミーがいる向き
FVector UPlayerDodgeComponent::GetEnemyRote()
{
	return EnemyRote;
}



UAbilitySystemComponent* UPlayerDodgeComponent::GetDodgeAbilirtSystemComponent()
{
	return AbilitySystemComponent;
}
