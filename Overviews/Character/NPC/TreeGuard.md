# Tree Guard
[1. Behavior Tree](#Behavior-Tree)   
[2. Detect Bound](#Detect-Bound)   
[3. Init Tree Guard Settings](#Init-Tree-Guard-Settings)   
[4. Combat](#Combat)   
</br>


## Behavior Tree
###### 전
<img src="https://github.com/yolong1020/EldenRing/assets/87303898/95c304ff-a756-47a0-8e9c-7d9f399f5720"></img>   
</br>


## Detect Bound
###### 캐릭터를 중심으로 3개의 영역을 통해 추격과 공격, 기동으로 연계하는 Service입니다. 
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/b020d7bd-eff3-433c-8039-ef7906b0c69b" width="49.5%" height="49.5%"></img>   
  https://github.com/yolong1020/EldenRing/blob/2fb19a7c590b8cb05dd116ac91068cc81dbf7f1a/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/Services/BTService_Detect.cpp#L17-L33
  
  - ###### Red   : 범위 내에 적이 위치한다면 돌진 공격이 가능하며 범위를 벗어나는 순간 추격이 시작됩니다.
  - ###### Blue  : 범위 내에 적이 위치한다면 단일 공격을 실행할 수 있습니다.
  - ###### Black : 범위 내에 적이 위치한다면 단일 공격과 콤보 공격을 실행할 수 있습니다.
</br>

## Init Tree Guard Settings
###### Tree Guard의 블랙보드와 몸통 Collision, Action 데이터 등을 초기화합니다.
  <img src="https://github.com/yolong1020/EldenRing/assets/87303898/1e557d19-af9d-4e0b-be3f-c78c477171be" width="49.5%" height="49.5%"></img>
  https://github.com/yolong1020/EldenRing/blob/d5f795bbce272476ba0d7c9afb9bfc4c067c4ecb/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/C3251.cpp#L29-L45
  https://github.com/yolong1020/EldenRing/blob/d5f795bbce272476ba0d7c9afb9bfc4c067c4ecb/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/C3251.cpp#L58-L145 </br>


## Combat
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
     3. ###### 단일 공격은 자신으로부터 적의 위치에 따라 공격 방법을 다르게 전개하며, 적이 후방에 있다면 공격을 취소하고 차기 Action을 Task에서 선택하도록 합니다.</br>만약 적의 위치가 차기 Action의 Action Bound에 만족하지 않는다면 이 역시 공격을 취소하도록 합니다.</br>공격 강도는 선택된 Action ID에 명시된 Attack Weight로 설정합니다.
        <img src="https://github.com/yolong1020/EldenRing/assets/87303898/5a275326-923b-4b02-941e-c5558afaee12" width="49.5%" height="49.5%"></img>
        https://github.com/yolong1020/EldenRing/blob/81b539b61a82d83bdfb7b2efb76a33a464785c8d/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/C3251.cpp#L391-L458
     5. ###### 콤보 공격은 이전 Action ID를 통해 파생되는 공격 중 무작위로 선정하여 Action의 공격 방향이 적과 일치하면 공격을 실행합니다.</br>만약 방향이 일치하지 않다면 공격을 취소합니다.</br>공격 강도는 선택된 Action ID에 명시된 Attack Weight로 설정합니다.
        <img src="https://github.com/yolong1020/EldenRing/assets/87303898/467894cc-b5dc-4a5c-8a54-c1c43af0819d" width="49.5%" height="49.5%"></img>
        https://github.com/yolong1020/EldenRing/blob/b1b47bfa08a3ea2424d944f71bf9af1667e910d3/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/C3251.cpp#L460-L499 </br>

  3. ###### 공격 외 Moving Action은 적으로부터 거리를 두거나 방향을 일치시키는데 사용하는 Action입니다.
     <img src="https://github.com/yolong1020/EldenRing/assets/87303898/0d82740e-d172-4d1f-9e2d-6f90123cf82b" width="49.5%" height="49.5%"></img>
     <img src="https://github.com/yolong1020/EldenRing/assets/87303898/854285d1-3257-4671-8374-97cb2278bc77" width="49.5%" height="49.5%"></img>
     https://github.com/yolong1020/EldenRing/blob/3463b8fcaaff2853e22dee95b2f6790c7926ee41/Source/EldenRing/Private/Characters/NPC/Enemies/C3251/C3251.cpp#L501-L512 </br>
