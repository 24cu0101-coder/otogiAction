//アタックコリジョンを管理しているコンポーネント

#pragma once

#include "CoreMinimal.h"
#include "AttackCollisionComponent.generated.h"

class USphereComponent;
class UBoxComponent;
class UCapsuleComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UAttackCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	//コンストラクタ
	UAttackCollisionComponent();

protected:
	//ゲームが始まった時に呼ばれる
	virtual void BeginPlay() override;

public:	
	//毎フレーム処理される
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void ExcuteAreaAttack(float Radius, FName TargetTag, float Damage);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	USphereComponent* SphereAttackCollision;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	//UBoxComponent* BoxAttackCollision;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	//UCapsuleComponent* CapsuleAttackCollision;

	//BoxCollisionの幅
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Collision|Param")
	float BoxWidth = 0.f;
	//BoxCollisionの奥行
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision|Param")
	float BoxLength = 0.f;
	//BoxCollisionの高さ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision|Param")
	float BoxHeight = 0.f;

	//SphereCollisionの半径
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision|Param")
	float SphereRadius = 0.f;

	//CapsuleCollisionの半径
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision|Param")
	float CapsuleRadius = 0.f;
	//CapsuleCollisionの高さ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision|Param")
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

	//攻撃判定をとる対象

};
