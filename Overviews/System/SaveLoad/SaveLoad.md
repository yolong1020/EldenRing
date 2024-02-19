# Save & Load
[1. Open Game Level](#Open-Game-Level)   
[2. Open by VolumeBox](#Open-by-VolumeBox)   
</br>


## Save Game
###### 게임을 종료하거나 체크 포인트 활성 및 아이템 거래 상황에서 저장이 발생됩니다.
<img src="https://github.com/yolong1020/EldenRing/assets/87303898/508aa5db-3251-4e37-8720-ef8ab9de7901" width="49.5%" height="49.5%"></br>

  1. ###### Menu Button에 바인딩 된 기능으로 화면을 Fade out으로 전환합니다. 이때 Fade out이 완전히 이뤄지면 GameInstance를 통해 Level을 이전합니다.
     https://github.com/yolong1020/EldenRing/blob/64d9445ed597ada15df737bc3a3a037e8492297e/Source/EldenRing/Private/Characters/Players/C0000.cpp#L769-L776
     https://github.com/yolong1020/EldenRing/blob/64d9445ed597ada15df737bc3a3a037e8492297e/Source/EldenRing/Private/System/UIManager.cpp#L78-L116

  2. ###### 데이터 중복을 방지하기 위해 플레이어를 제외한 슬롯의 모든 데이터를 삭제합니다.
     https://github.com/yolong1020/EldenRing/blob/0c22ea3db091b70ce347e421ef1c6525da478d41/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L367-L383
     
     1. ###### 플레이어 저장
        https://github.com/yolong1020/EldenRing/blob/0c22ea3db091b70ce347e421ef1c6525da478d41/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L52-L73
        https://github.com/yolong1020/EldenRing/blob/6571361fd2e65f39c1565b26b5093ed459994600/Source/EldenRing/Private/GameData/ERSaveData.cpp#L11-L21
        https://github.com/yolong1020/EldenRing/blob/6571361fd2e65f39c1565b26b5093ed459994600/Source/EldenRing/Private/GameData/ERSaveData.cpp#L23-L37
     1. ###### M
        https://github.com/yolong1020/EldenRing/blob/cbf78c1d5f11228634889b1c6f8319e61af4c5a2/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L447-L455
        https://github.com/yolong1020/EldenRing/blob/22eac6ed928320a3710896509221d9f466ecedcf/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L135-L147
        https://github.com/yolong1020/EldenRing/blob/997399c5544718c3393739593c7cc748d3c1a46e/Source/EldenRing/Private/GameData/ERSaveData.cpp#L39-L93
     1. ###### M
        https://github.com/yolong1020/EldenRing/blob/f3a58f08e48bff30bf54832873d9287e74cc7bf1/Source/EldenRing/Private/Characters/NPC/Vender/Vender.cpp#L245-L253
        https://github.com/yolong1020/EldenRing/blob/22eac6ed928320a3710896509221d9f466ecedcf/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L263-L275
        https://github.com/yolong1020/EldenRing/blob/1e2d77207e761592951b82b53947498560068d82/Source/EldenRing/Private/GameData/ERSaveData.cpp#L105-L113
     1. ###### M
        https://github.com/yolong1020/EldenRing/blob/9707e8b6c5dccdd2145024019270f3c4906f2124/Source/EldenRing/Private/FieldObject/FieldObject.cpp#L22-L28
        https://github.com/yolong1020/EldenRing/blob/22eac6ed928320a3710896509221d9f466ecedcf/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L212-L224
        https://github.com/yolong1020/EldenRing/blob/5de49fcb150304115303472db0b75118350dd4e6/Source/EldenRing/Private/GameData/ERSaveData.cpp#L95-L103
     1. ###### M
        https://github.com/yolong1020/EldenRing/blob/acb23901afdb98a9a45e08a03bb9b3be3107b887/Source/EldenRing/Private/UI/Tutorial/TutorialVolumeBox_CallOut.cpp#L65-L71
        https://github.com/yolong1020/EldenRing/blob/22eac6ed928320a3710896509221d9f466ecedcf/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L314-L326
        https://github.com/yolong1020/EldenRing/blob/dad6afdf1baa07093f377a3671635032093c8114/Source/EldenRing/Private/GameData/ERSaveData.cpp#L115-L124


## Restart Game
###### 플레이어가 사망 후 게임을 다시 시작하도록 
<img src="https://github.com/yolong1020/EldenRing/assets/87303898/865ae562-b195-4e21-afd6-a75187b99103" width="49.5%" height="49.5%"></br>

  1. ###### GameCharacter의 사망 연출이 종료되면 GameMode에 게임 재시작을 요청합니다.
     https://github.com/yolong1020/EldenRing/blob/2e31075e7eb907ff42a181d5348179df51fadf97/Source/EldenRing/Private/Characters/Players/C0000.cpp#L400-L415
     https://github.com/yolong1020/EldenRing/blob/2e31075e7eb907ff42a181d5348179df51fadf97/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1222-L1229
     https://github.com/yolong1020/EldenRing/blob/a0d7366564eba9f62fe294702066f0320fe39bcd/Source/EldenRing/Private/GameBase/ERGameMode.cpp#L71-L90 </br>
  2. ###### 필드에 플레이어가 없다면 각 항목별로 오브젝트들을 저장된 시점으로 다시 로드합니다.
     https://github.com/yolong1020/EldenRing/blob/a0d7366564eba9f62fe294702066f0320fe39bcd/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L44-L50
     
