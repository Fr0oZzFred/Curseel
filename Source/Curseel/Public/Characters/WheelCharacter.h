#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "GameplayEffectTypes.h"
#include "Characters/Abilities/CharacterASC.h"
#include "Characters/Abilities/AttributeSets/AttributeSetBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "WheelCharacter.generated.h"

UCLASS()
class CURSEEL_API AWheelCharacter : public ACharacterBase
{
	GENERATED_BODY()
public:
	AWheelCharacter(const class FObjectInitializer& ObjectInitializer);

protected:

	// Actual hard pointer to AbilitySystemComponent
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|WheelCharacters")
	class UCharacterASC* HardRefAbilitySystemComponent;

	// Actual hard pointer to AttributeSetBase
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|WheelCharacters")
	class UAttributeSetBase* HardRefAttributeSetBase;

	virtual void BeginPlay() override;
};