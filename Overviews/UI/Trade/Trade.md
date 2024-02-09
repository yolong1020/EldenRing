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

  ###### [01 - 01] 마우스 Input을 통한 거래로 우 클릭은 팝업없이 현재 아이템의 소유자를 확인 후 구매 / 판매 기능을 수행합니다.</br>좌 클릭의 경우 NPC 보관함에서 구매 팝업이 활성되며 거래가 진행됩니다.
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/c070af3b-3b30-40b8-a621-7022385dd62b" width="49.5%" height="49.5%"></>
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/06255a66-19e5-476e-9327-0ed88479c5bc" width="49.5%" height="49.5%"></br>
  https://github.com/yolong1020/EldenRing/blob/117b7a1f1e985fdc4adfa030ed3973386def500b/Source/EldenRing/Private/UI/Trade/TradeItem.cpp#L88-L129
  ###### [01 - 02] 마우스 Drag Drop에 의한 거래로 마우스 Input의 좌 클릭처럼 팝업이 활성되어 판매 거래가 진행됩니다.
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/7d62837f-9e3f-4038-928e-5eb6180df07f" width="49.5%" height="49.5%"></br>
  https://github.com/yolong1020/EldenRing/blob/decb4891e7cbb23ae9f938fd0b817115d63d86e1/Source/EldenRing/Private/UI/Trade/TradeInteractionGrid.cpp#L145-L197
  ###### [02] 키보드 Input에 의한 거래로 플레이어가 아이템 구매 시 즉시 착용을 함께 수행합니다.</br>만약 아이템이 플레이어 소유라면 즉시 착용만 수행하게 됩니다.
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/76c1d8a2-f5dd-40b7-9a1a-1a7d409a481d" width="49.5%" height="49.5%"></br>
  https://github.com/yolong1020/EldenRing/blob/117b7a1f1e985fdc4adfa030ed3973386def500b/Source/EldenRing/Private/UI/Trade/TradeItem.cpp#L184-L245
  </br>
  ###### [03] 구매 / 판매 여부에 따라 캐릭터별로 재화 갱신이 달라지며, 아이템 즉시 착용이 진행되는 경우 보관함에는 수납하지 않습니다.</br> 거래가 종료되면 게임의 진행 상황을 저장합니다.
  https://github.com/yolong1020/EldenRing/blob/bc678c39609bf7e91b604d6f22b202db1336da32/Source/EldenRing/Private/UI/Trade/TradeItem.cpp#L272-L311
