# NPC
###### Player를 제외한 모든 캐릭터들의 Base Class이며, 특정 오브젝트를 집결지로 등록하고 에디터에서 정해진 정찰 지점들을 순회하는 정찰 기능을 포함하고 있습니다.</br>일반(Repose) / 경계(Vigilance) 상황을 구분하며, Health Bar Widget을 제공합니다.
[1. Init Default Component & Values](#Init-Default-Component-&-Values)   
[2. Patrol](#Patrol)   
[3. Assembly Point](#Assembly-Point)   
</br>


## Init Default Component & Values
  ###### NPC에 해당하는 모든 캐릭터의 Component와 초기값을 설정합니다.
  </br>

  1.
     1. Health Bar와 PawnSensing Component, 그리고 Level의 로드 상태에 따라 Spawn 위치를 초기화합니다.
     https://github.com/yolong1020/EldenRing/blob/e6bc14be5355ea76a3c64fa6c43ca390684d1d26/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L105-L130
     https://github.com/yolong1020/EldenRing/blob/6e79f5f9faabbc8e9469d0ce1b7bdebdd020a588/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L151-L154 </br>
     2. Level의 로드가 완료되지 않았다면, Level의 로드 완료 시점에 호출될 Event로 등록해줍니다.
     https://github.com/yolong1020/EldenRing/blob/ec2bd163de2ae2409d90b577fef49e66b5eb3700/Source/EldenRing/Public/GameBase/ERGameInstance.h#L35-L39
     https://github.com/yolong1020/EldenRing/blob/3e40e9ea14935ca05790f050c704db9ef6d152f8/Source/EldenRing/Private/Levels/GameField.cpp#L65-L108 </br>


## Patrol
  ###### 에디터에서 지정된 patrol Point들을 정찰 구역으로 설정하여 NPC가 순회 정찰을 진행합니다.
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/40e9e4ee-a3ad-4f68-8f2e-2b401f9d0a03" width="49.5%" height="49.5%"></img>
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/e8abfa97-d34c-4d93-8ef5-19489a25dca0" width="49.5%" height="49.5%"></img>   
  </br>
  
  1. 에디터에서 설정한 정찰 시간만큼 타이머를 설정하고 캐릭터와 목적지 방향의 일치 여부에 따라 방향 전환 또는 전진을 진행합니다.
  https://github.com/yolong1020/EldenRing/blob/9df4da650f57f5ce2e5188f5e43b8a6efd15b771/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L426-L438
  https://github.com/yolong1020/EldenRing/blob/782c932b733fb24e3fea2aafafa5399941292032/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L301-L308
  </br>
  2.
     1. 전진하는 경우, AI Controller - MoveToLocation을 실행합니다.
     https://github.com/yolong1020/EldenRing/blob/5a8b244925fe1a501707bdec3c12b2b2de467878/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L525-L542
     2. 방향 전환을 한 경우, 모션이 끝나는 시점에서 Anim Notify에 의해 OnTurnEnd()을 호출 후 목적지까지 전진을 실행합니다.
     https://github.com/yolong1020/EldenRing/blob/5a8b244925fe1a501707bdec3c12b2b2de467878/Source/EldenRing/Private/Characters/NPC/GameCharacter_AnimInstance.cpp#L153-L168 </br>
  3. 각 자식 클래스에서 목적지에 도달했을 때, 처리할 행동들을 결정해줍니다. (ex : C4311)
  https://github.com/yolong1020/EldenRing/blob/3dfcb18ab1d642012182c9589f30a13c399d12cc/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L721-L735
  https://github.com/yolong1020/EldenRing/blob/5a8b244925fe1a501707bdec3c12b2b2de467878/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L305-L319 </br>


## Assembly Point
  ###### 캐릭터 에디터에서 설정된 정찰 시간 이후에 배정된 집결지에서 휴식을 취하며 타 NPC와 정찰을 교대합니다.
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
