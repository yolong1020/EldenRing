![Charactor](https://github.com/yolong1020/EldenRing/assets/87303898/63739cc3-0bff-4242-9309-f3c2f8cea049)
#### 1. GameCharacter
* Init Default Component & Values
  > Mesh, Collision 등 Component 생성 및 초기화 과정
* Death Process : 
  > 캐릭터 사망시 Component 처리와 이펙트 연출 과정
  ###### Step 01. 콜리전 비활성화 및 재생된 Montage Section 시간 도출.
  https://github.com/yolong1020/EldenRing/blob/c53f020d33f9d04b90e30f01418c4b8a379f6536/Source/EldenRing/Private/Characters/GameCharacter.cpp#L199-L223
  ###### Step 02. Controller 및 MovementComponent 비활성 후 캐릭터와 무기 Fade Out 진행.
  https://github.com/yolong1020/EldenRing/blob/c53f020d33f9d04b90e30f01418c4b8a379f6536/Source/EldenRing/Private/Characters/GameCharacter.cpp#L321-L362
  ###### Step 03. 이펙트 연출 후 엑터 파괴
  https://github.com/yolong1020/EldenRing/blob/c53f020d33f9d04b90e30f01418c4b8a379f6536/Source/EldenRing/Private/Characters/GameCharacter.cpp#L162-L168
  ###### Step 04. 엑터가 소유한 무기 파괴
  https://github.com/yolong1020/EldenRing/blob/c53f020d33f9d04b90e30f01418c4b8a379f6536/Source/EldenRing/Private/Characters/GameCharacter.cpp#L177-L191
