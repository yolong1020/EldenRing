![Trade](https://github.com/yolong1020/EldenRing/assets/87303898/eabc3cfa-695d-4695-b852-332ed7646cd4)

# Trade
### * Init trade widget
  > Blueprint에서 설정한 Slot Count에 따라 거래 보관함 슬롯 수를 설정하고 상호 작용을 위한 초기화 작업을 진행합니다.

  ###### Trade Manager를 통하여 Trade의 각 위젯들을 초기화합니다. (Slot count, Delegate, etc)
  https://github.com/yolong1020/EldenRing/blob/9bc3b0f4022cdfa966450509556193acb8d53ced/Source/EldenRing/Private/System/TradeManager.cpp#L60-L100
  </br>
  
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/c8c9cb52-a810-4796-b3aa-d2649d55d505" width="60%" height="60%">
  
  ###### TradeGrid는 화면에 출력될 각 캐릭터들의 보관함 슬롯을 담당하는 위젯입니다.
  https://github.com/yolong1020/EldenRing/blob/16f8271c446ef841156cb116a36278ef2d22b4a0/Source/EldenRing/Private/UI/Trade/TradeGrid.cpp#L17-L41
  ###### TradeInteractionGrid는 실제 아이템과 캐릭터들의 보관함 상호작용을 담당하는 위젯입니다.
  https://github.com/yolong1020/EldenRing/blob/16f8271c446ef841156cb116a36278ef2d22b4a0/Source/EldenRing/Private/UI/Trade/TradeInteractionGrid.cpp#L28-L44
  </br>
  
---
### * Open trade
  > Trade Manager를 통하여 거래창을 활성화하며 활성되는 시점에서 보관된 Item들을 갱신합니다.

  ###### [Step 01] Trade를 활성화하며 동시에 'Esc' 입력시 최상위 UI 판별을 위해 UI Manager에 등록해줍니다.
  https://github.com/yolong1020/EldenRing/blob/dcd26958cb061a08392d17ad0e40ba8278bde2e0/Source/EldenRing/Private/System/TradeManager.cpp#L18-L46
  </br>
  ###### [Step 02] TradeInteractionGrid를 갱신하여 각 캐릭터들의 보관함에 존재하는 아이템들을 Canvas Slot에 포함시킵니다.
  https://github.com/yolong1020/EldenRing/blob/239239bd7b779bae6986782cf5861d1e0b00a894/Source/EldenRing/Private/System/TradeManager.cpp#L125-L137
  https://github.com/yolong1020/EldenRing/blob/065770f13537e7d9442c659c0653f61069d1a681/Source/EldenRing/Private/UI/Trade/TradeInteractionGrid.cpp#L46-L80
  </br>
  ###### [Step 03] Canvas Slot에 포함된 아이템들은 'UItemObject'의 정보를 토대로 텍스쳐 이미지와 크기를 갱신합니다.
  https://github.com/yolong1020/EldenRing/blob/2c3e7d450dd827127f4ce4ad46fe18e74c02adc0/Source/EldenRing/Private/UI/Trade/TradeItem.cpp#L247-L259
  </br>
  
---
### * Purchase & Sell Item
  > Vender NPC로부터 재화를 소모하여 아이템을 구매하거나 아이템을 판매하여 재화를 얻을 수 있습니다.

  ###### [01] 특정 위치를 지정하지 않고 아이템을 적재할 경우, 순차적으로 적재가 가능한 위치를 탐색 후 적재합니다. 모든 슬롯을 순회함에도 적재가 불가능하다면 아이템을 회전시켜 한번 더 적재를 시도합니다.
  https://github.com/yolong1020/EldenRing/blob/117b7a1f1e985fdc4adfa030ed3973386def500b/Source/EldenRing/Private/UI/Trade/TradeItem.cpp#L88-L129
  https://github.com/yolong1020/EldenRing/blob/117b7a1f1e985fdc4adfa030ed3973386def500b/Source/EldenRing/Private/UI/Trade/TradeItem.cpp#L184-L245
  https://github.com/yolong1020/EldenRing/blob/117b7a1f1e985fdc4adfa030ed3973386def500b/Source/EldenRing/Private/UI/Trade/TradeItem.cpp#L272-L319
