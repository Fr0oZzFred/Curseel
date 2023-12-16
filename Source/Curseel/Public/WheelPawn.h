#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/CapsuleComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "WheelActionsInputConfig.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "WheelPawn.generated.h"

UCLASS()
class CURSEEL_API AWheelPawn : public APawnBase {
	GENERATED_BODY()
public:
	AWheelPawn(const class FObjectInitializer& ObjectInitializer);
protected:
#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> LeftCharacterMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> FrontCharacterMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> RightCharacterMesh;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UFloatingPawnMovement> FloatingMovement;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UCameraComponent* CamComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USceneComponent* WheelCenter;
	

	TArray<TObjectPtr<USkeletalMeshComponent>> Characters;
	APlayerController* PC;
	bool IsUsingGamepad() const;
#pragma endregion
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


#pragma region Characters
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WheelPawn")
	float Angle = 45.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WheelPawn")
	float Radius = 100.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WheelPawn")
	int CurrentCharacterIndex = 1;

	UFUNCTION(BlueprintCallable, Category = "WheelPawn")
	void TurnWheel(bool bCW);
	void UpdateWheelFormation();
	void SetCharPos(TObjectPtr<USkeletalMeshComponent> Char, FVector Pos);
#pragma endregion

#pragma region Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UWheelActionsInputConfig* InputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMappingWheel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMappingSystem;

	//Wheel
	void Move(const FInputActionValue& Value);
	void Orient(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	void Dash(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);

	//System
	void Pause(const FInputActionValue& Value);
#pragma endregion
};
