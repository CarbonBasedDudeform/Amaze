#include "Pawn.h"


Pawn::Pawn() : WorldX(0.0f), WorldY(0.0f), Size(0.0f)
{
}


Pawn::~Pawn()
{
	delete _texture;
}