// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FString ObjectName = GetOwner()->GetName();
	//FString ObjPos = GetOwner()->GetTransform().GetLocation().ToString();
	//UE_LOG(LogTemp, Warning, TEXT("%s Grabber Position report at %s"), *ObjectName, *ObjPos);

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		//physics component found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s Missing physics handle component."), *ObjectName);
	}
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	FVector PlayerLocation;
	FRotator PlayerRotator;
	FString ObjectName = GetOwner()->GetName();
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotator);
	//UE_LOG(LogTemp, Warning, TEXT("%s Grabber Position report at %s"), *ObjectName, *(PlayerLocation.ToString()));
	//UE_LOG(LogTemp, Warning, TEXT("%s Grabber Direction report at %s"), *ObjectName, *(PlayerRotator.ToString()));

	float Reach = 100.0f;
	FVector LineTracEnd = PlayerLocation + PlayerRotator.Vector() * Reach;
	

	DrawDebugLine(
		GetWorld(),
		PlayerLocation,
		LineTracEnd,
		FColor(255, 0, 0),
		false, -1, 0,
		20.0f
		);

	FHitResult Hit;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType
		(
			OUT Hit,
			PlayerLocation,
			LineTracEnd,
			FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
			TraceParameters
			);

	//Hit.Actor.Get()->GetName();
	AActor* ActorHit = Hit.GetActor();
	if(ActorHit)
		UE_LOG(LogTemp, Warning, TEXT(" Grabber hit at %s"), *(ActorHit->GetName()));
}

