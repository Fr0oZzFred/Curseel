// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/CharacterPlayerController.h"

void ACharacterPlayerController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
	ACharacterPlayerState* PS = GetPlayerState<ACharacterPlayerState>();
	if (PS) {
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}