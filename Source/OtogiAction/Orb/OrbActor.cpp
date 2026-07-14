#include "OrbActor.h"
#include "Components/StaticMeshComponent.h"
#include "OtogiAction/Component/Status/StatusComponent.h"

AOrbActor::AOrbActor()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

    RootComponent = MeshComponent;
}

void AOrbActor::BeginPlay()
{
    Super::BeginPlay();

    StartLocation = GetActorLocation();
}

//吸われる状態
void AOrbActor::StartAbsorb(AActor* Target)
{
    bIsAbsorbing = true;
    TargetActor = Target;
}

//OwnerにEnemyを設定
void AOrbActor::SetOwnerEnemy(AActor* Enemy)
{
    OwnerEnemy = Enemy;
}

void AOrbActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 吸われているならPlayerへ飛ぶ
// 吸われているならPlayerへ飛ぶ
    if (bIsAbsorbing)
    {
        if (TargetActor)
        {
            // Playerへの方向を計算
            FVector Direction =
                (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();

            // 移動速度
            float MoveSpeed = 800.f;

            // Playerへ移動
            SetActorLocation(
                GetActorLocation() + Direction * MoveSpeed * DeltaTime);

            // Playerとの距離を測る
            float Distance = FVector::Dist(
                GetActorLocation(),
                TargetActor->GetActorLocation());

            // Playerに到着したら
            if (Distance < 50.f)
            {
                // Orbを落としたEnemyのStatusComponentを取得
                if (OwnerEnemy && !OwnerEnemy->IsPendingKillPending())
                {
                    UStatusComponent* Status =
                        OwnerEnemy->FindComponentByClass<UStatusComponent>();

                    if (Status)
                    {
                        // すでに死亡しているならダメージを与えない
                        if (!Status->IsDead())
                        {
                            // EnemyのHPを減らす
                            Status->TakeDamage(10.f);

                            UE_LOG(LogTemp, Warning,
                                TEXT("Orb Damage Enemy : %s"),
                                *OwnerEnemy->GetName());
                        }
                    }
                }

                // オーブを消す
                Destroy();
            }

        }

        return;
    }
    // 吸われていないときは浮く
    FloatTime += DeltaTime;

    FVector NewLocation = StartLocation;

    NewLocation.Z += FMath::Sin(FloatTime * FloatSpeed) * FloatHeight;

    SetActorLocation(NewLocation);
}