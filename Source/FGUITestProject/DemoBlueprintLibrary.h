#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FairyGUI.h"
#include "DemoBlueprintLibrary.generated.h"

UCLASS()
class FGUITESTPROJECT_API UDemoBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    static void ScaleUpWindow(UGWindow* Window);

    UFUNCTION(BlueprintCallable)
    static void ScaleDownWindow(UGWindow* Window);
};