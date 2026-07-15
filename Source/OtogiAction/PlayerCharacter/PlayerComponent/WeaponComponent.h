// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

//武器のスタイルを定義
UENUM(BlueprintType)
enum class  EPlayerWeaponStyle : uint8
{
	Nomal	UMETA(DisplayName = "Normal"),
	Kintaro	UMETA(DisplayName = "Kintaro"),
};

//前方宣言
class UNiagaraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

	//スタイルの切り替え
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetWeaponStyle(EPlayerWeaponStyle PlayerStyle);
	
	//現在のスタイルを取得
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	EPlayerWeaponStyle GetCurrentStyle() const { return CurrentStyle; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	//刀アクター
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AActor>WeaponActorClass;

	//金太郎オーラ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Reference")
	UNiagaraComponent* KintaroEffect;

	//タグが追加・消失したときに呼ばれる関数
	void OnKintaroTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
private:
	// 現在のスタイル
	UPROPERTY(VisibleAnywhere, Category = "Weapon | State")
	EPlayerWeaponStyle CurrentStyle = EPlayerWeaponStyle::Nomal;

	void SetActorHidden(AActor* TargetActor, bool bNewHidden);
};
