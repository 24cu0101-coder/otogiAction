#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHPWidget.generated.h"

class UProgressBar;

UCLASS()
class OTOGIACTION_API UEnemyHPWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// HPバー更新
	UFUNCTION(BlueprintCallable)
	void SetHP(float CurrentHP, float MaxHP);

protected:

	virtual void NativeConstruct() override;

	// Widget内のProgressBarと名前を合わせる
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HPBar;
};