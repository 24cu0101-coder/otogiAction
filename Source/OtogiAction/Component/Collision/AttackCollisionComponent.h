//アタックコリジョンを管理しているコンポーネント

#pragma once

#include "CoreMinimal.h"
#include "CollisionShape.h"
#include "AttackCollisionComponent.generated.h"

class USphereComponent;
class UBoxComponent;
class UCapsuleComponent;

struct FCollisionShape;

UENUM(BlueprintType)
enum class EAttackCollisionType : uint8
{
	Sphere   UMETA(DisplayName = "Sphere"),
	Box      UMETA(DisplayName = "Box"),
	Capsule  UMETA(DisplayName = "Capsule")
};

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

	//BoxCollisionの幅
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Collision|Param|Box")
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	EAttackCollisionType CollisionType;

	// 実際のコリジョン計算用
	FCollisionShape CustomCollisionShape;
};
