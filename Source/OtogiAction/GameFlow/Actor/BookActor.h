// 担当：飯島

//タイトル画面で使用するBookActor
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BookActor.generated.h"

UCLASS()
class OTOGIACTION_API ABookActor : public AActor
{
	GENERATED_BODY()
	
public:	
	//コンストラクタ
	ABookActor();

protected:
	//スポーンしたときに処理される
	virtual void BeginPlay() override;

public:	
	// 毎フレーム処理される
	virtual void Tick(float DeltaTime) override;

	//メッシュ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootComp;

	//閉じた本用メッシュ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ClosedBookMesh;

	//開いた本用メッシュ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* OpenBookMesh;

	// 開いた本に切り替える関数
	UFUNCTION(BlueprintCallable, Category = "Book")
	void OpenBook();
};
