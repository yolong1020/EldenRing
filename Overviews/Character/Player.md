#### 1. GameCharacter
* Init Default Component & Values
  > Mesh, Collision 등 Component 생성 및 초기화 과정
* Death Process : 
  > 캐릭터 사망시 Component 처리와 이펙트 연출 과정
  ###### Step 01. 현재 Level의 플레이 가능 여부와 Load 상태에 따른 초기화 및 HUD, Camera Timeline, Item 초기화.
  https://github.com/yolong1020/EldenRing/blob/97e6878111e31dc812742b57458777b65f565e7c/Source/EldenRing/Private/Characters/Players/C0000.cpp#L136-L212
