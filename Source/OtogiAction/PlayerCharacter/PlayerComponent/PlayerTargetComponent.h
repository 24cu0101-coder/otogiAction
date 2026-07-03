//ターゲットロックオンコンポーネント

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerTargetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UPlayerTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerTargetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	//ターゲットの切り替え
	void ToggleTargetLock();

	//攻撃時にワープ対象がいるか否か返す関数
	AActor* GetSoftLockTarget(float SoftLockRadius);

	//サークル範囲内で最も近い敵を探す関数
	AActor* GetCloseEnemyInRadius(float Radius);

	//ゲッター関数
	FORCEINLINE AActor* GetCurrentTargetActor() const { return CurrentTargetActor; }
	FORCEINLINE bool IsTargeting()const { return CurrentTargetActor != nullptr; }

private:
	//現在ターゲットしているアクター
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Target", meta = (AllowPrivateAccess = true));
	AActor* CurrentTargetActor;

	//ターゲットロックオンをするための最大射程
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Target Setting", meta = (AllowPrivateAccess = true));
	float TargetRange = 1500.f;

	//ターゲット検索用の球体の半径
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Target Setting", meta = (AllowPrivateAccess = true));
	float TargetRadius = 500.f;

	//前方の敵を探す
	AActor* FindBestTargetForward();

};
