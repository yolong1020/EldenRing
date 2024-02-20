# EldenRing Project
###### 포토폴리오 목적으로 엘든링의 내용들을 재해석하여 제작한 데모 프로젝트입니다. 

* Unreal Engine 5.3.2
* Microsoft Visual Studio Community 2022
* 2023.11.01 ~ 2024.01.27 (3개월 : 구현 기간)
* 2023.03.07 ~ 2024.06.22 (3개월 : 리소스 기간)
</br>

## Video
###### HDR이 적용된 2K 인코딩 영상입니다. 하단 썸네일에서 영상을 확인할 수 있습니다.
* CPU : Ryzen 7 5800X
* GPU : RTX 3080TI
* RAM : 32gb Ram

[![Video Label](http://img.youtube.com/vi/ZO0a9uATi-o/0.jpg)](https://youtu.be/ZO0a9uATi-o)   
</br>

## Technical Overview
###### 하위 링크들에서 각각의 기능들을 어떻게 구현했는가에 관해 확인할 수 있습니다.

   1. #### Character
      - [GameCharacter](https://github.com/yolong1020/EldenRing/blob/main/README.md#gamecharacter)
         - [Player](https://github.com/yolong1020/EldenRing/blob/main/Overviews/Character/Player.md)
         - [NPC](https://github.com/yolong1020/EldenRing/blob/main/Overviews/Character/NPC/NPC.md)
           1. [Vender](https://github.com/yolong1020/EldenRing/blob/main/Overviews/Character/NPC/Vender.md)
           2. [Soldier of Godrick](https://github.com/yolong1020/EldenRing/blob/main/Overviews/Character/NPC/SoldierOfGodrick.md)
           3. [Tree Guard](https://github.com/yolong1020/EldenRing/blob/main/Overviews/Character/NPC/TreeGuard.md)

   2. #### UI
      - [Inventory](https://github.com/yolong1020/EldenRing/blob/main/Overviews/UI/Inventory/Inventory.md)
      - [Trade](https://github.com/yolong1020/EldenRing/blob/main/Overviews/UI/Trade/Trade.md)
      - [Tutorial](https://github.com/yolong1020/EldenRing/blob/main/Overviews/UI/Tutorial/Tutorial.md)

   3. #### System
      - [Level](https://github.com/yolong1020/EldenRing/blob/main/Overviews/System/Level/Level.md)
      - [Save & Load](https://github.com/yolong1020/EldenRing/blob/main/Overviews/System/SaveLoad/SaveLoad.md)
   ---

   ### GameCharacter
   ###### ACharacter를 상속한 게임 내 모든 캐릭터들의 최상위 Base Class이며, 캐릭터의 초기화, 사망, Setter, Getter와 같은 공통 기능들을 제공합니다.
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/63739cc3-0bff-4242-9309-f3c2f8cea049"></img>
   [1. Init Default Component & Values](#Init-Default-Component--Values)   
   [2. Death Process](#Death-Process)   
   </br>

   #### Init Default Component & Values
   ###### Mesh, Collision 등 Component 생성 및 초기화를 진행합니다.
   https://github.com/yolong1020/EldenRing/blob/950ee49b7ccfe5641f6d3f72a9011447d7e19e05/Source/EldenRing/Private/Characters/GameCharacter.cpp#L85-L160
   </br>

   #### Death Process
   ###### 캐릭터 사망시 Component 처리와 이펙트 연출 과정입니다.
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/467bd7d2-00b7-4ae9-8d56-89c00ca0d035" width="49.5%" height="49.5%"></img>
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/a01165f9-5743-49bb-881b-fe81aeae8808" width="49.5%" height="49.5%"></img>   
   </br>

   1. ###### 콜리전 비활성화 및 재생된 Montage Section 시간을 도출합니다.
      https://github.com/yolong1020/EldenRing/blob/c53f020d33f9d04b90e30f01418c4b8a379f6536/Source/EldenRing/Private/Characters/GameCharacter.cpp#L199-L223 </br>
   2. ###### Controller 및 MovementComponent 비활성 후 캐릭터와 무기 Fade Out을 진행합니다.
      https://github.com/yolong1020/EldenRing/blob/c53f020d33f9d04b90e30f01418c4b8a379f6536/Source/EldenRing/Private/Characters/GameCharacter.cpp#L321-L362 </br>
   3. ###### 이펙트 연출 후 엑터를 파괴합니다.
      https://github.com/yolong1020/EldenRing/blob/c53f020d33f9d04b90e30f01418c4b8a379f6536/Source/EldenRing/Private/Characters/GameCharacter.cpp#L162-L168 </br>
   4. ###### 엑터가 소유한 무기를 파괴합니다.
      https://github.com/yolong1020/EldenRing/blob/c53f020d33f9d04b90e30f01418c4b8a379f6536/Source/EldenRing/Private/Characters/GameCharacter.cpp#L177-L191 </br>
