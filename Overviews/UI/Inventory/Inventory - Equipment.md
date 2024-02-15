# Inventory - Equipment
###### 획득한 아이템을 캐릭터에게 장착, 해제시켜 Attribute에 반영하는 기능을 제공합니다. 무기의 경우 HUD의 Quick Slot에 노출됩니다.   
</br>

## Equip item
###### Equipment Manager를 통하여 Item을 캐릭터에게 착용시킵니다.   
</br>

  1. ###### 착용하려는 아이템 유형과 좌/우 위치(무기 한정)를 확인하여 슬롯에 장착시킵니다.
     https://github.com/yolong1020/EldenRing/blob/7e6dee018db108b2a9c756429f89ef9cc52c282b/Source/EldenRing/Private/System/EquipmentManager.cpp#L36-L59
     https://github.com/yolong1020/EldenRing/blob/7e6dee018db108b2a9c756429f89ef9cc52c282b/Source/EldenRing/Private/UI/Inventory/InventoryEquipSlot.cpp#L47-L62 </br>
  2.
     1. ###### 착용하려는 아이템의 매쉬를 캐릭터에 반영합니다.
        https://github.com/yolong1020/EldenRing/blob/f49792ab9b9dc99f1ca0b7278e9a456d946769f9/Source/EldenRing/Private/System/EquipmentManager.cpp#L180-L224
     2. ###### 무기의 경우, Weapon_Actor에서 무기의 매쉬를 사용자에게 부착합니다.
        https://github.com/yolong1020/EldenRing/blob/9f8fef7608cfddae2743db98a7142e9308236109/Source/EldenRing/Private/Items/Weapons/Weapon_Actor.cpp#L190-L202
     3. ###### 갑옷의 경우, UItemObject의 정보를 토대로 캐릭터의 일부 Material을 숨김 처리합니다.
        https://github.com/yolong1020/EldenRing/blob/f49792ab9b9dc99f1ca0b7278e9a456d946769f9/Source/EldenRing/Private/System/EquipmentManager.cpp#L226-L242
     4. ###### 무기를 착용하였다면 PlayerHUD의 QuickSlot을 갱신합니다.
        https://github.com/yolong1020/EldenRing/blob/55e7ecacb75266760a2f1ac112eddb4c23ba78b1/Source/EldenRing/Private/HUD/Status/PlayerHUD.cpp#L97-L126 </br> 
  3. ###### 아이템의 옵션을 캐릭터의 Attribute에 적용시킵니다.
     https://github.com/yolong1020/EldenRing/blob/7e6dee018db108b2a9c756429f89ef9cc52c282b/Source/EldenRing/Private/Characters/Players/C0000.cpp#L113-L121
     https://github.com/yolong1020/EldenRing/blob/7e6dee018db108b2a9c756429f89ef9cc52c282b/Source/EldenRing/Private/Characters/Component/AttributeComponent.cpp#L71-L86 </br>

### * Unequip item
  > Equipment Manager를 통하여 캐릭터에 착용된 Item을 해제시킵니다.

  ###### [01] 해제하려는 아이템 유형과 좌/우 위치(무기 한정)를 확인하여 슬롯에서 해제시킵니다.
  https://github.com/yolong1020/EldenRing/blob/9cb9d5313e1965c89f7e2fdd8e660c3f75f9a38d/Source/EldenRing/Private/System/EquipmentManager.cpp#L61-L93
  https://github.com/yolong1020/EldenRing/blob/07913ab06541dbf8959c6f1a4a7fc33016dc2247/Source/EldenRing/Private/UI/Inventory/InventoryEquipSlot.cpp#L64-L72
  </br>
  ###### [Step 02 - 02] 무기의 경우, 해제시킨 무기를 파괴하고 캐릭터가 보유한 무기 포인터를 제거한 뒤 PlayerHUD를 갱신시킵니다.
  https://github.com/yolong1020/EldenRing/blob/687e1f73144e985e35ab330b625f68a243aa6e76/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1237-L1253
  ###### [Step 02 - 03] 갑옷의 경우, 해제시킨 갑옷의 매쉬 컴포넌트를 해제하고 포인터를 제거한 뒤 캐릭터의 Material들을 갱신시킵니다.
  ```C++
	/// [UEquipmentManager::TryUnequipItem] 81
	m_equipments[int32(Type) - 1].Key->SetSkeletalMesh(nullptr);
	m_equipments[int32(Type) - 1].Key->UnregisterComponent();
	m_equipments[int32(Type) - 1].Key = nullptr;
  ```
  https://github.com/yolong1020/EldenRing/blob/687e1f73144e985e35ab330b625f68a243aa6e76/Source/EldenRing/Private/System/EquipmentManager.cpp#L226-L242
  </br>
  ###### [Step 03] 아이템의 옵션을 캐릭터의 Attribute에 제시킵니다.
  https://github.com/yolong1020/EldenRing/blob/7e6dee018db108b2a9c756429f89ef9cc52c282b/Source/EldenRing/Private/Characters/Players/C0000.cpp#L113-L121
  https://github.com/yolong1020/EldenRing/blob/687e1f73144e985e35ab330b625f68a243aa6e76/Source/EldenRing/Private/Characters/Component/AttributeComponent.cpp#L88-L102
  </br> 
