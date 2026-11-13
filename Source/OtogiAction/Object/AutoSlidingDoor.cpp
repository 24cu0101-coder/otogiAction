//担当：飯島
//ドアの開閉処理

#include "AutoSlidingDoor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"

// コンストラクタ
AAutoSlidingDoor::AAutoSlidingDoor()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false;

    // ルート
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = SceneRoot;

    // 外枠
    DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
    DoorFrame->SetupAttachment(SceneRoot);

    // 左扉
    LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
    LeftDoor->SetupAttachment(SceneRoot);

    // 右扉
    RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
    RightDoor->SetupAttachment(SceneRoot);

    // 検知用Box
    DetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectionBox"));
    DetectionBox->SetupAttachment(SceneRoot);

    DetectionBox->SetBoxExtent(FVector(200, 200, 150));

    DetectionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    DetectionBox->SetCollisionResponseToAllChannels(ECR_Ignore);

    DetectionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    DetectionBox->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AAutoSlidingDoor::BeginPlay()
{
	Super::BeginPlay();
	
    // 初期位置を保存
    LeftClosedLocation = LeftDoor->GetRelativeLocation();
    RightClosedLocation = RightDoor->GetRelativeLocation();

    // Y軸方向に左右へ開く
    LeftOpenLocation = LeftClosedLocation + FVector(0, -OpenDistance, 0);

    RightOpenLocation = RightClosedLocation + FVector(0, OpenDistance, 0);

    // イベント登録
    DetectionBox->OnComponentBeginOverlap.AddDynamic(this, &AAutoSlidingDoor::OnDetectionBeginOverlap);

    DetectionBox->OnComponentEndOverlap.AddDynamic(this, &AAutoSlidingDoor::OnDetectionEndOverlap);

    // 開始時点でプレイヤーが範囲内にいる場合にも対応
    TArray<AActor*> InitialActors;
    DetectionBox->GetOverlappingActors(InitialActors, APawn::StaticClass());

    for (AActor* Actor : InitialActors)
    {
        if (APawn* Pawn = Cast<APawn>(Actor))
        {
            if (Pawn->IsPlayerControlled())
            {
                OverlappingPlayers.Add(Actor);
            }
        }
    }

    if (!OverlappingPlayers.IsEmpty())
    {
        OpenDoor();
    }
}

// Called every frame
void AAutoSlidingDoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateDoor(DeltaTime);
}

//ドアの更新
void AAutoSlidingDoor::UpdateDoor(float DeltaTime)
{
    if (DoorState != EDoorState::opening && DoorState != EDoorState::closing)
    {
        SetActorTickEnabled(false);
        return;
    }

    const float Direction = DoorState == EDoorState::opening ? 1.0f : -1.0f;

    // 開閉率を更新
    OpenAlpha = FMath::Clamp(OpenAlpha + Direction * DeltaTime / OpenDuration,0.0f,1.0f);

    // 加速・減速をつける
    const float SmoothAlpha = FMath::InterpEaseInOut(0.0f, 1.0f, OpenAlpha, 2.0f);

    // 左右の扉を補間
    LeftDoor->SetRelativeLocation(FMath::Lerp(LeftClosedLocation,LeftOpenLocation,SmoothAlpha));

    RightDoor->SetRelativeLocation(FMath::Lerp(RightClosedLocation,RightOpenLocation,SmoothAlpha));

    // 開き終わった
    if (DoorState == EDoorState::opening &&OpenAlpha >= 1.0f)
    {
        DoorState = EDoorState::open;
        SetActorTickEnabled(false);
    }

    // 閉じ終わった
    else if (DoorState == EDoorState::closing &&
        OpenAlpha <= 0.0f)
    {
        DoorState = EDoorState::closed;
        SetActorTickEnabled(false);
    }
}

//開らく判定
void AAutoSlidingDoor::OnDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    APawn* Pawn = Cast<APawn>(OtherActor);

    if (!Pawn || !Pawn->IsPlayerControlled())
    {
        return;
    }

    OverlappingPlayers.Add(Pawn);

    OpenDoor();
}

//閉じる判定
void AAutoSlidingDoor::OnDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    APawn* Pawn = Cast<APawn>(OtherActor);

    if (!Pawn)
    {
        return;
    }

    OverlappingPlayers.Remove(Pawn);

    // まだ範囲内にプレイヤーがいる場合は閉じない
    if (OverlappingPlayers.IsEmpty())
    {
        CloseDoor();
    }
}


void AAutoSlidingDoor::OpenDoor()
{
    if (DoorState == EDoorState::open ||DoorState == EDoorState::opening)
    {
        return;
    }

    DoorState = EDoorState::opening;

    SetActorTickEnabled(true);
}

void AAutoSlidingDoor::CloseDoor()
{
    if (DoorState == EDoorState::closed ||
        DoorState == EDoorState::closing)
    {
        return;
    }

    DoorState = EDoorState::closing;

    SetActorTickEnabled(true);
}

