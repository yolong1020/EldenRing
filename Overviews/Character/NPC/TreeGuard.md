# Tree Guard
<img src="https://github.com/yolong1020/EldenRing/assets/87303898/95c304ff-a756-47a0-8e9c-7d9f399f5720"></img>
#### * Init Tree Guard Settings
  > Tree Guard의 블랙보드와 몸통 Collision, 전투 모션 데이터 등을 초기화합니다.

  https://github.com/yolong1020/EldenRing/blob/d5f795bbce272476ba0d7c9afb9bfc4c067c4ecb/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/C3251.cpp#L29-L45
  https://github.com/yolong1020/EldenRing/blob/d5f795bbce272476ba0d7c9afb9bfc4c067c4ecb/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/C3251.cpp#L58-L145
  </br>
---
#### * Combat
  > 전투 태세에 돌입하면 추격을 진행한 후 일반 공격, 강 공격, 대쉬 공격 중 선택하여 공격을 시도합니다. 이후 이전 공격에 따라 차기 공격을 이어서 전개합니다.
  </br>
  ###### [Step 01] 적이 Pawn Seen에 의해 목격되면 보스 전투가 시작됩니다.
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/09dbc467-6faf-44e5-a86f-23624dfdca80" width="49.5%" height="49.5%"></img>
  https://github.com/yolong1020/EldenRing/blob/d5f795bbce272476ba0d7c9afb9bfc4c067c4ecb/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/C3251.cpp#L316-L331
  https://github.com/yolong1020/EldenRing/blob/a5455bfdda8acc41876678487b29fe6ab15f5433/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/C3251.cpp#L352-L368
  </br>
  ###### [Step 02] Anim Notify - OnNextAttack 호출로 차기 공격이 가능하다면 3가지 조건(1. 이전 공격이 성공한 경우 / 2.공격 범위에 대상이 있는 경우 / 3. 무작위 추가 공격)을 확인합니다. 만약 하나라도 해당하게 된다면 다음 공격을 이어 진행합니다.
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/a1fce18b-87c9-4e06-b9de-3e4ef4a5c019"></img>
  https://github.com/yolong1020/EldenRing/blob/7795ef6b643fb2224b25a0332ff301d7ef567cfb/Source/EldenRing/Private/Characters/NPC/GameCharacter_AnimInstance.cpp#L135-L139
  https://github.com/yolong1020/EldenRing/blob/17577c2e55ffe4bc18ee74eb15d494718b2eff2c/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L718-L741
  https://github.com/yolong1020/EldenRing/blob/17577c2e55ffe4bc18ee74eb15d494718b2eff2c/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L515-L535
  </br>
  ###### [Step 03] 차기 공격이 불가능하거나 중지됐다면, 공격을 종료하고 현재 병사의 상태와 적의 범위 내 위치를 확인하여 다음 행동을 결정합니다.
  https://github.com/yolong1020/EldenRing/blob/d44c1adb3ed052a712606a23e5945b0cf78561ee/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L743-L753
  </br>
---
#### * Confront
  > 대치 태세에 돌입하면 앞 / 뒤 / 좌 / 우 방향으로 움직이며 적을 주시합니다. 이때 적이 속한 영역(추격 / 경계 / 공격)이 변경되거나 무작위로 차기 행동이 공격으로 결정되면 Combat 과정이 실행됩니다. 
  </br>
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/e73c97ba-ac30-4e54-9001-1cfa633f3d00" width="49.5%" height="49.5%"></img>
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/79c26ec6-f65e-40b3-84ff-a35d74f3eed0" width="49.5%" height="49.5%"> 

  ###### [Step 01] 대치 태세 돌입과 함께 4방향 중 무작위로 한 방향을 선정하여 이동을 진행하며, 다음 행동을 선택하기까지 타이머를 설정합니다.
  https://github.com/yolong1020/EldenRing/blob/7a1a74ed948d9dc7180165b7ba28291389b92dd1/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L388-L425
  </br>
  ###### [Step 02] 추가 대치와 공격 중 차기 행동을 결정하여 진행합니다.
  https://github.com/yolong1020/EldenRing/blob/13da7ea0c6d228f2be811e1b79f300a0fa977c83/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L535-L564
  </br>
