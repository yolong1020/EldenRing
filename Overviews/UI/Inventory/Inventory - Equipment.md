# Inventory - Equipment
### * Equip item
  > Equipment Manager를 통하여 Item을 캐릭터에게 착용시킵니다.

  ###### [Step 01] 착용하려는 아이템 유형과 좌/우 위치(무기 한정)를 확인하여 슬롯에 장착시킵니다.
  https://github.com/yolong1020/EldenRing/blob/7e6dee018db108b2a9c756429f89ef9cc52c282b/Source/EldenRing/Private/System/EquipmentManager.cpp#L36-L59
  https://github.com/yolong1020/EldenRing/blob/7e6dee018db108b2a9c756429f89ef9cc52c282b/Source/EldenRing/Private/UI/Inventory/InventoryEquipSlot.cpp#L47-L62
  </br>
  ###### [Step 02] 착용하려는 아이템의 매쉬를 캐릭터에 반영하며 PlayerHUD의 QuickSlot을 갱신합니다.
  https://github.com/yolong1020/EldenRing/blob/f49792ab9b9dc99f1ca0b7278e9a456d946769f9/Source/EldenRing/Private/System/EquipmentManager.cpp#L180-L224
  https://github.com/yolong1020/EldenRing/blob/55e7ecacb75266760a2f1ac112eddb4c23ba78b1/Source/EldenRing/Private/HUD/Status/PlayerHUD.cpp#L97-L126
  </br> 
  ###### [Step 03] 아이템의 옵션을 캐릭터의 Attribute에 적용시킵니다.
  https://github.com/yolong1020/EldenRing/blob/7e6dee018db108b2a9c756429f89ef9cc52c282b/Source/EldenRing/Private/Characters/Players/C0000.cpp#L113-L121
  https://github.com/yolong1020/EldenRing/blob/7e6dee018db108b2a9c756429f89ef9cc52c282b/Source/EldenRing/Private/Characters/Component/AttributeComponent.cpp#L71-L86
  </br>

---
### * Add item to storage
  > 아이템과 재화를 보관하며 필요에 따라 착용하거나 버릴 수 있습니다.

  ###### [01] 특정 위치를 지정하지 않고 아이템을 적재할 경우, 순차적으로 적재가 가능한 위치를 탐색 후 적재합니다. 모든 슬롯을 순회함에도 적재가 불가능하다면 아이템을 회전시켜 한번 더 적재를 시도합니다.
  https://github.com/yolong1020/EldenRing/blob/91a7a54264022be27051620a753d14886cf3f565/Source/EldenRing/Private/System/StorageManager.cpp#L127-L149
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

---
### * Remove item to storage
  > 아이템을 착용하거나 필드에 버리는 경우, 보관함에서 해당 아이템을 제거합니다.
  
  ###### 모든 슬롯을 순회하며 해당하는 아이템의 포인터를 제거해줍니다. </br>만약 위젯이 노출되고 있다면, 적재된 아이템이 노출되지 않도록 InteractionGrid를 갱신합니다.
  https://github.com/yolong1020/EldenRing/blob/345d258673e6c6e69667b41a85b8ae642f124db3/Source/EldenRing/Private/System/StorageManager.cpp#L229-L240
  </br>
