//攻撃用こりじょんのコンポーネント

#include "AttackCollisionComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

//コンストラクタ
UAttackCollisionComponent::UAttackCollisionComponent()
{
	//TickはOffに
	PrimaryComponentTick.bCanEverTick = true;

	//目の前スポーン型コリジョンを作成
	FrontSpawnHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FrontSpawnHitbox"));
	// 武器トレース型コリジョンを作成
	WeaponHitbox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponHitbox"));

	//判定をOFF
	FrontSpawnHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


//ゲームが始まったときに呼ばれる関数
void UAttackCollisionComponent::BeginPlay()
{
	Super::BeginPlay();

	//ヒットした際のイベントを登録
	FrontSpawnHitBox->OnComponentBeginOverlap.AddDynamic(this, &UAttackCollisionComponent::OnHitOverlap);
	WeaponHitbox->OnComponentBeginOverlap.AddDynamic(this, &UAttackCollisionComponent::OnHitOverlap);

	//コンポーネントの持ち主にアタッチ
	ACharacter* OwnerChara = Cast<ACharacter>(GetOwner());
	if (OwnerChara && OwnerChara->GetMesh())
	{
		//目の前スポーン型の設定
		FrontSpawnHitBox->SetupAttachment(OwnerChara->GetRootComponent());
		FrontSpawnHitBox->SetRelativeLocation(FVector(150.f, 0.f, 0.f));
		
		//武器トレース型の設定
		WeaponHitbox->AttachToComponent(OwnerChara->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_r"));
		WeaponHitbox->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

	}
}


// Called every frame
void UAttackCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
//コリジョン開始処理
void UAttackCollisionComponent::AttackCollisionWindow(EAttackCollisionType Type, float DamageAmount)
{
	// ダメージ量を記憶
	CurrentAttackDamage = DamageAmount;

	// ヒットリストをリセット
	HitActors.Empty();

	UPrimitiveComponent* TargetComp = (Type == EAttackCollisionType::FrontSpawn) ? Cast<UPrimitiveComponent>(FrontSpawnHitBox) : Cast<UPrimitiveComponent>(WeaponHitbox);

	if (TargetComp)
	{
		//持ち主への当たり判定を除外
		TargetComp->IgnoreActorWhenMoving(GetOwner(), true);

		//判定開始
		TargetComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}
//コリジョン終了処理
void UAttackCollisionComponent::AttackCollisionCloseWindow(EAttackCollisionType Type)
{
	UPrimitiveComponent* TargetComp = (Type == EAttackCollisionType::FrontSpawn) ? Cast<UPrimitiveComponent>(FrontSpawnHitBox) : Cast<UPrimitiveComponent>(WeaponHitbox);
	if (TargetComp)
	{
		TargetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UAttackCollisionComponent::OnHitOverlap(
	UPrimitiveComponent*OverlappedComp,
	AActor*OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetOwner()) return; 
	if (HitActors.Contains(OtherActor)) return;   

	// ヒットリストに登録
	HitActors.Add(OtherActor);

	//ダメージデリゲートを使ってダメージ追加
	UGameplayStatics::ApplyDamage(
		OtherActor,
		CurrentAttackDamage,
		GetOwner()->GetInstigatorController(),
		GetOwner(),
		UDamageType::StaticClass());
}

