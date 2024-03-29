# Soldier of Godrick
[1. Change Battle Mode](#Change-Battle-Mode)   
[2. Detect Bound](#Detect-Bound)   
[3. Combat](#Combat)   
[4. Confront](#Confront)   
</br>


## Change Battle Mode
###### 정찰 혹은 휴식 중 전투 상황이 발생하면 NPC가 전투 태세로 전환합니다.  

  1.
     1. ###### 비전투 상황에서 적(플레이어)이 PawnSeen에 감지되면 전투 태세로 전환합니다.
        <img src="https://github.com/yolong1020/EldenRing/assets/87303898/e0b2874b-5ffb-4518-9a47-ce553fb4c4b7" width="49.5%" height="49.5%"></img>
        https://github.com/yolong1020/EldenRing/blob/954786d152d35815853399236b731258e0fe9d88/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L243-L258 </br>
     3. ###### 비전투 상황에서 적(플레이어)에게 피격받게 되면 피격 동작을 재생한 뒤 전투 태세로 전환합니다.
        <img src="https://github.com/yolong1020/EldenRing/assets/87303898/9b454323-8dab-439c-a768-69dcaf978844" width="49.5%" height="49.5%"></img>
        https://github.com/yolong1020/EldenRing/blob/954786d152d35815853399236b731258e0fe9d88/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L65-L86
        https://github.com/yolong1020/EldenRing/blob/954786d152d35815853399236b731258e0fe9d88/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L770-L789 </br>
  2. ###### 전투 상황에 돌입하면 보유한 무기를 꺼내며 전투를 준비합니다.
     https://github.com/yolong1020/EldenRing/blob/233c4f3a063e5d102b1eb215b4d19cca4fc687ae/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L314-L326 </br>
  3. ###### Anim Notify의 호출로 전투 준비가 마무리되면 목표와 방향을 확인한 후 돌격하거나 혹은 방향 전환을 시도합니다.
     <img src="https://github.com/yolong1020/EldenRing/assets/87303898/a834a35b-8d8d-4612-97fb-565b1ef47b85"></img>
     https://github.com/yolong1020/EldenRing/blob/73244f3847390d239d978cd85bed74aa4f8cc33b/Source/EldenRing/Private/Characters/NPC/GameCharacter_AnimInstance.cpp#L117-L121
     https://github.com/yolong1020/EldenRing/blob/ff8ab74441625804f894632f27d72827a478767a/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L665-L703 </br>


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
