// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoObject.h"

void UDemoObject::OnDemoStart_Implementation()
{

}

void UDemoObject::OnDemoEnd_Implementation()
{

}

void UDemoObject::OnDemoTick_Implementation()
{

}

UWorld* UDemoObject::GetWorld() const {
    if (!HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
        return GetOuter()->GetWorld();
    else
        return nullptr;
}