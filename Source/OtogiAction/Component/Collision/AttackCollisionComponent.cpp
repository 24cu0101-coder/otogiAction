//攻撃用コリジョンのコンポーネント

#include "AttackCollisionComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"
#include "Components/SphereComponent.h"
//コンストラクタ
UAttackCollisionComponent::UAttackCollisionComponent()
{
	//TickはOffに
	PrimaryComponentTick.bCanEverTick = true;

	SphereAttackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
}


//ゲームが始まったときに呼ばれる関数
void UAttackCollisionComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UAttackCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttackCollisionComponent::ExcuteAreaAttack(float Radius, FName TargetTag, float Damage)
{
    //コンポーネントの親クラスを取得
    AActor* Owner = GetOwner();
    if (!Owner) return;

    // 自身の現在位置
    FVector Center = Owner->GetActorLocation();
    //判定を前後左右に動かす
    Center += Owner->GetActorForwardVector() * ForwardOffset;
    Center += Owner->GetActorRightVector() * SideOffset;

    TArray<FOverlapResult> OverlapResults;
    FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(Radius);

    // 検索対象のオブジェクトタイプ（PawnとWorldDynamic）
    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
    ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

    // クエリ設定（攻撃の実行者自身は無視する）
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Owner);

    // 範囲内のオブジェクトをスキャン
    bool bHasOverlap = GetWorld()->OverlapMultiByObjectType(
        OverlapResults,     //
        Center,             //球の中心
        FQuat::Identity,    //
        ObjectQueryParams,  //検索対象のオブジェクト
        CollisionSphere,    //判定をとる形
        QueryParams         //無視するオブジェクト
    );

    // デバッグ用の球体を表示（赤色で1秒間表示）
    DrawDebugSphere(GetWorld(), Center, Radius, 16, FColor::Red, false, 1.0f);

    if (bHasOverlap)
    {
        // 重複して同じアクターにダメージを与えないよう、すでにダメージを与えたアクターを記憶する配列
        TArray<AActor*> DamagedActors;

        for (const FOverlapResult& Result : OverlapResults)
        {
            AActor* OverlappedActor = Result.GetActor();

            // アクターが存在し、指定のタグを持っており、まだダメージを与えていない場合
            if (OverlappedActor && OverlappedActor->ActorHasTag(TargetTag))
            {
                // UE5標準のダメージ適用処理
                UGameplayStatics::ApplyDamage(
                    OverlappedActor,
                    Damage,
                    Owner->GetInstigatorController(),
                    Owner,
                    UDamageType::StaticClass()
                );
                // 二重ヒット防止リストに追加
                DamagedActors.Add(OverlappedActor);

                UE_LOG(LogTemp, Warning, TEXT("[%s] Attacked [%s]; Damage: %f"), *Owner->GetName(), *OverlappedActor->GetName(), Damage);
            }
        }
    }
}