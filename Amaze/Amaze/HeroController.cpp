#include "HeroController.h"

HeroController::HeroController(HeroPawn * pawn) : Controller(), _pawn(pawn)
{
	_view = new sf::View(sf::Vector2f(_pawn->WorldX, _pawn->WorldY), sf::Vector2f(GameProperties::SCREEN_WIDTH, GameProperties::SCREEN_HEIGHT));
	_speed = 1.0f;
}


HeroController::~HeroController()
{
}

sf::View * HeroController::GetView()
{
	return _view;
}

void HeroController::UpdateView(){
	_view->setCenter(sf::Vector2f(_pawn->WorldX, _pawn->WorldY));
}

void HeroController::MoveLeft() {
	_pawn->WorldX -= _speed;
	UpdateView();
}

void HeroController::MoveRight() {
	_pawn->WorldX += _speed;
	UpdateView();
}

void HeroController::MoveDown() {
	_pawn->WorldY += _speed;
	UpdateView();
}

void HeroController::MoveUp() {
	_pawn->WorldY -= _speed;
	UpdateView();
}

void HeroController::Process(BlockedDirections blocked) {
	if (!blocked.Left && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		MoveLeft();
	}
	if (!blocked.Right && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		MoveRight();
	}
	if (!blocked.Up && sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		MoveUp();
	}
	if (!blocked.Down && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		MoveDown();
	}
}