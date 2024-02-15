# Vender
###### 사용자가 직접 조작하는 캐릭터로 APlayerController를 사용하며, Level의 게임 플레이 제공 여부에 따라 생성됩니다. 5회 콤보가 가능한 일반 공격과 2회 콤보의 강 공격이 전투 상황에서 제공되고 또한 방패 보유시 방어와 반격이 가능합니다. 기본 움직임으로는 전력 질주와 회피, 점프 기능을 제공되고 이 기능들은 지구력을 공격과 함께 지구력을 소모하게 됩니다. 그 외 Lock On과 포션 섭취, Interaction Manager에 의해 NPC 혹은 사물과 상호 작용, 마지막으로 Inventory Manager와 Trade Manager에 의한 아이템 관리 및 거래가 구현되어 있습니다.
[1. Init Vender Settings](#Init-Vender-Settings)   
[2. Interaction](#Interaction)   
[3. Combat](#Combat)   
[4. Confront](#Confront)   
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
     <img src="https://github.com/yolong1020/EldenRing/assets/87303898/905ecb30-ab7d-41e7-a224-3fc5ba8f397f" width="49.5%" height="49.5%"></img>
     https://github.com/yolong1020/EldenRing/blob/6b38e1c98a77f94b902d5b61e34b656e4f60da7f/Source/EldenRing/Private/UI/PopUp/InteractionPopUp.cpp#L99-L112
     https://github.com/yolong1020/EldenRing/blob/06d5ac00a61008476953dfcb67f150318bbdba29/Source/EldenRing/Private/System/NPCInteractionManager.cpp#L112-L136

     1. ###### StartCommunicate는 NPC와 대화만 가능한 상호 작용 UI를 활성합니다.
        https://github.com/yolong1020/EldenRing/blob/06d5ac00a61008476953dfcb67f150318bbdba29/Source/EldenRing/Private/System/NPCInteractionManager.cpp#L77-L92
        
     2. ###### StartOptionCommunicate는 NPC와 대화뿐만 아니라 거래가 가능한 상호 작용 UI를 활성합니다.
        https://github.com/yolong1020/EldenRing/blob/06d5ac00a61008476953dfcb67f150318bbdba29/Source/EldenRing/Private/System/NPCInteractionManager.cpp#L94-L110

  3. ###### 사용자 조작이 실행되면 상인에게 등록된 연출 카메라로 전환을 실행 후 다이얼로그를 활성화합니다.
     https://github.com/yolong1020/EldenRing/blob/7454685f6f5655769f78f88afbd5babf3fad7702/Source/EldenRing/Private/UI/Communicate/CommunicateCallOut.cpp#L62-L97 </br>

     
     
## Detect Bound
###### 캐릭터를 중심으로 3개의 영역을 통해 적을 감지하여 추격과 공격, 대치 상황을 결정합니다.
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/56efd8a8-b65f-4cc3-97b6-87934c75724a" width="49.5%" height="49.5%"></img>   
  
  - ###### Red   : 범위 내에 적이 위치하고 PawnSeen에 감지되었다면 추격을 진행하며 범위를 벗어나는 순간 추격이 중단됩니다.
  - ###### Blue  : 범위 내에 적이 위치하고 대치 상황이라면 현 상황을 이어갑니다. 범위를 벗어난다면 추격을 진행하게 됩니다.
  - ###### Black : 범위 내에 적이 위치한다면 공격을 실행할 수 있습니다.
</br>


## Combat
###### 전투 태세에 돌입하면 추격을 진행한 후 일반 공격, 강 공격, 대쉬 공격 중 선택하여 공격을 시도합니다. 이후 이전 공격에 따라 차기 공격을 이어서 전개합니다.
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/69093a96-9670-496f-a775-234e8d35a1be" width="49.5%" height="49.5%"></img>
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/481f1e8c-f422-4610-bc70-976d104b8486" width="49.5%" height="49.5%"></img>   
  </br>

  1. ###### 적이 있는 위치까지 도달한 경우, 공격을 시도합니다.
     https://github.com/yolong1020/EldenRing/blob/9f94c6c33a0fd357d99e4a230b46e7b57cd2c3e8/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L702-L716 </br>
  2. ###### Anim Notify - OnNextAttack 호출로 차기 공격이 가능하다면 3가지 조건(1. 이전 공격 성공 / 2.공격 범위 내 대상 존재 / 3. 무작위 추가 공격) 중 하나라도 해당할 때 다음 공격을 이어 진행합니다.
     <img src="https://github.com/yolong1020/EldenRing/assets/87303898/a1fce18b-87c9-4e06-b9de-3e4ef4a5c019"></img>
     https://github.com/yolong1020/EldenRing/blob/7795ef6b643fb2224b25a0332ff301d7ef567cfb/Source/EldenRing/Private/Characters/NPC/GameCharacter_AnimInstance.cpp#L135-L139
     https://github.com/yolong1020/EldenRing/blob/17577c2e55ffe4bc18ee74eb15d494718b2eff2c/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L718-L741
     https://github.com/yolong1020/EldenRing/blob/17577c2e55ffe4bc18ee74eb15d494718b2eff2c/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L515-L535 </br>
  3. ###### 차기 공격이 불가능하거나 중지됐다면, 공격을 종료하고 현재 병사의 상태와 적의 범위 내 위치를 확인하여 다음 행동을 결정합니다.
     https://github.com/yolong1020/EldenRing/blob/d44c1adb3ed052a712606a23e5945b0cf78561ee/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L743-L753 </br>


## Confront
###### 대치 태세에 돌입하면 앞 / 뒤 / 좌 / 우 방향으로 움직이며 적을 주시합니다.</br>이때 적이 속한 영역(추격 / 경계 / 공격)이 변경되거나 무작위로 차기 행동이 공격으로 결정되면 Combat 과정이 실행됩니다. 
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/e73c97ba-ac30-4e54-9001-1cfa633f3d00" width="49.5%" height="49.5%"></img>
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/79c26ec6-f65e-40b3-84ff-a35d74f3eed0" width="49.5%" height="49.5%"></img>   
  </br>

  1. ###### 대치 태세 돌입과 함께 4방향 중 무작위로 한 방향을 선정하여 이동을 진행하며, 다음 행동을 선택하기까지 타이머를 설정합니다.
     https://github.com/yolong1020/EldenRing/blob/7a1a74ed948d9dc7180165b7ba28291389b92dd1/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L388-L425 </br>
  2. ###### 추가 대치와 공격 중 차기 행동을 결정하여 진행합니다.
     https://github.com/yolong1020/EldenRing/blob/13da7ea0c6d228f2be811e1b79f300a0fa977c83/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L535-L564 </br>

