#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Characters/Abilities/CharacterASC.h"
#include "Characters/Abilities/AttributeSets/AttributeSetBase.h"
#include "GEC_Health.generated.h"

UCLASS()
class CURSEEL_API UGEC_Health : public UGameplayEffectExecutionCalculation {
	GENERATED_BODY()
public:
	UGEC_Health();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
