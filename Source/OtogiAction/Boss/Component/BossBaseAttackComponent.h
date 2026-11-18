// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossBaseAttackComponent.generated.h"

// 攻撃終了を外部（BT Taskなど）に通知するためのデリゲート宣言
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEndedSignature);

UCLASS(Blueprintable,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OTOGIACTION_API UBossBaseAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossBaseAttackComponent();

	// BT Taskなどがバインドする終了通知イベント
	UPROPERTY(BlueprintAssignable, Category = "Attack|Events")
	FOnAttackEndedSignature OnAttackEnded;

	// 現在攻撃中かどうかのゲッター
	UFUNCTION(BlueprintPure, Category = "Attack|State")
	bool IsAttacking() const { return bIsAttacking; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// モンタージュ再生終了時のコールバック
	UFUNCTION()
	void OnMontageEndedCallback(UAnimMontage* Montage, bool bInterrupted);

	// 再生する攻撃モンタージュ（エディタや子クラスから設定可能）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Animation")
	TObjectPtr<UAnimMontage> AttackMontage;

	// 現在攻撃中かどうかのフラグ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack|State")
	bool bIsAttacking = false;

	// 子クラスからモンタージュを再生するための共通ヘルパー関数
	UFUNCTION(BlueprintCallable, Category = "Attack|Animation")
	bool PlayAttackMontage(UAnimMontage* MontageToPlay, float PlayRate = 1.0f, FName StartSectionName = NAME_None);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 攻撃開始関数
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
	void StartAttack();
	virtual void StartAttack_Implementation();

	// 攻撃終了関数
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
	void EndAttack();
	virtual void EndAttack_Implementation();
};
