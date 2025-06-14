// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontendPlayerController.h"

#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"

void AFrontendPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	TArray<AActor*> CameraActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(this,ACameraActor::StaticClass(),"Default", CameraActors);
	if (CameraActors.Num() > 0)
	{
		SetViewTarget(CameraActors[0]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No camera actor found with tag 'Default'"));
	}
}
