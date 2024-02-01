# Player
#### * Init Player Settings
  > Level 상태에 따라서 HUD, Camera Timeline, 플레이에 필요한 Item 등 초기화합니다.   

  ###### Step 01. 현 Level의 플레이 가능 여부와 Load 상태에 따른 초기화 진행
  https://github.com/yolong1020/EldenRing/blob/a021831dab5f79b7e6faff2f2ab6fc9c5b5b9216/Source/EldenRing/Private/Characters/Players/C0000.cpp#L136-L212
  ###### Step 02. 신규 게임 시작에 따른 Item 초기화
  ```C++
  /// [BeginPlay] 189
  instance->IsNewGame() ? SetDefaultItems() : instance->LoadPlayer(this);
  ```
  https://github.com/yolong1020/EldenRing/blob/bc151a656f7846c367529f4340f0c87f574dcde0/Source/EldenRing/Private/Characters/Players/C0000.cpp#L78-L111
  https://github.com/yolong1020/EldenRing/blob/0f9888e552ddbf77796e6e03775caa458ebac7fe/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L75-L133
  ###### Step 03. Level의 Load 상태에 따른 캐릭터 시작 애니메이션 재생 및 Input 활성
  https://github.com/yolong1020/EldenRing/blob/bc151a656f7846c367529f4340f0c87f574dcde0/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1595-L1622
  
---
#### * Combat
  > HUD, Camera Timeline, Item 등 초기화.
  ###### Step 01. 현 Level의 플레이 가능 여부와 Load 상태에 따른 초기화 진행
---
#### * Movement
  > HUD, Camera Timeline, Item 등 초기화.
  ###### Step 01. 현 Level의 플레이 가능 여부와 Load 상태에 따른 초기화 진행
---
#### * Potion Comsume
  > HUD, Camera Timeline, Item 등 초기화.
  ###### Step 01. 현 Level의 플레이 가능 여부와 Load 상태에 따른 초기화 진행
---
#### * Item Storage
  > HUD, Camera Timeline, Item 등 초기화.
  ###### Step 01. 현 Level의 플레이 가능 여부와 Load 상태에 따른 초기화 진행
---
#### * Item Trade
  > HUD, Camera Timeline, Item 등 초기화.
  ###### Step 01. 현 Level의 플레이 가능 여부와 Load 상태에 따른 초기화 진행
