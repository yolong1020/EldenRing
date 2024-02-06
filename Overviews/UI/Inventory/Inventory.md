# Inventory
### * Init storage widget
  > Blueprint에서 설정한 Slot Count에 따라 보관함 슬롯 수를 설정하고 상호 작용을 위한 초기화 작업을 진행합니다.

  ###### Storage Manager를 통하여 Inventory의 각 위젯들을 초기화합니다. (Slot count, Delegate, etc)
  https://github.com/yolong1020/EldenRing/blob/d8a4482dee125ba06ce66e03dfea90628b27c71f/Source/EldenRing/Private/System/StorageManager.cpp#L42-L85
  https://github.com/yolong1020/EldenRing/blob/d8a4482dee125ba06ce66e03dfea90628b27c71f/Source/EldenRing/Private/UI/Inventory/InventoryGrid.cpp#L47-L66
  </br>
  
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/f37436e7-ffbe-409c-9bcd-98da953456e8" width="60%" height="60%">
  
  ###### InventorySlotGrid는 화면에 출력될 보관함 슬롯 수를 담당하는 위젯입니다.
  https://github.com/yolong1020/EldenRing/blob/66be893c43cf52f5783ec93ff1e88fb007b92325/Source/EldenRing/Private/UI/Inventory/InventorySlotGrid.cpp#L20-L48
  ###### InventoryInteractionGrid는 실제 아이템과 보관함의 상호작용을 담당하는 위젯입니다.
  https://github.com/yolong1020/EldenRing/blob/d8a4482dee125ba06ce66e03dfea90628b27c71f/Source/EldenRing/Private/UI/Inventory/InventoryInteractionGrid.cpp#L24-L39
  </br>
---

### * Open storage
  > Storage Manager를 통하여 보관함을 활성화하며 활성되는 시점에서 보관된 Item들을 갱신합니다.

  ###### [Step 01] Inventory를 활성화하며 동시에 'Esc' 입력시 최상위 UI 판별을 위해 UI Manager에 등록해줍니다.
  https://github.com/yolong1020/EldenRing/blob/808bb8d461a42c4bc8d677944969d10fdc64c29e/Source/EldenRing/Private/System/StorageManager.cpp#L23-L35
  </br>
  ###### [Step 02] InteractionGrid를 갱신하여 보관함에 존재하는 아이템들을 Canvas Slot에 포함시킵니다.
  https://github.com/yolong1020/EldenRing/blob/674fd8903ad223d87e64afd43a168944b8cececc/Source/EldenRing/Private/UI/Inventory/InventoryInteractionGrid.cpp#L41-L76
  </br>
  ###### [Step 03] Canvas Slot에 포함된 아이템들은 'UItemObject'의 정보를 토대로 텍스쳐 이미지와 크기를 갱신합니다.
  https://github.com/yolong1020/EldenRing/blob/75633c135b4a24c70af3405552cb2083240139c8/Source/EldenRing/Private/UI/Inventory/InventoryItem.cpp#L218-L244
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
  ###### [04] 적재는 좌측 상단을 기준으로 시작되며, 기준으로부터 아이템 크기까지 각 슬롯에 해당하는 아이템의 포인터를 할당합니다. </br>만약 위젯이 노출중에 있다면, 적재된 아이템이 노출될 수 있도록 InteractionGrid를 갱신합니다.
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/e7857442-af04-49ef-a15a-7173275bc311" width="40%" height="40%">
  https://github.com/yolong1020/EldenRing/blob/88fb131d94fdb9e1bd3609f015b66e5eee64da4d/Source/EldenRing/Private/System/StorageManager.cpp#L198-L227
  </br>
---

### * Remove item to storage
  > 아이템을 착용하거나 필드에 버리는 경우, 보관함에서 해당 아이템을 제거합니다.
  
  ###### 모든 슬롯을 순회하며 해당되는 아이템의 포인터를 제거해줍니다. </br>만약 위젯이 노출중에 있다면, 적재된 아이템이 노출되지 않도록 InteractionGrid를 갱신합니다.
  https://github.com/yolong1020/EldenRing/blob/345d258673e6c6e69667b41a85b8ae642f124db3/Source/EldenRing/Private/System/StorageManager.cpp#L229-L240
  </br>
  
