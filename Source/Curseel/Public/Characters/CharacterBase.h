#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Curseel/Curseel.h"
#include "Characters/Abilities/CharacterASC.h"
#include "Characters/Abilities/CharacterGameplayAbility.h"
#include "Characters/Abilities/AttributeSets/AttributeSetBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "CharacterBase.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, ACharacterBase*, Character);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCharacterStunnedDelegate, ACharacterBase*, Character, bool, Stunned);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCharacterHealthChangedDelegate, ACharacterBase*, Character, float, NewHealth, float, OldHealth);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterMoveSpeedChangedDelegate, ACharacterBase*, Character);

UCLASS()
class CURSEEL_API ACharacterBase : public APawnBase, public IAbilitySystemInterface {
	GENERATED_BODY()

public:
	//ACharacterBase(const class FObjectInitializer& ObjectInitializer);
};
