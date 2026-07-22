//攻撃を食らった時のリアクションのbaseクラス
//子クラスに継承して使ってね

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UHitReactionBaseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UUHitReactionBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUHitReactionBaseComponent();

	//スタン値の加算
	UFUNCTION(BlueprintCallable, Category = "Stun")
	virtual void AddStunPoint(float StunAmount);

	//ダメージリアクション
	virtual void PlayHitReaction(float DamageAmount);

	//スタン中かどうかを取得する関数
	UFUNCTION(BlueprintCallable, Category = "Stun")
	bool IsStunned() const { return bIsStunned; }

	//スタン状態とタイマーをリセットする関数
	UFUNCTION(BlueprintCallable, Category = "Stun")
	virtual void ResetStun();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//スタン値が最大値まで行ったらスタンする
	virtual void OnStunMax();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	//スタンパラメーター
	//超えたらスタンになってしまうマックス値
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stun")
	float MaxStunPoint = 100.f;

	//この時間が経過したらスタン値が回復し始める
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stun")
	float StunRecoveryDelay = 3.f;

	//秒間あたりのスタン値の回復値
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stun")
	float StunRecoveryRate = 10.f;

	//現在のスタン値
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stun")
	float  CurrentStunPoint = 0.f;

	//スタンしているか
	bool bIsStunned = false;

	//タイマーハンドル
	FTimerHandle StunRecoveryTimerHandle;

private:
	//スタン値を減らしていく関数
	void DecreaseStunOverTime();
};
