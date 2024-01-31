#### 1. GameCharacter
* Init Player Settings
  > HUD, Camera Timeline, Item 초기화.
  ###### Step 01. 현 Level의 플레이 가능 여부와 Load 상태 확인
  https://github.com/yolong1020/EldenRing/blob/bc151a656f7846c367529f4340f0c87f574dcde0/Source/EldenRing/Private/Characters/Players/C0000.cpp#L138-L151
  ###### Step 02. Tag 부여, 맴버 변수, HUD, LockOn Collision 등 초기화
  https://github.com/yolong1020/EldenRing/blob/bc151a656f7846c367529f4340f0c87f574dcde0/Source/EldenRing/Private/Characters/Players/C0000.cpp#L153-L187
  ###### Step 03. 신규 게임 시작에 따른 Item 초기화
  ```C++
  /// [BeginPlay] 189
  instance->IsNewGame() ? SetDefaultItems() : instance->LoadPlayer(this);
  ```
  https://github.com/yolong1020/EldenRing/blob/bc151a656f7846c367529f4340f0c87f574dcde0/Source/EldenRing/Private/Characters/Players/C0000.cpp#L78-L111
  https://github.com/yolong1020/EldenRing/blob/bc151a656f7846c367529f4340f0c87f574dcde0/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L52-L73
  ###### Step 04. Level의 Load 상태에 따른 캐릭터 시작 애니메이션 재생 및 Input 활성
  https://github.com/yolong1020/EldenRing/blob/bc151a656f7846c367529f4340f0c87f574dcde0/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1595-L1622
