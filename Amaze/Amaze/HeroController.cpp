#include "HeroController.h"

HeroController::HeroController(HeroPawn * pawn, PhysicsSystem * physics) : Controller(),
	_pawn(pawn),
	_direction(sf::Vector2f(0,-1)),
	_physics(physics)
{
	_view = std::make_unique<sf::View>(sf::Vector2f(_pawn->WorldX, _pawn->WorldY), sf::Vector2f(GameProperties::SCREEN_WIDTH, GameProperties::SCREEN_HEIGHT));
	_speed = 0.1f;
}


HeroController::~HeroController()
{
}

sf::View * HeroController::GetView()
{
	return _view.get();
}

void HeroController::UpdateView(){
	_view->setCenter(sf::Vector2f(_pawn->WorldX, _pawn->WorldY));
}

void HeroController::MoveLeft(float timeDelta) {
	_pawn->WorldX -= _speed * timeDelta;
	_rotation = -90;
	_pawn->SetRotation(_rotation);
	_direction.x = -1;
	_direction.y = 0;
	UpdateView();
}

void HeroController::MoveRight(float timeDelta) {
	_pawn->WorldX += _speed * timeDelta;
	_rotation = 90;
	_pawn->SetRotation(_rotation);
	_direction.x = 1;
	_direction.y = 0;
	UpdateView();
}

void HeroController::MoveDown(float timeDelta) {
	_pawn->WorldY += _speed * timeDelta;
	_rotation = 180;
	_pawn->SetRotation(_rotation);
	_direction.y = 1;
	_direction.x = 0;
	UpdateView();
}

void HeroController::MoveUp(float timeDelta) {
	_pawn->WorldY -= _speed * timeDelta;
	_rotation = 0;
	_pawn->SetRotation(_rotation);
	_direction.y = -1;
	_direction.x = 0;
	UpdateView();
}

void HeroController::Process(BlockedDirections blocked, float timeDelta) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		auto laser = std::make_unique<Laser>(_pawn->GetPosition(), _direction,_rotation, "Textures/laser.png");
		laser->parent = _pawn;
		_physics->AddCollidable(laser.get());
		Lasers.push_back(std::move(laser));
	}

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