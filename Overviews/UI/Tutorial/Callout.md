# Callout
[1. Init trade widget](#Init-trade-widget)   
[2. Open trade](#Open-trade)   
[3. Purchase & Sell Item](#purchase--sell-item)   
</br>


## Init tutorial input data
###### 튜토리얼로 노출될 텍스트와 안내하고자 하는 Action, 입력할 key를 데이터로 생성합니다.
<img src="https://github.com/yolong1020/EldenRing/assets/87303898/48002333-ce16-4759-bd36-5059f068b0a4" width="50%" height="50%">
</br>

- ###### T
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/1900ad25-252d-4830-b0f7-adba3508fedf" width="49.5%" height="49.5%"></br>
  https://github.com/yolong1020/EldenRing/blob/923edcec6840de011a514f9d695daa7c1bfb142c/Source/EldenRing/Private/System/UIManager.cpp#L161-L167
  ```C++
        ///  [AC0000::GetHit] 491
	float percent = m_attribute->GetHealthPercent();
	if (percent < 0.5 && false == m_ui_mgr->IsActiveCallout(FName("Potion")))
	{
		m_ui_mgr->OpenTutorialCallout(FName("Potion"), EInputActionType::EIAT_Potion, false);
	}
  ```
  </br>
- ###### T
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/e60e4b7e-3bfb-46fc-b7bd-a5048bb759e2" width="49.5%" height="49.5%"></br>
  https://github.com/yolong1020/EldenRing/blob/ede3beb90bdd83522042c45519b1ba995fa21a15/Source/EldenRing/Private/System/UIManager.cpp#L147-L152 </br>
- ###### T
  https://github.com/yolong1020/EldenRing/blob/16f8271c446ef841156cb116a36278ef2d22b4a0/Source/EldenRing/Private/UI/Trade/TradeInteractionGrid.cpp#L28-L44 </br>


## Open trade
###### Trade Manager를 통하여 거래창을 활성화하며 활성되는 시점에서 보관된 Item들을 갱신합니다.

  1. ###### Trade를 활성화하며 동시에 'Esc' 입력시 최상위 UI 판별을 위해 UI Manager에 등록해줍니다.
     https://github.com/yolong1020/EldenRing/blob/dcd26958cb061a08392d17ad0e40ba8278bde2e0/Source/EldenRing/Private/System/TradeManager.cpp#L18-L46 </br>
  2. ###### TradeInteractionGrid를 갱신하여 각 캐릭터들의 보관함에 존재하는 아이템들을 Canvas Slot에 포함시킵니다.
     https://github.com/yolong1020/EldenRing/blob/239239bd7b779bae6986782cf5861d1e0b00a894/Source/EldenRing/Private/System/TradeManager.cpp#L125-L137
     https://github.com/yolong1020/EldenRing/blob/065770f13537e7d9442c659c0653f61069d1a681/Source/EldenRing/Private/UI/Trade/TradeInteractionGrid.cpp#L46-L80 </br>
  3. ###### Canvas Slot에 포함된 아이템들은 'UItemObject'의 정보를 토대로 텍스쳐 이미지와 크기를 갱신합니다.
     https://github.com/yolong1020/EldenRing/blob/2c3e7d450dd827127f4ce4ad46fe18e74c02adc0/Source/EldenRing/Private/UI/Trade/TradeItem.cpp#L247-L259 </br>


## Purchase & Sell Item
###### Vender NPC로부터 재화를 소모하여 아이템을 구매하거나 아이템을 판매하여 재화를 얻을 수 있습니다.

  1.
     1. ###### 마우스 Input을 통한 거래로 우 클릭은 팝업없이 현재 아이템의 소유자를 확인 후 구매 / 판매 기능을 수행합니다.</br>좌 클릭의 경우 NPC 보관함에서 구매 팝업이 활성되며 거래가 진행됩니다.
        <img src="https://github.com/yolong1020/EldenRing/assets/87303898/c070af3b-3b30-40b8-a621-7022385dd62b" width="49.5%" height="49.5%"></img>
        <img src="https://github.com/yolong1020/EldenRing/assets/87303898/06255a66-19e5-476e-9327-0ed88479c5bc" width="49.5%" height="49.5%"></br>
        https://github.com/yolong1020/EldenRing/blob/117b7a1f1e985fdc4adfa030ed3973386def500b/Source/EldenRing/Private/UI/Trade/TradeItem.cpp#L88-L129
     2. ###### 마우스 Drag Drop을 통한 거래로 마우스 Input 좌 클릭처럼 팝업이 활성되어 판매 거래가 진행됩니다.
        <img src="https://github.com/yolong1020/EldenRing/assets/87303898/7d62837f-9e3f-4038-928e-5eb6180df07f" width="49.5%" height="49.5%"></br>
        https://github.com/yolong1020/EldenRing/blob/decb4891e7cbb23ae9f938fd0b817115d63d86e1/Source/EldenRing/Private/UI/Trade/TradeInteractionGrid.cpp#L145-L197 </br>  
  2. ###### 키보드 Input에 의한 거래로 플레이어가 아이템 구매 시 즉시 착용을 함께 수행합니다.</br>만약 아이템이 플레이어 소유라면 즉시 착용만 수행하게 됩니다.
     <img src="https://github.com/yolong1020/EldenRing/assets/87303898/76c1d8a2-f5dd-40b7-9a1a-1a7d409a481d" width="49.5%" height="49.5%"></img>
     <img src="https://github.com/yolong1020/EldenRing/assets/87303898/9cb8009e-e8e6-4251-a56c-df0e312db1ff" width="49.5%" height="49.5%"></br>
     https://github.com/yolong1020/EldenRing/blob/117b7a1f1e985fdc4adfa030ed3973386def500b/Source/EldenRing/Private/UI/Trade/TradeItem.cpp#L184-L245 </br>
  3. ###### 구매 / 판매 여부에 따라 캐릭터별로 재화 갱신이 달라지며, 아이템 즉시 착용이 진행되는 경우 보관함에는 수납하지 않습니다.</br> 거래가 종료되면 게임의 진행 상황을 저장합니다.
     https://github.com/yolong1020/EldenRing/blob/bc678c39609bf7e91b604d6f22b202db1336da32/Source/EldenRing/Private/UI/Trade/TradeItem.cpp#L272-L311
