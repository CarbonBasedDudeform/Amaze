#include "Pawn.h"


Pawn::Pawn() : WorldX(0.0f), WorldY(0.0f), Size(0.0f), IsBlocking(false), TakesDamage(false), DoesDamage(false), DamageAmount(0), parent(nullptr)
{
}


Pawn::~Pawn()
{
}

void Pawn::DoDamage(float amount)
{
	if (TakesDamage) {
		Health -= amount;
	}
}
