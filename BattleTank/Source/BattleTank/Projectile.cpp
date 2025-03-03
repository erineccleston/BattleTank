// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
	SetRootComponent(CollisionMesh);
	CollisionMesh->SetNotifyRigidBodyCollision(true);
	CollisionMesh->SetVisibility(false);

	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
	LaunchBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
	ImpactBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ImpactBlast->bAutoActivate = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
	ProjectileMovement->bAutoActivate = false;

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(FName("Radial Force"));
	RadialForce->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();	
	CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	//LaunchBlast->Deactivate();
	//ImpactBlast->Activate();
	RadialForce->FireImpulse();

	UGameplayStatics::ApplyRadialDamage
	(
		GetWorld(),
		ProjectileDamage,
		GetActorLocation(),
		RadialForce->Radius,
		UDamageType::StaticClass(),
		TArray<AActor*>() //damage all actors in radius		
	);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactBlast->Template, GetActorLocation());
	Destroy();

	//SetRootComponent(ImpactBlast);
	//CollisionMesh->DestroyComponent();

	//FTimerHandle timerHandle;
	//GetWorld()->GetTimerManager().SetTimer(timerHandle, this, somemethod, 0.0f);
}

void AProjectile::LaunchProjectile(float Speed)
{
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	ProjectileMovement->Activate();
}
