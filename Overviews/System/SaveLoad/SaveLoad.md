# Save & Load
[1. Init tutorial](#Init-tutorial)   
[2. Open by VolumeBox](#Open-by-VolumeBox)   
</br>


## Open Game Level
###### .
<img src="https://github.com/yolong1020/EldenRing/assets/87303898/743546f9-9289-4582-8b8f-83112424e151" width="49.5%" height="49.5%"></br>

  1. ###### Menu Button에 바인딩 된 기능으로 화면을 Fade out으로 전환합니다. 이때 Fade out이 완전히 이뤄지면 GameInstance를 통해 Level을 이전합니다.
     https://github.com/yolong1020/EldenRing/blob/39634ee819d0714303cc4d5af97fa5f35337fb3d/Source/EldenRing/Private/UI/Button/MenuButton.cpp#L105-L135
     https://github.com/yolong1020/EldenRing/blob/4d117987abadea10455cbc55d59209165b2cac92/Source/EldenRing/Private/UI/Common/BlackOut.cpp#L23-L29
     https://github.com/yolong1020/EldenRing/blob/4d117987abadea10455cbc55d59209165b2cac92/Source/EldenRing/Private/UI/Common/BlackOut.cpp#L48-L64
     https://github.com/yolong1020/EldenRing/blob/022c9b492339c4f6fc691983b4580ca716efd9b6/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L25-L42
  2. ###### Level이 전환되면서 정리된 Fade out을 다시 활성시키고 연출이 종료되는 시점에 Loading UI를 활성하도록 합니다.</br>이후 Fade in 연출을 진행시키고 연출이 종료되면 Data Layer를 불러옵니다.
     https://github.com/yolong1020/EldenRing/blob/9c322e6ec78552f56cef627a650794c96b7b23b7/Source/EldenRing/Private/Levels/GameField.cpp#L25-L58
     https://github.com/yolong1020/EldenRing/blob/bbcdebbdb8264787bf79a3faf1892a23dbd867ed/Source/EldenRing/Private/Levels/BaseLevelScript.cpp#L24-L45
     1. ###### Landscape는 스테이지 배경을 묶어놓은 Data Layer로 신규 게임 여부와 관계없이 항상 불러옵니다.
        <img src="https://github.com/yolong1020/EldenRing/assets/87303898/b327af62-7bcc-4477-a596-86dd4cc9b93e" width="49.5%" height="49.5%"></img>
     3. ###### L
        <img src="https://github.com/yolong1020/EldenRing/assets/87303898/6e3e2dbf-ba72-4477-aa4b-50f12958bf88" width="49.5%" height="49.5%"></img>
        
     https://github.com/yolong1020/EldenRing/blob/46dcac86b62ef35ece11449933ad06c8def74194/Source/EldenRing/Private/UI/Loading/Loading.cpp#L26-L72
     https://github.com/yolong1020/EldenRing/blob/adec67bd5096a1d3cb514c26fcbd2cacbd72ac66/Source/EldenRing/Private/Levels/GameField.cpp#L17-L21
     https://github.com/yolong1020/EldenRing/blob/adec67bd5096a1d3cb514c26fcbd2cacbd72ac66/Source/EldenRing/Private/Levels/GameField.cpp#L58-L101
     https://github.com/yolong1020/EldenRing/blob/022c9b492339c4f6fc691983b4580ca716efd9b6/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L19-L23
     </br>
     ```C++
     ///  [AC0000::BeginPlay] 210
     const bool load_complete = instance->IsLevelLoadComplete();
     load_complete ? GameStartAction() : instance->AddEventLevelLoadComplete(this, &AC0000::GameStartAction);
     ```
  4.
     - ###### C
       <img src="https://github.com/yolong1020/EldenRing/assets/87303898/1900ad25-252d-4830-b0f7-adba3508fedf" width="49.5%" height="49.5%"></br>
       ```C++
       const bool load_complete = instance->IsLevelLoadComplete();
       load_complete ? GameStartAction() : instance->AddEventLevelLoadComplete(this, &AC0000::GameStartAction);
       ```
       https://github.com/yolong1020/EldenRing/blob/923edcec6840de011a514f9d695daa7c1bfb142c/Source/EldenRing/Private/System/UIManager.cpp#L161-L167
       https://github.com/yolong1020/EldenRing/blob/159c4f8797d3484c2aedf8094cba9c9dcca11b96/Source/EldenRing/Private/UI/Tutorial/TutorialCallout.cpp#L22-L28 </br>
       
     - ###### G
       <img src="https://github.com/yolong1020/EldenRing/assets/87303898/e60e4b7e-3bfb-46fc-b7bd-a5048bb759e2" width="49.5%" height="49.5%"></br>
       ```C++
       ///  [AGameField::StartStage] 96
       TArray<FTutorialPersistentData> datas;
       FTutorialPersistentData Dodge(FName("Dodge"), EInputActionType::EIAT_Dodge, ETutorialPosition::ETP_Top, true);
       datas.Add(Dodge);
       FTutorialPersistentData Sprint(FName("Sprint(Hold)"), EInputActionType::EIAT_Sprint, ETutorialPosition::ETP_Mid, true);
       datas.Add(Sprint);
       FTutorialPersistentData Walk(FName("Walk(Hold)"), EInputActionType::EIAT_Walk, ETutorialPosition::ETP_Bot, true);
       datas.Add(Walk);
	
       ui_mgr->OpenTutorialGroupCall(datas);
       ```
       https://github.com/yolong1020/EldenRing/blob/ede3beb90bdd83522042c45519b1ba995fa21a15/Source/EldenRing/Private/System/UIManager.cpp#L147-L152
       https://github.com/yolong1020/EldenRing/blob/a240d8f8a44e008507d892e5220b1ca4b62398c8/Source/EldenRing/Private/UI/Tutorial/TutorialGroupCall.cpp#L17-L53 </br>
       
  5. ###### 전달받은 텍스트로 안내 텍스트를 설정하고 Input Action Enum으로 데이터를 탐색하여 안내에 사용할 키를 찾고 변경합니다.</br>입력해야할 키의 크기로 위젯의 Icon활성 여부를 다르게 합니다.
     <img src="https://github.com/yolong1020/EldenRing/assets/87303898/83fea0c0-5363-4016-a3ad-8b57598e8847" width="50%" height="50%">
     https://github.com/yolong1020/EldenRing/blob/aa4a37dbb8b0598e6af7a4e16b9aac586ddf1ab8/Source/EldenRing/Private/UI/Tutorial/TutorialHUD.cpp#L19-L36 </br>


## Open by VolumeBox
###### 에디터에서 VolumeBox를 배치하여 캐릭터가 특정 영역에서 이벤트를 호출하는 방식으로 구성된 튜토리얼입니다.
<img src="https://github.com/yolong1020/EldenRing/assets/87303898/a912a8f3-a244-4183-8550-b14171d631fe"></img>   
</br>

  1. ###### 충돌이 감지되면 동일한 튜토리얼 팝업이 활성되어 있는지 확인 후 현재 활성되어있는 모든 튜토리얼 팝업을 종료시킵니다. 
     https://github.com/yolong1020/EldenRing/blob/4770417759e1bab38bef25cdf85db8bba8a8bd0b/Source/EldenRing/Private/UI/Tutorial/TutorialVolumeBox_CallOut.cpp#L45-L53
     https://github.com/yolong1020/EldenRing/blob/8e2c6fe9264d451dc78b1fd4b6f73f23485b14ae/Source/EldenRing/Private/System/UIManager.cpp#L183-L194 </br>
  2. ###### UI Manager에 팝업 활성을 요청하고 VolumeBox와 팝업을 비활성화시킬 타이머를 설정합니다.
     <img src="https://github.com/yolong1020/EldenRing/assets/87303898/6444c18c-1e0c-46be-bc08-7aa6d6a71734" width="49.5%" height="49.5%"></br>
     https://github.com/yolong1020/EldenRing/blob/4770417759e1bab38bef25cdf85db8bba8a8bd0b/Source/EldenRing/Private/UI/Tutorial/TutorialVolumeBox_CallOut.cpp#L73-L80
     https://github.com/yolong1020/EldenRing/blob/4770417759e1bab38bef25cdf85db8bba8a8bd0b/Source/EldenRing/Private/UI/Tutorial/TutorialVolumeBox_CallOut.cpp#L34-L43 </br>
  3. ###### 설정한 타이머와 무관하게 튜토리얼에서 제시하는 행동을 완료했다면, Callout을 찾아 비활성시킬 수 있습니다.
     <img src="https://github.com/yolong1020/EldenRing/assets/87303898/7df246bf-2d01-4563-ba6b-c7fe9c9f53bc" width="49.5%" height="49.5%"></br>
     https://github.com/yolong1020/EldenRing/blob/8e2c6fe9264d451dc78b1fd4b6f73f23485b14ae/Source/EldenRing/Private/Characters/Players/C0000.cpp#L581-L592 </br>
