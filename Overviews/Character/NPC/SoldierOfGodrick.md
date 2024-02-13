# Soldier of Godrick
#### * Change Battle Mode
  > 병사가 피격받거나 혹은 적을 발견했을시, 전투 태세로 변경합니다.
  </br>
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/e0b2874b-5ffb-4518-9a47-ce553fb4c4b7" width="49.5%" height="49.5%"></img>
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/9b454323-8dab-439c-a768-69dcaf978844" width="49.5%" height="49.5%"> 

  ###### [Step 01 - 01] 에디터에서 설정한 정찰 시간만큼 타이머를 설정하고 캐릭터와 목적지 방향의 일치 여부에 따라 방향 전환 또는 전진을 진행합니다.
  https://github.com/yolong1020/EldenRing/blob/954786d152d35815853399236b731258e0fe9d88/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L243-L258
  ###### [Step 01 - 02] 에디터에서 설정한 정찰 시간만큼 타이머를 설정하고 캐릭터와 목적지 방향의 일치 여부에 따라 방향 전환 또는 전진을 진행합니다.
  https://github.com/yolong1020/EldenRing/blob/954786d152d35815853399236b731258e0fe9d88/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L65-L86
  https://github.com/yolong1020/EldenRing/blob/954786d152d35815853399236b731258e0fe9d88/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L770-L789
  </br>
  https://github.com/yolong1020/EldenRing/blob/233c4f3a063e5d102b1eb215b4d19cca4fc687ae/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L314-L326
  https://github.com/yolong1020/EldenRing/blob/73244f3847390d239d978cd85bed74aa4f8cc33b/Source/EldenRing/Private/Characters/NPC/GameCharacter_AnimInstance.cpp#L117-L121
  https://github.com/yolong1020/EldenRing/blob/ff8ab74441625804f894632f27d72827a478767a/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L665-L703
  </br>
---
#### * Assembly Point
  > 캐릭터 에디터에서 설정된 정찰 시간 이후에 배정된 집결지에서 휴식을 취하며 타 NPC와 정찰을 교대합니다.
  </br>
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/40a91a11-3099-44c4-bbec-99b3f48f7f46" width="49.5%" height="49.5%"></img>
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/25a3b779-5c6a-4efc-a22b-b287428e12e5" width="49.5%" height="49.5%"> 

  ###### [Step 01] 에디터에서 Actor에 집결지를 등록시켰다면 정찰 이후 휴식을 요청할 수 있도록 Event를 등록합니다.
  https://github.com/yolong1020/EldenRing/blob/6506fdbd5ceb2d4ac179de25628c3aefc26d5c90/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L93-L96
  https://github.com/yolong1020/EldenRing/blob/6506fdbd5ceb2d4ac179de25628c3aefc26d5c90/Source/EldenRing/Private/FieldObject/AssemblyPoint/AssemblyPoint.cpp#L76-L92
  </br>
  ###### [Step 02] 정찰 중 타이머가 종료되면 소속된 집결지에 휴식을 요청합니다.
  https://github.com/yolong1020/EldenRing/blob/36045d4e07d39f0f9d77df92cd67cdad0a472381/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L98-L102
  https://github.com/yolong1020/EldenRing/blob/0ab9d78cbdc2e640e78fec7b6f2f88c9b0b30b2d/Source/EldenRing/Private/FieldObject/AssemblyPoint/AssemblyPoint.cpp#L47-L53
  </br>
  ###### [Step 03] 집결지에 연결된 휴식처는 예약되거나 사용중인 상태를 확인하고 집결지에 Callback을 보내거나 혹은 대기합니다.
  https://github.com/yolong1020/EldenRing/blob/d7daa2f1c786f453d7356ce52d4ccedaeb59806f/Source/EldenRing/Private/FieldObject/AssemblyPoint/AssemblePointObject.cpp#L65-L83
  </br>
  ###### [Step 04 - 01] 휴식처에서 이용이 가능하다 판단이되면 집결지에 휴식처 사용 요청을 전달합니다.
  https://github.com/yolong1020/EldenRing/blob/99ec7e873cf0fdcc9b40f3d243c9fbd3dc5e399e/Source/EldenRing/Private/FieldObject/AssemblyPoint/AssemblyPoint.cpp#L55-L70
  ###### [Step 04 - 02] 만약 휴식중이던 NPC가 있다면 휴식 종료를 명령합니다.
  https://github.com/yolong1020/EldenRing/blob/21f7a92373efa785afd98093886e1999363acb96/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L400-L407
  ###### [Step 04 - 03] 휴식을 예약한 NPC는 휴식처로 이동 진행과 상태를 변경합니다.
  https://github.com/yolong1020/EldenRing/blob/d10785f149dba33bd42fc35a94303d3e6a9a9410/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L280-L292
  </br>
  ###### [Step 05] 휴식처에 도달하면 집결지와 방향을 확인한 뒤 휴식을 진행합니다. 이후 휴식처에서 사용 시간을 기록합니다.
  https://github.com/yolong1020/EldenRing/blob/bf02f644eb1fa4c8f7d13f3891d9a7f980e9b794/Source/EldenRing/Private/FieldObject/AssemblyPoint/AssemblePointObject.cpp#L48-L63
  https://github.com/yolong1020/EldenRing/blob/bf02f644eb1fa4c8f7d13f3891d9a7f980e9b794/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L461-L487
  </br>
