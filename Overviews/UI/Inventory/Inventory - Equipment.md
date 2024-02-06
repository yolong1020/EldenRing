# Inventory - Equipment
### * Equip item
  > Equipment Manager를 통하여 Item을 캐릭터에게 착용시킵니다.

  ###### [Step 01] 착용하려는 아이템 유형과 좌/우 위치(무기 한정)를 확인하여 슬롯에 장착시킵니다.
  https://github.com/yolong1020/EldenRing/blob/7e6dee018db108b2a9c756429f89ef9cc52c282b/Source/EldenRing/Private/System/EquipmentManager.cpp#L36-L59
  https://github.com/yolong1020/EldenRing/blob/7e6dee018db108b2a9c756429f89ef9cc52c282b/Source/EldenRing/Private/UI/Inventory/InventoryEquipSlot.cpp#L47-L62
  </br>
  ###### [Step 02 - 01] 착용하려는 아이템의 매쉬를 캐릭터에 반영합니다.
  https://github.com/yolong1020/EldenRing/blob/f49792ab9b9dc99f1ca0b7278e9a456d946769f9/Source/EldenRing/Private/System/EquipmentManager.cpp#L180-L224
  ###### [Step 02 - 02] 무기의 경우, Weapon_Actor에서 무기의 매쉬를 사용자에게 부착합니다.
  https://github.com/yolong1020/EldenRing/blob/9f8fef7608cfddae2743db98a7142e9308236109/Source/EldenRing/Private/Items/Weapons/Weapon_Actor.cpp#L190-L202
  ###### [Step 02 - 03] 갑옷의 경우, UItemObject의 정보를 토대로 캐릭터의 일부 Material을 숨김 처리합니다.
  https://github.com/yolong1020/EldenRing/blob/f49792ab9b9dc99f1ca0b7278e9a456d946769f9/Source/EldenRing/Private/System/EquipmentManager.cpp#L226-L242
  ###### [Step 02 - 04] 무기를 착용하였다면 PlayerHUD의 QuickSlot을 갱신합니다.
  https://github.com/yolong1020/EldenRing/blob/55e7ecacb75266760a2f1ac112eddb4c23ba78b1/Source/EldenRing/Private/HUD/Status/PlayerHUD.cpp#L97-L126
  </br> 
  ###### [Step 03] 아이템의 옵션을 캐릭터의 Attribute에 적용시킵니다.
  https://github.com/yolong1020/EldenRing/blob/7e6dee018db108b2a9c756429f89ef9cc52c282b/Source/EldenRing/Private/Characters/Players/C0000.cpp#L113-L121
  https://github.com/yolong1020/EldenRing/blob/7e6dee018db108b2a9c756429f89ef9cc52c282b/Source/EldenRing/Private/Characters/Component/AttributeComponent.cpp#L71-L86
  </br>

---
### * Unequip item
  > Equipment Manager를 통하여 캐릭터에 착용된 Item을 해제시킵니다.

  ###### [01] 해제하려는 아이템 유형과 좌/우 위치(무기 한정)를 확인하여 슬롯에서 해제시킵니다.
  https://github.com/yolong1020/EldenRing/blob/9cb9d5313e1965c89f7e2fdd8e660c3f75f9a38d/Source/EldenRing/Private/System/EquipmentManager.cpp#L61-L93
  https://github.com/yolong1020/EldenRing/blob/07913ab06541dbf8959c6f1a4a7fc33016dc2247/Source/EldenRing/Private/UI/Inventory/InventoryEquipSlot.cpp#L64-L72
  </br>
  ###### [02] 특정 슬롯의 인덱스를 확인 후 적재를 시도합니다.
  https://github.com/yolong1020/EldenRing/blob/91a7a54264022be27051620a753d14886cf3f565/Source/EldenRing/Private/System/StorageManager.cpp#L151-L157
  </br>
  ###### [03] 특정 슬롯의 타일 위치를 확인 후 적재를 시도합니다.
  https://github.com/yolong1020/EldenRing/blob/88fb131d94fdb9e1bd3609f015b66e5eee64da4d/Source/EldenRing/Private/System/StorageManager.cpp#L159-L166
  </br>
  ###### [04] 적재는 좌측 상단을 기준으로 시작되며, 기준으로부터 아이템 크기까지 각 슬롯에 해당하는 아이템의 포인터를 할당합니다. </br>만약 위젯이 노출되고 있다면, 적재된 아이템이 노출될 수 있도록 InteractionGrid를 갱신합니다.
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/e7857442-af04-49ef-a15a-7173275bc311" width="40%" height="40%">
  https://github.com/yolong1020/EldenRing/blob/88fb131d94fdb9e1bd3609f015b66e5eee64da4d/Source/EldenRing/Private/System/StorageManager.cpp#L198-L227
  </br>
