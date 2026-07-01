
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "../Component/Collision/AttackCollisionComponent.h"
#include "AttackCollisionNotifyState.generated.h"


UCLASS()
class OTOGIACTION_API UAttackCollisionNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	//攻撃タイプ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	EAttackCollisionType CollisionType = EAttackCollisionType::FrontSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float  Damage = 20.f;

	// 通知の瞬間
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	// 通知の終わり
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
