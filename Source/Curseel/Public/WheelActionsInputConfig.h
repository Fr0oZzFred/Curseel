#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "WheelActionsInputConfig.generated.h"

/**
 * 
 */
UCLASS()
class CURSEEL_API UWheelActionsInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Wheel")
    UInputAction* InputMove;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Wheel")
    UInputAction* InputOrientation;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Wheel")
    UInputAction* InputAttack;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Wheel")
    UInputAction* InputDash;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Wheel")
    UInputAction* InputInteract;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Wheel")
    UInputAction* InputTurn;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input System")
    UInputAction* InputPause;
};
