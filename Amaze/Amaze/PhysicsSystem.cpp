#include "PhysicsSystem.h"


PhysicsSystem * PhysicsSystem::_instance = nullptr;
PhysicsSystem * PhysicsSystem::GetInstance() {
	if (_instance == nullptr) _instance = new PhysicsSystem();

	return _instance;
}

PhysicsSystem::PhysicsSystem()
{
}


PhysicsSystem::~PhysicsSystem()
{
}

bool PhysicsSystem::AreColliding(Pawn * one, Pawn * two)
{
	//take the X and Y of one, 
	return false;
}
