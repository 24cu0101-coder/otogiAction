#include "ArrowActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AArrowActor::AArrowActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// 当たり判定のセットアップ
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(10.0f);
	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = CollisionComp;

	// メッシュのセットアップ
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 飛翔コンポーネントのセットアップ
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 2000.0f;
	ProjectileMovement->MaxSpeed = 2000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true; // 飛ぶ方向に合わせて矢の向きを変える
	ProjectileMovement->ProjectileGravityScale = 0.2f;   // 少し重力で落ちるようにする

	// 5秒後に自動消滅
	InitialLifeSpan = 5.0f;
}

void AArrowActor::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AArrowActor::OnOverlapBegin);
}

void AArrowActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 自分自身、または矢を撃った本人（Owner）には当たらないようにする
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner())
	{
		return;
	}

	// 相手にダメージを通知
	UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());

	// 矢を消す
	Destroy();
}