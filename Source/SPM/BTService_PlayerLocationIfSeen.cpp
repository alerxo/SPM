// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"

#include "AIController.h"
#include "SpiderbotController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
	NodeName = TEXT("Update Player Location If Seen");
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return;
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	if (PlayerPawn == nullptr)
	{
		return;
	}

	ASpiderbotController* SpiderbotController = Cast<ASpiderbotController>(OwnerComp.GetOwner());
	if (SpiderbotController == nullptr)
	{
		return;
	}
	
	const float DistanceToPlayer = FVector::Dist(PlayerPawn->GetActorLocation(), OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation());
	
	if (OwnerComp.GetAIOwner()->LineOfSightTo(PlayerPawn) && DistanceToPlayer <= SpiderbotController->GetVisionRange())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}
}