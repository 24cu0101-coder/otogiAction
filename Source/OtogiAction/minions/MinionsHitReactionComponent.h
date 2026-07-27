#pragma once

#include "OtogiAction/Component/UHitReactionBaseComponent.h"
#include "CoreMinimal.h"
#include "MinionsHitReactionComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OTOGIACTION_API UMinionsHitReactionComponent : public UUHitReactionBaseComponent
{
	GENERATED_BODY()

public:
	UMinionsHitReactionComponent();

	// ヒットリアクション
	virtual void PlayHitReaction(float DamageAmount) override;

protected:

	// スタン時
	virtual void OnStunMax() override;

	// 怯み時
	virtual void OnFearMax() override;

	// スタンダウン時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Reaction")
	float StunDownDuration = 4.0f;

	// 軽い被弾モンタージュ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* LightHitMontage;

	// スタンダウンモンタージュ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* StunKnockdownMontage;

private:

	FTimerHandle StunDownTimerHandle;

	void RecoverFromStun();
};