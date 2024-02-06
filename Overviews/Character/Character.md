![Charactor](https://github.com/yolong1020/EldenRing/assets/87303898/63739cc3-0bff-4242-9309-f3c2f8cea049)
#### [1. GameCharacter](https://github.com/yolong1020/EldenRing/blob/main/Overviews/Character/GameCharacter.md)
> ACharacter를 상속한 게임 내 모든 캐릭터들의 최상위 Base Class이며, 캐릭터의 초기화, 사망, Setter, Getter와 같은 공통 기능들을 제공합니다.
  
#### [2. Player](https://github.com/yolong1020/EldenRing/blob/main/Overviews/Character/Player.md)
> 사용자가 직접 조작하는 캐릭터로 APlayerController를 사용하며, Level의 게임 플레이 제공 여부에 따라 생성됩니다.
5회 콤보가 가능한 일반 공격과 2회 콤보의 강 공격이 전투 상황에서 제공되고 또한 방패 보유시 방어와 반격이 가능합니다.
기본 움직임으로는 전력 질주와 회피, 점프 기능을 제공되고 이 기능들은 지구력을 공격과 함께 지구력을 소모하게 됩니다.
그 외 Lock On과 포션 섭취, Interaction Manager에 의해 NPC 혹은 사물과 상호 작용, 마지막으로 Inventory Manager와 Trade Manager에 의한 아이템 관리 및 거래가 구현되어 있습니다.

#### [3. NPC](https://github.com/yolong1020/EldenRing/blob/main/Overviews/Character/Player.md)
> Player를 제외한 모든 캐릭터들의 Base Class이며, 특정 오브젝트를 집결지로 등록하고 에디터에서 정해진 정찰 지점들을 순회하는 정찰 기능을 포함하고 있습니다.
일반(Repose) / 경계(Vigilance) 상황을 구분하며, Health Bar Widget을 제공합니다.
