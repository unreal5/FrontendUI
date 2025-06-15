#pragma once

namespace  DEBUG
{
	static void Print(const FString& Message, int32 InKey = -1, const FColor& InColor = FColor::MakeRandomColor())
	{
		if (!GEngine)
		{
			return;
		}
		
		GEngine->AddOnScreenDebugMessage(InKey, 5.f, InColor, Message);
	}
}
