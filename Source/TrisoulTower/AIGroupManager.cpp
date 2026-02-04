// Fill out your copyright notice in the Description page of Project Settings.


#include "AIGroupManager.h"

//Debug variables to track duration of tick function
TArray<float> tickTimes = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
int tickIdx = 0;

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
		UE_LOG(LogTemp, Warning, TEXT("Point %d: %s"), i, *GetHexPos(i).ToString());
		AddPoint(GetHexPos(i), 0);
	}
	
	TArray<FVector2D> Keys;
	Points.GetKeys(Keys);

	//Print Points
	//for (int i = 0; i < Points.Num(); i++) {
	//	UE_LOG(LogTemp, Warning, TEXT("Point %d: %s"), i, *Keys[i].ToString());
	//}


}


// Called every frame
void UAIGroupManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	uint64_t StartTime = FPlatformTime::Cycles64();//Start Time, for debugging

	//Enemies aren't being unassigned from points properly
	//Enemies are still targeting invalid points

	TArray<AActor*> AllEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), AllEnemies);
	
	TArray<FVector2D> Keys;
	Points.GetKeys(Keys);

	int checkLevel = 0;
	int ringLevel = 1;
	int inRing = 1;
	int ringMax = 2;

	while (inRing > 0) {//For each point ring level
		inRing = 0;
		checkLevel++;

		for (int i = 0; i < Points.Num(); i++) {//count assigned enemies
			FNavPoint* checkPoint = &Points[Keys[i]];
		
			if (checkPoint->priority == checkLevel && checkPoint->AIGuy != nullptr) {

				if (!checkPoint->isValid) checkPoint->AIGuy->needPoint = true;

				if (checkPoint->AIGuy->needPoint) {
					checkPoint->AIGuy = nullptr;
				} else if (AllEnemies.Contains(checkPoint->AIGuy)) {
					if (checkLevel > ringLevel) {//If the last ring wasn't full, this guy needs a new spot
						checkPoint->AIGuy->needPoint = true;
						checkPoint->AIGuy = nullptr;
					}
					else {//If the guy doesn't need a new point and can't advance to a closer ring, remove from enemy list
						AllEnemies[AllEnemies.Find(checkPoint->AIGuy)] = nullptr;
						inRing++;
					}
				}
			}
		}

		FString DebugMessage = FString::Printf(TEXT("Ring %d: %d"), checkLevel, inRing);
    	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Cyan, DebugMessage);

		if (inRing >= ringMax) {
			ringLevel++;
			ringMax *= 2;
		}

	}

	//There are now the enemies at assigned points and unassigned enemies 
	
	TArray<AActor*> LostEnemies;//An array of unassigned enemies, to be sorted by distance to the player

	FVector PlayerPos = PlayerActor->GetActorLocation();

	for (int i = 0; i < AllEnemies.Num(); i++) {
		if (AllEnemies[i] != nullptr) {

			float playerDist = FVector::Dist(AllEnemies[i]->GetActorLocation(), PlayerPos);

			if (LostEnemies.Num() == 0) LostEnemies.Add(AllEnemies[i]);
			else if (FVector::Dist(LostEnemies.Last()->GetActorLocation(), PlayerPos) < playerDist) LostEnemies.Add(AllEnemies[i]);
			else {
				for (int j = 0; j < LostEnemies.Num(); j++) {
					if (FVector::Dist(LostEnemies[j]->GetActorLocation(), PlayerPos) > playerDist) {
						LostEnemies.Insert(AllEnemies[i], j);
						break;
					}
				}
			}

		}
	}

	FString DebugMessagee = FString::Printf(TEXT("Unassigned: %d"), LostEnemies.Num());
    GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Cyan, DebugMessagee);

	//Assign all the unassigned guys
	for (AActor* enemyActor : LostEnemies) {
		//UE_LOG(LogTemp, Warning, TEXT("Enemy: %s"), *enemyActor->GetName());
		//ABaseEnemy* enemy = Cast<ABaseEnemy>(enemyActor);
		AssignPoint(Cast<ABaseEnemy>(enemyActor), ringLevel);

		inRing++;

		if (inRing >= ringMax) {
			ringLevel++;
			inRing = 0;
			ringMax *= 2;
		}
	}

	Points.GetKeys(Keys);

	//Update point positions for enemies
	for (int i = 0; i < Points.Num(); i++) {

		//Debugging Visuals
		FVector aer = PlayerActor->GetActorLocation() + FVector(Keys[i].X, Keys[i].Y, Points[Keys[i]].priority * 32.0f);
		FColor maroo = FColor::Green;
		if (!Points[Keys[i]].isValid) maroo = FColor::Red;
		else if (Points[Keys[i]].priority >= 2) {
			maroo = FColor::Blue;
			//maroo.B = 1.0f - (0.1f * ((float)Points[Keys[i]].priority));
			//maroo.B = ((Points[Keys[i]].priority - 1) / FMath::Clamp(checkLevel - 1, 1, checkLevel + 1));
		}

		DrawDebugSphere(
			GetWorld(),
			aer,
			24.0f,
			12,
			maroo,
			false,
			0.0,
			0,
			2.0f
		);

		//Update the target location of each enemy relative to the player
		FNavPoint* check_point = &Points[Keys[i]];
		if (check_point->isValid && check_point->AIGuy != nullptr) {
			if (check_point->priority > 1) check_point->AIGuy->AssignPoint(Keys[i], check_point->priority);
			else check_point->AIGuy->AssignPoint(Keys[i].GetSafeNormal() * StartDist, check_point->priority);
		}

		if (check_point->priority > checkLevel + 1) Points.Remove(Keys[i]);
		//Somehow a bunch of new ring keep getting made and also all the guys rearagne themselves out of the first ring
		//Likley that guys are being removed from points but the reference to them is not removed so all the points think they have guys
		//Fix the bug where they all leave the first ring and then optimize
		//One of the issues could be that the enemy target dist is too small, but others should still try and fill the space
		//The script needs to move enemies down if a level isn't full
		//The problem is probably that the ringLevel isn't programed as intended, and empty rings are counted as full 
		//Update: The program says the rings are sorted as intented, but the enemies don't go to the right points, sometimes they are stick unassigned
	}
	
	uint64_t EndTime = FPlatformTime::Cycles64();//End Time
	float DurationNS = FPlatformTime::ToSeconds64(EndTime - StartTime);//Tick Duration

	tickTimes[tickIdx] = DurationNS;
	tickIdx++;
	if (tickIdx >= tickTimes.Num()) tickIdx = 0;

	float tickAvg = 0.0;
	for (int i = 0; i < tickTimes.Num(); i++) {
		tickAvg += tickTimes[i];
	}
	tickAvg /= tickTimes.Num();

	FString DebugMessage = FString::Printf(TEXT("Tick Time: %f"), tickAvg);
    GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow, DebugMessage);
}


