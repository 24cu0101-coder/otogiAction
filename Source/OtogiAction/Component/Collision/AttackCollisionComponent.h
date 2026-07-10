//アタックコリジョンを管理しているコンポーネント

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackCollisionComponent.generated.h"

//前方宣言
class UBoxComponent;
class UCapsuleComponent;

//攻撃の判定の種類
UENUM(BlueprintType)
enum class EAttackCollisionType : uint8
{
	FrontSpawn,
	WeaponTrace
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

public:
	//アニメーションノーティファイステートで使う開始処理
	void AttackCollisionWindow(EAttackCollisionType Type, float DamageAmount);

	//終了処理
	void AttackCollisionCloseWindow(EAttackCollisionType Type);

private:
	//目の前に現れるコリジョン
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	UBoxComponent* FrontSpawnHitBox;
		
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	UCapsuleComponent* WeaponHitbox;

	//現在の攻撃のダメージ量を保存する変数
	float CurrentAttackDamage;

	//重複ヒット防止リスト
	UPROPERTY()
	TArray<AActor*> HitActors;

	//ヒット時のイベント関数
	UFUNCTION()
	void OnHitOverlap(
		UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};
