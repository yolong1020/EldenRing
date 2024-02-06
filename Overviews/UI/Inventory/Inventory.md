# Inventory
### * Init Storage Widget
  > Blueprint에서 설정한 Slot Count에 따라 보관함 칸 수를 설정하고 상호 작용을 위한 초기화 작업을 진행합니다.

  ###### [Step 01] Inventory의 각 Widget들을 초기화합니다. (Slot count, Delegate, etc)
  https://github.com/yolong1020/EldenRing/blob/d8a4482dee125ba06ce66e03dfea90628b27c71f/Source/EldenRing/Private/System/StorageManager.cpp#L42-L85
  https://github.com/yolong1020/EldenRing/blob/d8a4482dee125ba06ce66e03dfea90628b27c71f/Source/EldenRing/Private/UI/Inventory/InventoryGrid.cpp#L47-L66
  </br>
  
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/f37436e7-ffbe-409c-9bcd-98da953456e8" width="60%" height="60%">
  
  ###### InventorySlotGrid는 화면에 출력될 보관함 칸 수를 담당하는 Widget입니다.
  https://github.com/yolong1020/EldenRing/blob/66be893c43cf52f5783ec93ff1e88fb007b92325/Source/EldenRing/Private/UI/Inventory/InventorySlotGrid.cpp#L20-L48
  ###### InventoryInteractionGrid는 실제 Item과 보관함의 상호작용을 담당하는 Widget입니다.
  https://github.com/yolong1020/EldenRing/blob/d8a4482dee125ba06ce66e03dfea90628b27c71f/Source/EldenRing/Private/UI/Inventory/InventoryInteractionGrid.cpp#L24-L39
  </br>

  ###### [Step 02] Inventory를 열게되면 'Esc' 입력시 최상위 UI 판별을 위해 UI Manager에 등록해줍니다. 이후 InteractionGrid를 갱신하여 보관함에 존재하는 Item들을 Canvas Slot에 포함시킵니다.
  https://github.com/yolong1020/EldenRing/blob/808bb8d461a42c4bc8d677944969d10fdc64c29e/Source/EldenRing/Private/System/StorageManager.cpp#L23-L35
  https://github.com/yolong1020/EldenRing/blob/674fd8903ad223d87e64afd43a168944b8cececc/Source/EldenRing/Private/UI/Inventory/InventoryInteractionGrid.cpp#L41-L76
  </br>

### * Item Storage
  > Item과 재화를 보관하며 필요에 따라 착용하거나 버릴 수 있습니다.

  ###### [Step 01] Inventory를 활성화하며 동시에 'Esc' 입력시 최상위 UI 판별을 위해 UI Manager에 등록해줍니다.
  https://github.com/yolong1020/EldenRing/blob/808bb8d461a42c4bc8d677944969d10fdc64c29e/Source/EldenRing/Private/System/StorageManager.cpp#L23-L35
  https://github.com/yolong1020/EldenRing/blob/674fd8903ad223d87e64afd43a168944b8cececc/Source/EldenRing/Private/UI/Inventory/InventoryInteractionGrid.cpp#L41-L76
  </br>
  
