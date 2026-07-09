//攻撃用こりじょんのコンポーネント

#include "AttackCollisionComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

//コンストラクタ
UAttackCollisionComponent::UAttackCollisionComponent()
{
	//TickはOffに
	PrimaryComponentTick.bCanEverTick = true;

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
