// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Item/ItemInfo.h"
#include "UI/Item/ItemOption.h"
#include "UI/Item/ItemPrice.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Items/ItemObject.h"
#include "Items/ItemProperties.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/InvalidationBox.h"
#include "System/DataTableManager.h"
#include "Macro/DebugMacros.h"

#define LOCTEXT_NAMESPACE "OptionValue"

void UItemInfo::UpdateItemInfo(FVector2D Position, const FVector2D& SlotSize, const TObjectPtr<UItemObject> ItemObject, const bool& IsShowPrice)
{
	m_position		= Position;
	m_slot_size		= SlotSize;
	m_item_object	= ItemObject;

	const FItemData* item_data = m_item_object->GetItemData();
	CHECK_INVALID_PTR(item_data)

	ItemName->SetText(FText::FromString(item_data->ItemName));
	ItemDesc->SetText(FText::FromString(item_data->ItemDescription.Replace(L"/n",L"\n")));
	Price->SetVisibility(IsShowPrice ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	EDragAndDropHUD current = m_item_object->GetCurrentHUD();
	FString l_text, r_text;
	if (IsShowPrice)
	{
		EItemTradeType type;
		if (current == EDragAndDropHUD::EDAD_Vender) 
		{ 
			type	= EItemTradeType::EITT_Purchase;
			l_text	= TEXT("구매");
			r_text	= TEXT("구매 후 착용");
		}
		else if (current == EDragAndDropHUD::EDAD_Inventory) 
		{ 
			type	= EItemTradeType::EITT_Sell;
			l_text	= TEXT("판매");
			r_text	= TEXT("착용");
		}

		Price->InitPriceText(type, type == EItemTradeType::EITT_Sell ? item_data->Cost * 0.5 : item_data->Cost);
	}
	else
	{
		if		(current == EDragAndDropHUD::EDAD_Equipment) { l_text = TEXT("해제"); }
		else if (current == EDragAndDropHUD::EDAD_Inventory) { l_text = TEXT("착용"); }
		r_text = TEXT("버리기");
	}
	L_Desc->SetText(FText::FromString(l_text));
	R_Desc->SetText(FText::FromString(r_text));

	ClearOptions();
	AddOptions();

	FLatentActionInfo action;
	action.CallbackTarget = this;
	action.Linkage = 0;
	action.ExecutionFunction = FName("SetRenderPosition");
	UKismetSystemLibrary::Delay(GetWorld(), 0, action);
}

void UItemInfo::SetRenderPosition()
{

	UCanvasPanelSlot* slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(InvalidationBox);
	CHECK_INVALID_PTR(slot)

	double offsetX = 10;
	FVector2D position = m_position;
	switch (m_item_object->GetCurrentHUD())
	{
		case EDragAndDropHUD::EDAD_Inventory:
		{
			position.X -= offsetX;
			slot->SetAlignment(FVector2D(1, 0));
		} break;
		case EDragAndDropHUD::EDAD_Vender:
		case EDragAndDropHUD::EDAD_Equipment:
		{
			position.X += (m_slot_size.X + offsetX);
			slot->SetAlignment(FVector2D(0, 0));
		} break;
	}

	slot->SetPosition(position);
	AddToViewport();
}

void UItemInfo::AddOptions()
{
	UDataTable* data_table = m_data_mgr->GetDataTable(EDataTableType::EDTT_ItemProp);
	CHECK_INVALID_PTR(data_table)

	FName name = m_item_object->GetRowName();
	FItemProperties* properties = data_table->FindRow<FItemProperties>(name, name.ToString());
	CHECK_INVALID_PTR(properties);

	for (int32 i = 0; i < (int32)EItemOptionType::EIOT_Max; i++)
	{
		EItemOptionType type = (EItemOptionType)i;
		FText value;

		switch (type)
		{
		case EItemOptionType::EIOT_Attack:
			if (properties->Attack.X == 0)	continue; value = FText::Format(LOCTEXT("OptionValue", "{0} ~ {1}"), properties->Attack.X, properties->Attack.Y); break;
		case EItemOptionType::EIOT_Shield:
			if (properties->Shield.X == 0)	continue; value = FText::Format(LOCTEXT("OptionValue", "{0} ~ {1}"), properties->Shield.X, properties->Shield.Y); break;
		case EItemOptionType::EIOT_Hp:
			if (properties->Hp == 0)		continue; value = FText::Format(LOCTEXT("OptionValue", "+ {0}"), properties->Hp); break;
		case EItemOptionType::EIOT_Stamina:
			if (properties->Stamina == 0)	continue; value = FText::Format(LOCTEXT("OptionValue", "+ {0}"), properties->Stamina); break;
		case EItemOptionType::EIOT_Power:
			if (properties->Power == 0)		continue; value = FText::Format(LOCTEXT("OptionValue", "+ {0}"), properties->Power); break;
		}

		UItemOption* option = CreateWidget<UItemOption>(this, m_option_class);
		option->InitOptionText(type, value);

		UVerticalBoxSlot* slot = Cast<UVerticalBoxSlot>(OptionList->AddChild(option));
		CHECK_INVALID_PTR(slot)

		slot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	}
}

void UItemInfo::ClearOptions()
{
	CHECK_INVALID_PTR(OptionList)
	OptionList->ClearChildren();
}

void UItemInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	UGameInstance* instance = UGameplayStatics::GetGameInstance(this);
	CHECK_INVALID_PTR(instance)

	m_data_mgr = instance->GetSubsystem<UDataTableManager>();
	CHECK_INVALID_SMART_PTR(m_data_mgr)

	m_option_class = LoadClass<UItemOption>(nullptr, TEXT("WidgetBlueprint'/Game/Blueprint/UI/ItemInfo/WBP_Option.WBP_Option_C'"));
	CHECK_INVALID_PTR(m_option_class)
}

int32 UItemInfo::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int result = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	m_widget_size = AllottedGeometry.GetLocalSize();

	return result;
}

#undef LOCTEXT_NAMESPACE