#include "Bolt.h"
#include "Particles/ParticleSystemComponent.h"

ABolt::ABolt()
{
	Damage = BOLTDAMAGE;

	BoltEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BoltEffect"));
	BoltEffect->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FindBoltEffect(TEXT("ParticleSystem'/Game/VFX/P_BossBolt.P_BossBolt'"));
	if (FindBoltEffect.Succeeded())
	{
		BoltEffect->SetTemplate(FindBoltEffect.Object);
		BoltEffect->SetVisibility(true);
	}
}