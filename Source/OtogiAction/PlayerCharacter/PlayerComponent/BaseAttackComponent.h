// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UBaseAttackComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName hanage;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision|Param|Box")
	float BoxWidth = 0.f;
	//BoxCollisionの奥行
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision|Param|Box")
	float BoxLength = 0.f;
	//BoxCollisionの高さ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision|Param|Box")
	float BoxHeight = 0.f;

	//SphereCollisionの半径
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision|Param|Sphere")
	float SphereRadius = 0.f;

	//CapsuleCollisionの半径
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision|Param|Capsule")
	float CapsuleRadius = 0.f;
	//CapsuleCollisionの高さ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision|Param|Capsule")
	float CapsuleHeight = 0.f;

	//コリジョンをどれだけ前に出すか
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision|Param")
	float ForwardOffset = 0.f;

	//コリジョンをどれだけ横に出すか
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision|Param")
	float SideOffset = 0.f;

	//与えるダメージの変数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision|Param")
	float GiveDamage = 0.f;


};
