# Player
### * Init Player Settings
  > Level 상태에 따라서 HUD, Camera Timeline, 플레이에 필요한 Item 등 초기화합니다.    

  ###### [Step 01] 현재 Level의 플레이 가능 여부와 Load 상태에 따른 초기화를 진행합니다.
  https://github.com/yolong1020/EldenRing/blob/a021831dab5f79b7e6faff2f2ab6fc9c5b5b9216/Source/EldenRing/Private/Characters/Players/C0000.cpp#L136-L212
  </br>
  ###### [Step 02] 'New Game' 혹은 'Load Game'에 따라 Item을 다르게 초기화합니다.
  ```C++
  /// [BeginPlay] 189
  instance->IsNewGame() ? SetDefaultItems() : instance->LoadPlayer(this);
  ```
  https://github.com/yolong1020/EldenRing/blob/bc151a656f7846c367529f4340f0c87f574dcde0/Source/EldenRing/Private/Characters/Players/C0000.cpp#L78-L111
  https://github.com/yolong1020/EldenRing/blob/0f9888e552ddbf77796e6e03775caa458ebac7fe/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L75-L133
  </br>
  ###### [Step 03] Level의 Load 상태에 따른 캐릭터 시작 애니메이션 재생 및 Input을 활성합니다.
  https://github.com/yolong1020/EldenRing/blob/bc151a656f7846c367529f4340f0c87f574dcde0/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1595-L1622
  </br>
  
---
### * Combat
  > 조작에 따라 단일, 이전의 공격을 바탕으로 한 콤보 공격으로 구별됩니다. 그 외 방패를 통한 방어와 반격 기능을 제공합니다.

  ###### [01] 전투 대상의 Execution Collision(Front / Back)에 Overlap 되면 플레이어는 처형 공격을 실행할 수 있습니다.
  https://github.com/yolong1020/EldenRing/blob/4e07652fec8037129c94ac1eb022e142e030ac5f/Source/EldenRing/Private/Characters/Players/C0000.cpp#L851-L880
  </br>
  ###### [02] 전력 질주와 점프, 일반 상태 그리고 강공격 Input 입력 여부에 따라 서로 다른 공격이 전개됩니다.
  https://github.com/yolong1020/EldenRing/blob/4e07652fec8037129c94ac1eb022e142e030ac5f/Source/EldenRing/Private/Characters/Players/C0000.cpp#L883-L942
  </br>
  ###### [03] Weapon_Actor의 Collision과 충돌한 대상이 캐릭터인지 방어자의 Parry Collision, 방패의 Collision인지에 따라 피격 결과를 적용시킵니다.
  https://github.com/yolong1020/EldenRing/blob/4070f5c523d417e907e46127052dd90aae241624/Source/EldenRing/Private/Items/Weapons/MeleeAttack_Actor.cpp#L72-L163
  </br>
  ###### [04] 추가 공격 유효 시간을 벗어나면 EndAttack이 호출되어 콤보, 공격 가능 상태, 캐릭터 행동 상태 등을 기본값으로 복구시킵니다.
  https://github.com/yolong1020/EldenRing/blob/2f43079d34e3767f013851c836d4480db5fd268a/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1368-L1377
  </br>
  
---
### * Movement
  > HUD, Camera Timeline, Item 등 초기화.
  ###### Step 01. 현 Level의 플레이 가능 여부와 Load 상태에 따른 초기화 진행
  https://github.com/yolong1020/EldenRing/blob/6a40836945163b30e9dd8aaf3c548b9b92d56edd/Source/EldenRing/Private/Characters/Players/C0000.cpp#L661-L745
  </br>
  ###### Step 01. 현 Level의 플레이 가능 여부와 Load 상태에 따른 초기화 진행
  https://github.com/yolong1020/EldenRing/blob/6a40836945163b30e9dd8aaf3c548b9b92d56edd/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1011-L1042
  </br>
  ###### Step 01. 현 Level의 플레이 가능 여부와 Load 상태에 따른 초기화 진행
  https://github.com/yolong1020/EldenRing/blob/6a40836945163b30e9dd8aaf3c548b9b92d56edd/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1115-L1148
  </br>
  
---
### * Potion Comsume
  > HUD, Camera Timeline, Item 등 초기화.
  ###### Step 01. 현 Level의 플레이 가능 여부와 Load 상태에 따른 초기화 진행
---
### * Item Storage
  > HUD, Camera Timeline, Item 등 초기화.
  ###### Step 01. 현 Level의 플레이 가능 여부와 Load 상태에 따른 초기화 진행
---
### * Item Trade
  > HUD, Camera Timeline, Item 등 초기화.
  ###### Step 01. 현 Level의 플레이 가능 여부와 Load 상태에 따른 초기화 진행
