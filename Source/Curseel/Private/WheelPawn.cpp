#include "WheelPawn.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>

AWheelPawn::AWheelPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	//Capsule
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->InitCapsuleSize(140.0f, 280.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	CapsuleComponent->SetGenerateOverlapEvents(true);

	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
	CapsuleComponent->SetCanEverAffectNavigation(false);
	CapsuleComponent->bDynamicObstacle = true;
	RootComponent = CapsuleComponent;

	//Cam
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(CapsuleComponent);

	SpringArmComp->TargetArmLength = 800.0f;
	SpringArmComp->SocketOffset = FVector(0.0f, 0.0f, 1000.0f);
	SpringArmComp->TargetOffset = FVector(100.0f, 0.0f, 0.0f);

	SpringArmComp->bDoCollisionTest = false;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bEnableCameraLag = true;

	CamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
	CamComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules::KeepRelativeTransform);
	CamComp->SetRelativeRotation(FRotator(0.0f, -55.0f, 0.0f));

	//Roots
	WheelCenter = CreateDefaultSubobject<USceneComponent>(TEXT("WheelCenter"));
	WheelCenter->SetupAttachment(CapsuleComponent);

	LeftCharacterRoot = CreateOptionalDefaultSubobject<USceneComponent>(TEXT("LeftCharacterRoot"));
	FrontCharacterRoot = CreateOptionalDefaultSubobject<USceneComponent>(TEXT("FrontCharacterRoot"));
	RightCharacterRoot = CreateOptionalDefaultSubobject<USceneComponent>(TEXT("RightCharacterRoot"));
	CharactersRoot.Add(LeftCharacterRoot);
	CharactersRoot.Add(FrontCharacterRoot);
	CharactersRoot.Add(RightCharacterRoot);

	for (auto& r : CharactersRoot) {
		r->SetupAttachment(WheelCenter);
	}

	//Characters
	LeftCharacterMesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftCharacterMesh"));
	if (LeftCharacterMesh) {
		LeftCharacterMesh->AlwaysLoadOnClient = true;
		LeftCharacterMesh->AlwaysLoadOnServer = true;
		LeftCharacterMesh->bOwnerNoSee = false;
		LeftCharacterMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
		LeftCharacterMesh->bCastDynamicShadow = true;
		LeftCharacterMesh->bAffectDynamicIndirectLighting = true;
		LeftCharacterMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		LeftCharacterMesh->SetupAttachment(CapsuleComponent);
		static FName MeshCollisionProfileName(TEXT("CharacterMesh"));
		LeftCharacterMesh->SetCollisionProfileName(MeshCollisionProfileName);
		LeftCharacterMesh->SetGenerateOverlapEvents(false);
		LeftCharacterMesh->SetCanEverAffectNavigation(false);
		Characters.Add(LeftCharacterMesh);
	}

	FrontCharacterMesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("FrontCharacterMesh"));
	if (FrontCharacterMesh) {
		FrontCharacterMesh->AlwaysLoadOnClient = true;
		FrontCharacterMesh->AlwaysLoadOnServer = true;
		FrontCharacterMesh->bOwnerNoSee = false;
		FrontCharacterMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
		FrontCharacterMesh->bCastDynamicShadow = true;
		FrontCharacterMesh->bAffectDynamicIndirectLighting = true;
		FrontCharacterMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		FrontCharacterMesh->SetupAttachment(CapsuleComponent);
		static FName MeshCollisionProfileName(TEXT("CharacterMesh"));
		FrontCharacterMesh->SetCollisionProfileName(MeshCollisionProfileName);
		FrontCharacterMesh->SetGenerateOverlapEvents(false);
		FrontCharacterMesh->SetCanEverAffectNavigation(false);
		Characters.Add(FrontCharacterMesh);
	}

	RightCharacterMesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("RightCharacterMesh"));
	if (RightCharacterMesh) {
		RightCharacterMesh->AlwaysLoadOnClient = true;
		RightCharacterMesh->AlwaysLoadOnServer = true;
		RightCharacterMesh->bOwnerNoSee = false;
		RightCharacterMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
		RightCharacterMesh->bCastDynamicShadow = true;
		RightCharacterMesh->bAffectDynamicIndirectLighting = true;
		RightCharacterMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		RightCharacterMesh->SetupAttachment(CapsuleComponent);
		static FName MeshCollisionProfileName(TEXT("CharacterMesh"));
		RightCharacterMesh->SetCollisionProfileName(MeshCollisionProfileName);
		RightCharacterMesh->SetGenerateOverlapEvents(false);
		RightCharacterMesh->SetCanEverAffectNavigation(false);
		Characters.Add(RightCharacterMesh);
	}

	int i = 0;
	for (auto& c : Characters) {
		c->SetupAttachment(CharactersRoot[i++]);
		c->AddRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}

	FloatingMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingMovement"));
	if (FloatingMovement) {
		FloatingMovement->UpdatedComponent = CapsuleComponent;
	}
}

bool AWheelPawn::IsUsingGamepad() const {
	if (!PC) return false;
	
	if (UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(PC->GetLocalPlayer())) {
		return CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad;
	}
	return false;
}

void AWheelPawn::BeginPlay() {
	Abilities.Add(AttackAbility);
	Abilities.Add(TurnLeftAbility);
	Abilities.Add(TurnRightAbility);
	Abilities.Add(DashAbility);
	Super::BeginPlay();
	FloatingMovement->MaxSpeed = GetMoveSpeed();
}

void AWheelPawn::OnConstruction(const FTransform& Transform) {
	UpdateWheelFormation();
}

void AWheelPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	PC = Cast<APlayerController>(GetController());
	
	if (!PC) return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMappingWheel, 0);
	Subsystem->AddMappingContext(InputMappingSystem, 10);

	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	PEI->BindAction(InputActions->InputMove, ETriggerEvent::Triggered, this, &AWheelPawn::Move);
	PEI->BindAction(InputActions->InputOrientation, ETriggerEvent::Triggered, this, &AWheelPawn::Orient);
	PEI->BindAction(InputActions->InputAttack, ETriggerEvent::Triggered, this, &AWheelPawn::Attack);
	PEI->BindAction(InputActions->InputDash, ETriggerEvent::Triggered, this, &AWheelPawn::Dash);
	PEI->BindAction(InputActions->InputInteract, ETriggerEvent::Triggered, this, &AWheelPawn::Interact);
	PEI->BindAction(InputActions->InputTurn, ETriggerEvent::Triggered, this, &AWheelPawn::Turn);
	PEI->BindAction(InputActions->InputPause, ETriggerEvent::Triggered, this, &AWheelPawn::Pause);
}

void AWheelPawn::TurnWheel(bool bCW) {
	CurrentCharacterIndex += bCW ? -1 : -2;
	CurrentCharacterIndex += 3;
	CurrentCharacterIndex %= 3;

	UpdateWheelFormation();
}

void AWheelPawn::UpdateWheelFormation() {
	//0 will be left
	//1 will be front
	//2 will be right
	TObjectPtr<USceneComponent> UpdatedCharacters[] =
	{ LeftCharacterRoot, FrontCharacterRoot, RightCharacterRoot };

	//CurrentIndex will become Front Char
	switch (CurrentCharacterIndex) {
		case 0:
			UpdatedCharacters[0] = RightCharacterRoot;
			UpdatedCharacters[1] = LeftCharacterRoot;
			UpdatedCharacters[2] = FrontCharacterRoot;
		break;
		case 1:
			UpdatedCharacters[0] = LeftCharacterRoot;
			UpdatedCharacters[1] = FrontCharacterRoot;
			UpdatedCharacters[2] = RightCharacterRoot;
		break;
		case 2:
			UpdatedCharacters[0] = FrontCharacterRoot;
			UpdatedCharacters[1] = RightCharacterRoot;
			UpdatedCharacters[2] = LeftCharacterRoot;
		break;
		default:
		break;
	}

	//Front Char
	SetRootPos(UpdatedCharacters[1], FVector(Radius, 0.0f, 0.0f));

	//Sides Char
	FVector NewPos = FVector(
		UKismetMathLibrary::DegCos(Angle) * Radius,
		UKismetMathLibrary::DegSin(Angle) * Radius,
		0.0f);

	//Right Char
	SetRootPos(UpdatedCharacters[2], NewPos);

	//Left Char
	NewPos.Y = -NewPos.Y;
	SetRootPos(UpdatedCharacters[0], NewPos);
}

void AWheelPawn::SetRootPos(TObjectPtr<USceneComponent> Root, FVector Pos) {
	FRotator NewRot = UKismetMathLibrary::FindRelativeLookAtRotation(
		FTransform::Identity, Pos);

	Root->SetRelativeLocationAndRotation(Pos, NewRot);
}
#pragma region Inputs
void AWheelPawn::Move(const FInputActionValue& Value) {
	FVector2D v = Value.Get<FVector2D>();
	FloatingMovement->AddInputVector(UKismetMathLibrary::GetRightVector(GetControlRotation()) * v.X);
	FloatingMovement->AddInputVector(UKismetMathLibrary::GetForwardVector(GetControlRotation()) * v.Y);
}

void AWheelPawn::Orient(const FInputActionValue& Value) {
	FVector2D v = Value.Get <FVector2D>();

	if (IsUsingGamepad()) {
		AimDirection = FVector(v.Y, -v.X, 0.0f);
		WheelCenter->SetRelativeRotation(FRotator(0.0, -UKismetMathLibrary::DegAtan2(v.X, v.Y), 0.0));
		return;
	}
	//Mouse
	FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(PC);
	FVector2D WheelScreenPos;

	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		PC,WheelCenter->GetComponentLocation(), WheelScreenPos, false);
	WheelScreenPos -= MousePos;
	WheelScreenPos.Normalize();
	AimDirection = FVector(WheelScreenPos.Y, -WheelScreenPos.X, 0.0f);

	WheelCenter->SetRelativeRotation(FRotator(
		0.0, -UKismetMathLibrary::DegAtan2(WheelScreenPos.X, WheelScreenPos.Y), 0.0));
}

void AWheelPawn::Attack(const FInputActionValue& Value) {
	AbilitySystemComponent->TryActivateAbilityByClass(AttackAbility);
}

void AWheelPawn::Dash(const FInputActionValue& Value) {
	AbilitySystemComponent->TryActivateAbilityByClass(DashAbility);
}

void AWheelPawn::Interact(const FInputActionValue& Value) {
	OnInteract.Broadcast();
}

void AWheelPawn::Turn(const FInputActionValue& Value) {
	if (Value.Get<float>() > 0.0f)
		AbilitySystemComponent->TryActivateAbilityByClass(TurnRightAbility);
	else
		AbilitySystemComponent->TryActivateAbilityByClass(TurnLeftAbility);
}


//System
void AWheelPawn::Pause(const FInputActionValue& Value) {
	OnPauseAsked.Broadcast();
}
void AWheelPawn::MoveSpeedChanged(const FOnAttributeChangeData& Data) {
	Super::MoveSpeedChanged(Data);
	FloatingMovement->MaxSpeed = Data.NewValue;
}
#pragma endregion