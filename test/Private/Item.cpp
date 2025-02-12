#include "Item.h"

DEFINE_LOG_CATEGORY(LogSparta);
float a = 0;
AItem::AItem()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);
	

	// ��� �����ͷ� ó�� �����ϰ� �����Ƿ� ���ʿ�����
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Resources/Props/SM_Chair.SM_Chair"));
	//if (MeshAsset.Succeeded()) {
	//	StaticMeshComp->SetStaticMesh(MeshAsset.Object);
	//}
	//
	//static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/Resources/Materials/M_Metal_Gold.M_Metal_Gold"));
	//if (MaterialAsset.Succeeded()){
	//	StaticMeshComp->SetMaterial(0, MaterialAsset.Object);
	//}
	//UE_LOG(LogSparta, Warning, TEXT("%s Constructor"), *GetName());

	PrimaryActorTick.bCanEverTick = true; // tick�� ����ϹǷ� Ʈ��
	RotationSpeed = 90.0f;
}
/*
void AItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//UE_LOG(LogSparta, Warning, TEXT("%s PostInitializeComponents"), *GetName());
}
*/
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!FMath::IsNearlyZero(RotationSpeed)) // ��� �ڵ�? (0�� �ƴ� ��Ȳ)
	{
		AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
	}
}
/*
void AItem::Destroyed()
{
	//UE_LOG(LogSparta, Warning, TEXT("%s Destroyed"), *GetName());
	Super::Destroyed();
}

void AItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//UE_LOG(LogSparta, Warning, TEXT("%s EndPlay"), *GetName());
	Super::EndPlay(EndPlayReason);
}
*/
void AItem::BeginPlay() {
	Super::BeginPlay();
	//UE_LOG(LogSparta, Warning, TEXT("%s BeginPlay"), *GetName());
	//UE_LOG(LogSparta, Warning, TEXT("aaaaaaaaaa"));
	//UE_LOG(LogSparta, Display, TEXT("aaaaaaaaaa"));
	//UE_LOG(LogSparta, Error, TEXT("aaaaaaaaaa"));

	/*
	SetActorLocation(FVector(300.0f, 200.0f, 100.0f));
	SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
	SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));
	//SetActorScale3D(FVector(2.0f)); // ���� ���� 
	*/

	/*
	// �̷������� �ѹ��� ���� ����
	FVector NewLocation(300.f, 200.0f, 100.0f);
	FRotator NewRotation(0.0f, 90.0f, 0.0f);
	FVector NewScale(2.0f);
	FTransform NewTransform(NewRotation, NewLocation, NewScale); 
	SetActorTransform(NewTransform);
	*/

	OnItemPickUP();
}

void AItem::ResetActorPosition()
{
	SetActorLocation(FVector::ZeroVector);
}

float AItem::GetRotationSpeed() const
{
	return RotationSpeed;
}

