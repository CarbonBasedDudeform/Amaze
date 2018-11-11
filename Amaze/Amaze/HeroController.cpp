#include "HeroController.h"

HeroController::HeroController(HeroPawn * pawn) : Controller(), _pawn(pawn)
{
	_view = new sf::View(sf::Vector2f(_pawn->WorldX, _pawn->WorldY), sf::Vector2f(GameProperties::SCREEN_WIDTH, GameProperties::SCREEN_HEIGHT));
	_speed = 0.1f;
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

void HeroController::MoveLeft(float timeDelta) {
	_pawn->WorldX -= _speed * timeDelta;
	UpdateView();
}

void HeroController::MoveRight(float timeDelta) {
	_pawn->WorldX += _speed * timeDelta;
	UpdateView();
}

void HeroController::MoveDown(float timeDelta) {
	_pawn->WorldY += _speed * timeDelta;
	UpdateView();
}

void HeroController::MoveUp(float timeDelta) {
	_pawn->WorldY -= _speed * timeDelta;
	UpdateView();
}

void HeroController::Process(BlockedDirections blocked, float timeDelta) {
	if (!blocked.Left.Blocked && ( sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A) ) )
	{
		MoveLeft(timeDelta);
	}
	if (!blocked.Right.Blocked && (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
	{
		MoveRight(timeDelta);
	}
	if (!blocked.Up.Blocked && (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)))
	{
		MoveUp(timeDelta);
	}
	if (!blocked.Down.Blocked && (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
	{
		MoveDown(timeDelta);
	}
}