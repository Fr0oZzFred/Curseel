#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CharacterASC.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, UCharacterASC*, SourceASC, float, BaseDamage, float, ModifiedDamage);

UCLASS()
class CURSEEL_API UCharacterASC : public UAbilitySystemComponent {
	GENERATED_BODY()
public:
	bool CharacterAbilitiesGiven = false;
	bool StartupEffectsApplied = false;

	FReceivedDamageDelegate ReceivedDamage;

	virtual void ReceiveDamage(UCharacterASC* SourceASC, float BaseDamage, float ModifiedDamage);
};
