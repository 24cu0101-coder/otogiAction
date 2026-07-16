// ヒットしたダメージに対してのリアクションステータス

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HitReactionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UHitReactionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHitReactionComponent();

	//被弾したときに呼ばれる関数
	void PlayHitReaction(float DamageAmount);

	void  RequestGetUp();

	//ダウン状態化を取得するゲッター
	bool IsDowned() const { return bIsDowned; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//ヒットリアクションモンタージュ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anims")
	UAnimMontage* LightHitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anims")
	UAnimMontage* HeavyHitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anims")
	UAnimMontage* GetupMontage;

	// この数値以上のダメージを受けたらダウン状態にする
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float HeavyDamageThreshold = 20.0f;

	// 通常のけぞりのノックバック力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float LightKnockbackForce = 400.0f;

	// 吹っ飛びダウン時のノックバック力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float HeavyKnockbackForce = 1200.0f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool bIsDowned = false;

	// 起き上がりアニメーションが終了した時のコールバック
	UFUNCTION()
	void OnGetUpFinished(UAnimMontage* Montage, bool bInterrupted);
};
