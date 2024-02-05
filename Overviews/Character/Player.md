# Player
### * Init Player Settings
  > Level 상태에 따라서 HUD, Camera Timeline, 플레이에 필요한 Item 등 초기화합니다.    

  ###### [Step 01] 현재 Level의 플레이 가능 여부와 Load 상태에 따른 초기화를 진행합니다.
  https://github.com/yolong1020/EldenRing/blob/a021831dab5f79b7e6faff2f2ab6fc9c5b5b9216/Source/EldenRing/Private/Characters/Players/C0000.cpp#L136-L212
  </br>
  ###### [Step 02] 'New Game' 혹은 'Load Game'에 따라 Item을 다르게 초기화합니다.
  ```C++
  /// [BeginPlay] 189
  instance->IsNewGame() ? SetDefaultItems() : instance->LoadPlayer(this);
  ```
  https://github.com/yolong1020/EldenRing/blob/bc151a656f7846c367529f4340f0c87f574dcde0/Source/EldenRing/Private/Characters/Players/C0000.cpp#L78-L111
  https://github.com/yolong1020/EldenRing/blob/0f9888e552ddbf77796e6e03775caa458ebac7fe/Source/EldenRing/Private/GameBase/ERGameInstance.cpp#L75-L133
  </br>
  ###### [Step 03] Level의 Load 상태에 따른 캐릭터 시작 애니메이션 재생 및 Input을 활성합니다.
  https://github.com/yolong1020/EldenRing/blob/bc151a656f7846c367529f4340f0c87f574dcde0/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1595-L1622
  </br>
  
