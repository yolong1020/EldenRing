// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/CommonFunctions.h"

const EGameDirection FCommonFunctions::FindDirection(const AActor* Origin, const FVector& Target)
{
	if (nullptr == Origin) return EGameDirection::EGD_None;

	const FVector forward = Origin->GetActorForwardVector();
	const FVector position = Origin->GetActorLocation();

	//	Lower Impact Point to the Enemy's Actor Location.Z
	const FVector target_lowered(Target.X, Target.Y, position.Z);
	const FVector direction = (target_lowered - position).GetSafeNormal();

	//	Forward * ToHit = |Forward||ToHit| * cos(theta)
	//	|Forward| = 1, |ToHit| =1, so Foward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(forward, direction);

	//	Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);

	//	Convert from Radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	//	If CrossProduct points down, Theta should be negative.
	const FVector cross = FVector::CrossProduct(forward, direction);
	if (cross.Z < 0) Theta *= -1.f;

	EGameDirection target_from = EGameDirection::EGD_Back;
	if (Theta >= -45.f && Theta < 45.f)
		target_from = EGameDirection::EGD_Front;
	else if (Theta >= -135.f && Theta < -45.f)
		target_from = EGameDirection::EGD_Left;
	else if (Theta >= 45.f && Theta < 135.f)
		target_from = EGameDirection::EGD_Right;

	return target_from;
}
