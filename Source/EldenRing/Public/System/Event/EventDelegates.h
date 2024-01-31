#pragma once

#include "CoreMinimal.h"

DECLARE_MULTICAST_DELEGATE(FRequestAllowUsing)
DECLARE_MULTICAST_DELEGATE(FWaitAllowUsing)

DECLARE_DELEGATE_OneParam(FReportUsingPointObject, class AAssemblePointObject* const PointObject);