---
### * Combat
  > 조작에 따라 단일, 이전의 공격을 바탕으로 한 콤보 공격으로 구별됩니다. 그 외 방패를 통한 방어와 반격 기능을 제공합니다.

  ###### [01] 전투 대상의 Execution Collision(Front / Back)에 Overlap 되면 플레이어는 처형 공격을 실행할 수 있습니다.
  https://github.com/yolong1020/EldenRing/blob/4e07652fec8037129c94ac1eb022e142e030ac5f/Source/EldenRing/Private/Characters/Players/C0000.cpp#L851-L880
  </br>
  ###### [02] 전력 질주와 점프, 일반 상태 그리고 강공격 Input 입력 여부에 따라 서로 다른 공격이 전개됩니다.
  https://github.com/yolong1020/EldenRing/blob/4e07652fec8037129c94ac1eb022e142e030ac5f/Source/EldenRing/Private/Characters/Players/C0000.cpp#L883-L942
  </br>
  ###### [03] Weapon_Actor의 Collision과 충돌한 대상이 캐릭터인지 방어자의 Parry Collision, 방패의 Collision인지에 따라 피격 결과를 적용시킵니다.
  https://github.com/yolong1020/EldenRing/blob/4070f5c523d417e907e46127052dd90aae241624/Source/EldenRing/Private/Items/Weapons/MeleeAttack_Actor.cpp#L72-L163
  </br>
  ###### [04] 추가 공격 유효 프레임을 벗어나면 Montage - EndAttack이 호출되어 콤보, 공격 가능 상태, 캐릭터 행동 상태 등을 기본값으로 복구시킵니다.
  ![Charactor](https://github.com/yolong1020/EldenRing/assets/87303898/8a4bed56-27cc-47b6-b137-190c7f87a0f3)
  https://github.com/yolong1020/EldenRing/blob/2f43079d34e3767f013851c836d4480db5fd268a/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1368-L1377
  </br>
  
---
### * Movement
  > 조작과 캐릭터의 현재 행동에 따라 전력질주, 뛰기, 걷기가 활성되며, 특정 조건에 해당하지 않는다면 회피와 점프 기능을 이용할 수 있습니다.

  ###### [01] LockOn 상태와 전력질주 Input에 따라 전력질주, 뛰기, 걷기로 전/후, 좌/우 이동을 결정합니다.
  https://github.com/yolong1020/EldenRing/blob/6a40836945163b30e9dd8aaf3c548b9b92d56edd/Source/EldenRing/Private/Characters/Players/C0000.cpp#L661-L745
  </br>
  ###### [02] 특정 조건에 해당하지 않는 경우, 입력된 방향 혹은 캐릭터 후방으로 회피를 시도합니다. LockOn 상태에서는 목표 바라보며 회피를 시도합니다.
  https://github.com/yolong1020/EldenRing/blob/6a40836945163b30e9dd8aaf3c548b9b92d56edd/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1011-L1042
  </br>
  ###### [03] 특정 조건에 해당하지 않는 경우, 입력된 방향으로 도약을 시도합니다. Landed 호출을 통해 착지를 시도하며, 도약 중 피격을 받게되면 그 즉시 추락합니다.
  https://github.com/yolong1020/EldenRing/blob/6a40836945163b30e9dd8aaf3c548b9b92d56edd/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1115-L1148
  https://github.com/yolong1020/EldenRing/blob/2f43079d34e3767f013851c836d4480db5fd268a/Source/EldenRing/Private/Characters/Players/C0000.cpp#L235-L274
  https://github.com/yolong1020/EldenRing/blob/05121f2477a82426bed1d6281e809fd4ec7ab43b/Source/EldenRing/Private/Characters/Players/C0000.cpp#L470-L478
  </br>
    
---
### * Potion Comsume
  > 플레이어의 입력으 현재 HP를 회복하는 기능입니다.</br>
    회복 횟수에는 제한이 있으며, 모든 포션을 소모했을 경우 해당하는 애니메이션이 재생되고 HP는 회복되지 않습니다.</br>
    또한, 포션 복용 중 피격을 받게 되면 소모된 포션은 복구되지 않고 HP 역시 회복되지 않습니다.

  ###### [Step 01] 캐릭터의 하반신은 Input에 따라 움직일 수 있도록 척추를 기준으로 Blend를 실행하고 잔여 포션 수에 따라 애니메이션을 결정합니다.
  https://github.com/yolong1020/EldenRing/blob/1a8f4ec6559928de5514a7bfc880b2c52b592252/Source/EldenRing/Private/Characters/Players/C0000.cpp#L1093-L1115
  </br>
  ###### [Step 02] 애니메이션 재생 후 성공적으로 Montage - OnSuccessConsume이 호출되면 Attribute와 HUD의 HP 정보를 갱신합니다.
  ![Charactor](https://github.com/yolong1020/EldenRing/assets/87303898/c808a976-430a-4f0d-9f2e-a1395a89f019)
  https://github.com/yolong1020/EldenRing/blob/1a8f4ec6559928de5514a7bfc880b2c52b592252/Source/EldenRing/Private/Characters/Players/C0000.cpp#L581-L592
  </br>

---
### * Item Storage
  > Item과 재화를 보관하며 필요에 따라 착용하거나 버릴 수 있습니다.

  ###### [Step 01] Inventory의 각 Widget들을 초기화합니다. (Slot count, Delegate, etc)
  https://github.com/yolong1020/EldenRing/blob/d8a4482dee125ba06ce66e03dfea90628b27c71f/Source/EldenRing/Private/System/StorageManager.cpp#L42-L85
  https://github.com/yolong1020/EldenRing/blob/d8a4482dee125ba06ce66e03dfea90628b27c71f/Source/EldenRing/Private/UI/Inventory/InventoryGrid.cpp#L47-L66
  </br>
  
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/f37436e7-ffbe-409c-9bcd-98da953456e8" width="60%" height="60%">
  ###### [Step 01 - 01] InventorySlotGrid는 화면에 출력될 보관함 칸 수를 담당하는 Widget입니다.
  https://github.com/yolong1020/EldenRing/blob/66be893c43cf52f5783ec93ff1e88fb007b92325/Source/EldenRing/Private/UI/Inventory/InventorySlotGrid.cpp#L20-L48
  
  ###### [Step 01 - 02] InventoryInteractionGrid는 실제 Item과 보관함의 상호작용을 담당하는 Widget입니다.
  https://github.com/yolong1020/EldenRing/blob/d8a4482dee125ba06ce66e03dfea90628b27c71f/Source/EldenRing/Private/UI/Inventory/InventoryInteractionGrid.cpp#L24-L39
  </br>

  ###### [Step 02] Inventory를 활성화하며 동시에 'Esc' 입력시 최상위 UI 판별을 위해 UI Manager에 등록해줍니다.
  https://github.com/yolong1020/EldenRing/blob/808bb8d461a42c4bc8d677944969d10fdc64c29e/Source/EldenRing/Private/System/StorageManager.cpp#L23-L35
  </br>

  ###### [Step 02] Inventory를 활성화하며 동시에 'Esc' 입력시 최상위 UI 판별을 위해 UI Manager에 등록해줍니다.
  </br>
    
---
### * Item Trade
  > HUD, Camera Timeline, Item 등 초기화.
  ###### Step 01. 현 Level의 플레이 가능 여부와 Load 상태에 따른 초기화 진행
