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
#include "CharacterAttack.h"
#include "WheelPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPauseAskedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractDelegate);

UCLASS()
class CURSEEL_API AWheelPawn : public APawnBase {
	GENERATED_BODY()
public:
	AWheelPawn(const class FObjectInitializer& ObjectInitializer);
protected:
#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CapsuleComponent;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UCameraComponent* CamComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USceneComponent> WheelCenter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> LeftCharacterRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> FrontCharacterRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> RightCharacterRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> LeftCharacterMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> FrontCharacterMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> RightCharacterMesh;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UFloatingPawnMovement> FloatingMovement;
	

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<USkeletalMeshComponent>> Characters;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<USceneComponent>> CharactersRoot;

	APlayerController* PC;
	bool IsUsingGamepad() const;
#pragma endregion
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


#pragma region ASC
	//Added in Abilities in Begin Play
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Abilities")
	TSubclassOf<class UCharacterGameplayAbility> AttackAbility; 
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Abilities")
	TSubclassOf<class UCharacterGameplayAbility> TurnLeftAbility;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Abilities")
	TSubclassOf<class UCharacterGameplayAbility> TurnRightAbility;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Abilities")
	TSubclassOf<class UCharacterGameplayAbility> DashAbility;
#pragma endregion


#pragma region Characters
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WheelPawn")
	float Angle = 45.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WheelPawn")
	float Radius = 100.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WheelPawn")
	int CurrentCharacterIndex = 1;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WheelPawn")
	TArray<class UCharacterAttack*> CharactersAttack;

	UFUNCTION(BlueprintCallable, Category = "WheelPawn")
	void TurnWheel(bool bCW);
	void UpdateWheelFormation();
	void SetRootPos(TObjectPtr<USceneComponent> Root, FVector Pos);
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


#pragma region Delegates
	virtual void MoveSpeedChanged(const FOnAttributeChangeData& Data);


	UPROPERTY(BlueprintAssignable, Category = "Wheel|Actions")
	FPauseAskedDelegate OnPauseAsked;
	UPROPERTY(BlueprintAssignable, Category = "Wheel|Actions")
	FInteractDelegate OnInteract;

#pragma endregion
};
