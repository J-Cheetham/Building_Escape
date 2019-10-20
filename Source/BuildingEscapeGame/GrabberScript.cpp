// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabberScript.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabberScript::UGrabberScript()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabberScript::BeginPlay()
{
	Super::BeginPlay();

	FindAttachedPhysicsHandle();
	FindAttachedInputComponent();
}


// Called every frame
void UGrabberScript::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	///Set up the line tracing using linetracesinglebyobjecttype
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	//Move the grabbed component to the end of the line trace
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}



FHitResult UGrabberScript::GetFirstPhysicsBodyInReach()
{
	//Gets the player view point location and rotation
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	///Set up the line tracing using linetracesinglebyobjecttype
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	FHitResult Hit;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);

	///See the name of what we hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()))
	}

	return Hit;
}

///Looks for attached input component that only appears at runtime
void UGrabberScript::FindAttachedInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component found"))
			InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabberScript::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabberScript::Release);
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is missing Input Component"), *GetOwner()->GetName())
	}
}

///Looks for the physics handle attached to the pawn blueprint
void UGrabberScript::FindAttachedPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Physics Handle found"))
	}
	//Error message if no physic handle component attached
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is missing physics handle component"), *GetOwner()->GetName())
	}
}

void UGrabberScript::Grab()
{
	//Line trace and see if we grab something
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"))
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	///If the hit result returns an actor then attach a physics handle to that actor
	if (ActorHit)
	{
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation());
	}

}

void UGrabberScript::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"))
		PhysicsHandle->ReleaseComponent();
}


