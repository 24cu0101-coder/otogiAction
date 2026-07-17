// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseAttackComponent.generated.h"

//前方宣言
class APlayerCharacter;
class UMoveComponent;
class UAttackCollisionComponent;



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UBaseAttackComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	////プレイヤーのアクター
	//UPROPERTY(Transient)
	//APlayerCharacter* PlayerActor;

	////キャラ移動コンポーネント
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovementComp", meta = (AllowPrivateAccess = "true"))
	//UMoveComponent* MCC;

	////Abilityシステムコンポーネントクラス変数
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack.Ability")
	//UAbilitySystemComponent* AbilitySystemComponent;
};
