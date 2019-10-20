// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "GrabberScript.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPEGAME_API UGrabberScript : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabberScript();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void FindAttachedInputComponent();

	void FindAttachedPhysicsHandle();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FHitResult GetFirstPhysicsBodyInReach();

		
private:
	//How far ahead of the player can we grab objects
	float Reach = 100.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UInputComponent* InputComponent = nullptr;

	//Ray-cast and grab/release what's in reach
	void Grab();
	void Release(); 

	//Returns current start of line cast
	FVector GetLineCastStart();

	//Returns current end of the line cast
	FVector GetLineCastEnd();
};
