# Level
[1. Open Game Level](#Open-Game-Level)   
[2. Restart Level](#Restart-Level)   
</br>


## Open Game Level
###### 클라이언트를 실행하여 게임 필드에 진입할때 신규 게임의 유무에 따라 Data Layer를 다르게 불러와 게임을 설정합니다. 
<img src="https://github.com/yolong1020/EldenRing/assets/87303898/743546f9-9289-4582-8b8f-83112424e151" width="49.5%" height="49.5%"></br>

  1. ###### Menu Button에 바인딩 된 기능으로 화면을 Fade out으로 전환합니다. 이때 Fade out이 완전히 이뤄지면 GameInstance를 통해 Level을 이전합니다.
     https://github.com/yolong1020/EldenRing/blob/39634ee819d0714303cc4d5af97fa5f35337fb3d/Source/EldenRing/Private/UI/Button/MenuButton.cpp#L105-L135
     https://github.com/yolong1020/EldenRing/blob/4d117987abadea10455cbc55d59209165b2cac92/Source/EldenRing/Private/UI/Common/BlackOut.cpp#L23-L29
     https://github.com/yolong1020/EldenRing/blob/4d117987abadea10455cbc55d59209165b2cac92/Source/EldenRing/Private/UI/Common/BlackOut.cpp#L48-L64
     https://github.com/yolong1020/EldenRing/blob/022c9b492339c4f6fc691983b4580ca716efd9b6/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L25-L42 </br>
     
  2. ###### Level이 전환되면서 정리된 Fade out을 다시 활성시키고 연출이 종료되는 시점에 Loading UI를 활성하도록 합니다.</br>이후 Fade in 연출을 진행시키고 연출이 종료되면 Data Layer를 불러옵니다.
     https://github.com/yolong1020/EldenRing/blob/9c322e6ec78552f56cef627a650794c96b7b23b7/Source/EldenRing/Private/Levels/GameField.cpp#L25-L58
     https://github.com/yolong1020/EldenRing/blob/bbcdebbdb8264787bf79a3faf1892a23dbd867ed/Source/EldenRing/Private/Levels/BaseLevelScript.cpp#L24-L45
     1. ###### Landscape는 스테이지 배경을 묶어놓은 Data Layer로 신규 게임 여부와 관계없이 항상 불러옵니다.
        <img src="https://github.com/yolong1020/EldenRing/assets/87303898/b327af62-7bcc-4477-a596-86dd4cc9b93e" width="49.5%" height="49.5%"></img>
     2. ###### DefaultObject는 최초 게임을 실행했을 때, 플레이어를 제외한 모든 게임 오브젝트들이 속한 Data Layer입니다.
        <img src="https://github.com/yolong1020/EldenRing/assets/87303898/6e3e2dbf-ba72-4477-aa4b-50f12958bf88" width="49.5%" height="49.5%"></img>
     3. ###### FieldObjectData는 저장된 게임을 실행했을 때, 플레이어와 NPC, 파괴 가능한 오브젝트를 제외한 나머지들이 속한 Data Layer입니다.
        <img src="https://github.com/yolong1020/EldenRing/assets/87303898/7ceb9279-1d30-48ab-a11f-f12c6b784413" width="49.5%" height="49.5%"></img>
        </br>

  3. ###### Loading UI는 진행 상황을 갱신하고 완료되면 Fade out 연출과 함께 비활성화 시킵니다. 이후 게임을 시작할 Timer를 준비합니다.
     https://github.com/yolong1020/EldenRing/blob/46dcac86b62ef35ece11449933ad06c8def74194/Source/EldenRing/Private/UI/Loading/Loading.cpp#L26-L72
     https://github.com/yolong1020/EldenRing/blob/adec67bd5096a1d3cb514c26fcbd2cacbd72ac66/Source/EldenRing/Private/Levels/GameField.cpp#L17-L21 </br>

  4. ###### 타이머 설정 시간 이후 플레이어를 필드에 소환하고 만약 신규 게임이 아니라면, 저장된 오브젝트들을 소환하도록 합니다.</br>이후 Level Load Complete Event를 호출하여 각 오브젝트들이 필요한 설정들을 진행할 수 있도록 합니다.
     https://github.com/yolong1020/EldenRing/blob/adec67bd5096a1d3cb514c26fcbd2cacbd72ac66/Source/EldenRing/Private/Levels/GameField.cpp#L58-L101
     https://github.com/yolong1020/EldenRing/blob/022c9b492339c4f6fc691983b4580ca716efd9b6/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L19-L23
     ```C++
     ///  [AC0000::BeginPlay] 210
     const bool load_complete = instance->IsLevelLoadComplete();
     load_complete ? GameStartAction() : instance->AddEventLevelLoadComplete(this, &AC0000::GameStartAction);
     ```
     </br>


## Restart Level
###### 플레이어가 사망 후 게임을 다시 시작하도록 
<img src="https://github.com/yolong1020/EldenRing/assets/87303898/865ae562-b195-4e21-afd6-a75187b99103" width="49.5%" height="49.5%"></br>

  1. ###### GameCharacter의 사망 연출이 종료되면 UninitAndDestroy 과정을 진행하고 이후 GameMode에 게임을 다시 시작하도록 요청합니다.
     https://github.com/yolong1020/EldenRing/blob/2e31075e7eb907ff42a181d5348179df51fadf97/Source/EldenRing/Private/Characters/Players/C0000.cpp#L400-L415
     https://github.com/yolong1020/EldenRing/blob/2e31075e7eb907ff42a181d5348179df51fadf97/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1222-L1229
     https://github.com/yolong1020/EldenRing/blob/a0d7366564eba9f62fe294702066f0320fe39bcd/Source/EldenRing/Private/GameBase/ERGameMode.cpp#L71-L90
     https://github.com/yolong1020/EldenRing/blob/a0d7366564eba9f62fe294702066f0320fe39bcd/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L44-L50
     
