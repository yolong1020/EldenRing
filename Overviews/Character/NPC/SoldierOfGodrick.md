# Soldier of Godrick
#### * Change Battle Mode
  > 정찰 혹은 휴식 중 전투 상황이 발생하면 NPC가 전투 태세로 전환합니다.
  </br>
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/e0b2874b-5ffb-4518-9a47-ce553fb4c4b7" width="49.5%" height="49.5%"></img>
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/9b454323-8dab-439c-a768-69dcaf978844" width="49.5%" height="49.5%"> 

  ###### [Step 01 - 01] 비전투 상황에서 적(플레이어)이 PawnSeen에 감지되면 전투 태세로 전환합니다.
  https://github.com/yolong1020/EldenRing/blob/954786d152d35815853399236b731258e0fe9d88/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L243-L258
  ###### [Step 01 - 02] 비전투 상황에서 적(플레이어)에게 피격받게되면 피격 모션을 재생한 뒤 전투 태세로 전환합니다.
  https://github.com/yolong1020/EldenRing/blob/954786d152d35815853399236b731258e0fe9d88/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L65-L86
  https://github.com/yolong1020/EldenRing/blob/954786d152d35815853399236b731258e0fe9d88/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L770-L789
  </br>
  ###### [Step 02] 전투 상황에 돌입하면 보유한 무기를 꺼내며 전투를 준비합니다.
  https://github.com/yolong1020/EldenRing/blob/233c4f3a063e5d102b1eb215b4d19cca4fc687ae/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L314-L326
  </br>
  ###### [Step 03] Anim Notify의 호출로 전투 준비가 마무리되면 목표와 방향을 확인한 후 돌격하거나 혹은 방향 전환을 시도합니다.
  https://github.com/yolong1020/EldenRing/blob/73244f3847390d239d978cd85bed74aa4f8cc33b/Source/EldenRing/Private/Characters/NPC/GameCharacter_AnimInstance.cpp#L117-L121
  https://github.com/yolong1020/EldenRing/blob/ff8ab74441625804f894632f27d72827a478767a/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L665-L703
  </br>
---
#### * Combat
  > 캐릭터 에디터에서 설정된 정찰 시간 이후에 배정된 집결지에서 휴식을 취하며 타 NPC와 정찰을 교대합니다.
  </br>
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/40a91a11-3099-44c4-bbec-99b3f48f7f46" width="49.5%" height="49.5%"></img>
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/25a3b779-5c6a-4efc-a22b-b287428e12e5" width="49.5%" height="49.5%"> 

  ###### [Step 01] 에디터에서 Actor에 집결지를 등록시켰다면 정찰 이후 휴식을 요청할 수 있도록 Event를 등록합니다.
  https://github.com/yolong1020/EldenRing/blob/6506fdbd5ceb2d4ac179de25628c3aefc26d5c90/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L93-L96
  https://github.com/yolong1020/EldenRing/blob/6506fdbd5ceb2d4ac179de25628c3aefc26d5c90/Source/EldenRing/Private/FieldObject/AssemblyPoint/AssemblyPoint.cpp#L76-L92
  </br>
