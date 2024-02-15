# Vender
###### 사용자가 직접 조작하는 캐릭터로 APlayerController를 사용하며, Level의 게임 플레이 제공 여부에 따라 생성됩니다. 5회 콤보가 가능한 일반 공격과 2회 콤보의 강 공격이 전투 상황에서 제공되고 또한 방패 보유시 방어와 반격이 가능합니다. 기본 움직임으로는 전력 질주와 회피, 점프 기능을 제공되고 이 기능들은 지구력을 공격과 함께 지구력을 소모하게 됩니다. 그 외 Lock On과 포션 섭취, Interaction Manager에 의해 NPC 혹은 사물과 상호 작용, 마지막으로 Inventory Manager와 Trade Manager에 의한 아이템 관리 및 거래가 구현되어 있습니다.
[1. Init Vender Settings](#Init-Vender-Settings)   
[2. Interaction](#Interaction)   
[3. Trade](#Trade)   
[4. Observer Event](#Observer-Event)   
</br>


## Init Vender Settings
###### 상인의 Interaction Collision, Observer Event, 판매 물품 등을 초기화합니다.   
<img src="https://github.com/yolong1020/EldenRing/assets/87303898/dfe59b0a-b98f-4906-bba8-4e8223f5d269" width="49.5%" height="49.5%"></img>
<img src="https://github.com/yolong1020/EldenRing/assets/87303898/9f752ea2-e3f4-4309-b036-6d80032d9c9e" width="49.5%" height="49.5%"></img>
https://github.com/yolong1020/EldenRing/blob/9bbdd3573ed007ca5d21635021f044b7a39776d4/Source/EldenRing/Private/Characters/NPC/Vender/Vender.cpp#L46-L115 </br>


## Interaction
###### Interaction Collision을 통해 대화 및 거래를 진행합니다.   
</br>

  1. ###### NPC와 상호 작용을 위한 Popup UI를 활성화합니다.
     <img src="https://github.com/yolong1020/EldenRing/assets/87303898/6efc4ede-907a-4875-8b25-da10796d3b4a" width="49.5%" height="49.5%"></img>
     https://github.com/yolong1020/EldenRing/blob/76c15adfad13dc543afc0a4e612eebef0dde77ae/Source/EldenRing/Private/Characters/NPC/Vender/Vender.cpp#L222-L233
     https://github.com/yolong1020/EldenRing/blob/fcfc4de0477e09a17c3777194dfb107fd6432348/Source/EldenRing/Private/System/NPCInteractionManager.cpp#L26-L42
     https://github.com/yolong1020/EldenRing/blob/fcfc4de0477e09a17c3777194dfb107fd6432348/Source/EldenRing/Private/UI/PopUp/InteractionPopUp.cpp#L22-L61 </br>
  2. ###### 사용자 조작이 감지되면 상인에게 등록된 연출 카메라로 전환을 실행 후 다이얼로그를 활성화합니다.
     <img src="https://github.com/yolong1020/EldenRing/assets/87303898/9c573ec3-36d4-4a66-8731-ca63ba4789d0" width="49.5%" height="49.5%"></img>
     <img src="https://github.com/yolong1020/EldenRing/assets/87303898/a52f015d-b491-4b1e-a8c0-30b303e7bc5b" width="49.5%" height="49.5%"></img>
     https://github.com/yolong1020/EldenRing/blob/6b38e1c98a77f94b902d5b61e34b656e4f60da7f/Source/EldenRing/Private/UI/PopUp/InteractionPopUp.cpp#L99-L112
     https://github.com/yolong1020/EldenRing/blob/06d5ac00a61008476953dfcb67f150318bbdba29/Source/EldenRing/Private/System/NPCInteractionManager.cpp#L112-L136

     1. ###### StartCommunicate는 오직 NPC와 대화만을 시작하기 위한 함수입니다.
        https://github.com/yolong1020/EldenRing/blob/06d5ac00a61008476953dfcb67f150318bbdba29/Source/EldenRing/Private/System/NPCInteractionManager.cpp#L77-L92
        
     2. ###### StartOptionCommunicate는 NPC와 대화뿐만 아니라 여러 기능이 포함된 상호 작용을 시작하기 위한 함수입니다.
        https://github.com/yolong1020/EldenRing/blob/06d5ac00a61008476953dfcb67f150318bbdba29/Source/EldenRing/Private/System/NPCInteractionManager.cpp#L94-L110

  3. ###### 상대 캐릭터가 무기를 손에 착용 중이라면 NPC는 위협받는 상황의 다이얼로그를 출력합니다.</br>무기를 착용하지 않았다면 마지막 다이얼로그를 기점으로 대화가 진행됩니다.
     <img src="https://github.com/yolong1020/EldenRing/assets/87303898/7d8503c8-55ca-4466-b824-d71b7ef88f61" width="49.5%" height="49.5%"></img>
     <img src="https://github.com/yolong1020/EldenRing/assets/87303898/2185ea4f-27c2-4f8b-a0fb-4e8bd68fdb94" width="49.5%" height="49.5%"></img>
     https://github.com/yolong1020/EldenRing/blob/7454685f6f5655769f78f88afbd5babf3fad7702/Source/EldenRing/Private/UI/Communicate/CommunicateCallOut.cpp#L62-L97 </br>

  4. ###### 사용자 조작으로 다이얼로그가 업데이트되면 현재 다이얼로그에 명시된 Options로부터 테이블에서 차기 다이얼로그를 찾아 대화를 갱신합니다.</br>Trigger Event가 활성되어 있고 Event Functions에 기능이 있다면 순차적으로 기능을 실행합니다.
     <img src="https://github.com/yolong1020/EldenRing/assets/87303898/ccf07354-3210-4566-a590-28fa1327a9ea"></img>
     https://github.com/yolong1020/EldenRing/blob/79a67115117f06931986e3c4eb5f9c3bfc287220/Source/EldenRing/Private/UI/Communicate/CommunicateCallOut.cpp#L39-L60
     https://github.com/yolong1020/EldenRing/blob/79a67115117f06931986e3c4eb5f9c3bfc287220/Source/EldenRing/Private/UI/Communicate/CommunicateCallOut.cpp#L133-L162 </br>

  5. ###### 다이얼로그 데이터에 Button Type이 명시되었고 Event Function으로 SelectOption이 있는 경우라면 Select Popup을 활성하고 각 버튼에 기능을 바인딩합니다.
     <img src="https://github.com/yolong1020/EldenRing/assets/87303898/e2b25780-a6f4-477b-bb0f-0145871bb102" width="49.5%" height="49.5%"></img>
     https://github.com/yolong1020/EldenRing/blob/79a67115117f06931986e3c4eb5f9c3bfc287220/Source/EldenRing/Private/UI/Communicate/CommunicateCallOut.cpp#L188-L201
     https://github.com/yolong1020/EldenRing/blob/f00b6884414a0925be8a92609950e9f87d553c32/Source/EldenRing/Private/System/NPCInteractionManager.cpp#L51-L74
     https://github.com/yolong1020/EldenRing/blob/acee9a4b3f1c631e0e61c76854ab00efe662585a/Source/EldenRing/Private/UI/PopUp/SelectionPopUp.cpp#L56-L86 </br>

     
## Trade
###### Trade Manager를 통해 거래창을 활성화하고 상인이 가진 판매 물품들을 UI에 등록합니다.
<img src="https://github.com/yolong1020/EldenRing/assets/87303898/993eee19-f933-416c-81b1-2566d4fdc047" width="49.5%" height="49.5%"></img>   
https://github.com/yolong1020/EldenRing/blob/ed11ad611e3bb18e52de447d6298a00c52477770/Source/EldenRing/Private/UI/Communicate/CommunicateCallOut.cpp#L203-L214
https://github.com/yolong1020/EldenRing/blob/ed11ad611e3bb18e52de447d6298a00c52477770/Source/EldenRing/Private/System/TradeManager.cpp#L18-L46 </br>


## Observer Event
###### 특정 상황에 대해 등록된 Observer Event를 통하여 상인을 이동시키고 마지막 다이얼로그를 변경합니다.
<img src="https://github.com/yolong1020/EldenRing/assets/87303898/d7634f0b-4989-4d5a-b8c0-9f540a5aeb98" width="49.5%" height="49.5%"></img>
https://github.com/yolong1020/EldenRing/blob/dabd8129d746f36959fbded0c68a4d6e132cfec6/Source/EldenRing/Private/FieldObject/Interactable/Bonefire.cpp#L50-L71
https://github.com/yolong1020/EldenRing/blob/800d0838f0fd4e904f75e179c47b8c0c04dd9e1f/Source/EldenRing/Private/Characters/NPC/Vender/Vender.cpp#L197-L215 </br>
