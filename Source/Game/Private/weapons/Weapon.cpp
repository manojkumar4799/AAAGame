// Fill out your copyright notice in the Description page of Project Settings.


#include "weapons/Weapon.h"
#include "Characeter/GameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
		
}

void AWeapon::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::Equip(USceneComponent* inParent, FName socketname)
{
	AttachMeshToSocket(inParent, socketname);
	itemState = EItemState::EIS_Equipped;
	if (bladeSound) {
		UGameplayStatics::PlaySoundAtLocation(this, bladeSound, GetActorLocation());

	}
	if (sphere) {
		sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::AttachMeshToSocket(USceneComponent* inParent, const FName& socketname)
{
	FAttachmentTransformRules transformRules(EAttachmentRule::SnapToTarget, true);
	staticMesh->AttachToComponent(inParent, transformRules, socketname);
}
