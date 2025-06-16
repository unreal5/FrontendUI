// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncAction_PushSoftWidget.h"

#include "Subsystem/FrontendUISubsystem.h"
#include "Widget/Widget_ActivatableBase.h"

UAsyncAction_PushSoftWidget* UAsyncAction_PushSoftWidget::PushSoftWidget(const UObject* WorldContextObject,
                                                                         APlayerController* OwningPlayerController,
                                                                         TSoftClassPtr<UWidget_ActivatableBase>
                                                                         InSoftWidgetClass,
                                                                         FGameplayTag InWidgetStackTag,
                                                                         bool bFocusOnNewlyPushedWidget)
{
	checkf(!InSoftWidgetClass.IsNull(), TEXT("PushSoftWidgetToStack was passed a null soft widget class!"));

	if (!GEngine) return nullptr;

	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		auto Node = NewObject<UAsyncAction_PushSoftWidget>();
		Node->CachedOwningWorld = World;
		Node->CachedOwningPlayerController = OwningPlayerController;
		Node->CachedSoftWidgetClass = InSoftWidgetClass;
		Node->CachedWidgetStackTag = InWidgetStackTag;
		Node->bCachedFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;

		Node->RegisterWithGameInstance(World);
		return Node;
	}
	return nullptr;
}

void UAsyncAction_PushSoftWidget::Activate()
{
	// Super::Activate();
	auto FrontendUISubsystem = UFrontendUISubsystem::Get(CachedOwningWorld.Get());
	FrontendUISubsystem->PushSoftWidgetToStackAsync(CachedWidgetStackTag,
	                                                CachedSoftWidgetClass,
	                                                [this](EAsyncPushWidgetState InPushState,
	                                                       UWidget_ActivatableBase* PushedWidget)
	                                                {
		                                                switch (InPushState)
		                                                {
		                                                case EAsyncPushWidgetState::OnCreatedBeforePush:
			                                                PushedWidget->SetOwningPlayer(
				                                                CachedOwningPlayerController.Get());
			                                                OnWidgetCreatedBeforePush.Broadcast(PushedWidget);
			                                                break;
		                                                case EAsyncPushWidgetState::AfterPush:
			                                                AfterPush.Broadcast(PushedWidget);
			                                                if (bCachedFocusOnNewlyPushedWidget)
			                                                {
				                                                if (UWidget* WidgetToFocus = PushedWidget->GetDesiredFocusTarget())
				                                                {
					                                                WidgetToFocus->SetFocus();
				                                                }
			                                                }
		                                                	SetReadyToDestroy();
			                                                break;
		                                                default:
			                                                break;
		                                                }
	                                                });
}
