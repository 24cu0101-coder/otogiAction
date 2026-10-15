
#include "TutorialPoint.h"

#include "TutorialActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/Character.h"

ATutorialPoint::ATutorialPoint()
{
    PrimaryActorTick.bCanEverTick = false;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(
        TEXT("TriggerBox")
    );

    RootComponent = TriggerBox;

    TriggerBox->SetCollisionEnabled(
        ECollisionEnabled::QueryOnly
    );

    TriggerBox->SetCollisionResponseToAllChannels(
        ECR_Ignore
    );

    TriggerBox->SetCollisionResponseToChannel(
        ECC_Pawn,
        ECR_Overlap
    );
}

void ATutorialPoint::BeginPlay()
{
    Super::BeginPlay();

    TriggerBox->OnComponentBeginOverlap.AddDynamic(
        this,
        &ATutorialPoint::OnTriggerBeginOverlap
    );

    // 管理Actorが未設定ならレベル内から探す
    if (!TutorialManager)
    {
        TutorialManager =
            Cast<ATutorialActor>(
                UGameplayStatics::GetActorOfClass(
                    GetWorld(),
                    ATutorialActor::StaticClass()
                )
            );
    }
}

void ATutorialPoint::OnTriggerBeginOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    // プレイヤー以外は無視
    if (OtherActor != UGameplayStatics::GetPlayerCharacter(this, 0))
    {
        return;
    }
    // Pointのライトを消す
    if (UPointLightComponent* Light =
        FindComponentByClass<UPointLightComponent>())
    {
        Light->SetVisibility(false);
    }

    if (TutorialManager)
    {
        TutorialManager->CompleteStep(StepIndex);
    }

    // 同じ目的地が何度も反応しないようにする
    TriggerBox->SetCollisionEnabled(
        ECollisionEnabled::NoCollision
    );
}
