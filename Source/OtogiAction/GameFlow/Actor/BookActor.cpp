// Fill out your copyright notice in the Description page of Project Settings.


#include "BookActor.h"

//コンストラクタ
ABookActor::ABookActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    //
    RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
    SetRootComponent(RootComp);

    //登録した本の生成
    ClosedBookMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClosedBookMesh"));
    ClosedBookMesh->SetupAttachment(RootComp);

    OpenBookMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OpenBookMesh"));
    OpenBookMesh->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void ABookActor::BeginPlay()
{
	Super::BeginPlay();
	
    //最初は閉じた本を表示させ開いた本は非表示に
    if (ClosedBookMesh) ClosedBookMesh->SetVisibility(true);
    if (OpenBookMesh) OpenBookMesh->SetVisibility(false);
}

//毎フレーム処理される
void ABookActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABookActor::OpenBook()
{
    //ほんの表示を切り替える
    if (ClosedBookMesh) ClosedBookMesh->SetVisibility(false);
    if (OpenBookMesh) OpenBookMesh->SetVisibility(true);
}

