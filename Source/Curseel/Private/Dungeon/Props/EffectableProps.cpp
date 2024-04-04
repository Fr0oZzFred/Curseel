#include "Dungeon/Props/EffectableProps.h"

AEffectableProps::AEffectableProps() : Super() {
	FluidMeshComponent = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("FluidMesh"));
	if (FluidMeshComponent) {
		FluidMeshComponent->SetupAttachment(MeshComponent, TEXT("None"));
		static FName MeshCollisionProfileName(TEXT("NoCollision"));
		FluidMeshComponent->SetCollisionProfileName(MeshCollisionProfileName);
		FluidMeshComponent->SetSimulatePhysics(false);
		FluidMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FluidMeshComponent->SetCanEverAffectNavigation(false);
		FluidMeshComponent->SetGenerateOverlapEvents(false);
	}

	VFX_OnHit = CreateOptionalDefaultSubobject<UNiagaraComponent>(TEXT("VFX_OnHit"));
	if (VFX_OnHit) {
		VFX_OnHit->SetAutoActivate(false);
		VFX_OnHit->SetupAttachment(MeshComponent, TEXT("None"));
		VFX_OnHit->SetBoundsScale(10.0f);
	}

	InitEffectableProps();
}

void AEffectableProps::BeginPlay() {
	Super::BeginPlay();
	InitEffectableProps();
	MeshComponent->OnComponentHit.AddDynamic(this, &AEffectableProps::OnHit);
}

void AEffectableProps::OnWheelCharacterAOE_Overlaped(AWheelPawn* Wheel) {
	FluidMeshComponent->SetVisibility(false);
	Super::OnWheelCharacterAOE_Overlaped(Wheel);


	//Apply Effect On Wheel
	UAbilitySystemComponent* ASC = Wheel->GetAbilitySystemComponent();
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(ASC->GetAvatarActor());

	FGameplayEffectSpecHandle NewHandle = ASC->MakeOutgoingSpec(
		EffectablePropsAsset->Effect, 0.0f, EffectContext);

	if (NewHandle.IsValid()) {
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
			NewHandle, EffectablePropsAsset->EffectTag, EffectablePropsAsset->EffectMagnitude);
		FActiveGameplayEffectHandle ActiveGEHandle =
			ASC->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
	}

	//Play Steal

	UGameplayStatics::PlaySound2D(GetWorld(), EffectablePropsAsset->StealSound, 0.5f);

	auto* StealFX = UNiagaraFunctionLibrary::SpawnSystemAttached(EffectablePropsAsset->StealVFX, Wheel->GetRootComponent(),
		FName("None"), FVector::Zero(), FRotator::ZeroRotator,EAttachLocation::KeepRelativeOffset, true);
	StealFX->SetVariablePosition(FName("TargetPos"), this->GetActorLocation());
	StealFX->SetVariableLinearColor(FName("Color"), EffectablePropsAsset->StealColor);

	//OnDestroyVFX
	StealFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
		EffectablePropsAsset->OnDestroyFX, MeshComponent->GetComponentLocation());

	StealFX->SetVariableLinearColor(FName("Color"), EffectablePropsAsset->DestroyColor);
}

void AEffectableProps::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	InitEffectableProps();
}

void AEffectableProps::InitEffectableProps() {

	if (!IsValid(EffectablePropsAsset)) return;


	VFX_OnHit->SetAsset(EffectablePropsAsset->OnHitFX);
	VFX_OnHit->SetVariableLinearColor(FName("Color"), EffectablePropsAsset->HitColor);
	FluidMeshComponent->SetStaticMesh(EffectablePropsAsset->FluidMesh);
	FluidMeshComponent->SetMaterial(0, EffectablePropsAsset->FluidMeshMat);
}

void AEffectableProps::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	VFX_OnHit->Activate(true);
	if (GetWorldTimerManager().GetTimerRemaining(TimerHandle) != -1) return; 

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AEffectableProps::CheckVelocity, 0.5f, true);
}

void AEffectableProps::CheckVelocity() {
	LinearVelocity = MeshComponent->GetPhysicsLinearVelocity(FName("None"));
	if (LinearVelocity.IsNearlyZero(20.0f)) {
		VFX_OnHit->Deactivate();
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}
}
