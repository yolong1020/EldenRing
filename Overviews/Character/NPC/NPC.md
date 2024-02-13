# NPC
#### * Init Default Component & Values
  > NPC에 해당하는 모든 캐릭터의 Component와 초기값을 설정합니다.

  ###### [Step 01 - 01] Health Bar와 PawnSensing Component, 그리고 Level의 로드 상태에 따라 Spawn 위치를 초기화합니다.
  https://github.com/yolong1020/EldenRing/blob/e6bc14be5355ea76a3c64fa6c43ca390684d1d26/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L105-L130
  https://github.com/yolong1020/EldenRing/blob/6e79f5f9faabbc8e9469d0ce1b7bdebdd020a588/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L151-L154
  </br>
  ###### [Step 01 - 02] Level의 로드가 완료되지 않았다면, Level의 로드 완료 시점에 호출될 Event로 등록해줍니다.
  https://github.com/yolong1020/EldenRing/blob/ec2bd163de2ae2409d90b577fef49e66b5eb3700/Source/EldenRing/Public/GameBase/ERGameInstance.h#L35-L39
  https://github.com/yolong1020/EldenRing/blob/3e40e9ea14935ca05790f050c704db9ef6d152f8/Source/EldenRing/Private/Levels/GameField.cpp#L65-L108
  </br>
---
#### * Patrol
  > 캐
  </br>
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/40e9e4ee-a3ad-4f68-8f2e-2b401f9d0a03" width="49.5%" height="49.5%"></img>
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/e8abfa97-d34c-4d93-8ef5-19489a25dca0" width="49.5%" height="49.5%"> 

  ###### Step 01. 콜리전 비활성화 및 재생된 Montage Section 시간을 도출합니다.
  https://github.com/yolong1020/EldenRing/blob/c53f020d33f9d04b90e30f01418c4b8a379f6536/Source/EldenRing/Private/Characters/GameCharacter.cpp#L199-L223
  </br>
---
#### * Assemble Point
  > 캐릭터 사망시 Component 처리와 이펙트 연출 과정입니다.
  </br>
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/25a3b779-5c6a-4efc-a22b-b287428e12e5" width="49.5%" height="49.5%"> 

  ###### Step 01. 콜리전 비활성화 및 재생된 Montage Section 시간을 도출합니다.
  https://github.com/yolong1020/EldenRing/blob/c53f020d33f9d04b90e30f01418c4b8a379f6536/Source/EldenRing/Private/Characters/GameCharacter.cpp#L199-L223
  </br>
