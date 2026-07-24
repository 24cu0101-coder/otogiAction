// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerDeathComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UPlayerDeathComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerDeathComponent();

	//死亡処理関数
	void Dead();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//死亡時のモンタージュ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
	UAnimMontage* DeathMontage;

	//リスポーン時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death")
	float RespawnDelay = 3.0f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	//リスポーンタイマースタート
	void RespawnTimerend();

	// 死亡時のコントローラーを保持しておく変数
	UPROPERTY()
	AController* CachedController;
		
};
