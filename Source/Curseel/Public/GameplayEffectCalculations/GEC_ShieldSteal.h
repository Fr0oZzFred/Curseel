#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Characters/Abilities/CharacterASC.h"
#include "Characters/Abilities/AttributeSets/AttributeSetBase.h"
#include "GEC_ShieldSteal.generated.h"

UCLASS()
class CURSEEL_API UGEC_ShieldSteal : public UGameplayEffectExecutionCalculation {
	GENERATED_BODY()
public:
	UGEC_ShieldSteal();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
