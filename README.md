EldenRing Project
===============================
###### 포토폴리오 목적으로 엘든링의 내용들을 재해석하여 제작한 데모 프로젝트입니다. 

* Unreal Engine 5.3.2
* Microsoft Visual Studio Community 2022
* 2023.11.01 ~ 2024.01.27 (3개월 : 구현 기간)
* 2023.03.07 ~ 2024.06.22 (3개월 : 리소스 기간)

Video
----------
###### HDR이 적용된 2K 인코딩 영상입니다.
* CPU : Ryzen 7 5800X
* GPU : RTX 3080TI
* RAM : 32gb Ram

[![Video Label](http://img.youtube.com/vi/ZO0a9uATi-o/0.jpg)](https://youtu.be/ZO0a9uATi-o)
</br>

Technical Overview
------------------

### Character

![Charactor](https://github.com/yolong1020/EldenRing/assets/87303898/63739cc3-0bff-4242-9309-f3c2f8cea049)
- [GameCharacter](https://github.com/yolong1020/EldenRing/blob/main/Overviews/Character/GameCharacter.md)
- [Player](https://github.com/yolong1020/EldenRing/blob/main/Overviews/Character/Player.md)
#### [NPC](https://github.com/yolong1020/EldenRing/blob/main/Overviews/Character/NPC/NPC.md)
> Player를 제외한 모든 캐릭터들의 Base Class이며, 특정 오브젝트를 집결지로 등록하고 에디터에서 정해진 정찰 지점들을 순회하는 정찰 기능을 포함하고 있습니다.
일반(Repose) / 경계(Vigilance) 상황을 구분하며, Health Bar Widget을 제공합니다.
  - [Vender](https://github.com/yolong1020/EldenRing/blob/main/Overviews/Character/NPC/Vender.md)
  - [Soldier of Godrick](https://github.com/yolong1020/EldenRing/blob/main/Overviews/Character/NPC/SoldierOfGodrick.md)
  - [Tree Guard](https://github.com/yolong1020/EldenRing/blob/main/Overviews/Character/NPC/TreeGuard.md)

### UI
- [Inventory](https://github.com/yolong1020/EldenRing/blob/main/Overviews/UI/Inventory/Inventory.md)
- [Trade](https://github.com/yolong1020/EldenRing/blob/main/Overviews/UI/Trade/Trade.md)
