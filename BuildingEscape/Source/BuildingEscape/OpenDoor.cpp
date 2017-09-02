// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"

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
	if (!PressurePlate)
		UE_LOG(LogTemp, Error, TEXT("%s Missing PressurePlate."), *GetOwner()->GetName());
}

void UOpenDoor::OpenDoor()
{
	FRotator NewRotation = FRotator(0.0f, -140.0f, 0.0f);
	//if(Owner)
		//Owner->SetActorRotation(NewRotation);

	OnOpenRequest.Broadcast();

}

void UOpenDoor::CloseDoor()
{
	FRotator NewRotation = FRotator(0.0f, -90.0f, 0.0f);
	Owner->SetActorRotation(NewRotation);
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		//Poll the trigger volume
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}


	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;

	TArray<AActor*> OverlappingActors;

	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (const auto& Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

