#include "OrbActor.h"
#include "Components/StaticMeshComponent.h"
#include "OtogiAction/Component/Status/StatusComponent.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/SkillGaugeComponent.h"
#include "OrbSpawnComponent.h"
#include "Kismet/GameplayStatics.h"

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

    // 自動吸収
    if (!bIsAbsorbing)
    {
        APlayerCharacter* Player =
            Cast<APlayerCharacter>(
                UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

        if (Player)
        {
            float Distance = FVector::Dist(
                GetActorLocation(),
                Player->GetActorLocation());

            if (Distance <= AbsorbRange)
            {
                StartAbsorb(Player);
            }
        }
    }

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
                            Status->TakeDamage(OrbDamage);
                        }
                    }
                }

                APlayerCharacter* Player = Cast<APlayerCharacter>(TargetActor);

                if (Player)
                {

                    if (USkillGaugeComponent* Gauge = Player->FindComponentByClass<USkillGaugeComponent>())
                    {

                        Gauge->ModifyGauge(50.f);

                        Player->UpdateSkillGaugeUI();
                    }
                    else
                    {
                    }
                }
                else
                {
                }

                if (SpawnComponent)
                {
                    SpawnComponent->ReturnOrb();
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

void AOrbActor::SetOrbDamage(float InDamage)
{
    OrbDamage = InDamage;
}

void AOrbActor::SetSpawnComponent(UOrbSpawnComponent* InComponent)
{
    SpawnComponent = InComponent;
}