#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Characters/Player/CharacterPlayerState.h"
#include "Characters/Player/CharacterPlayerController.h"
#include "Characters/Abilities/CharacterASC.h"
#include <Kismet/KismetMathLibrary.h>
#include "PlayerCharacter.generated.h"

UCLASS()
class CURSEEL_API APlayerCharacter : public ACharacterBase {
	GENERATED_BODY()
public:
	APlayerCharacter(const class FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

protected:

	bool ASCInputBound = false;
	FGameplayTag DeadTag;
	FGameplayTag StunTag;
	UFUNCTION(BlueprintCallable, Category = "Player Character")
	bool CanMove() const;
	void InitiliazeStartingValues(ACharacterPlayerState* PS);
	virtual void OnRep_PlayerState() override;
	void BindASCInput();
};