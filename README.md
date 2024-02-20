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
   ---
   ## Player
   ###### 사용자가 직접 조작하는 캐릭터로 APlayerController를 사용하며, Level의 게임 플레이 제공 여부에 따라 캐릭터가 생성됩니다. 5회 콤보가 가능한 일반 공격과 2회 콤보의 강 공격이 전투 상황에서 제공되고 또한 방패 보유시 방어와 반격이 가능합니다. 기본 움직임으로는 전력 질주와 회피, 점프 기능을 제공되고 이 기능들은 지구력을 공격과 함께 지구력을 소모하게 됩니다. 그 외 Lock On과 포션 섭취, Interaction Manager에 의해 NPC 혹은 사물과 상호 작용, 마지막으로 Inventory Manager와 Trade Manager에 의한 아이템 관리 및 거래가 구현되어 있습니다.
   [1. Init Player Settings](#Init-Player-Settings)   
   [2. Lock On](#Lock-On)   
   [3. Combat](#Combat)   
   [4. Movement](#Movement)   
   [5. Potion Comsume](#Potion-Comsume)   
   </br>


   #### Init Player Settings
   ###### Level 상태에 따라서 HUD, Camera Timeline, 플레이에 필요한 Item 등 초기화합니다.

   1. ###### 현재 Level의 플레이 가능 여부와 Load 상태에 따른 초기화를 진행합니다.
      https://github.com/yolong1020/EldenRing/blob/a021831dab5f79b7e6faff2f2ab6fc9c5b5b9216/Source/EldenRing/Private/Characters/Players/C0000.cpp#L136-L212 </br>

   2. ###### 'New Game' 혹은 'Load Game'에 따라 Item을 다르게 초기화합니다. Save & Load에 관한 자세한 내용은 [여기](https://github.com/yolong1020/EldenRing/blob/main/Overviews/System/SaveLoad/SaveLoad.md)에서 확인이 가능합니다.
      ```C++
      /// [BeginPlay] 189
      instance->IsNewGame() ? SetDefaultItems() : instance->LoadPlayer(this);
      ```
      https://github.com/yolong1020/EldenRing/blob/bc151a656f7846c367529f4340f0c87f574dcde0/Source/EldenRing/Private/Characters/Players/C0000.cpp#L78-L111
      https://github.com/yolong1020/EldenRing/blob/0f9888e552ddbf77796e6e03775caa458ebac7fe/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L75-L133 </br>

   3. ###### Level의 Load 상태에 따른 캐릭터 시작 애니메이션 재생 및 Input을 활성합니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/04e4f6de-3d92-4d66-a744-ab2061494a0f" width="49.5%" height="49.5%"></img>
      https://github.com/yolong1020/EldenRing/blob/bc151a656f7846c367529f4340f0c87f574dcde0/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1595-L1622 </br>


   #### Lock On
   ###### 뷰포트 내에 적이 존재하고 Collision 범위에 들어오면 적을 포커싱하여 전투를 진행할 수 있습니다.
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/5b8bd512-a506-474c-9d4c-b640a083e471" width="49.5%" height="49.5%"></img>
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/756587c1-8e98-4d55-8af8-9722951918be" width="49.5%" height="49.5%"></img>   
   </br>

   1. ###### 키 입력을 통해 Lock On을 활성화하면 Collision이 활성화되면서 적의 유효성을 확인하여 UI를 노시킵니다.</br>만약 일정 시간 대상을 찾지 못하 Lock On은 해제되면서 Collision이 비활성화됩니다.
      https://github.com/yolong1020/EldenRing/blob/bffd81e510eca5529be087329825bcd068424a52/Source/EldenRing/Private/Characters/Players/C0000.cpp#L954-L984
      https://github.com/yolong1020/EldenRing/blob/0b1ea10f68d000574508785aae7ef1f5adac72a2/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1152-L1179
      https://github.com/yolong1020/EldenRing/blob/78f2c4bf964312e00dcad4478b1e75edb5c3fc40/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1210-L1220
      https://github.com/yolong1020/EldenRing/blob/0b1ea10f68d000574508785aae7ef1f5adac72a2/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1614-L1621 </br>
   2. ###### 하나 이상의 적들이 Collision에 감지되 컨테이너에 적들의 포인터가 적재되며, 입력을 통해 플레이어와 가까운 순서로 다른 적에게 포커싱을 이동시킵니다.
      https://github.com/yolong1020/EldenRing/blob/bffd81e510eca5529be087329825bcd068424a52/Source/EldenRing/Private/Characters/Players/C0000.cpp#L986-L1009 </br>
   3. ###### 활성화된 UI는 적의 척추 위치를 추적하며 화면에서 Draw되게 됩니다.
      https://github.com/yolong1020/EldenRing/blob/bffd81e510eca5529be087329825bcd068424a52/Source/EldenRing/Private/System/UIManager.cpp#L196-L207
      https://github.com/yolong1020/EldenRing/blob/bffd81e510eca5529be087329825bcd068424a52/Source/EldenRing/Private/HUD/LockOn/LockOn.cpp#L12-L27 </br>


   #### Combat
   ###### 조작에 따라 단일, 이전의 공격을 바탕으로 한 콤보 공격으로 구별됩니다. 그 외 방패를 통한 방어와 반격 기능을 제공합니다.
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/a0c6171a-8fd2-4d7a-a4e3-f48deecae369" width="49.5%" height="49.5%"></img>
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/4ccc6b0c-d8f4-423c-a7b0-f5fcf8249d9c" width="49.5%" height="49.5%"></img>   
   </br>

   1. ###### 전투 대상의 Execution Collision(Front / Back)에 Overlap 되면 플레이어는 처형 공격을 실행할 수 있습니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/f0ea4db7-e74a-4097-98ef-352036c0d959" width="49.5%" height="49.5%"></img>  
      https://github.com/yolong1020/EldenRing/blob/4e07652fec8037129c94ac1eb022e142e030ac5f/Source/EldenRing/Private/Characters/Players/C0000.cpp#L851-L880 </br>
   2. ###### 전력 질주와 점프, 일반 상태 그리고 강공격 Input 입력 여부에 따라 서로 다른 공격이 전개됩니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/d7dd26b7-1240-458f-9f78-146763bf7085" width="49.5%" height="49.5%"></img>
      https://github.com/yolong1020/EldenRing/blob/4e07652fec8037129c94ac1eb022e142e030ac5f/Source/EldenRing/Private/Characters/Players/C0000.cpp#L883-L942 </br>
   3. ###### Weapon_Actor의 Collision과 충돌한 대상이 캐릭터인지 방어자의 Parry Collision, 방패의 Collision인지에 따라 피격 결과를 다르게 적용시킵니다.</br>만약 Weapon_Actor가 Parry Collision과 충돌했다면, 사용자를 기절 상태로 변경시키고 Execution Collision - Front를 활성시킵니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/4c3561e8-b9d5-4cf7-aa27-b5ec06bf5b82" width="49.5%" height="49.5%"></img>
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/2860b4e7-a969-4b94-b501-5cf7975cc48c" width="49.5%" height="49.5%"></img>
      https://github.com/yolong1020/EldenRing/blob/4070f5c523d417e907e46127052dd90aae241624/Source/EldenRing/Private/Items/Weapons/MeleeAttack_Actor.cpp#L72-L163 </br>
   4. ###### 추가 공격 유효 프레임을 벗어나면 Montage - EndAttack이 호출되어 콤보, 공격 가능 상태, 캐릭터 행동 상태 등을 기본값으로 복구시킵니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/8a4bed56-27cc-47b6-b137-190c7f87a0f3"></img>
      https://github.com/yolong1020/EldenRing/blob/2f43079d34e3767f013851c836d4480db5fd268a/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1368-L1377 </br>


   #### Movement
   ###### 조작과 캐릭터의 현재 행동에 따라 전력질주, 뛰기, 걷기가 활성되며, 특정 조건에 해당하지 않는다면 회피와 점프 기능을 이용할 수 있습니다.

   1. ###### LockOn 상태와 전력질주 Input에 따라 전력질주, 뛰기, 걷기로 전/후, 좌/우 이동을 결정합니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/7dcd1d46-6d88-4844-8477-65da15a638fd" width="49.5%" height="49.5%"></img>
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/413f7ab8-55b9-4e18-9a9d-cba08c4ff348" width="49.5%" height="49.5%"></img>  
      https://github.com/yolong1020/EldenRing/blob/6a40836945163b30e9dd8aaf3c548b9b92d56edd/Source/EldenRing/Private/Characters/Players/C0000.cpp#L661-L745 </br>
   2. ###### 특정 조건에 해당하지 않는 경우, 입력된 방향 혹은 캐릭터 후방으로 회피를 시도합니다.</br>LockOn 상태에서는 목표 바라보며 회피를 시도합니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/cd782b77-1a46-4de1-9f02-e33573f16795" width="49.5%" height="49.5%"></img>
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/c6226863-2773-4e7d-ad63-5404d6aed113" width="49.5%" height="49.5%"></img>   
      https://github.com/yolong1020/EldenRing/blob/6a40836945163b30e9dd8aaf3c548b9b92d56edd/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1011-L1042 </br>
   4. ###### 특정 조건에 해당하지 않는 경우, 입력된 방향으로 도약을 시도합니다.</br>Landed 호출을 통해 착지를 시도하며, 도약 중 피격을 받게되면 그 즉시 추락합니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/4b0d60e8-02be-4a47-b30f-a6999b0576d7" width="49.5%" height="49.5%"></img>
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/504dee50-bcc1-4759-b4e9-46b582f7e46e" width="49.5%" height="49.5%"></img> 
      https://github.com/yolong1020/EldenRing/blob/6a40836945163b30e9dd8aaf3c548b9b92d56edd/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1115-L1148
      https://github.com/yolong1020/EldenRing/blob/2f43079d34e3767f013851c836d4480db5fd268a/Source/EldenRing/Private/Characters/Players/C0000.cpp#L235-L274
      https://github.com/yolong1020/EldenRing/blob/05121f2477a82426bed1d6281e809fd4ec7ab43b/Source/EldenRing/Private/Characters/Players/C0000.cpp#L470-L478 </br>


   #### Potion Comsume
   ###### 플레이어의 입력으 현재 HP를 회복하는 기능입니다.</br>회복 횟수에는 제한이 있으며, 모든 포션을 소모했을 경우 해당하는 애니메이션이 재생되고 HP는 회복되지 않습니다.</br>또한, 포션 복용 중 피격을 받게 되면 소모된 포션은 복구되지 않고 HP 역시 회복되지 않습니다.

   1. ###### 캐릭터의 하반신은 Input에 따라 움직일 수 있도록 척추를 기준으로 Blend를 실행하고 잔여 포션 수에 따라 애니메이션을 결정합니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/21199819-2cd4-41e6-986b-ab250d340baa" width="49.5%" height="49.5%"></img>
      https://github.com/yolong1020/EldenRing/blob/1a8f4ec6559928de5514a7bfc880b2c52b592252/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1093-L1115 </br>
   2. ###### 애니메이션 재생 후 성공적으로 Montage - OnSuccessConsume이 호출되면 Attribute와 HUD의 HP 정보를 갱신합니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/c808a976-430a-4f0d-9f2e-a1395a89f019"></img>
      https://github.com/yolong1020/EldenRing/blob/1a8f4ec6559928de5514a7bfc880b2c52b592252/Source/EldenRing/Private/Characters/Players/C0000.cpp#L581-L592 </br>
