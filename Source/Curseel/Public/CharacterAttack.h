#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterAttack.generated.h"

UCLASS()
class CURSEEL_API UCharacterAttack : public UDataAsset
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WheelPawn")
	TSubclassOf<class AActor> CharacterAttackAOE;
};
