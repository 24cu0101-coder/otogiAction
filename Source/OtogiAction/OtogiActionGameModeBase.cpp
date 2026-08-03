// Fill out your copyright notice in the Description page of Project Settings.


#include "OtogiActionGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "OtogiAction/minions/MinionsCharacter.h"
#include "GameFramework/Character.h"


//コンストラクタ
AOtogiActionGameModeBase::AOtogiActionGameModeBase()
{

}

//生成されたときとゲームが始まった時に呼ばれる処理
void AOtogiActionGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//ゲーム開始直後は初期位置であるプレイヤースタートで始まる。
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC && PC->GetPawn())
	{
		CurrentCheckpointTransform = PC->GetPawn()->GetActorTransform();
	}

	//Minionの初期位置保存
	TArray<AActor*>Minions;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMinionsCharacter::StaticClass(), Minions);

	MinionSpawnTransforms.Empty();
	for (AActor* Actor : Minions)
	{
		MinionSpawnTransforms.Add(Actor->GetActorTransform());
	}
}

//チェックポイントのアクターに触れたら呼ばれてスポーン地点が変わる
void AOtogiActionGameModeBase::SetCheckPoint(FTransform NewTransform)
{
	CurrentCheckpointTransform = NewTransform;
	UE_LOG(LogTemp, Warning, TEXT("Checkpont Update"));
}

//プレイヤーの復活処理
void AOtogiActionGameModeBase::RespawmPlayer(AController* TargetController)
{
	if (!TargetController)return;

	//もともとのアクターを処分
	APawn* OldPawn = TargetController->GetPawn();
	if (OldPawn)
	{
		OldPawn->Destroy();
	}

	// 現在いるMinionを全て削除
	TArray<AActor*> Minions;

	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		AMinionsCharacter::StaticClass(),
		Minions);

	for (AActor* Actor : Minions)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}

	// 保存した位置にMinionを再生成
	for (const FTransform& SpawnTransform : MinionSpawnTransforms)
	{
		GetWorld()->SpawnActor<AMinionsCharacter>(
			MinionClass,
			SpawnTransform);
	}

	//チェックポイントにプレイヤーを生成
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	//デフォルトのスポーンクラスをPlayerCharacterに設定
	if (DefaultPawnClass)
	{
		APawn* NewPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, CurrentCheckpointTransform, SpawnParams);
		if (NewPawn)
		{
			//コントローラーの権限を移動
			TargetController->Possess(NewPawn);
		}

	}


}

//シーン遷移
void AOtogiActionGameModeBase::ChangeLevel(FName LevelName)
{
	if (!LevelName.IsNone())
	{
		UGameplayStatics::OpenLevel(this, LevelName);
	}
}
