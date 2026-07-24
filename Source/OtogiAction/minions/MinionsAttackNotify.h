#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MinionsAttackNotify.generated.h"


UCLASS()
class OTOGIACTION_API UMinionsAttackNotify : public UAnimNotify
{
	GENERATED_BODY()

public:

	virtual void Notify(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation
	) override;


public:

	//攻撃範囲
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Radius = 200.f;


	//ダメージ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Damage = 20.f;


	//前方向へのずらし
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float ForwardOffset = 100.f;


	//横方向へのずらし
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float SideOffset = 0.f;

};