// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAttackBaseComponent.h"

// Sets default values for this component's properties
UEnemyAttackBaseComponent::UEnemyAttackBaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

//攻撃開始時に呼ばれる関数
void UEnemyAttackBaseComponent::ExecuteAttack()
{
	//delegateの発火
	if (StartAttackHandle.IsBound())
	{
		StartAttackHandle.Broadcast();
	}
}

//攻撃終了時に呼ばれる関数
void UEnemyAttackBaseComponent::FinishAttack(bool bSuccess)
{

	// コンポーネント自身をオーナーから削除してメモリから解放する
	DestroyComponent();
}


// Called when the game starts
void UEnemyAttackBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEnemyAttackBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