void UAIGroupManager::AddPoint(FVector2D at, int prio)
{

	TArray<FVector2D> Keys;
	Points.GetKeys(Keys);

	//if (at.Length() < StartDist - 1) return;

	//This part is a mess
	for (int i = 0; i < Keys.Num(); i++) {
		if (FVector2D::Distance(at, Keys[i]) < 8.0f) {//((at - Keys[i]).Length() < StartDist - 1) {
			int NewPrio = Points[Keys[i]].priority;
			if (NewPrio > prio + 1) {
				Points[Keys[i]].priority = prio + 1;
			}
			UE_LOG(LogTemp, Warning, TEXT("Bad Point %f: %s"), (at - Keys[i]).Length(), *at.ToString());
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

void UAIGroupManager::AssignPoint(ABaseEnemy* ai, int minRing)
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

		FNavPoint* check = &Points[Keys[i]];

		if (check->isValid && check->AIGuy == nullptr && check->priority >= minRing) {

			float pointWeight = 0.0f;

			//Calculate weight of each point relative to enemy
			pointWeight = (float)check->priority * 12.0f;
			pointWeight *= FMath::Sqrt(((tpos + Keys[i]) - pos).Size());
			//pointWeight *= FVector2D::DotProduct((tpos + Keys[i] - pos).GetSafeNormal(), Keys[i].GetSafeNormal());
			
			//Find the best point
			if (pointWeight >= 0 && (bestWeight == -1 || pointWeight < bestWeight)) {
				bestWeight = pointWeight;
				best = check;
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

	//Right now before check point, the assigned AIGuy is made null
	//This is because otherwise, every point ends up with the same AIGuy
	//This causes errors when trying to trade points
	//The system needs to:
	//		reassign ai points after they attack
	//		allow trading points with nothing
	//		make sure guys from further rings move in
	//		maybe

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