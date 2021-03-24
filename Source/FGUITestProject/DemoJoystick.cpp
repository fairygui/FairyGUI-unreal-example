#include "DemoJoystick.h"

void UDemoJoystick::OnDemoStart_Implementation()
{
    UUIPackage::AddPackage(TEXT("/Game/UI/Joystick"), this);

    MainView = UUIPackage::CreateObject("Joystick", "Main", this)->As<UGComponent>();
    MainView->MakeFullScreen();
    MainView->SetParentToRoot();

    Joystick = MakeShared<FJoystickModule>(MainView);

    UGObject* TextField = MainView->GetChild("n9");

    Joystick->OnMoving.BindLambda([TextField](float Degree) {
        TextField->SetText(FString::Printf(TEXT("%f"), Degree));
    });

    Joystick->OnMoveEnd.BindLambda([TextField]() {
        TextField->SetText(G_EMPTY_STRING);
    });
}

void UDemoJoystick::OnDemoEnd_Implementation()
{
    UUIPackage::RemovePackage("Joystick", this);
}