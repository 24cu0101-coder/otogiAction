//アタックコリジョンを管理しているコンポーネント

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackCollisionComponent.generated.h"

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


private:
};
