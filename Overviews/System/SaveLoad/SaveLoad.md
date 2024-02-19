# Save & Load
[1. Save Player](#Save-Player)   
[2. Save Objects](#Save-Objects)   
[3. Load Player](#Load-Player)   
[4. Load-Objects](#Load-Objects)   
</br>


## Save Player
###### 게임을 종료하거나 체크 포인트 활성 및 아이템 거래, 사망 상황에서 저장이 발생됩니다.

   1.
      1. ###### 게임 종료에 의한 저장 : 게임을 종료할때, 팝업에 바인딩 된 기능을 이용하게 게임을 저장합니다.
         <img src="https://github.com/yolong1020/EldenRing/assets/87303898/508aa5db-3251-4e37-8720-ef8ab9de7901" width="49.5%" height="49.5%"></img>   
         </br>
         https://github.com/yolong1020/EldenRing/blob/64d9445ed597ada15df737bc3a3a037e8492297e/Source/EldenRing/Private/Characters/Players/C0000.cpp#L769-L776
         https://github.com/yolong1020/EldenRing/blob/64d9445ed597ada15df737bc3a3a037e8492297e/Source/EldenRing/Private/System/UIManager.cpp#L78-L116 </br>
         
      2. ###### 체크 포인트에 의한 저장 : 화톳불 상호 작용으로 체크 포인트 기능이 작동되어 게임을 저장합니다.
         <img src="https://github.com/yolong1020/EldenRing/assets/87303898/d7634f0b-4989-4d5a-b8c0-9f540a5aeb98" width="49.5%" height="49.5%"></img>   
         </br>
         https://github.com/yolong1020/EldenRing/blob/3578ea4734424524193e99ac7fe3af6e40049190/Source/EldenRing/Private/UI/PopUp/InteractionPopUp.cpp#L115-L138
         https://github.com/yolong1020/EldenRing/blob/73825fa184b635dea13c29460bc5894efc84ecbe/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1412-L1416
         https://github.com/yolong1020/EldenRing/blob/73825fa184b635dea13c29460bc5894efc84ecbe/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1424-L1431
         https://github.com/yolong1020/EldenRing/blob/73825fa184b635dea13c29460bc5894efc84ecbe/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1556-L1583 </br>

   2. ###### USaveGame을 상속받은 UERSaveData에 플레이어에 관련된 모든 데이터들을 저장합니다.
      https://github.com/yolong1020/EldenRing/blob/0c22ea3db091b70ce347e421ef1c6525da478d41/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L52-L73
      https://github.com/yolong1020/EldenRing/blob/6571361fd2e65f39c1565b26b5093ed459994600/Source/EldenRing/Private/GameData/ERSaveData.cpp#L11-L21
      https://github.com/yolong1020/EldenRing/blob/6571361fd2e65f39c1565b26b5093ed459994600/Source/EldenRing/Private/GameData/ERSaveData.cpp#L23-L37 </br>


## Save Objects
###### 플레이어를 제외한 필드에 존재하는 오브젝트들은 Observer Manager에 'EOET_SaveData'로 등록된 이벤트에 의해 저장되며,</br>중복 저장을 방지하게 위해 이벤트 호출 전 반드시 초기화 과정을 거칩니다.
```C++
UERGameInstance* instance = Cast<UERGameInstance>(GetGameInstance()); 
CHECK_INVALID_PTR(instance) 
UObserverManager* observer_mgr = instance->GetSubsystem<UObserverManager>();
CHECK_INVALID_PTR(observer_mgr)
 	 
instance->ResetSaveData(); 
observer_mgr->TriggerEvent(EObserverEventType::EOET_SaveData);
```
https://github.com/yolong1020/EldenRing/blob/0c22ea3db091b70ce347e421ef1c6525da478d41/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L367-L383 </br>

   1. ###### NPC들은 Event에 SaveCharacterData를 바인딩하여 캐릭터의 위치, 클래스 유형, 무기 정보, 행동, 정찰 등을 기본으로 저장합니다.</br>만약 상인 유형이라면, 다이얼로그와 연출 카메라, 이동 장소 및 판매 아이템을 추가로 저장합니다.
      https://github.com/yolong1020/EldenRing/blob/cbf78c1d5f11228634889b1c6f8319e61af4c5a2/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L447-L455
      https://github.com/yolong1020/EldenRing/blob/22eac6ed928320a3710896509221d9f466ecedcf/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L135-L147
      https://github.com/yolong1020/EldenRing/blob/997399c5544718c3393739593c7cc748d3c1a46e/Source/EldenRing/Private/GameData/ERSaveData.cpp#L39-L93 </br>
      
   2. ###### 화톳불이나 상인처럼 상호 작용이 가능한 오브젝트들은 고유 이름과 최초 상호 작용 여부를 저장합니다.
      https://github.com/yolong1020/EldenRing/blob/f3a58f08e48bff30bf54832873d9287e74cc7bf1/Source/EldenRing/Private/Characters/NPC/Vender/Vender.cpp#L245-L253
      https://github.com/yolong1020/EldenRing/blob/22eac6ed928320a3710896509221d9f466ecedcf/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L263-L275
      https://github.com/yolong1020/EldenRing/blob/1e2d77207e761592951b82b53947498560068d82/Source/EldenRing/Private/GameData/ERSaveData.cpp#L105-L113
      
   3. ###### 항아리나 정찰지, 집결지와 같은 오브젝트들은 위치, 클래스 유형을 저장합니다.
      https://github.com/yolong1020/EldenRing/blob/9707e8b6c5dccdd2145024019270f3c4906f2124/Source/EldenRing/Private/FieldObject/FieldObject.cpp#L22-L28
      https://github.com/yolong1020/EldenRing/blob/22eac6ed928320a3710896509221d9f466ecedcf/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L212-L224
      https://github.com/yolong1020/EldenRing/blob/5de49fcb150304115303472db0b75118350dd4e6/Source/EldenRing/Private/GameData/ERSaveData.cpp#L95-L103
      
   1. ###### VolumeBox 같은 튜토리얼 오브젝트들은 위치, 클래스 유형, Callout Popup 정보들을 저장합니다.
      https://github.com/yolong1020/EldenRing/blob/acb23901afdb98a9a45e08a03bb9b3be3107b887/Source/EldenRing/Private/UI/Tutorial/TutorialVolumeBox_CallOut.cpp#L65-L71
      https://github.com/yolong1020/EldenRing/blob/22eac6ed928320a3710896509221d9f466ecedcf/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L314-L326
      https://github.com/yolong1020/EldenRing/blob/dad6afdf1baa07093f377a3671635032093c8114/Source/EldenRing/Private/GameData/ERSaveData.cpp#L115-L124 </br>


## Load Player
###### 메인 타이틀에서 'Load Game' 버튼에 의한 게임 시작과 플레이어 사망 후 게임을 재시작할 때, 저장된 플레이어 정보를 불러옵니다.
```C++
/// [AC0000::BeginPlay] 189
instance->IsNewGame() ? SetDefaultItems() : instance->LoadPlayer(this);
```

   1.
      1. ###### Load Game에 의한 불러오기 :</br>NewGame을 제외한 ERGameInstance의 'm_is_new_game' 변수는 항시 false 상태이며,</br>NewGame에서도 사망에 의한 재시작은 변수를 false로 변경하기 때문에 플레이어 캐릭터 초기화 시 저장된 정보를 불러오게 됩니다.
         <img src="https://github.com/yolong1020/EldenRing/assets/87303898/508aa5db-3251-4e37-8720-ef8ab9de7901" width="49.5%" height="49.5%"></img>   
         </br>
         
      2. ###### 플레이어 사망에 의한 불러오기 :</br>캐릭터의 사망 연출이 종료되면 GameMode에 게임 재시작을 요청하고 캐릭터를 다시 초기화하며 저장된 정보를 불러오게 됩니다.
         <img src="https://github.com/yolong1020/EldenRing/assets/87303898/865ae562-b195-4e21-afd6-a75187b99103" width="49.5%" height="49.5%"></img>   
         </br>
         https://github.com/yolong1020/EldenRing/blob/2e31075e7eb907ff42a181d5348179df51fadf97/Source/EldenRing/Private/Characters/Players/C0000.cpp#L400-L415
         https://github.com/yolong1020/EldenRing/blob/2e31075e7eb907ff42a181d5348179df51fadf97/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1222-L1229
         https://github.com/yolong1020/EldenRing/blob/a0d7366564eba9f62fe294702066f0320fe39bcd/Source/EldenRing/Private/GameBase/ERGameMode.cpp#L71-L90 </br>
         
   2. ###### 플레이어를 마지막에 저장된 위치로 이동키고 착용중인 아이템과 보관된 아이템들의 UItemObject를 생성, 검증 후 각 분류에 맞게 위치시킵니다.
      https://github.com/yolong1020/EldenRing/blob/64f315c377d4aab7c93b16c3b7e04cd8962d87ec/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L75-L133 </br>
      
     
## Load Objects
###### 메인 타이틀에서 'Load Game' 버튼에 의한 게임 시작과 플레이어 사망 후 게임을 재시작할 때, 저장된 오브젝트 정보를 불러옵니다.

   1.
      1. ###### Load Game에 의한 불러오기 :</br>Load Game으로 게임을 실행한 경우 FieldObject Data Layer를 불러와 별도로 오브젝트들을 생성하고 데이터를 연결합니다.
         https://github.com/yolong1020/EldenRing/blob/099acb762a396881c10e7f615b94861e4ecfad93/Source/EldenRing/Private/Levels/BaseLevelScript.cpp#L24-L45
         https://github.com/yolong1020/EldenRing/blob/099acb762a396881c10e7f615b94861e4ecfad93/Source/EldenRing/Private/Levels/GameField.cpp#L58-L78 </br>
      2. ###### 플레이어 사망에 의한 불러오기 :</br>플레이어 캐릭터의 사망 연출이 종료되면 GameMode에 게임 재시작을 요청하고 필드의 오브젝트들을 불러옵니다.
         https://github.com/yolong1020/EldenRing/blob/099acb762a396881c10e7f615b94861e4ecfad93/Source/EldenRing/Private/GameBase/ERGameMode.cpp#L71-L90 </br>

   2. ###### Load Game에 의한 불러오기 :</br>NewGame이 아닌 경우 FieldObject Data Layer를 불러와 별도로 오브젝트들을 생성합니다.
      https://github.com/yolong1020/EldenRing/blob/a0d7366564eba9f62fe294702066f0320fe39bcd/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L44-L50

      1. ###### 필드에 이미 NPC가 존재하는 경우, 해당 NPC들을 모두 파괴합니다.
         https://github.com/yolong1020/EldenRing/blob/a595cea3153c037ce10390a86ac9ac7f5d812d20/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L149-L210
     
