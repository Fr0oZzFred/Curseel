#include "Dungeon/Props/DestructibleProps.h"

ADestructibleProps::ADestructibleProps() : Super() {
	if (MeshComponent) {
		static FName MeshCollisionProfileName(TEXT("PhysicsActor"));
		MeshComponent->SetCollisionProfileName(MeshCollisionProfileName);
		MeshComponent->SetSimulatePhysics(true);
	}

	GeoCollectionComponent = CreateOptionalDefaultSubobject<UGeometryCollectionComponent>(TEXT("PropsDestruction"));
	if (GeoCollectionComponent) {
		GeoCollectionComponent->SetupAttachment(MeshComponent);
		GeoCollectionComponent->SetSimulatePhysics(false);
		GeoCollectionComponent->bHiddenInGame = true;
		static FName GeoCollectionProfileName(TEXT("Destructible"));
		GeoCollectionComponent->SetCollisionProfileName(GeoCollectionProfileName);
		GeoCollectionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	InitDestructibleProps();
}

void ADestructibleProps::OnConstruction(const FTransform& Transform) {
	InitDestructibleProps();
}

void ADestructibleProps::OnWheelCharacterAOE_Overlaped(AWheelPawn* Wheel) {
	Super::OnWheelCharacterAOE_Overlaped(Wheel);

	if (!IsValid(PropsAsset)) return;

	//Hide Static Mesh
	MeshComponent->SetSimulatePhysics(false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetVisibility(false);

	//Show Geo Collection
	GeoCollectionComponent->SetHiddenInGame(false);
	GeoCollectionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GeoCollectionComponent->SetSimulatePhysics(true);


	GetWorld()->SpawnActor<AFieldSystemActor>(
		PropsAsset->FieldSystem,
		MeshComponent->GetComponentLocation(),
		FRotator(0.0f,0.0f,0.0f),
		FActorSpawnParameters()
	);


	UGameplayStatics::PlaySound2D(GetWorld(), PropsAsset->DestroySound, 0.5f);
}

void ADestructibleProps::InitDestructibleProps() {
	if (!IsValid(PropsAsset)) return;
	Mesh = PropsAsset->DestructibleMesh;
	MeshComponent->SetStaticMesh(Mesh);
	GeoCollectionComponent->SetRestCollection(PropsAsset->RestCollection);
}
