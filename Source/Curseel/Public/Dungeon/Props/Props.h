#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WheelPawn.h"
#include "Props.generated.h"

UCLASS()
class CURSEEL_API AProps : public AActor {
	GENERATED_BODY()

public:
	AProps();
private:

	FComponentBeginOverlapSignature OnOverlap;
protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	void HandleOverlap(AActor* OtherActor);
	virtual void OnWheelCharacterAOE_Overlaped(AWheelPawn* Wheel);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USceneComponent> PropsRootComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> Mesh;

	UFUNCTION()
	void OnMeshComponentOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

public:
	virtual void Tick(float DeltaTime) override;

};
