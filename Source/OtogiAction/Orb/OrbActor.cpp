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

//‹z‚í‚ê‚éó‘Ô
void AOrbActor::StartAbsorb(AActor* Target)
{
    bIsAbsorbing = true;
    TargetActor = Target;
}

//Owner‚ÉEnemy‚ğİ’è
void AOrbActor::SetOwnerEnemy(AActor* Enemy)
{
    OwnerEnemy = Enemy;
}

void AOrbActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // ‹z‚í‚ê‚Ä‚¢‚é‚È‚çPlayer‚Ö”ò‚Ô
// ‹z‚í‚ê‚Ä‚¢‚é‚È‚çPlayer‚Ö”ò‚Ô
if (bIsAbsorbing)
{
    if (TargetActor)
    {
        // Player‚Ö‚Ì•ûŒü‚ğŒvZ
        FVector Direction =
            (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();

        // ˆÚ“®‘¬“x
        float MoveSpeed = 800.f;

        // Player‚ÖˆÚ“®
        SetActorLocation(
            GetActorLocation() + Direction * MoveSpeed * DeltaTime);

        // Player‚Æ‚Ì‹——£‚ğ‘ª‚é
        float Distance = FVector::Dist(
            GetActorLocation(),
            TargetActor->GetActorLocation());

        // Player‚É“’…‚µ‚½‚ç
        if (Distance < 50.f)
        {
            // Orb‚ğ—‚Æ‚µ‚½Enemy‚ÌStatusComponent‚ğæ“¾
            if (OwnerEnemy && !OwnerEnemy->IsPendingKillPending())
            {
                UStatusComponent* Status =
                    OwnerEnemy->FindComponentByClass<UStatusComponent>();

                if (Status)
                {
                    // ‚·‚Å‚É€–S‚µ‚Ä‚¢‚é‚È‚çƒ_ƒ[ƒW‚ğ—^‚¦‚È‚¢
                    if (!Status->IsDead())
                    {
                        // Enemy‚ÌHP‚ğŒ¸‚ç‚·
                        Status->TakeDamage(10.f);

                        UE_LOG(LogTemp, Warning,
                            TEXT("Orb Damage Enemy : %s"),
                            *OwnerEnemy->GetName());
                    }
                }
            }

            // ƒI[ƒu‚ğÁ‚·
            Destroy();
        }

    }

    return;
}
    // ‹z‚í‚ê‚Ä‚¢‚È‚¢‚Æ‚«‚Í•‚‚­
    FloatTime += DeltaTime;

    FVector NewLocation = StartLocation;

    NewLocation.Z += FMath::Sin(FloatTime * FloatSpeed) * FloatHeight;

    SetActorLocation(NewLocation);
}