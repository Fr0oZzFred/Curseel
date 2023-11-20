#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CharacterASC.h"
#include "Curseel/Curseel.h"
#include "CharacterGameplayAbility.generated.h"

UCLASS()
class CURSEEL_API UCharacterGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UCharacterGameplayAbility();
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	AbilityID AbilityInputID = AbilityID::None;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	AbilityID AbilityID = AbilityID::None;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	bool ActivateAbilityOnGranted = false;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};