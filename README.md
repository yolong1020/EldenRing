EldenRing Project
===============================
포토폴리오 목적으로 엘든링의 내용들을 재해석하여 제작한 데모 프로젝트입니다. 

* Unreal Engine 5.3.2
* Microsoft Visual Studio Community 2022
* 2023.11.01 ~ 2024.01.27 (3개월)

Video
----------
[![Video Label](http://img.youtube.com/vi/ZO0a9uATi-o/0.jpg)](https://youtu.be/ZO0a9uATi-o)

Technical Overview
------------------

### 1. Character
![Charactor](https://github.com/yolong1020/EldenRing/assets/87303898/63739cc3-0bff-4242-9309-f3c2f8cea049)
#### 1. GameCharacter
* Init Default Component & Values
* Death Process
  '''C++
  void AGameCharacter::OnDeath(TWeakObjectPtr<UAnimMontage> Montage, const float& Duration)
{
	m_mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_sphere_execution_front->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_sphere_execution_back->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	float fadeout_duration = Duration;
	if (fadeout_duration == 0)
	{
		TArray<FCompositeSection> sections = Montage->CompositeSections;
		for (auto section : sections)
		{
			if (section.SectionName == m_section_name)
			{
				const UAnimSequenceBase* sequence = section.GetLinkedSequence();
				fadeout_duration = sequence->GetPlayLength() - 0.2f;
				break;
			}
		}
	}

	FadeOutCharacter(fadeout_duration);
}
  '''
