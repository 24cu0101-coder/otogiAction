// Fill out your copyright notice in the Description page of Project Settings.


#include "ChainActor.h"

// Sets default values
AChainActor::AChainActor()
{ 
	PrimaryActorTick.bCanEverTick = false;

	//Rootの生成
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	//ジオメトリーコレクションの生成
	ChainGeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("ChainGeometryCollection"));
	ChainGeometryCollection->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void AChainActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChainActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//ジオメトリーコレクションを発火
void AChainActor::BreakChain()
{
	if (ChainGeometryCollection)
	{
		// 物理を有効化
		ChainGeometryCollection->SetSimulatePhysics(true);

		// ジオメトリーコレクションの破壊を発生させる
		FVector Origin = ChainGeometryCollection->GetComponentLocation();
		ChainGeometryCollection->ApplyKinematicField(500.0f, Origin);
		ChainGeometryCollection->WakeAllRigidBodies();
	}
}

