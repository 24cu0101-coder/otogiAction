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
	void PlayHitReaction(AActor* Attacker);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//ヒットリアクションモンタージュ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anims")
	UAnimMontage* HitReactMontage;

	//ノックバックの強さ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	float KnockbackForce = 800.0f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
