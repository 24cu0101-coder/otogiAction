// Fill out your copyright notice in the Description page of Project Settings.


#include "AxActor.h"

// Sets default values
AAxActor::AAxActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAxActor::BeginPlay()
{
	Super::BeginPlay();

	// スポーンした瞬間の位置・回転・正面方向を記憶
	StartLocation = GetActorLocation();
	StartRotation = GetActorRotation();
	ForwardVector = GetActorForwardVector();
	
}

// Called every frame
void AAxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ElapsedTime < TotalSwingTime)
	{
		ElapsedTime += DeltaTime;
		// 0.0 〜 1.0 への進行度を計算
		float Alpha = FMath::Clamp(ElapsedTime / TotalSwingTime, 0.0f, 1.0f);

		// 💡 位置の計算：前方に進みながら、放物線（山なり）を描くようにZ軸（高さ）にサイン波を足す
		float ArcHeight = 80.f; // 💡 山なりの頂点の高さ（どれくらい上に膨らむか）
		float VerticalOffset = FMath::Sin(Alpha * PI) * ArcHeight; // 奇麗な放物線を作る

		// 前進＋沈み込みの基本直線
		FVector BaseLocation = FMath::Lerp(StartLocation, StartLocation + (ForwardVector * 250.f) - FVector(0, 0, 100.f), Alpha);
		// そこに山なりの高さをプラス
		FVector NewLocation = BaseLocation + FVector(0, 0, VerticalOffset);

		// 💡 回転の計算：前方にギュンと叩きつけるためにPitchを回転させる
		FRotator NewRotation = StartRotation;
		NewRotation.Pitch += (Alpha * -110.f); // マイナス方向に傾けることで前方へ振り下ろす動きになる

		// 座標の反映
		SetActorLocationAndRotation(NewLocation, NewRotation);
	}
}

