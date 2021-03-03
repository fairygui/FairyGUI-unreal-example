// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DemoObject.h"
#include "DemoJoystick.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class FGUITESTPROJECT_API UDemoJoystick : public UDemoObject
{
    GENERATED_BODY()

public:
    virtual void OnDemoStart_Implementation() override;

    virtual void OnDemoEnd_Implementation() override;
};
