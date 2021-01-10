// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "BuildingEscape.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
	//FRotator NewRotation = FRotator(0.f, OpenAngle, 0.f);

	//Owner->SetActorRotation(NewRotation);
	OnOpenRequest.Broadcast();
}

void UOpenDoor::CloseDoor()
{
	//FRotator NewRotation = FRotator(0.f, 0.f, 0.f);

	//Owner->SetActorRotation(NewRotation);
	OnCloseRequest.Broadcast();
}

void UOpenDoor::StopOpenDoor()
{
	OnStopRequest.Broadcast();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(GetTotalMassOfActorsOnPlate() > 150.f){
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	else
	{
		StopOpenDoor();
	}
	
	
	// Check if it's time to close the door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
		CloseDoor();
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for(const auto& Actor: OverlappingActors){
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate, %f"), *Actor->GetName(), TotalMass);
	}
	return TotalMass;
}
