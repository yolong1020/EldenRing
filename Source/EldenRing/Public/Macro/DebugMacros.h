#pragma once
#include "DrawDebugHelpers.h"

#define CHECK_INVALID_PTR(Ptr) checkf(Ptr, TEXT("%s is null."), TEXT(#Ptr)) if(nullptr == Ptr) return;
#define CHECK_INVALID_SMART_PTR(Ptr) checkf(Ptr.IsValid(), TEXT("%s is null."), TEXT(#Ptr)) if (nullptr == Ptr) return;
#define CHECK_INVALID_PTR_RetVal(Ptr) checkf(Ptr, TEXT("%s is null."), TEXT(#Ptr));
#define CHECK_INVALID_SMART_PTR_RetVal(Ptr) checkf(Ptr.IsValid(), TEXT("%s is null."), TEXT(#Ptr));
