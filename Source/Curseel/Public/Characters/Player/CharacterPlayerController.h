#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CharacterPlayerState.h"
#include "CharacterPlayerController.generated.h"

UCLASS()
class CURSEEL_API ACharacterPlayerController : public APlayerController {
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;
};