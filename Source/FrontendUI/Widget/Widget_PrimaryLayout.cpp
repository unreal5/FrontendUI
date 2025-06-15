// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_PrimaryLayout.h"
#include "FrontendDebugHelper.h"
UCommonActivatableWidgetContainerBase* UWidget_PrimaryLayout::FindWidgetStackByTag(const FGameplayTag& InStackTag) const
{
	checkf(RegisteredWidgetStackMap.Contains(InStackTag), TEXT("Can not find widget stack with tag %s"), *InStackTag.ToString());
	return RegisteredWidgetStackMap.FindRef(InStackTag);
}

void UWidget_PrimaryLayout::RegisterWidgetStack(FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack)
{
	if (!IsDesignTime())
	{
		if (!RegisteredWidgetStackMap.Contains(InStackTag))
		{
			RegisteredWidgetStackMap.Add(InStackTag, InStack);

			// DEBUG::Print(FString::Printf(TEXT("Registered widget stack with tag %s"), *InStackTag.ToString()), -1, FColor::Green);
		}
	}
}
