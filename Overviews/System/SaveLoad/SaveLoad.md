# Save & Load
[1. Open Game Level](#Open-Game-Level)   
[2. Open by VolumeBox](#Open-by-VolumeBox)   
</br>


## Save Game
###### .
<img src="https://github.com/yolong1020/EldenRing/assets/87303898/743546f9-9289-4582-8b8f-83112424e151" width="49.5%" height="49.5%"></br>

  1. ###### Menu Button에 바인딩 된 기능으로 화면을 Fade out으로 전환합니다. 이때 Fade out이 완전히 이뤄지면 GameInstance를 통해 Level을 이전합니다.
     https://github.com/yolong1020/EldenRing/blob/64d9445ed597ada15df737bc3a3a037e8492297e/Source/EldenRing/Private/Characters/Players/C0000.cpp#L769-L776
     https://github.com/yolong1020/EldenRing/blob/64d9445ed597ada15df737bc3a3a037e8492297e/Source/EldenRing/Private/System/UIManager.cpp#L78-L116
     https://github.com/yolong1020/EldenRing/blob/0c22ea3db091b70ce347e421ef1c6525da478d41/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L52-L73
     https://github.com/yolong1020/EldenRing/blob/0c22ea3db091b70ce347e421ef1c6525da478d41/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L367-L383
     https://github.com/yolong1020/EldenRing/blob/cbf78c1d5f11228634889b1c6f8319e61af4c5a2/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L447-L455


## Restart Game
###### 플레이어가 사망 후 게임을 다시 시작하도록 
<img src="https://github.com/yolong1020/EldenRing/assets/87303898/865ae562-b195-4e21-afd6-a75187b99103" width="49.5%" height="49.5%"></br>

  1. ###### GameCharacter의 사망 연출이 종료되면 GameMode에 게임 재시작을 요청합니다.
     https://github.com/yolong1020/EldenRing/blob/2e31075e7eb907ff42a181d5348179df51fadf97/Source/EldenRing/Private/Characters/Players/C0000.cpp#L400-L415
     https://github.com/yolong1020/EldenRing/blob/2e31075e7eb907ff42a181d5348179df51fadf97/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1222-L1229
     https://github.com/yolong1020/EldenRing/blob/a0d7366564eba9f62fe294702066f0320fe39bcd/Source/EldenRing/Private/GameBase/ERGameMode.cpp#L71-L90 </br>
  2. ###### 필드에 플레이어가 없다면 각 항목별로 오브젝트들을 저장된 시점으로 다시 로드합니다.
     https://github.com/yolong1020/EldenRing/blob/a0d7366564eba9f62fe294702066f0320fe39bcd/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L44-L50
     
