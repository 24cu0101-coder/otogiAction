// Fill out your copyright notice in the Description page of Project Settings.


#include "SstrongAttackComponent2.h"

// Sets default values for this component's properties
USstrongAttackComponent2::USstrongAttackComponent2()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USstrongAttackComponent2::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USstrongAttackComponent2::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

