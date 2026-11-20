// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BossBaseAttackTask.generated.h"

class UBossBaseAttackComponent;

/**
 * ボスの各種攻撃コンポーネントを汎用的に
 */

UCLASS()
class OTOGIACTION_API UBossBaseAttackTask : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBossBaseAttackTask();

protected:
	// タスク開始時
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// タスク中断時
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	//エディタ上で選択したクラスの表示名をノード名に反映させる
	virtual FString GetStaticDescription() const override;

	// 攻撃終了通知を受け取るコールバック
	UFUNCTION()
	virtual void HandleAttackEnded();

	// デリゲートの解除やキャッシュクリアを行う共通ヘルパー
	virtual void CleanupTask();

	// エディタ上で実行したい攻撃コンポーネントのクラスを指定する
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<UBossBaseAttackComponent> TargetAttackComponentClass;

	// 子クラスからもアクセスできるよう protected に配置
	TWeakObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;
	TWeakObjectPtr<UBossBaseAttackComponent> CachedAttackComp;
};
