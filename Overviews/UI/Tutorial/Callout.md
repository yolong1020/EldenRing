# Callout
[1. Init tutorial](#Init-tutorial)   
[2. Open by VolumeBox](#Open-by-VolumeBox)   
</br>


## Init tutorial
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


## Open by VolumeBox
###### 에디터에서 VolumeBox를 배치하여 캐릭터가 특정 영역에서 이벤트를 호출하는 방식으로 구성된 튜토리얼입니다.
<img src="https://github.com/yolong1020/EldenRing/assets/87303898/a912a8f3-a244-4183-8550-b14171d631fe"></img>   
</br>

  1. ###### 충돌이 감지되면 동일한 튜토리얼 팝업이 활성되어 있는지 확인 후 현재 활성되어있는 모든 튜토리얼 팝업을 종료시킵니다. 
     https://github.com/yolong1020/EldenRing/blob/4770417759e1bab38bef25cdf85db8bba8a8bd0b/Source/EldenRing/Private/UI/Tutorial/TutorialVolumeBox_CallOut.cpp#L45-L53 </br>
  2. ###### UI Manager에 팝업 활성을 요청하고 VolumeBox와 팝업을 비활성화시킬 타이머를 에디터에서 설정한 Duration만큼 설정합니다.
     https://github.com/yolong1020/EldenRing/blob/4770417759e1bab38bef25cdf85db8bba8a8bd0b/Source/EldenRing/Private/UI/Tutorial/TutorialVolumeBox_CallOut.cpp#L73-L80
     https://github.com/yolong1020/EldenRing/blob/4770417759e1bab38bef25cdf85db8bba8a8bd0b/Source/EldenRing/Private/UI/Tutorial/TutorialVolumeBox_CallOut.cpp#L34-L43 </br>
  4. ###### Canvas Slot에 포함된 아이템들은 'UItemObject'의 정보를 토대로 텍스쳐 이미지와 크기를 갱신합니다.
     https://github.com/yolong1020/EldenRing/blob/2c3e7d450dd827127f4ce4ad46fe18e74c02adc0/Source/EldenRing/Private/UI/Trade/TradeItem.cpp#L247-L259 </br>
     https://github.com/yolong1020/EldenRing/blob/bc678c39609bf7e91b604d6f22b202db1336da32/Source/EldenRing/Private/UI/Trade/TradeItem.cpp#L272-L311
