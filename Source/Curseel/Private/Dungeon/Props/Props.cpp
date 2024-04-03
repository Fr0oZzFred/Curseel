#include "Dungeon/Props/Props.h"

AProps::AProps() {
	PrimaryActorTick.bCanEverTick = true;
	PropsRootComponent = CreateOptionalDefaultSubobject<USceneComponent>(TEXT("Root"));

	MeshComponent = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("PropsMesh"));
	if (MeshComponent) {
		MeshComponent->SetupAttachment(PropsRootComponent, TEXT("None"));
		static FName MeshCollisionProfileName(TEXT("BlockAllDynamic"));
		MeshComponent->SetCollisionProfileName(MeshCollisionProfileName);
		MeshComponent->SetGenerateOverlapEvents(true);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MeshComponent->SetCanEverAffectNavigation(false);
		if (Mesh) MeshComponent->SetStaticMesh(Mesh);
	}


}

void AProps::BeginPlay() {
	Super::BeginPlay();
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AProps::OnMeshComponentOverlap);
}

void AProps::OnConstruction(const FTransform& Transform) {
	if (Mesh) MeshComponent->SetStaticMesh(Mesh);
}

void AProps::HandleOverlap(AActor* OtherActor) {
	if (!OtherActor->ActorHasTag(TEXT("AOE")))					return;
	if (!OtherActor->GetOwner()->ActorHasTag(TEXT("Player")))	return;
	
	AWheelPawn* Wheel = Cast<AWheelPawn, AActor>(OtherActor->GetOwner());
	if (IsValid(Wheel))
		OnWheelCharacterAOE_Overlaped(Wheel);


}

void AProps::OnWheelCharacterAOE_Overlaped(AWheelPawn* Wheel) {
}

void AProps::OnMeshComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	HandleOverlap(OtherActor);
}

void AProps::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

