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

	//FString ObjectName = GetOwner()->GetName();
	//FString ObjPos = GetOwner()->GetTransform().GetLocation().ToString();
	//UE_LOG(LogTemp, Warning, TEXT("%s Grabber Position report at %s"), *ObjectName, *ObjPos);

	FindPhysicsHandleComponent();
	SetupInputComponent();
	
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input component found."));
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s Missing input component."), *GetOwner()->GetName());
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		//physics component found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s Missing physics handle component."), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerLocation;
	FRotator PlayerRotator;
	FString ObjectName = GetOwner()->GetName();
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotator);

	float Reach = 100.0f;
	FVector LineTracEnd = PlayerLocation + PlayerRotator.Vector() * Reach;

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTracEnd);
	}

}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key pressed."));
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();

	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		//PhysicsHandle->GrabComponent(
		//	ComponentToGrab,
		//	NAME_None,
		//	ComponentToGrab->GetOwner()->GetActorLocation(),
		//	true
		//	);
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()
			);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Released key."));
	PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector PlayerLocation;
	FRotator PlayerRotator;
	FString ObjectName = GetOwner()->GetName();
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotator);

	float Reach = 100.0f;
	FVector LineTracEnd = PlayerLocation + PlayerRotator.Vector() * Reach;

	/*DrawDebugLine(
		GetWorld(),
		PlayerLocation,
		LineTracEnd,
		FColor(255, 0, 0),
		false, -1, 0,
		20.0f
		);*/

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

	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
		UE_LOG(LogTemp, Warning, TEXT(" Grabber hit at %s"), *(ActorHit->GetName()));

	return Hit;
}