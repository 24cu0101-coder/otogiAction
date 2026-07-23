#include "OrbSpawnComponent.h"
#include "OtogiAction/Component/Status/StatusComponent.h"
#include "OrbActor.h"

UOrbSpawnComponent::UOrbSpawnComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UOrbSpawnComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UOrbSpawnComponent::SpawnOrbs(AActor* OwnerEnemy,float Damage)
{
    // OrbClassまたはOwnerEnemyが設定されていなければ終了
    if (!OrbClass || !OwnerEnemy)
    {
        return;
    }

    // EnemyのStatusComponentを取得
    UStatusComponent* StatusComponent =OwnerEnemy->FindComponentByClass<UStatusComponent>();

    if (!StatusComponent)
    {
        return;
    }

    // ダメージを受ける前の現在HP
    float CurrentHP = StatusComponent->GetCurrentHP();

    // 現在HP以上のダメージではオーブを生成しない
    float ActualDamage = FMath::Min(Damage, CurrentHP);

    // まだオーブに変換していないダメージ
    float RemainingDamage = ActualDamage;

    // ダメージが残っている間オーブを生成
    while (RemainingDamage > 0.f)
    {
        // オーブ1個が持つダメージ（最大10）
        float OrbDamage = FMath::Min(10.f, RemainingDamage);

        // Enemyの位置を取得
        FVector SpawnLocation = GetOwner()->GetActorLocation();

        // 少しランダムな位置にスポーン
        SpawnLocation += FVector(
            FMath::RandRange(-150.f, 150.f),
            FMath::RandRange(-150.f, 150.f),
            150.f);

        // Orb生成
        AOrbActor* SpawnedOrb =GetWorld()->SpawnActor<AOrbActor>(OrbClass,SpawnLocation, FRotator::ZeroRotator);

        // 生成できたらOwnerとダメージを設定
        if (SpawnedOrb)
        {
            // このEnemyが出したOrbとして登録
            SpawnedOrb->SetOwnerEnemy(OwnerEnemy);

            // Orbが持つダメージを設定
            SpawnedOrb->SetOrbDamage(OrbDamage);
        }

        // オーブにした分のダメージを減らす
        RemainingDamage -= OrbDamage;
    }
}