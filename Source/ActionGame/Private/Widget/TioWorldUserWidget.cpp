// Fill out your copyright notice in the Description page of Project Settings.


#include "TioWorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"

void UTioWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(AttachActor))
	{
		RemoveFromParent();

		UE_LOG(LogTemp, Warning, TEXT("AttachActor Is No Longer Valid, removing health bar"));
		return;
	}

	FVector2D ScreenPostion;
	bool bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachActor->GetActorLocation() + WorldOffset, ScreenPostion);
	
	if (bIsOnScreen)
	{
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPostion /= Scale;
		
		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPostion);
		}
	}

	if (ParentSizeBox)
	{
		ParentSizeBox->SetVisibility(bIsOnScreen ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}
