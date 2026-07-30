// Fill out your copyright notice in the Description page of Project Settings.


#include "InputBufferComponent.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/NormalAttack/NormalComboAttackComponent.h"


// Sets default values for this component's properties
UInputBufferComponent::UInputBufferComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UInputBufferComponent::BeginPlay()
{
	Super::BeginPlay();

	//アビリティシステムコンポーネント取得
	ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());

	//アビリティシステムコンポーネントを取得できていれば
	if (ASC)
	{
		//先行入力中のタグを登録
		FGameplayTag TypeAheadTag = FGameplayTag::RequestGameplayTag(TEXT("InputBufferReception"), false);

		//タグ終了時にイベントを発動
		ASC->RegisterGameplayTagEvent(TypeAheadTag, EGameplayTagEventType::NewOrRemoved)
			.AddUObject(this, &UInputBufferComponent::ExecuteBoffereFunction);
	}	
}

bool UInputBufferComponent::InputBufferFlag()
{
	//先行入力を受け付けるタグ　
	FGameplayTag TypeAheadTag = FGameplayTag::RequestGameplayTag(TEXT("InputBufferReception"));

	//returnで返すboolの値(何もなければfalse)
	bool Function = false;

	//アビリティシステムコンポーネントがあるなら
	if (ASC)
	{
		//タグを検知して
		if (ASC->HasMatchingGameplayTag(TypeAheadTag))
		{
			//タグがあるならtureに
			Function = true;
		}
	}

	//boolを返す	
	return Function;


}


//関数を保存、もしくは実行する関数(各コンポーネントで呼び出す　引数には各アクションを実行する関数を入れる)
void UInputBufferComponent::KeepOrExeFunction(TFunction<void()> Function)
{
	if (!Function)return;

	//入力受付のタグがあれば
	if (InputBufferFlag())
	{
		//関数を保存
		NextFunction = Function;
	}
	//なければ
	else
	{
		//そのまま実行
		Function();
	}
}

//保存していた関数を実行する関数(タグを検知して呼び出す)
void UInputBufferComponent::ExecuteBoffereFunction(const FGameplayTag Tag, int32 NewCount)
{
	//関数があれば
	if (NextFunction)
	{
		//保存していた関数を実行
		TFunction<void()> LocalFunction = NextFunction;

		LocalFunction();

		//保存していた関数を消去
		NextFunction = nullptr;
	}
}


void UInputBufferComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

