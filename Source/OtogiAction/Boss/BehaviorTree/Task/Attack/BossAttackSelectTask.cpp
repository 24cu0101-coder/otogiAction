// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAttackSelectTask.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

UBossAttackSelectTask::UBossAttackSelectTask()
{
	NodeName = TEXT("Select Next Action");

	// キーのフィルタリング設定
	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBossAttackSelectTask, TargetActorKey), AActor::StaticClass());
	AIStateKey.AddEnumFilter(this, GET_MEMBER_NAME_CHECKED(UBossAttackSelectTask, AIStateKey), StaticEnum<EEnemyState>());

	// デフォルトの抽選候補（近距離用と中距離用の例）
	FBossAttackCandidate Normal;
	Normal.State = EEnemyState::NormalAttack;
	Normal.Weight = 1.0f;
	Normal.MinDistance = 0.0f;
	Normal.MaxDistance = 300.0f;

	FBossAttackCandidate Combo;
	Combo.State = EEnemyState::ComboAttack;
	Combo.Weight = 1.5f;
	Combo.MinDistance = 0.0f;
	Combo.MaxDistance = 450.0f;

	CandidateList = { Normal, Combo };
}

EBTNodeResult::Type UBossAttackSelectTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!AIController || !BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	// 自分自身(Pawn)とターゲット(プレイヤー)を取得
	APawn* ControlledPawn = AIController->GetPawn();
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!ControlledPawn || !TargetActor)
	{
		return EBTNodeResult::Failed;
	}
	
	//プレイヤーとの水平距離を計算
	const float DistanceToTarget = FVector::Dist2D(ControlledPawn->GetActorLocation(), TargetActor->GetActorLocation());
	const EEnemyState CurrentState = static_cast<EEnemyState>(BlackboardComp->GetValueAsEnum(AIStateKey.SelectedKeyName));

	//条件（距離・重み）に合う候補を抽出しつつ、合計の重みを計算
	TArray<FBossAttackCandidate> ValidCandidates;
	float TotalWeight = 0.0f;

	for (const FBossAttackCandidate& Candidate : CandidateList)
	{
		// 重みが0以下、または距離が範囲外ならスキップ
		if (Candidate.Weight <= 0.0f)
		{
			continue;
		}
		if (DistanceToTarget < Candidate.MinDistance || DistanceToTarget > Candidate.MaxDistance)
		{
			continue;
		}

		ValidCandidates.Add(Candidate);
		TotalWeight += Candidate.Weight;
	}

	//「直前と同じ行動を避ける」処理（有効な候補が2つ以上ある場合のみ除外）
	if (bAvoidSameState && ValidCandidates.Num() > 1)
	{
		for (int32 i = ValidCandidates.Num() - 1; i >= 0; --i)
		{
			if (ValidCandidates[i].State == CurrentState)
			{
				TotalWeight -= ValidCandidates[i].Weight;
				ValidCandidates.RemoveAt(i);
			}
		}
	}

	//有効な候補が1つもない場合の処理（距離が遠すぎる等）
	if (ValidCandidates.IsEmpty() || TotalWeight <= 0.0f)
	{
		if (FallbackState != EEnemyState::None)
		{
			BlackboardComp->SetValueAsEnum(AIStateKey.SelectedKeyName, static_cast<uint8>(FallbackState));
			return EBTNodeResult::Succeeded;
		}
		return EBTNodeResult::Failed;
	}

	//重み付きランダム選出
	float RandomValue = FMath::FRandRange(0.0f, TotalWeight);
	EEnemyState SelectedState = ValidCandidates.Last().State;

	for (const FBossAttackCandidate& Candidate : ValidCandidates)
	{
		if (RandomValue <= Candidate.Weight)
		{
			SelectedState = Candidate.State;
			break;
		}
		RandomValue -= Candidate.Weight;
	}

	// Blackboardに選定結果（Enum）を書き込む
	BlackboardComp->SetValueAsEnum(AIStateKey.SelectedKeyName, static_cast<uint8>(SelectedState));

	//プレイヤーとの距離、有力候補、現在のStateを確認
	UE_LOG(LogTemp, Warning, TEXT("Dist2D: %f, ValidCount: %d, CurrentState: %d"),
		DistanceToTarget, ValidCandidates.Num(), static_cast<int32>(CurrentState));

	return EBTNodeResult::Succeeded;
}

void UBossAttackSelectTask::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		TargetActorKey.ResolveSelectedKey(*BBAsset);
		AIStateKey.ResolveSelectedKey(*BBAsset);
	}
}
