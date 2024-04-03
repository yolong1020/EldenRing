<div align="center">
    <a href="https://github.com/yolong1020/EldenRing/blob/main/README.md#technical-overview">
        <img src="https://img.shields.io/badge/Go_to_Top-Click_here-ff69b4.svg" alt="Top Button">
    </a>
</div>


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
         - [Player](https://github.com/yolong1020/EldenRing/blob/main/README.md#player)
         - [NPC](https://github.com/yolong1020/EldenRing/blob/main/README.md#npc)
           1. [Vender](https://github.com/yolong1020/EldenRing/blob/main/README.md#vender)
           2. [Soldier of Godrick](https://github.com/yolong1020/EldenRing/blob/main/README.md#soldier-of-godrick)
           3. [Tree Guard](https://github.com/yolong1020/EldenRing/blob/main/README.md#tree-guard)

   2. #### UI
      - Inventory
        - [Equipment](https://github.com/yolong1020/EldenRing/blob/main/README.md#inventory---equipment)
        - [Storage](https://github.com/yolong1020/EldenRing/blob/main/README.md#inventory---storage)
      - [Trade](https://github.com/yolong1020/EldenRing/blob/main/README.md#trade-1)
      - [Tutorial](https://github.com/yolong1020/EldenRing/blob/main/README.md#tutorial)

   3. #### System
      - [Level](https://github.com/yolong1020/EldenRing/blob/main/README.md#level)
      - [Save & Load](https://github.com/yolong1020/EldenRing/blob/main/README.md#save--load)   
   </br>
   
   ---
   </br>
   
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
   </br>
   
   ### Player
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

   ---
   </br>

   ### NPC
   ###### Player를 제외한 모든 캐릭터들의 Base Class이며, 특정 오브젝트를 집결지로 등록하고 에디터에서 정해진 정찰 지점들을 순회하는 정찰 기능을 포함하고 있습니다.</br>일반(Repose) / 경계(Vigilance) 상황을 구분하며, Health Bar Widget을 제공합니다.
   [1. Init Default Component & Values](#init-default-component--values-1)   
   [2. Patrol](#Patrol)   
   [3. Assembly Point](#Assembly-Point)   
   </br>


#### Init Default Component & Values
###### NPC에 해당하는 모든 캐릭터의 Component와 초기값을 설정합니다.

   - ###### Health Bar와 PawnSensing Component, 그리고 Level의 로드 상태에 따라 Spawn 위치를 초기화합니다.
     https://github.com/yolong1020/EldenRing/blob/e6bc14be5355ea76a3c64fa6c43ca390684d1d26/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L105-L130
     https://github.com/yolong1020/EldenRing/blob/6e79f5f9faabbc8e9469d0ce1b7bdebdd020a588/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L151-L154 </br>
   - ###### Level의 로드가 완료되지 않았다면, Level의 로드 완료 시점에 호출될 Event로 등록해줍니다.
     https://github.com/yolong1020/EldenRing/blob/ec2bd163de2ae2409d90b577fef49e66b5eb3700/Source/EldenRing/Public/GameBase/ERGameInstance.h#L35-L39
     https://github.com/yolong1020/EldenRing/blob/3e40e9ea14935ca05790f050c704db9ef6d152f8/Source/EldenRing/Private/Levels/GameField.cpp#L65-L108 </br>


   #### Patrol
   ###### 에디터에서 지정된 patrol Point들을 정찰 구역으로 설정하여 NPC가 순회 정찰을 진행합니다.
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/40e9e4ee-a3ad-4f68-8f2e-2b401f9d0a03" width="49.5%" height="49.5%"></img>
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/e8abfa97-d34c-4d93-8ef5-19489a25dca0" width="49.5%" height="49.5%"></img>   
   </br>
  
   1. ###### 에디터에서 설정한 정찰 시간만큼 타이머를 설정하고 캐릭터와 목적지 방향의 일치 여부에 따라 방향 전환 또는 전진을 진행합니다.
      https://github.com/yolong1020/EldenRing/blob/9df4da650f57f5ce2e5188f5e43b8a6efd15b771/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L426-L438
      https://github.com/yolong1020/EldenRing/blob/782c932b733fb24e3fea2aafafa5399941292032/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L301-L308 </br>
   2.
      1. ###### 전진하는 경우, AI Controller - MoveToLocation을 실행합니다.
         https://github.com/yolong1020/EldenRing/blob/5a8b244925fe1a501707bdec3c12b2b2de467878/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L525-L542 </br>
      2. ###### 방향 전환을 한 경우, 모션이 끝나는 시점에서 Anim Notify에 의해 OnTurnEnd()을 호출 후 목적지까지 전진을 실행합니다.
         https://github.com/yolong1020/EldenRing/blob/5a8b244925fe1a501707bdec3c12b2b2de467878/Source/EldenRing/Private/Characters/NPC/GameCharacter_AnimInstance.cpp#L153-L168 </br>
      3. ###### 각 자식 클래스에서 목적지에 도달했을 때, 처리할 행동들을 결정해줍니다. (ex : C4311)
         https://github.com/yolong1020/EldenRing/blob/3dfcb18ab1d642012182c9589f30a13c399d12cc/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L721-L735
         https://github.com/yolong1020/EldenRing/blob/5a8b244925fe1a501707bdec3c12b2b2de467878/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L305-L319 </br>


   #### Assembly Point
   ###### 캐릭터 에디터에서 설정된 정찰 시간 이후에 배정된 집결지에서 휴식을 취하며 타 NPC와 정찰을 교대합니다.
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/40a91a11-3099-44c4-bbec-99b3f48f7f46" width="49.5%" height="49.5%"></img>
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/25a3b779-5c6a-4efc-a22b-b287428e12e5" width="49.5%" height="49.5%"></img>   
   </br>
  
   1. ###### 에디터에서 Actor에 집결지를 등록시켰다면 정찰 이후 휴식을 요청할 수 있도록 Event를 등록합니다.
      https://github.com/yolong1020/EldenRing/blob/6506fdbd5ceb2d4ac179de25628c3aefc26d5c90/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L93-L96
      https://github.com/yolong1020/EldenRing/blob/6506fdbd5ceb2d4ac179de25628c3aefc26d5c90/Source/EldenRing/Private/FieldObject/AssemblyPoint/AssemblyPoint.cpp#L76-L92 </br>
   2. ###### 정찰 중 타이머가 종료되면 소속된 집결지에 휴식을 요청합니다.
      https://github.com/yolong1020/EldenRing/blob/36045d4e07d39f0f9d77df92cd67cdad0a472381/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L98-L102
      https://github.com/yolong1020/EldenRing/blob/0ab9d78cbdc2e640e78fec7b6f2f88c9b0b30b2d/Source/EldenRing/Private/FieldObject/AssemblyPoint/AssemblyPoint.cpp#L47-L53 </br>
   3. ###### 집결지에 연결된 휴식처는 예약되거나 사용중인 상태를 확인하고 집결지에 Callback을 보내거나 혹은 대기합니다.
      https://github.com/yolong1020/EldenRing/blob/d7daa2f1c786f453d7356ce52d4ccedaeb59806f/Source/EldenRing/Private/FieldObject/AssemblyPoint/AssemblePointObject.cpp#L65-L83 </br>
   4.
      1. ###### 휴식처에서 이용이 가능하다 판단이되면 집결지에 휴식처 사용 요청을 전달합니다.
         https://github.com/yolong1020/EldenRing/blob/99ec7e873cf0fdcc9b40f3d243c9fbd3dc5e399e/Source/EldenRing/Private/FieldObject/AssemblyPoint/AssemblyPoint.cpp#L55-L70 </br>
      2. ###### 만약 휴식중이던 NPC가 있다면 휴식 종료를 명령합니다.
         https://github.com/yolong1020/EldenRing/blob/21f7a92373efa785afd98093886e1999363acb96/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L400-L407 </br>
      3. ###### 휴식을 예약한 NPC는 휴식처로 이동 진행과 상태를 변경합니다.
         https://github.com/yolong1020/EldenRing/blob/d10785f149dba33bd42fc35a94303d3e6a9a9410/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L280-L292 </br>
   5. ###### 휴식처에 도달하면 집결지와 방향을 확인한 뒤 휴식을 진행합니다. 이후 휴식처에서 사용 시간을 기록합니다.
      https://github.com/yolong1020/EldenRing/blob/bf02f644eb1fa4c8f7d13f3891d9a7f980e9b794/Source/EldenRing/Private/FieldObject/AssemblyPoint/AssemblePointObject.cpp#L48-L63
      https://github.com/yolong1020/EldenRing/blob/bf02f644eb1fa4c8f7d13f3891d9a7f980e9b794/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L461-L487 </br>


   ---
   </br>

   ### Vender
   [1. Init Vender Settings](#Init-Vender-Settings)   
   [2. Interaction](#Interaction)   
   [3. Trade](#Trade)   
   [4. Observer Event](#Observer-Event)   
   </br>


   #### Init Vender Settings
   ###### 상인의 Interaction Collision, Observer Event, 판매 물품 등을 초기화합니다.   
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/dfe59b0a-b98f-4906-bba8-4e8223f5d269" width="49.5%" height="49.5%"></img>
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/9f752ea2-e3f4-4309-b036-6d80032d9c9e" width="49.5%" height="49.5%"></img>
   https://github.com/yolong1020/EldenRing/blob/9bbdd3573ed007ca5d21635021f044b7a39776d4/Source/EldenRing/Private/Characters/NPC/Vender/Vender.cpp#L46-L115 </br>


   #### Interaction
   ###### Interaction Collision을 통해 대화 및 거래를 진행합니다.   
   </br>

   1. ###### NPC와 상호 작용을 위한 Popup UI를 활성화합니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/6efc4ede-907a-4875-8b25-da10796d3b4a" width="49.5%" height="49.5%"></img>
      https://github.com/yolong1020/EldenRing/blob/76c15adfad13dc543afc0a4e612eebef0dde77ae/Source/EldenRing/Private/Characters/NPC/Vender/Vender.cpp#L222-L233
      https://github.com/yolong1020/EldenRing/blob/fcfc4de0477e09a17c3777194dfb107fd6432348/Source/EldenRing/Private/System/NPCInteractionManager.cpp#L26-L42
      https://github.com/yolong1020/EldenRing/blob/fcfc4de0477e09a17c3777194dfb107fd6432348/Source/EldenRing/Private/UI/PopUp/InteractionPopUp.cpp#L22-L61 </br>
   2. ###### 사용자 조작이 감지되면 상인에게 등록된 연출 카메라로 전환을 실행 후 다이얼로그를 활성화합니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/9c573ec3-36d4-4a66-8731-ca63ba4789d0" width="49.5%" height="49.5%"></img>
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/a52f015d-b491-4b1e-a8c0-30b303e7bc5b" width="49.5%" height="49.5%"></img>
      https://github.com/yolong1020/EldenRing/blob/6b38e1c98a77f94b902d5b61e34b656e4f60da7f/Source/EldenRing/Private/UI/PopUp/InteractionPopUp.cpp#L99-L112
      https://github.com/yolong1020/EldenRing/blob/06d5ac00a61008476953dfcb67f150318bbdba29/Source/EldenRing/Private/System/NPCInteractionManager.cpp#L112-L136

      1. ###### StartCommunicate는 오직 NPC와 대화만을 시작하기 위한 함수입니다.
         https://github.com/yolong1020/EldenRing/blob/06d5ac00a61008476953dfcb67f150318bbdba29/Source/EldenRing/Private/System/NPCInteractionManager.cpp#L77-L92
        
      2. ###### StartOptionCommunicate는 NPC와 대화뿐만 아니라 여러 기능이 포함된 상호 작용을 시작하기 위한 함수입니다.
         https://github.com/yolong1020/EldenRing/blob/06d5ac00a61008476953dfcb67f150318bbdba29/Source/EldenRing/Private/System/NPCInteractionManager.cpp#L94-L110

   3. ###### 상대 캐릭터가 무기를 손에 착용 중이라면 NPC는 위협받는 상황의 다이얼로그를 출력합니다.</br>무기를 착용하지 않았다면 마지막 다이얼로그를 기점으로 대화가 진행됩니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/7d8503c8-55ca-4466-b824-d71b7ef88f61" width="49.5%" height="49.5%"></img>
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/2185ea4f-27c2-4f8b-a0fb-4e8bd68fdb94" width="49.5%" height="49.5%"></img>
      https://github.com/yolong1020/EldenRing/blob/7454685f6f5655769f78f88afbd5babf3fad7702/Source/EldenRing/Private/UI/Communicate/CommunicateCallOut.cpp#L62-L97 </br>

   4. ###### 사용자 조작으로 다이얼로그가 업데이트되면 현재 다이얼로그에 명시된 Options로부터 테이블에서 차기 다이얼로그를 찾아 대화를 갱신합니다.</br>Trigger Event가 활성되어 있고 Event Functions에 기능이 있다면 순차적으로 기능을 실행합니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/ccf07354-3210-4566-a590-28fa1327a9ea"></img>
      https://github.com/yolong1020/EldenRing/blob/79a67115117f06931986e3c4eb5f9c3bfc287220/Source/EldenRing/Private/UI/Communicate/CommunicateCallOut.cpp#L39-L60
      https://github.com/yolong1020/EldenRing/blob/79a67115117f06931986e3c4eb5f9c3bfc287220/Source/EldenRing/Private/UI/Communicate/CommunicateCallOut.cpp#L133-L162 </br>

   5. ###### 다이얼로그 데이터에 Button Type이 명시되었고 Event Function으로 SelectOption이 있는 경우라면 Select Popup을 활성하고 각 버튼에 기능을 바인딩합니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/e2b25780-a6f4-477b-bb0f-0145871bb102" width="49.5%" height="49.5%"></img>
      https://github.com/yolong1020/EldenRing/blob/79a67115117f06931986e3c4eb5f9c3bfc287220/Source/EldenRing/Private/UI/Communicate/CommunicateCallOut.cpp#L188-L201
      https://github.com/yolong1020/EldenRing/blob/f00b6884414a0925be8a92609950e9f87d553c32/Source/EldenRing/Private/System/NPCInteractionManager.cpp#L51-L74
      https://github.com/yolong1020/EldenRing/blob/acee9a4b3f1c631e0e61c76854ab00efe662585a/Source/EldenRing/Private/UI/PopUp/SelectionPopUp.cpp#L56-L86 </br>

     
   #### Trade
   ###### Trade Manager를 통해 거래창을 활성화하고 상인이 가진 판매 물품들을 UI에 등록합니다.</br>[Trade에 대한 상세 내용은 여기에서 확인할 수 있습니다.](https://github.com/yolong1020/EldenRing/blob/main/README.md#trade-1)
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/993eee19-f933-416c-81b1-2566d4fdc047" width="49.5%" height="49.5%"></img>   
   https://github.com/yolong1020/EldenRing/blob/ed11ad611e3bb18e52de447d6298a00c52477770/Source/EldenRing/Private/UI/Communicate/CommunicateCallOut.cpp#L203-L214
   https://github.com/yolong1020/EldenRing/blob/ed11ad611e3bb18e52de447d6298a00c52477770/Source/EldenRing/Private/System/TradeManager.cpp#L18-L46 </br>


   #### Observer Event
   ###### 특정 상황에 대해 등록된 Observer Event를 통하여 상인을 이동시키고 마지막 다이얼로그를 변경합니다.
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/d7634f0b-4989-4d5a-b8c0-9f540a5aeb98" width="49.5%" height="49.5%"></img>
   https://github.com/yolong1020/EldenRing/blob/dabd8129d746f36959fbded0c68a4d6e132cfec6/Source/EldenRing/Private/FieldObject/Interactable/Bonefire.cpp#L50-L71
   https://github.com/yolong1020/EldenRing/blob/800d0838f0fd4e904f75e179c47b8c0c04dd9e1f/Source/EldenRing/Private/Characters/NPC/Vender/Vender.cpp#L197-L215 </br>

   ---
   </br>

   ### Soldier of Godrick
   [1. Change Battle Mode](#Change-Battle-Mode)   
   [2. Detect Bound](#Detect-Bound)   
   [3. Combat](#combat-1)   
   [4. Confront](#Confront)   
   </br>


   #### Change Battle Mode
   ###### 정찰 혹은 휴식 중 전투 상황이 발생하면 NPC가 전투 태세로 전환합니다.  

   1.
      1. ###### 비전투 상황에서 적(플레이어)이 PawnSeen에 감지되면 전투 태세로 전환합니다.
         <img src="https://github.com/yolong1020/EldenRing/assets/87303898/e0b2874b-5ffb-4518-9a47-ce553fb4c4b7" width="49.5%" height="49.5%"></img>
         https://github.com/yolong1020/EldenRing/blob/954786d152d35815853399236b731258e0fe9d88/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L243-L258 </br>
      2. ###### 비전투 상황에서 적(플레이어)에게 피격받게 되면 피격 동작을 재생한 뒤 전투 태세로 전환합니다.
         <img src="https://github.com/yolong1020/EldenRing/assets/87303898/9b454323-8dab-439c-a768-69dcaf978844" width="49.5%" height="49.5%"></img>
         https://github.com/yolong1020/EldenRing/blob/954786d152d35815853399236b731258e0fe9d88/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L65-L86
         https://github.com/yolong1020/EldenRing/blob/954786d152d35815853399236b731258e0fe9d88/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L770-L789 </br>
   2. ###### 전투 상황에 돌입하면 보유한 무기를 꺼내며 전투를 준비합니다.
      https://github.com/yolong1020/EldenRing/blob/233c4f3a063e5d102b1eb215b4d19cca4fc687ae/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L314-L326 </br>
   3. ###### Anim Notify의 호출로 전투 준비가 마무리되면 목표와 방향을 확인한 후 돌격하거나 혹은 방향 전환을 시도합니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/a834a35b-8d8d-4612-97fb-565b1ef47b85"></img>
      https://github.com/yolong1020/EldenRing/blob/73244f3847390d239d978cd85bed74aa4f8cc33b/Source/EldenRing/Private/Characters/NPC/GameCharacter_AnimInstance.cpp#L117-L121
      https://github.com/yolong1020/EldenRing/blob/ff8ab74441625804f894632f27d72827a478767a/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L665-L703 </br>


   #### Detect Bound
   ###### 캐릭터를 중심으로 3개의 영역을 통해 적을 감지하여 추격과 공격, 대치 상황을 결정합니다.
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/56efd8a8-b65f-4cc3-97b6-87934c75724a" width="49.5%" height="49.5%"></img>   
  
   - ###### Red   : 범위 내에 적이 위치하고 PawnSeen에 감지되었다면 추격을 진행하며 범위를 벗어나는 순간 추격이 중단됩니다.
   - ###### Blue  : 범위 내에 적이 위치하고 대치 상황이라면 현 상황을 이어갑니다. 범위를 벗어난다면 추격을 진행하게 됩니다.
   - ###### Black : 범위 내에 적이 위치한다면 공격을 실행할 수 있습니다.
   </br>


   #### Combat
   ###### 전투 태세에 돌입하면 추격을 진행한 후 일반 공격, 강 공격, 대쉬 공격 중 선택하여 공격을 시도합니다. 이후 이전 공격에 따라 차기 공격을 이어서 전개합니다.
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/69093a96-9670-496f-a775-234e8d35a1be" width="49.5%" height="49.5%"></img>
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/481f1e8c-f422-4610-bc70-976d104b8486" width="49.5%" height="49.5%"></img>   
   </br>

   1. ###### 적이 있는 위치까지 도달한 경우, 공격을 시도합니다.
      https://github.com/yolong1020/EldenRing/blob/9f94c6c33a0fd357d99e4a230b46e7b57cd2c3e8/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L702-L716 </br>
   2. ###### Anim Notify - OnNextAttack 호출로 차기 공격이 가능하다면 3가지 조건(1. 이전 공격 성공 / 2.공격 범위 내 대상 존재 / 3. 무작위 추가 공격) 중 하나라도 해당할 때 다음 공격을 이어 진행합니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/a1fce18b-87c9-4e06-b9de-3e4ef4a5c019"></img>
      https://github.com/yolong1020/EldenRing/blob/7795ef6b643fb2224b25a0332ff301d7ef567cfb/Source/EldenRing/Private/Characters/NPC/GameCharacter_AnimInstance.cpp#L135-L139
      https://github.com/yolong1020/EldenRing/blob/17577c2e55ffe4bc18ee74eb15d494718b2eff2c/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L718-L741
      https://github.com/yolong1020/EldenRing/blob/17577c2e55ffe4bc18ee74eb15d494718b2eff2c/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L515-L535 </br>
   3. ###### 차기 공격이 불가능하거나 중지됐다면, 공격을 종료하고 현재 병사의 상태와 적의 범위 내 위치를 확인하여 다음 행동을 결정합니다.
      https://github.com/yolong1020/EldenRing/blob/d44c1adb3ed052a712606a23e5945b0cf78561ee/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L743-L753 </br>


   #### Confront
   ###### 대치 태세에 돌입하면 앞 / 뒤 / 좌 / 우 방향으로 움직이며 적을 주시합니다.</br>이때 적이 속한 영역(추격 / 경계 / 공격)이 변경되거나 무작위로 차기 행동이 공격으로 결정되면 Combat 과정이 실행됩니다. 
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/e73c97ba-ac30-4e54-9001-1cfa633f3d00" width="49.5%" height="49.5%"></img>
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/79c26ec6-f65e-40b3-84ff-a35d74f3eed0" width="49.5%" height="49.5%"></img>   
   </br>

   1. ###### 대치 태세 돌입과 함께 4방향 중 무작위로 한 방향을 선정하여 이동을 진행하며, 다음 행동을 선택하기까지 타이머를 설정합니다.
      https://github.com/yolong1020/EldenRing/blob/7a1a74ed948d9dc7180165b7ba28291389b92dd1/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L388-L425 </br>
   2. ###### 추가 대치와 공격 중 차기 행동을 결정하여 진행합니다.
      https://github.com/yolong1020/EldenRing/blob/13da7ea0c6d228f2be811e1b79f300a0fa977c83/Source/EldenRing/Private/Characters/NPC/Enemies/C4311/C4311.cpp#L535-L564 </br>

   ---
   </br>

   ### Tree Guard
   [1. Behavior Tree](#Behavior-Tree)   
   [2. Detect Bound](#detect-bound-1)   
   [3. Init Tree Guard Settings](#Init-Tree-Guard-Settings)   
   [4. Combat](#combat-2)   
   </br>


   #### Behavior Tree
   ###### 플레이어를 감지하면 공격 범위와 현재 Action 실행 유무로 공격과 이동을 결정하도록 노드를 구성하였습니다.
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/95c304ff-a756-47a0-8e9c-7d9f399f5720"></img>   
   </br>


   #### Detect Bound
   ###### 캐릭터를 중심으로 3개의 영역을 통해 적을 감지하여 추격과 공격, 기동으로 연계하는 Service입니다. 
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/b020d7bd-eff3-433c-8039-ef7906b0c69b" width="49.5%" height="49.5%"></img>   
   https://github.com/yolong1020/EldenRing/blob/2fb19a7c590b8cb05dd116ac91068cc81dbf7f1a/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/Services/BTService_Detect.cpp#L17-L33
  
   - ###### Red   : 범위 내에 적이 위치한다면 돌진 공격이 가능하며 범위를 벗어나는 순간 추격이 시작됩니다.
   - ###### Blue  : 범위 내에 적이 위치한다면 단일 공격을 실행할 수 있습니다.
   - ###### Black : 범위 내에 적이 위치한다면 단일 공격과 콤보 공격을 실행할 수 있습니다.
   </br>

   #### Init Tree Guard Settings
   ###### Tree Guard의 블랙보드와 몸통 Collision, Action 데이터 등을 초기화합니다.
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/1e557d19-af9d-4e0b-be3f-c78c477171be" width="49.5%" height="49.5%"></img>
   https://github.com/yolong1020/EldenRing/blob/d5f795bbce272476ba0d7c9afb9bfc4c067c4ecb/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/C3251.cpp#L29-L45
   https://github.com/yolong1020/EldenRing/blob/d5f795bbce272476ba0d7c9afb9bfc4c067c4ecb/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/C3251.cpp#L58-L145 </br>


   #### Combat
   ###### 전투 태세에 돌입하면 추격을 진행한 후 일반 공격, 강 공격, 돌진 공격 중 선택하여 공격을 시도합니다.</br>각 공격은 고유한 Action ID로 구분되며 파생되는 Action들과 Action 유형, 방향, 공격 범위 등의 데이터를 지니고 있습니다.</br>이 데이터를 통해 적과 자신의 방향에 따라 다양한 공격을 시도하고 현재 Action으로부터 콤보 공격으로 이어갈 수 있습니다.

   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/888feca3-8cec-4f47-9f10-c76d3c9058c7"></img>   
    
   1. ###### 적이 Pawn Seen에 의해 목격되면 보스 전투가 시작됩니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/09dbc467-6faf-44e5-a86f-23624dfdca80" width="49.5%" height="49.5%"></img>
      https://github.com/yolong1020/EldenRing/blob/d5f795bbce272476ba0d7c9afb9bfc4c067c4ecb/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/C3251.cpp#L316-L331
      https://github.com/yolong1020/EldenRing/blob/b2d30cab3821d6cc6ff444f5d95f6c6165250bd9/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/C3251.cpp#L350-L366 </br>
     
   2. ###### Target과 Acting 블랙보드가 변경됨에 따라 Task - Select next action을 통해 차기 행동을 결정합니다.</br>만약 이전 시도한 공격이 파생되는 Derived Attack을 가지고 있다면 해당 Action들을 통해 콤보를 전개할 수 있습니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/77072f13-da55-45f0-85d0-d9e18481d5b7" width="49.5%" height="49.5%"></img>
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/01aeedd5-16c8-4d34-aafa-ee87775125c3" width="49.5%" height="49.5%"></img>
      https://github.com/yolong1020/EldenRing/blob/af11b727778d31fb7a56716f8ce201783cca885c/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/Tasks/BTTask_SelectNextAction.cpp#L18-L73
      https://github.com/yolong1020/EldenRing/blob/b1b47bfa08a3ea2424d944f71bf9af1667e910d3/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/C3251.cpp#L514-L522
     
      1. ###### 돌진 공격은 현재 전투 페이즈에 따라 선택의 폭을 달리하고, 선택된 Action ID에 명시된 Attack Weight로 공격 강도를 설정합니다.
         <img src="https://github.com/yolong1020/EldenRing/assets/87303898/9f81b25b-29b8-4a42-90ab-617582b72f48" width="49.5%" height="49.5%"></img>
         https://github.com/yolong1020/EldenRing/blob/b1b47bfa08a3ea2424d944f71bf9af1667e910d3/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/C3251.cpp#L368-L389
      2. ###### 단일 공격은 자신으로부터 적의 위치에 따라 공격 방법을 다르게 전개하며, 적이 후방에 있다면 공격을 취소하고 차기 Action을 Task에서 선택하도록 합니다.</br>만약 적의 위치가 차기 Action의 Action Bound에 만족하지 않는다면 이 역시 공격을 취소하도록 합니다.</br>공격 강도는 선택된 Action ID에 명시된 Attack Weight로 설정합니다.
         <img src="https://github.com/yolong1020/EldenRing/assets/87303898/5a275326-923b-4b02-941e-c5558afaee12" width="49.5%" height="49.5%"></img>
         https://github.com/yolong1020/EldenRing/blob/81b539b61a82d83bdfb7b2efb76a33a464785c8d/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/C3251.cpp#L391-L458
      3. ###### 콤보 공격은 이전 Action ID를 통해 파생되는 공격 중 무작위로 선정하여 Action의 공격 방향이 적과 일치하면 공격을 실행합니다.</br>만약 방향이 일치하지 않다면 공격을 취소합니다.</br>공격 강도는 선택된 Action ID에 명시된 Attack Weight로 설정합니다.
         <img src="https://github.com/yolong1020/EldenRing/assets/87303898/467894cc-b5dc-4a5c-8a54-c1c43af0819d" width="49.5%" height="49.5%"></img>
         https://github.com/yolong1020/EldenRing/blob/b1b47bfa08a3ea2424d944f71bf9af1667e910d3/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/C3251.cpp#L460-L499 </br>
      4. ###### 공격이 취소되면 각 공격 유형별로 바인딩된 콜백으로 블랙보드의 값을 변경합니다.
         https://github.com/yolong1020/EldenRing/blob/2b98a6027654a6243781ea69ea04c5b3ca97b565/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/Tasks/BTTask_DashAttack.cpp#L16-L32
         https://github.com/yolong1020/EldenRing/blob/2b98a6027654a6243781ea69ea04c5b3ca97b565/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/Tasks/BTTask_SingleAttack.cpp#L16-L33
         https://github.com/yolong1020/EldenRing/blob/2b98a6027654a6243781ea69ea04c5b3ca97b565/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/Tasks/BTTask_ComboAttack.cpp#L17-L35 </br>
        
   3. ###### 공격 외 Moving Action은 적으로부터 거리를 두거나 방향을 일치시키는데 사용하는 Action입니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/0d82740e-d172-4d1f-9e2d-6f90123cf82b" width="49.5%" height="49.5%"></img>
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/854285d1-3257-4671-8374-97cb2278bc77" width="49.5%" height="49.5%"></img>
      https://github.com/yolong1020/EldenRing/blob/3463b8fcaaff2853e22dee95b2f6790c7926ee41/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/C3251.cpp#L501-L512
      https://github.com/yolong1020/EldenRing/blob/e757abfc3cfab038db0fc24a4db58aaa208578e5/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/Tasks/BTTask_MoveAction.cpp#L16-L34 </br>


   ---
   </br>
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/f6812ab4-5d8a-4001-98ec-545a0264dac5"></img>   

   ### Inventory - Equipment
   ###### 획득한 아이템을 캐릭터에게 장착, 해제시켜 Attribute에 반영하는 기능을 제공합니다. 무기의 경우 HUD의 Quick Slot에 노출됩니다.   
   [1. Equip item](#Equip-item)   
   [2. Unequip item](#Unequip-item)   
   </br>


   #### Equip item
   ###### Equipment Manager를 통하여 Item을 캐릭터에게 착용시킵니다.   
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/0c3c8a58-7018-4cdd-979b-c83948dc3028" width="49.5%" height="49.5%"></img>   
   </br>

   1. ###### 착용하려는 아이템 유형과 좌/우 위치(무기 한정)를 확인하여 슬롯에 장착시킵니다.
      https://github.com/yolong1020/EldenRing/blob/7e6dee018db108b2a9c756429f89ef9cc52c282b/Source/EldenRing/Private/System/EquipmentManager.cpp#L36-L59
      https://github.com/yolong1020/EldenRing/blob/7e6dee018db108b2a9c756429f89ef9cc52c282b/Source/EldenRing/Private/UI/Inventory/InventoryEquipSlot.cpp#L47-L62 </br>
   2.
      1. ###### 착용하려는 아이템의 매쉬를 캐릭터에 반영합니다.
         https://github.com/yolong1020/EldenRing/blob/f49792ab9b9dc99f1ca0b7278e9a456d946769f9/Source/EldenRing/Private/System/EquipmentManager.cpp#L180-L224
      2. ###### 무기의 경우, Weapon_Actor에서 무기의 매쉬를 사용자에게 부착합니다.
         https://github.com/yolong1020/EldenRing/blob/9f8fef7608cfddae2743db98a7142e9308236109/Source/EldenRing/Private/Items/Weapons/Weapon_Actor.cpp#L190-L202
      3. ###### 갑옷의 경우, UItemObject의 정보를 토대로 캐릭터의 일부 Material을 숨김 처리합니다.
         https://github.com/yolong1020/EldenRing/blob/f49792ab9b9dc99f1ca0b7278e9a456d946769f9/Source/EldenRing/Private/System/EquipmentManager.cpp#L226-L242
      4. ###### 무기를 착용하였다면 PlayerHUD의 QuickSlot을 갱신합니다.
         https://github.com/yolong1020/EldenRing/blob/55e7ecacb75266760a2f1ac112eddb4c23ba78b1/Source/EldenRing/Private/HUD/Status/PlayerHUD.cpp#L97-L126 </br> 
   3. ###### 아이템의 옵션을 캐릭터의 Attribute에 적용시킵니다.
      https://github.com/yolong1020/EldenRing/blob/7e6dee018db108b2a9c756429f89ef9cc52c282b/Source/EldenRing/Private/Characters/Players/C0000.cpp#L113-L121
      https://github.com/yolong1020/EldenRing/blob/7e6dee018db108b2a9c756429f89ef9cc52c282b/Source/EldenRing/Private/Characters/Component/AttributeComponent.cpp#L71-L86 </br>


   #### Unequip item
   ###### Equipment Manager를 통하여 캐릭터에 착용된 Item을 해제시킵니다.

   1. ###### 해제하려는 아이템 유형과 좌/우 위치(무기 한정)를 확인하여 슬롯에서 해제시킵니다.
      https://github.com/yolong1020/EldenRing/blob/9cb9d5313e1965c89f7e2fdd8e660c3f75f9a38d/Source/EldenRing/Private/System/EquipmentManager.cpp#L61-L93
      https://github.com/yolong1020/EldenRing/blob/07913ab06541dbf8959c6f1a4a7fc33016dc2247/Source/EldenRing/Private/UI/Inventory/InventoryEquipSlot.cpp#L64-L72 </br>
   2.
      1. ###### 무기의 경우, 해제시킨 무기를 파괴하고 캐릭터가 보유한 무기 포인터를 제거한 뒤 PlayerHUD를 갱신시킵니다.
         https://github.com/yolong1020/EldenRing/blob/687e1f73144e985e35ab330b625f68a243aa6e76/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1237-L1253 </br>
      2. ###### 갑옷의 경우, 해제시킨 갑옷의 매쉬 컴포넌트를 해제하고 포인터를 제거한 뒤 캐릭터의 Material들을 갱신시킵니다.
         ```C++
         /// [UEquipmentManager::TryUnequipItem] 81
	      m_equipments[int32(Type) - 1].Key->SetSkeletalMesh(nullptr);
	      m_equipments[int32(Type) - 1].Key->UnregisterComponent();
	      m_equipments[int32(Type) - 1].Key = nullptr;
         ```
         https://github.com/yolong1020/EldenRing/blob/687e1f73144e985e35ab330b625f68a243aa6e76/Source/EldenRing/Private/System/EquipmentManager.cpp#L226-L242 </br>
   3. ###### 아이템의 옵션을 캐릭터의 Attribute에서 제외시킵니다.
      https://github.com/yolong1020/EldenRing/blob/7e6dee018db108b2a9c756429f89ef9cc52c282b/Source/EldenRing/Private/Characters/Players/C0000.cpp#L113-L121
      https://github.com/yolong1020/EldenRing/blob/687e1f73144e985e35ab330b625f68a243aa6e76/Source/EldenRing/Private/Characters/Component/AttributeComponent.cpp#L88-L102 </br> 


   ### Inventory - Storage
   ###### 사용자가 획득한 아이템과 재화를 관리하는 UI로 획득한 아이템들은 필드에 버릴 수 있고 특정 NPC와 거래할 수 있습니다.   
   [1. Init storage widget](#Init-storage-widget)   
   [2. Open storage](#Open-storage)   
   [3. Add item to storage](#Add-item-to-storage)   
   [4. Remove item to storage](#Remove-item-to-storage)   
   </br>


   #### Init storage widget
   ###### Slot Count에 따라 보관함 슬롯 수를 설정하고 상호 작용을 위한 초기화 작업을 진행합니다.   

   - ###### Storage Manager를 통하여 Inventory의 각 위젯들을 초기화합니다. (Slot count, Delegate, etc)
     https://github.com/yolong1020/EldenRing/blob/d8a4482dee125ba06ce66e03dfea90628b27c71f/Source/EldenRing/Private/System/StorageManager.cpp#L42-L85
     https://github.com/yolong1020/EldenRing/blob/d8a4482dee125ba06ce66e03dfea90628b27c71f/Source/EldenRing/Private/UI/Inventory/InventoryGrid.cpp#L47-L66 </br>
  
     <img src="https://github.com/yolong1020/EldenRing/assets/87303898/f37436e7-ffbe-409c-9bcd-98da953456e8" width="60%" height="60%">
  
   - ###### InventorySlotGrid는 화면에 출력될 보관함 슬롯을 담당하는 위젯입니다.
     https://github.com/yolong1020/EldenRing/blob/66be893c43cf52f5783ec93ff1e88fb007b92325/Source/EldenRing/Private/UI/Inventory/InventorySlotGrid.cpp#L20-L48
   - ###### InventoryInteractionGrid는 실제 아이템과 보관함의 상호작용을 담당하는 위젯입니다.
     https://github.com/yolong1020/EldenRing/blob/d8a4482dee125ba06ce66e03dfea90628b27c71f/Source/EldenRing/Private/UI/Inventory/InventoryInteractionGrid.cpp#L24-L39 </br>

  
   #### Open storage
   ###### Storage Manager를 통하여 보관함을 활성화하며 활성되는 시점에서 보관된 Item들을 갱신합니다.
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/e5fb100b-1e9f-4d51-a1c5-86108ecea6ae" width="49.5%" height="49.5%"></img>   
   </br>

   1. ###### Inventory를 활성화하며 동시에 'Esc' 입력시 최상위 UI 판별을 위해 UI Manager에 등록해줍니다.
      https://github.com/yolong1020/EldenRing/blob/808bb8d461a42c4bc8d677944969d10fdc64c29e/Source/EldenRing/Private/System/StorageManager.cpp#L23-L35 </br>
   2. ###### InteractionGrid를 갱신하여 보관함에 존재하는 아이템들을 Canvas Slot에 포함시킵니다.
      https://github.com/yolong1020/EldenRing/blob/674fd8903ad223d87e64afd43a168944b8cececc/Source/EldenRing/Private/UI/Inventory/InventoryInteractionGrid.cpp#L41-L76 </br>
   3. ###### Canvas Slot에 포함된 아이템들은 'UItemObject'의 정보를 토대로 텍스쳐 이미지와 크기를 갱신합니다.
      https://github.com/yolong1020/EldenRing/blob/75633c135b4a24c70af3405552cb2083240139c8/Source/EldenRing/Private/UI/Inventory/InventoryItem.cpp#L218-L244 </br>
  

   #### Add item to storage
   ###### 아이템과 재화를 보관하며 필요에 따라 착용하거나 버릴 수 있습니다.
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/06255a66-19e5-476e-9327-0ed88479c5bc" width="49.5%" height="49.5%"></br>

   1.
      1. ###### 특정 위치를 지정하지 않고 아이템을 적재할 경우, 순차적으로 적재가 가능한 위치를 탐색 후 적재합니다. 모든 슬롯을 순회함에도 적재가 불가능하다면 아이템을 회전시켜 한번 더 적재를 시도합니다.
         https://github.com/yolong1020/EldenRing/blob/91a7a54264022be27051620a753d14886cf3f565/Source/EldenRing/Private/System/StorageManager.cpp#L127-L149 </br>
      2. ###### 특정 슬롯의 인덱스를 확인 후 적재를 시도합니다.
         https://github.com/yolong1020/EldenRing/blob/91a7a54264022be27051620a753d14886cf3f565/Source/EldenRing/Private/System/StorageManager.cpp#L151-L157 </br>
      3. ###### 특정 슬롯의 타일 위치를 확인 후 적재를 시도합니다.
         https://github.com/yolong1020/EldenRing/blob/88fb131d94fdb9e1bd3609f015b66e5eee64da4d/Source/EldenRing/Private/System/StorageManager.cpp#L159-L166 </br>
   2. ###### 적재는 좌측 상단을 기준으로 시작되며, 기준으로부터 아이템 크기까지 각 슬롯에 해당하는 아이템의 포인터를 할당합니다. </br>만약 위젯이 노출되고 있다면, 적재된 아이템이 노출될 수 있도록 InteractionGrid를 갱신합니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/e7857442-af04-49ef-a15a-7173275bc311" width="40%" height="40%">
      https://github.com/yolong1020/EldenRing/blob/88fb131d94fdb9e1bd3609f015b66e5eee64da4d/Source/EldenRing/Private/System/StorageManager.cpp#L198-L227 </br>


   #### Remove item to storage
   ###### 아이템을 착용하거나 필드에 버리는 경우, 보관함에서 해당 아이템을 제거합니다.
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/0c3c8a58-7018-4cdd-979b-c83948dc3028" width="49.5%" height="49.5%"></img>   
   </br>
  
   - ###### 모든 슬롯을 순회하며 해당하는 아이템의 포인터를 제거해줍니다. </br>만약 위젯이 노출되고 있다면, 적재된 아이템이 노출되지 않도록 InteractionGrid를 갱신합니다.
     https://github.com/yolong1020/EldenRing/blob/345d258673e6c6e69667b41a85b8ae642f124db3/Source/EldenRing/Private/System/StorageManager.cpp#L229-L240 </br>

  
   ---
   </br>
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/eabc3cfa-695d-4695-b852-332ed7646cd4"></img>   

   ### Trade
   [1. Init trade widget](#Init-trade-widget)   
   [2. Open trade](#Open-trade)   
   [3. Purchase & Sell Item](#purchase--sell-item)   
   </br>

   #### Init trade widget
   ###### Blueprint에서 설정한 Slot Count에 따라 거래 보관함 슬롯 수를 설정하고 상호 작용을 위한 초기화 작업을 진행합니다.

   - ###### Trade Manager를 통하여 Trade의 각 위젯들을 초기화합니다. (Slot count, Delegate, etc)
     https://github.com/yolong1020/EldenRing/blob/9bc3b0f4022cdfa966450509556193acb8d53ced/Source/EldenRing/Private/System/TradeManager.cpp#L60-L100 </br>
  
     <img src="https://github.com/yolong1020/EldenRing/assets/87303898/c8c9cb52-a810-4796-b3aa-d2649d55d505" width="60%" height="60%">
  
   - ###### TradeGrid는 화면에 출력될 각 캐릭터들의 보관함 슬롯을 담당하는 위젯입니다.
     https://github.com/yolong1020/EldenRing/blob/16f8271c446ef841156cb116a36278ef2d22b4a0/Source/EldenRing/Private/UI/Trade/TradeGrid.cpp#L17-L41
   - ###### TradeInteractionGrid는 실제 아이템과 캐릭터들의 보관함 상호작용을 담당하는 위젯입니다.
     https://github.com/yolong1020/EldenRing/blob/16f8271c446ef841156cb116a36278ef2d22b4a0/Source/EldenRing/Private/UI/Trade/TradeInteractionGrid.cpp#L28-L44 </br>


   #### Open trade
   ###### Trade Manager를 통하여 거래창을 활성화하며 활성되는 시점에서 보관된 Item들을 갱신합니다.

   1. ###### Trade를 활성화하며 동시에 'Esc' 입력시 최상위 UI 판별을 위해 UI Manager에 등록해줍니다.
      https://github.com/yolong1020/EldenRing/blob/dcd26958cb061a08392d17ad0e40ba8278bde2e0/Source/EldenRing/Private/System/TradeManager.cpp#L18-L46 </br>
   2. ###### TradeInteractionGrid를 갱신하여 각 캐릭터들의 보관함에 존재하는 아이템들을 Canvas Slot에 포함시킵니다.
      https://github.com/yolong1020/EldenRing/blob/239239bd7b779bae6986782cf5861d1e0b00a894/Source/EldenRing/Private/System/TradeManager.cpp#L125-L137
      https://github.com/yolong1020/EldenRing/blob/065770f13537e7d9442c659c0653f61069d1a681/Source/EldenRing/Private/UI/Trade/TradeInteractionGrid.cpp#L46-L80 </br>
   3. ###### Canvas Slot에 포함된 아이템들은 'UItemObject'의 정보를 토대로 텍스쳐 이미지와 크기를 갱신합니다.
      https://github.com/yolong1020/EldenRing/blob/2c3e7d450dd827127f4ce4ad46fe18e74c02adc0/Source/EldenRing/Private/UI/Trade/TradeItem.cpp#L247-L259 </br>


   #### Purchase & Sell Item
   ###### Vender NPC로부터 재화를 소모하여 아이템을 구매하거나 아이템을 판매하여 재화를 얻을 수 있습니다.

   1.
      1. ###### 마우스 Input을 통한 거래로 우 클릭은 팝업없이 현재 아이템의 소유자를 확인 후 구매 / 판매 기능을 수행합니다.</br>좌 클릭의 경우 NPC 보관함에서 구매 팝업이 활성되며 거래가 진행됩니다.
         <img src="https://github.com/yolong1020/EldenRing/assets/87303898/c070af3b-3b30-40b8-a621-7022385dd62b" width="49.5%" height="49.5%"></img>
         <img src="https://github.com/yolong1020/EldenRing/assets/87303898/06255a66-19e5-476e-9327-0ed88479c5bc" width="49.5%" height="49.5%"></br>
         https://github.com/yolong1020/EldenRing/blob/117b7a1f1e985fdc4adfa030ed3973386def500b/Source/EldenRing/Private/UI/Trade/TradeItem.cpp#L88-L129
      2. ###### 마우스 Drag Drop을 통한 거래로 마우스 Input 좌 클릭처럼 팝업이 활성되어 판매 거래가 진행됩니다.
         <img src="https://github.com/yolong1020/EldenRing/assets/87303898/7d62837f-9e3f-4038-928e-5eb6180df07f" width="49.5%" height="49.5%"></br>
         https://github.com/yolong1020/EldenRing/blob/decb4891e7cbb23ae9f938fd0b817115d63d86e1/Source/EldenRing/Private/UI/Trade/TradeInteractionGrid.cpp#L145-L197 </br>  
   2. ###### 키보드 Input에 의한 거래로 플레이어가 아이템 구매 시 즉시 착용을 함께 수행합니다.</br>만약 아이템이 플레이어 소유라면 즉시 착용만 수행하게 됩니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/76c1d8a2-f5dd-40b7-9a1a-1a7d409a481d" width="49.5%" height="49.5%"></img>
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/9cb8009e-e8e6-4251-a56c-df0e312db1ff" width="49.5%" height="49.5%"></br>
      https://github.com/yolong1020/EldenRing/blob/117b7a1f1e985fdc4adfa030ed3973386def500b/Source/EldenRing/Private/UI/Trade/TradeItem.cpp#L184-L245 </br>
   3. ###### 구매 / 판매 여부에 따라 캐릭터별로 재화 갱신이 달라지며, 아이템 즉시 착용이 진행되는 경우 보관함에는 수납하지 않습니다.</br> 거래가 종료되면 게임의 진행 상황을 저장합니다.
      https://github.com/yolong1020/EldenRing/blob/bc678c39609bf7e91b604d6f22b202db1336da32/Source/EldenRing/Private/UI/Trade/TradeItem.cpp#L272-L311 </br>

   ---
   </br>

   ### Tutorial
   [1. Init tutorial](#Init-tutorial)   
   [2. Open by VolumeBox](#Open-by-VolumeBox)   
   </br>


   #### Init tutorial
   ###### 노출될 튜토리얼 팝업을 데이터로 보관하고 필요에 따라 텍스트와 Action, 지연시간을 설정하여 Callout과 Group Callout 중 선택하여 호출합니다.

   1. ###### 튜토리얼로 노출될 텍스트와 안내하고자 하는 Action, 입력할 key를 데이터로 생성합니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/48002333-ce16-4759-bd36-5059f068b0a4" width="50%" height="50%">
      </br>

   2.
      - ###### Callout : 하나의 Action을 안내할때 사용하며 텍스트와 Input Action Enum을 인자로 전달합니다.
        <img src="https://github.com/yolong1020/EldenRing/assets/87303898/1900ad25-252d-4830-b0f7-adba3508fedf" width="49.5%" height="49.5%"></br>
        ```C++
        ///  [AC0000::GetHit] 491
        float percent = m_attribute->GetHealthPercent();
        if (percent < 0.5 && false == m_ui_mgr->IsActiveCallout(FName("Potion")))
        {
	      m_ui_mgr->OpenTutorialCallout(FName("Potion"), EInputActionType::EIAT_Potion, false);
        }
        ```
        https://github.com/yolong1020/EldenRing/blob/923edcec6840de011a514f9d695daa7c1bfb142c/Source/EldenRing/Private/System/UIManager.cpp#L161-L167
        https://github.com/yolong1020/EldenRing/blob/159c4f8797d3484c2aedf8094cba9c9dcca11b96/Source/EldenRing/Private/UI/Tutorial/TutorialCallout.cpp#L22-L28 </br>
       
      - ###### Group Callout : 둘 이상의 Action을 안내할때 사용하며 최대 3개까지 안내가 가능합니다.
        <img src="https://github.com/yolong1020/EldenRing/assets/87303898/e60e4b7e-3bfb-46fc-b7bd-a5048bb759e2" width="49.5%" height="49.5%"></br>
        ```C++
        ///  [AGameField::StartStage] 96
        TArray<FTutorialPersistentData> datas;
        FTutorialPersistentData Dodge(FName("Dodge"), EInputActionType::EIAT_Dodge, ETutorialPosition::ETP_Top, true);
        datas.Add(Dodge);
        FTutorialPersistentData Sprint(FName("Sprint(Hold)"), EInputActionType::EIAT_Sprint, ETutorialPosition::ETP_Mid, true);
        datas.Add(Sprint);
        FTutorialPersistentData Walk(FName("Walk(Hold)"), EInputActionType::EIAT_Walk, ETutorialPosition::ETP_Bot, true);
        datas.Add(Walk);
	
        ui_mgr->OpenTutorialGroupCall(datas);
        ```
        https://github.com/yolong1020/EldenRing/blob/ede3beb90bdd83522042c45519b1ba995fa21a15/Source/EldenRing/Private/System/UIManager.cpp#L147-L152
        https://github.com/yolong1020/EldenRing/blob/a240d8f8a44e008507d892e5220b1ca4b62398c8/Source/EldenRing/Private/UI/Tutorial/TutorialGroupCall.cpp#L17-L53 </br>
       
   3. ###### 전달받은 텍스트로 안내 텍스트를 설정하고 Input Action Enum으로 데이터를 탐색하여 안내에 사용할 키를 찾고 변경합니다.</br>입력해야할 키의 크기로 위젯의 Icon활성 여부를 다르게 합니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/83fea0c0-5363-4016-a3ad-8b57598e8847" width="50%" height="50%">
      https://github.com/yolong1020/EldenRing/blob/aa4a37dbb8b0598e6af7a4e16b9aac586ddf1ab8/Source/EldenRing/Private/UI/Tutorial/TutorialHUD.cpp#L19-L36 </br>


   #### Open by VolumeBox
   ###### 에디터에서 VolumeBox를 배치하여 캐릭터가 특정 영역에서 이벤트를 호출하는 방식으로 구성된 튜토리얼입니다.
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/a912a8f3-a244-4183-8550-b14171d631fe"></img>   
   </br>

   1. ###### 충돌이 감지되면 동일한 튜토리얼 팝업이 활성되어 있는지 확인 후 현재 활성되어있는 모든 튜토리얼 팝업을 종료시킵니다. 
      https://github.com/yolong1020/EldenRing/blob/4770417759e1bab38bef25cdf85db8bba8a8bd0b/Source/EldenRing/Private/UI/Tutorial/TutorialVolumeBox_CallOut.cpp#L45-L53
      https://github.com/yolong1020/EldenRing/blob/8e2c6fe9264d451dc78b1fd4b6f73f23485b14ae/Source/EldenRing/Private/System/UIManager.cpp#L183-L194 </br>
   2. ###### UI Manager에 팝업 활성을 요청하고 VolumeBox와 팝업을 비활성화시킬 타이머를 설정합니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/6444c18c-1e0c-46be-bc08-7aa6d6a71734" width="49.5%" height="49.5%"></br>
      https://github.com/yolong1020/EldenRing/blob/4770417759e1bab38bef25cdf85db8bba8a8bd0b/Source/EldenRing/Private/UI/Tutorial/TutorialVolumeBox_CallOut.cpp#L73-L80
      https://github.com/yolong1020/EldenRing/blob/4770417759e1bab38bef25cdf85db8bba8a8bd0b/Source/EldenRing/Private/UI/Tutorial/TutorialVolumeBox_CallOut.cpp#L34-L43 </br>
   3. ###### 설정한 타이머와 무관하게 튜토리얼에서 제시하는 행동을 완료했다면, Callout을 찾아 비활성시킬 수 있습니다.
      <img src="https://github.com/yolong1020/EldenRing/assets/87303898/7df246bf-2d01-4563-ba6b-c7fe9c9f53bc" width="49.5%" height="49.5%"></br>
      https://github.com/yolong1020/EldenRing/blob/8e2c6fe9264d451dc78b1fd4b6f73f23485b14ae/Source/EldenRing/Private/Characters/Players/C0000.cpp#L581-L592 </br>


   ---
   </br>

   ### Level
   [1. Open Game Level](#Open-Game-Level)   
   </br>


   #### Open Game Level
   ###### 클라이언트를 실행하여 게임 필드에 진입할때 신규 게임의 유무에 따라 Data Layer를 다르게 불러와 게임을 설정합니다. 
   <img src="https://github.com/yolong1020/EldenRing/assets/87303898/743546f9-9289-4582-8b8f-83112424e151" width="49.5%" height="49.5%"></br>

   1. ###### Menu Button에 바인딩 된 기능으로 화면을 Fade out으로 전환합니다. 이때 Fade out이 완전히 이뤄지면 GameInstance를 통해 Level을 이전합니다.
      https://github.com/yolong1020/EldenRing/blob/39634ee819d0714303cc4d5af97fa5f35337fb3d/Source/EldenRing/Private/UI/Button/MenuButton.cpp#L105-L135
      https://github.com/yolong1020/EldenRing/blob/4d117987abadea10455cbc55d59209165b2cac92/Source/EldenRing/Private/UI/Common/BlackOut.cpp#L23-L29
      https://github.com/yolong1020/EldenRing/blob/4d117987abadea10455cbc55d59209165b2cac92/Source/EldenRing/Private/UI/Common/BlackOut.cpp#L48-L64
      https://github.com/yolong1020/EldenRing/blob/022c9b492339c4f6fc691983b4580ca716efd9b6/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L25-L42 </br>
     
   2. ###### Level이 전환되면서 정리된 Fade out을 다시 활성시키고 연출이 종료되는 시점에 Loading UI를 활성하도록 합니다.</br>이후 Fade in 연출을 진행시키고 연출이 종료되면 Data Layer를 불러옵니다.
      https://github.com/yolong1020/EldenRing/blob/9c322e6ec78552f56cef627a650794c96b7b23b7/Source/EldenRing/Private/Levels/GameField.cpp#L25-L58
      https://github.com/yolong1020/EldenRing/blob/bbcdebbdb8264787bf79a3faf1892a23dbd867ed/Source/EldenRing/Private/Levels/BaseLevelScript.cpp#L24-L45
      1. ###### Landscape는 스테이지 배경을 묶어놓은 Data Layer로 신규 게임 여부와 관계없이 항상 불러옵니다.
         <img src="https://github.com/yolong1020/EldenRing/assets/87303898/b327af62-7bcc-4477-a596-86dd4cc9b93e" width="49.5%" height="49.5%"></img>
      2. ###### DefaultObject는 최초 게임을 실행했을 때, 플레이어를 제외한 모든 게임 오브젝트들이 속한 Data Layer입니다.
         <img src="https://github.com/yolong1020/EldenRing/assets/87303898/6e3e2dbf-ba72-4477-aa4b-50f12958bf88" width="49.5%" height="49.5%"></img>
      3. ###### FieldObjectData는 저장된 게임을 실행했을 때, 플레이어와 NPC, 항아리 오브젝트를 제외한 모든 것이 담긴 Data Layer입니다.</br>NPC와 항아리 오브젝트는 별도로 필드에 소환되며, 정찰지, 집결지 데이터 등은 고유 이름으로 저장되었기 때문에 FieldObjectData에서 같은 이름을 가진 데이터를 찾아 바인딩하게 됩니다.
         <img src="https://github.com/yolong1020/EldenRing/assets/87303898/7ceb9279-1d30-48ab-a11f-f12c6b784413" width="49.5%" height="49.5%"></img>
         </br>

   3. ###### Loading UI는 진행 상황을 갱신하고 완료되면 Fade out 연출과 함께 비활성화 시킵니다. 이후 게임을 시작할 Timer를 준비합니다.
      https://github.com/yolong1020/EldenRing/blob/46dcac86b62ef35ece11449933ad06c8def74194/Source/EldenRing/Private/UI/Loading/Loading.cpp#L26-L72
      https://github.com/yolong1020/EldenRing/blob/adec67bd5096a1d3cb514c26fcbd2cacbd72ac66/Source/EldenRing/Private/Levels/GameField.cpp#L17-L21 </br>

   4. ###### 타이머 설정 시간 이후 플레이어를 필드에 소환하고 만약 신규 게임이 아니라면, 저장된 오브젝트들을 소환하도록 합니다.</br>이후 Level Load Complete Event를 호출하여 각 오브젝트들이 필요한 설정들을 진행할 수 있도록 합니다.
      https://github.com/yolong1020/EldenRing/blob/adec67bd5096a1d3cb514c26fcbd2cacbd72ac66/Source/EldenRing/Private/Levels/GameField.cpp#L58-L101
      https://github.com/yolong1020/EldenRing/blob/022c9b492339c4f6fc691983b4580ca716efd9b6/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L19-L23
      ```C++
      ///  [AC0000::BeginPlay] 210
      const bool load_complete = instance->IsLevelLoadComplete();
      load_complete ? GameStartAction() : instance->AddEventLevelLoadComplete(this, &AC0000::GameStartAction);
      ```   
   </br>
   
   ---   
   </br>

   ### Save & Load
   [1. Save Player](#Save-Player)   
   [2. Save Objects](#Save-Objects)   
   [3. Load Player](#Load-Player)   
   [4. Load Objects](#Load-Objects)   
   </br>


   #### Save Player
   ###### 게임을 종료하거나 체크 포인트 활성 및 아이템 거래, 사망 상황에서 저장이 발생합니다.

   1.
      1. ###### 게임 종료에 의한 저장 :</br>게임을 종료할 때, 팝업에 바인딩 된 기능을 이용하게 게임을 저장합니다.
         <img src="https://github.com/yolong1020/EldenRing/assets/87303898/508aa5db-3251-4e37-8720-ef8ab9de7901" width="49.5%" height="49.5%"></img>   
         </br>
         https://github.com/yolong1020/EldenRing/blob/64d9445ed597ada15df737bc3a3a037e8492297e/Source/EldenRing/Private/Characters/Players/C0000.cpp#L769-L776
         https://github.com/yolong1020/EldenRing/blob/64d9445ed597ada15df737bc3a3a037e8492297e/Source/EldenRing/Private/System/UIManager.cpp#L78-L116 </br>
         
      2. ###### 체크 포인트에 의한 저장 :</br>화톳불 상호 작용으로 체크 포인트 기능이 작동되어 게임을 저장합니다.
         <img src="https://github.com/yolong1020/EldenRing/assets/87303898/d7634f0b-4989-4d5a-b8c0-9f540a5aeb98" width="49.5%" height="49.5%"></img>   
         </br>
         https://github.com/yolong1020/EldenRing/blob/3578ea4734424524193e99ac7fe3af6e40049190/Source/EldenRing/Private/UI/PopUp/InteractionPopUp.cpp#L115-L138
         https://github.com/yolong1020/EldenRing/blob/73825fa184b635dea13c29460bc5894efc84ecbe/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1412-L1416
         https://github.com/yolong1020/EldenRing/blob/73825fa184b635dea13c29460bc5894efc84ecbe/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1424-L1431
         https://github.com/yolong1020/EldenRing/blob/73825fa184b635dea13c29460bc5894efc84ecbe/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1556-L1583 </br>

   2. ###### USaveGame을 상속받은 UERSaveData에 플레이어에 관련된 모든 데이터를 저장합니다.
      https://github.com/yolong1020/EldenRing/blob/0c22ea3db091b70ce347e421ef1c6525da478d41/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L52-L73
      https://github.com/yolong1020/EldenRing/blob/6571361fd2e65f39c1565b26b5093ed459994600/Source/EldenRing/Private/GameData/ERSaveData.cpp#L11-L21
      https://github.com/yolong1020/EldenRing/blob/6571361fd2e65f39c1565b26b5093ed459994600/Source/EldenRing/Private/GameData/ERSaveData.cpp#L23-L37 </br>


   #### Save Objects
   ###### 플레이어를 제외한 필드에 존재하는 오브젝트는 Observer Manager에 'EOET_SaveData'로 등록된 이벤트에 의해 저장되며,</br>중복 저장을 방지하기 위해 이벤트 호출 전 반드시 초기화 과정을 거칩니다.
   ```C++
   UERGameInstance* instance = Cast<UERGameInstance>(GetGameInstance()); 
   CHECK_INVALID_PTR(instance) 
   UObserverManager* observer_mgr = instance->GetSubsystem<UObserverManager>();
   CHECK_INVALID_PTR(observer_mgr)
 	 
   instance->ResetSaveData(); 
   observer_mgr->TriggerEvent(EObserverEventType::EOET_SaveData);
   ```
   https://github.com/yolong1020/EldenRing/blob/0c22ea3db091b70ce347e421ef1c6525da478d41/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L367-L383 </br>

   1. ###### NPC들은 Event에 SaveCharacterData를 바인딩하여 캐릭터의 위치, 클래스 유형, 무기 정보, 행동, 정찰 등을 기본으로 저장합니다.</br>만약 상인 유형이라면, 다이얼로그와 연출 카메라, 이동 장소 및 판매 아이템을 추가로 저장합니다.
      https://github.com/yolong1020/EldenRing/blob/cbf78c1d5f11228634889b1c6f8319e61af4c5a2/Source/EldenRing/Private/Characters/NPC/NPC_Character.cpp#L447-L455
      https://github.com/yolong1020/EldenRing/blob/22eac6ed928320a3710896509221d9f466ecedcf/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L135-L147
      https://github.com/yolong1020/EldenRing/blob/997399c5544718c3393739593c7cc748d3c1a46e/Source/EldenRing/Private/GameData/ERSaveData.cpp#L39-L93 </br>
      
   2. ###### 화톳불이나 상인처럼 상호 작용이 가능한 오브젝트는 고유 이름과 최초 상호 작용 여부를 저장합니다.
      https://github.com/yolong1020/EldenRing/blob/f3a58f08e48bff30bf54832873d9287e74cc7bf1/Source/EldenRing/Private/Characters/NPC/Vender/Vender.cpp#L245-L253
      https://github.com/yolong1020/EldenRing/blob/22eac6ed928320a3710896509221d9f466ecedcf/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L263-L275
      https://github.com/yolong1020/EldenRing/blob/1e2d77207e761592951b82b53947498560068d82/Source/EldenRing/Private/GameData/ERSaveData.cpp#L105-L113
      
   3. ###### 항아리나 정찰지, 집결지와 같은 오브젝트는 위치, 클래스 유형을 저장합니다.
      https://github.com/yolong1020/EldenRing/blob/9707e8b6c5dccdd2145024019270f3c4906f2124/Source/EldenRing/Private/FieldObject/FieldObject.cpp#L22-L28
      https://github.com/yolong1020/EldenRing/blob/22eac6ed928320a3710896509221d9f466ecedcf/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L212-L224
      https://github.com/yolong1020/EldenRing/blob/5de49fcb150304115303472db0b75118350dd4e6/Source/EldenRing/Private/GameData/ERSaveData.cpp#L95-L103
      
   1. ###### VolumeBox 같은 튜토리얼 오브젝트는 위치, 클래스 유형, Callout 팝업 정보들을 저장합니다.
      https://github.com/yolong1020/EldenRing/blob/acb23901afdb98a9a45e08a03bb9b3be3107b887/Source/EldenRing/Private/UI/Tutorial/TutorialVolumeBox_CallOut.cpp#L65-L71
      https://github.com/yolong1020/EldenRing/blob/22eac6ed928320a3710896509221d9f466ecedcf/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L314-L326
      https://github.com/yolong1020/EldenRing/blob/dad6afdf1baa07093f377a3671635032093c8114/Source/EldenRing/Private/GameData/ERSaveData.cpp#L115-L124 </br>


   #### Load Player
   ###### 메인 타이틀에서 'Load Game' 버튼에 의한 게임 시작과 플레이어가 사망 후 게임을 재시작할 때, 저장된 플레이어의 정보를 불러옵니다.
   ```C++
   /// [AC0000::BeginPlay] 189
   instance->IsNewGame() ? SetDefaultItems() : instance->LoadPlayer(this);
   ```

   1.
      1. ###### Load Game에 의한 불러오기 :</br>NewGame을 제외한 ERGameInstance의 'm_is_new_game' 변수는 항시 false 상태이며,</br>New Game도 사망에 의한 재시작은 변수를 false로 변경하기 때문에 플레이어 캐릭터 초기화 시 저장된 정보를 불러오게 됩니다. 
         </br>
         
      2. ###### 플레이어 사망에 의한 불러오기 :</br>캐릭터의 사망 연출이 종료되면 GameMode에 게임 재시작을 요청하고 캐릭터를 다시 초기화하며 저장된 정보를 불러오게 됩니다.
         <img src="https://github.com/yolong1020/EldenRing/assets/87303898/865ae562-b195-4e21-afd6-a75187b99103" width="49.5%" height="49.5%"></img>   
         </br>
         https://github.com/yolong1020/EldenRing/blob/2e31075e7eb907ff42a181d5348179df51fadf97/Source/EldenRing/Private/Characters/Players/C0000.cpp#L400-L415
         https://github.com/yolong1020/EldenRing/blob/2e31075e7eb907ff42a181d5348179df51fadf97/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1222-L1229
         https://github.com/yolong1020/EldenRing/blob/a0d7366564eba9f62fe294702066f0320fe39bcd/Source/EldenRing/Private/GameBase/ERGameMode.cpp#L71-L90 </br>
         
   2. ###### 플레이어를 마지막에 저장된 위치로 이동키고 착용, 보관된 아이템들의 UItemObject를 생성, 검증 후 각 분류에 맞게 위치시킵니다.
      https://github.com/yolong1020/EldenRing/blob/64f315c377d4aab7c93b16c3b7e04cd8962d87ec/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L75-L133 </br>
      
     
   #### Load Objects
   ###### 메인 타이틀에서 'Load Game' 버튼에 의한 게임 시작과 플레이어 사망 후 게임을 재시작할 때, 저장된 오브젝트 정보를 불러옵니다.

   1.
      1. ###### Load Game에 의한 불러오기 :</br>Load Game으로 게임을 실행한 경우 FieldObject Data Layer를 불러와 별도로 오브젝트들을 생성하고 데이터를 연결합니다.
         https://github.com/yolong1020/EldenRing/blob/099acb762a396881c10e7f615b94861e4ecfad93/Source/EldenRing/Private/Levels/BaseLevelScript.cpp#L24-L45
         https://github.com/yolong1020/EldenRing/blob/099acb762a396881c10e7f615b94861e4ecfad93/Source/EldenRing/Private/Levels/GameField.cpp#L58-L78 </br>
      2. ###### 플레이어 사망에 의한 불러오기 :</br>플레이어 캐릭터의 사망 연출이 종료되면 GameMode에 게임 재시작을 요청하고 필드의 오브젝트들을 불러옵니다.
         https://github.com/yolong1020/EldenRing/blob/099acb762a396881c10e7f615b94861e4ecfad93/Source/EldenRing/Private/GameBase/ERGameMode.cpp#L71-L90 </br>

   2. ###### 필드에 이미 각 항목별로 오브젝트가 존재하는 경우, 해당 항목에 소속된 오브젝트를 모두 파괴하고 불러온 데이터에 따라 다시 Spawn하여 초기화를 진행합니다. 
      https://github.com/yolong1020/EldenRing/blob/a0d7366564eba9f62fe294702066f0320fe39bcd/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L44-L50 </br>

      1. ###### NPC는 Spawn 후 위치, 행동, 무기와 정찰, 집결지를 초기화하며 상인 여부에 따라 거래 아이템과 다이얼로그, 연출 카메라를 추가로 초기화합니다.
         https://github.com/yolong1020/EldenRing/blob/a3077be327f39c03df390dafddecd977e72c0488/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L149-L210
      1. ###### 항아리, 정찰지와 같은 Field Object는 Spawn 후 위치 정보만 초기화합니다.
         https://github.com/yolong1020/EldenRing/blob/a3077be327f39c03df390dafddecd977e72c0488/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L226-L261
      1. ###### 화톳불이나 상인과 같은 상호작용이 가능한 오브젝트는 Spawn 후 최초 상호작용 여부를 초기화합니다.
         https://github.com/yolong1020/EldenRing/blob/a3077be327f39c03df390dafddecd977e72c0488/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L277-L312
      1. ###### 튜토리얼 오브젝트는 위치와 안내할 데이터를 초기화합니다.
         https://github.com/yolong1020/EldenRing/blob/a3077be327f39c03df390dafddecd977e72c0488/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L328-L365

</body>
</html>
