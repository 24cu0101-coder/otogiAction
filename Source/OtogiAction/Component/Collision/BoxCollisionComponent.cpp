// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxCollisionComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"

// Sets default values for this component's properties
UBoxCollisionComponent::UBoxCollisionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UBoxCollisionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBoxCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//指定範囲内に攻撃
void UBoxCollisionComponent::ExcuteAreaAttack(float Width, float Height, float Length, FName TargetTag, float Damage)
{
	//コンポーネントの親クラスを取得
	AActor* Owner = GetOwner();
	if (!Owner) return;

	// 自身の現在位置
	FVector Center = Owner->GetActorLocation();
	//判定を前後左右に動かす
	Center += Owner->GetActorForwardVector() * ForwardOffset;
	Center += Owner->GetActorRightVector() * SideOffset;

	//矩形の大きさを指定
	FVector HalfExtent = FVector(Length * 0.5f, Width * 0.5f, Height * 0.5f);
	HitCollisionShape = FCollisionShape::MakeBox(HalfExtent);

	// 検索対象のオブジェクトタイプ（PawnとWorldDynamic）
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	// クエリ設定（攻撃の実行者自身は無視する）
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner);

	//当たったオブジェクトを入れる配列
	TArray<FOverlapResult> OverlapResults;

	// 範囲内のオブジェクトをスキャン
	bool bHasOverlap = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,     //当たったオブジェクト
		Center,             //コリジョンの中心
		FQuat::Identity,    //回転
		ObjectQueryParams,  //検索対象のオブジェクト
		HitCollisionShape,    //判定をとる形
		QueryParams         //無視するオブジェクト
	);

	if (bHasOverlap)
	{
		// 重複して同じアクターにダメージを与えないよう、すでにダメージを与えたアクターを記憶する配列
		TArray<AActor*> DamagedActors;

		for (const FOverlapResult& Result : OverlapResults)
		{
			AActor* OverlappedActor = Result.GetActor();

			// アクターが存在し、指定のタグを持っており、まだダメージを与えていない場合
			if (OverlappedActor && OverlappedActor->ActorHasTag(TargetTag) && !DamagedActors.Contains(OverlappedActor))
			{
				// UE5標準のダメージ適用処理
				UGameplayStatics::ApplyDamage(
					OverlappedActor,
					Damage,
					Owner->GetInstigatorController(),
					Owner,
					UDamageType::StaticClass()
				);

				//デバッグ用の表示
				DrawDebugBox(
					GetWorld(),
					Center,
					HalfExtent,
					Owner->GetActorRotation().Quaternion(), // キャラクターの向きに合わせる
					FColor::Red,                            // 赤色で描画
					false,                                  // ずっと残さない
					2.0f,                                   // 2秒間表示
					0,
					2.0f                                    // 線の太さ
				);
				// 二重ヒット防止リストに追加
				DamagedActors.Add(OverlappedActor);

				UE_LOG(LogTemp, Warning, TEXT("[%s] Attacked [%s]; Damage: %f"), *Owner->GetName(), *OverlappedActor->GetName(), Damage);
			}
		}
	}
}
