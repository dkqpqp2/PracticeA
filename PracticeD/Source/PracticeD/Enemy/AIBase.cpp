// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBase.h"
#include "AI/DefaultAIController.h"

// Sets default values
AAIBase::AAIBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	RootComponent = Root;
	Mesh->SetupAttachment(Root);

	Movement->SetUpdatedComponent(Root);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FClassFinder<AAIController> AIClass(TEXT("/Script/PracticeD.DefaultAIController"));
	if (AIClass.Class)
	{
		AIControllerClass = AIClass.Class;
	}

}

// Called when the game starts or when spawned
void AAIBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAIBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


