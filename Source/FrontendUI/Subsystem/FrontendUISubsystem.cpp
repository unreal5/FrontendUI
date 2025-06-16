// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontendUISubsystem.h"

#include "Engine/AssetManager.h"
#include "Widget/Widget_ActivatableBase.h"
#include "Widget/Widget_PrimaryLayout.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

UFrontendUISubsystem* UFrontendUISubsystem::Get(const UObject* WorldContextObject)
{
	if (!GEngine) return nullptr;

	UWorld* ThisWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	return UGameInstance::GetSubsystem<UFrontendUISubsystem>(ThisWorld->GetGameInstance());
}

bool UFrontendUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);
		return FoundClasses.IsEmpty();
	}
	return false;
}

void UFrontendUISubsystem::RegisterCreatedPrimaryLayout(UWidget_PrimaryLayout* InCreatedWidget)
{
	check(InCreatedWidget);
	CreatedPrimaryLayout = InCreatedWidget;
}

void UFrontendUISubsystem::PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag,
                                                      const TSoftClassPtr<UWidget_ActivatableBase>& InSoftWidgetClass,
                                                      TFunction<void (EAsyncPushWidgetState, UWidget_ActivatableBase*)>
                                                      AsyncPushCallback)
{
	check(!InSoftWidgetClass.IsNull());

	UAssetManager::Get()
		.GetStreamableManager()
		.RequestAsyncLoad(InSoftWidgetClass.ToSoftObjectPath(), FStreamableDelegate::CreateLambda(
			                  [this, InWidgetStackTag, InSoftWidgetClass,
				                  AsyncPushCallback]()
			                  {
				                  UClass* LoadedWidgetClass = InSoftWidgetClass.Get();
				                  check(LoadedWidgetClass);

				                  UCommonActivatableWidgetContainerBase* FoundWidgetStack = CreatedPrimaryLayout->
					                  FindWidgetStackByTag(InWidgetStackTag);

				                  UWidget_ActivatableBase* CreatedWidget = FoundWidgetStack->AddWidget<
					                  UWidget_ActivatableBase>(
					                  LoadedWidgetClass,
					                  [AsyncPushCallback](UWidget_ActivatableBase& CreatedWidgetInstance)
					                  {
						                  AsyncPushCallback(EAsyncPushWidgetState::OnCreatedBeforePush,
						                                    &CreatedWidgetInstance);
					                  });
				                  AsyncPushCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
			                  }));
}
