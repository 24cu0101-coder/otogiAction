//担当：飯島
//タイトルで使用する鎖の破壊用クラス
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "ChainActor.generated.h"

UCLASS()
class OTOGIACTION_API AChainActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// コンストラクタ
	AChainActor();

protected:
	// 生成されたときに処理される
	virtual void BeginPlay() override;

public:	
	//毎フレーム処理される
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootComp;

	//鎖のジオメトリーコレクション
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UGeometryCollectionComponent* ChainGeometryCollection;

	// 鎖を破壊する関数
	UFUNCTION(BlueprintCallable, Category = "Chain")
	void BreakChain();
};
