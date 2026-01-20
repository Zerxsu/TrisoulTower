// Fill out your copyright notice in the Description page of Project Settings.


#include "AIGroupManager.h"

// Sets default values for this component's properties
UAIGroupManager::UAIGroupManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAIGroupManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Target")), FoundActors);
	if (FoundActors.Num() > 0) PlayerActor = FoundActors[0];

	for (int i = 0; i < 6; i++) {
		//UE_LOG(LogTemp, Warning, TEXT("Point %d: %s"), i, *GetHexPos(i, 1).ToString());
		AddPoint(GetHexPos(i), 0);
	}
	
	TArray<FVector2D> Keys;
	Points.GetKeys(Keys);

	//Print Points
	//for (int i = 0; i < Points.Num(); i++) {
	//	UE_LOG(LogTemp, Warning, TEXT("Point %d: %s"), i, *Keys[i].ToString());
	//}

	//AddPoint(FVector2D(0, 0), 1);

	/*Temporary for debug
	TArray<AActor*> FoundEnemy;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), FoundEnemy);
	if (FoundActors.Num() > 0) {
		ABaseEnemy* Found = Cast<ABaseEnemy>(FoundActors[0]);
		if (Found) AssignPoint(Found);
	}
	*/

}


// Called every frame
void UAIGroupManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TArray<AActor*> AllEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), AllEnemies);
	

	TArray<FVector2D> Keys;
	Points.GetKeys(Keys);

	
	for (int i = 0; i < Points.Num(); i++) {
		
		if (Points[Keys[i]].AIGuy != nullptr) {
			if (!IsValid(Points[Keys[i]].AIGuy)) Points[Keys[i]].AIGuy = nullptr;//Unassign Invalid Enemies From Points
			else if (Points[Keys[i]].AIGuy->needPoint) TradePoint(Keys[i]);//Reassign Enemy points
		}

	}

	//Assign Unassigned Enemies
	for (AActor* enemyActor : AllEnemies) {
		//UE_LOG(LogTemp, Warning, TEXT("Enemy: %s"), *enemyActor->GetName());
		ABaseEnemy* enemy = Cast<ABaseEnemy>(enemyActor);
		
		if (enemy->needPoint) {
			AssignPoint(enemy);
		}
	}

	//When points are unassigned, a nearby enemy should be assigned to fill the space

	Points.GetKeys(Keys);

	//Update point positions for enemies
	for (int i = 0; i < Points.Num(); i++) {

		FVector aer = PlayerActor->GetActorLocation() + FVector(Keys[i].X, Keys[i].Y, Points[Keys[i]].priority * 32.0f);

		FColor maroo = FColor::Green;
		if (!Points[Keys[i]].isValid) maroo = FColor::Red;
		else if (Points[Keys[i]].priority >= 2) maroo = FColor::Blue;

		DrawDebugSphere(
			GetWorld(),
			aer,
			24.0f,
			12,
			maroo,
			false,
			0,
			0,
			2.0f
		);
		
		FNavPoint* check_point = &Points[Keys[i]];

		if (check_point->isValid) {
			if (check_point->AIGuy != nullptr) check_point->AIGuy->AssignPoint(Keys[i], check_point->priority);
		}

	}
	
}


void UAIGroupManager::AddPoint(FVector2D at, int prio)
{

	TArray<FVector2D> Keys;
	Points.GetKeys(Keys);

	if (at.Length() < Separation) return;

	//This part is a mess
	for (int i = 0; i < Keys.Num(); i++) {
		if ((at - Keys[i]).Length() < Separation) {
			int NewPrio = Points[Keys[i]].priority;
			if (NewPrio > prio + 1) {
				Points[Keys[i]].priority = prio + 1;
			}
			return;
		}
	}
		
	if (Keys.Contains(at)) {
		int NewPrio = Points[at].priority;
		if (NewPrio <= prio) {
			NewPrio = prio + 1;
			Points[at].priority = prio + 1;
		}

	} else {
		FNavPoint NewPoint;
		NewPoint.priority = prio + 1;
		NewPoint.isValid = true;
		NewPoint.AIGuy = nullptr;
		Points.Emplace(at, NewPoint);
	}

}

FNavPoint* UAIGroupManager::GetPoint(FVector2D at)
{
	return Points.Find(at);
}

void UAIGroupManager::AssignPoint(ABaseEnemy* ai)
{

	FVector AIpos = ai->GetActorLocation();
	FVector2D pos = FVector2D(AIpos.X, AIpos.Y);
	FVector targetPos = PlayerActor->GetActorLocation();
	FVector2D tpos = FVector2D(targetPos.X, targetPos.Y);

	TArray<FVector2D> Keys;
	Points.GetKeys(Keys);

	FNavPoint* best = nullptr;
	float bestWeight = -1.0f;
	FVector2D bestPos = pos;

	for (int i = 0; i < Points.Num(); i++) {//Look through every point

		if (Points[Keys[i]].isValid && Points[Keys[i]].AIGuy == nullptr) {

			float pointWeight = 0.0f;

			//Calculate weight of each point relative to enemy
			pointWeight = (float)Points[Keys[i]].priority * 30.0f;
			pointWeight *= FMath::Sqrt(((tpos + Keys[i]) - pos).Size());
			pointWeight *= FVector2D::DotProduct((tpos + Keys[i] - pos).GetSafeNormal(), Keys[i].GetSafeNormal());
			
			//Find the best point
			if (pointWeight >= 0 && (bestWeight == -1 || pointWeight < bestWeight)) {
				bestWeight = pointWeight;
				best = &Points[Keys[i]];
				bestPos = Keys[i];
			}
			
			UE_LOG(LogTemp, Warning, TEXT("Point %d: %s - %f"), i, *Keys[i].ToString(), pointWeight);

		}

	}

	UE_LOG(LogTemp, Warning, TEXT("Best: %s - %f"), *bestPos.ToString(), bestWeight);

	//Make new points surrounding assigned point
	if (bestWeight != -1) {
		for (int i = 0; i < 6; i++) {
			UE_LOG(LogTemp, Warning, TEXT("New Point %d: %s"), i, *GetHexPos(i, best->priority).ToString());
			AddPoint(bestPos + GetHexPos(i), best->priority);
		}
		
		ai->AssignPoint(bestPos, best->priority);
		Points[bestPos].AIGuy = ai;
	}
	

}

void UAIGroupManager::TradePoint(FVector2D at) {

	int prio = Points[at].priority;

	for (int i = 0; i < 6; i++) {
		FVector2D checkPos = at + GetHexPos(i);

		if (Points.Contains(checkPos)) {

			FNavPoint* checkPoint = &Points[checkPos];

			if (checkPoint->isValid && checkPoint->priority > prio) {
				if (checkPoint->AIGuy != nullptr) {
					
					ABaseEnemy* switchGuy = Points[at].AIGuy;

					checkPoint->AIGuy->AssignPoint(at, prio);
					Points[at].AIGuy = checkPoint->AIGuy;
					
					switchGuy->AssignPoint(checkPos, checkPoint->priority);
					checkPoint->AIGuy = switchGuy;
					break;
				}
			}
		}
	}

}

FVector2D UAIGroupManager::GetHexPos(int point, int dist)
{
	 
	return FVector2D(0, Separation * dist).GetRotated(point * 60);
}