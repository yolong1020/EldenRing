#pragma once
#include "DrawDebugHelpers.h"

#define DRAW_SPHERE(Location, Radius)				if (GetWorld()) DrawDebugSphere(GetWorld(), Location, Radius, 12, FColor::Red, true);
#define DRAW_SPHERE_DURATION(Location)				if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 8.f, 12, FColor::Orange, true, 5.f);
#define DRAW_SPHERE_SINGLE_FRAME(Location)			if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, false, -1.f);
#define DRAW_LINE(Start, End)						if (GetWorld()) DrawDebugLine(GetWorld(), Start, End, FColor::White, true, -1.f, 0, 1.f);
#define DRAW_LINE_SINGLE_FRAME(Start, End)			if (GetWorld()) DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, -1.f, 0, 1.f);
#define DRAW_POINT(Location)						if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 30.f, FColor::Green, true, 40.f);
#define DRAW_POINT_SINGLE_FRAME(Location)			if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 30.f, FColor::Green, false, 40.f);

#define DRAW_VECTOR(Start, End) if (GetWorld())											\
		{																				\
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, -1.f, 0, 1.f);		\
			DrawDebugPoint(GetWorld(), Start, 30.f, FColor::Green, true, 40.f);			\
			DrawDebugPoint(GetWorld(), End, 30.f, FColor::Green, true, 40.f);			\
		}	

#define DRAW_VECTOR_SINGLE_FRAME(Start, End) if (GetWorld())							\
		{																				\
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, -1.f, 0, 1.f);	\
			DrawDebugPoint(GetWorld(), Start, 30.f, FColor::Green, false, -1.f);		\
			DrawDebugPoint(GetWorld(), End, 30.f, FColor::Green, false, -1.f);			\
		}

#define CHECK_INVALID_PTR(Ptr) checkf(Ptr, TEXT("%s is null."), TEXT(#Ptr)) if(nullptr == Ptr) return;
#define CHECK_INVALID_SMART_PTR(Ptr) checkf(Ptr.IsValid(), TEXT("%s is null."), TEXT(#Ptr)) if (nullptr == Ptr) return;
#define CHECK_INVALID_PTR_RetVal(Ptr) checkf(Ptr, TEXT("%s is null."), TEXT(#Ptr));
#define CHECK_INVALID_SMART_PTR_RetVal(Ptr) checkf(Ptr.IsValid(), TEXT("%s is null."), TEXT(#Ptr));