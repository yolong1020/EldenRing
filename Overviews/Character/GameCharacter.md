# GameCharacter
###### ACharacter를 상속한 게임 내 모든 캐릭터들의 최상위 Base Class이며, 캐릭터의 초기화, 사망, Setter, Getter와 같은 공통 기능들을 제공합니다.
1. Init Default Component & Values
2. Death Process
</br>

## Init Default Component & Values
###### Mesh, Collision 등 Component 생성 및 초기화를 진행합니다.
</br>

## Death Process
###### 캐릭터 사망시 Component 처리와 이펙트 연출 과정입니다.
</br>
<img src="https://github.com/yolong1020/EldenRing/assets/87303898/a01165f9-5743-49bb-881b-fe81aeae8808" width="49.5%" height="49.5%"></img>
  
  ###### [Step 01] 콜리전 비활성화 및 재생된 Montage Section 시간을 도출합니다.
  https://github.com/yolong1020/EldenRing/blob/c53f020d33f9d04b90e30f01418c4b8a379f6536/Source/EldenRing/Private/Characters/GameCharacter.cpp#L199-L223
  ###### [Step 02] Controller 및 MovementComponent 비활성 후 캐릭터와 무기 Fade Out을 진행합니다.
  https://github.com/yolong1020/EldenRing/blob/c53f020d33f9d04b90e30f01418c4b8a379f6536/Source/EldenRing/Private/Characters/GameCharacter.cpp#L321-L362
  ###### [Step 03] 이펙트 연출 후 엑터를 파괴합니다.
  https://github.com/yolong1020/EldenRing/blob/c53f020d33f9d04b90e30f01418c4b8a379f6536/Source/EldenRing/Private/Characters/GameCharacter.cpp#L162-L168
  ###### [Step 04] 엑터가 소유한 무기를 파괴합니다.
  https://github.com/yolong1020/EldenRing/blob/c53f020d33f9d04b90e30f01418c4b8a379f6536/Source/EldenRing/Private/Characters/GameCharacter.cpp#L177-L191
