#include "engine/gameplay/PlayerEntity.h"

#include <cassert>
#include <stdlib.h> 
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "engine/Engine.h"
#include "engine/graphics/Manager.h"
#include "engine/network/Manager.h"
#include "engine/network/Client.h"
#include <enet/enet.h>
#include <iostream>

namespace engine
{
	namespace gameplay
	{
		PlayerEntity::PlayerEntity()
			: _x(0.f)
			, _y(0.f)
		{
			assert(graphics::Manager::instance);
			_shape = graphics::Manager::instance->createShape(graphics::Manager::ShapeType::CIRCLE);
			
			reinterpret_cast<sf::CircleShape*>(_shape)->setRadius(10.f);
			_shape->setFillColor(sf::Color(150, 50, 250));
			_shape->setOutlineThickness(2.f);
			_shape->setOutlineColor(sf::Color(250, 150, 100));

			ID = -1;
		}

		PlayerEntity::~PlayerEntity()
		{
			assert(graphics::Manager::instance);
			graphics::Manager::instance->destroyShape(_shape);
		}

		void PlayerEntity::update()
		{
			if (original && graphics::Manager::instance->hasFocus())
			{
				int dx = 0;
				int dy = 0;

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
					dx -= 1;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
					dx += 1;

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
					dy -= 1;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
					dy += 1;

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
					Fire();

				assert(Engine::instance);
				float deltaTime = Engine::instance->getDeltaTime();
				float deltaMove = deltaTime * 200; // px/s
				_x += dx * deltaMove;
				_y += dy * deltaMove;

				_shape->setPosition(_x, _y);

				if (ID != -1)
				{
					network::Manager::instance->SendPosition(_x, _y, ID);
				}
			}
		}

		void PlayerEntity::updatePosition(float X, float Y, int theID)
		{
			if (theID == ID)
			{
				_x = X;
				_y = Y;
				_shape->setPosition(_x, _y);
			}
		}

		void PlayerEntity::Fire()
		{
			float randX = static_cast <float> ((rand() / static_cast <float> (RAND_MAX)) * 2.0f) - 1.0f;
			float randY = static_cast <float> ((rand() / static_cast <float> (RAND_MAX)) * 2.0f) - 1.0f;
			network::Manager::instance->CreateBullet(_x, _y, randX, randY, ID);
		}

		void PlayerEntity::setOriginal(bool newBool)
		{
			original = newBool;
		}

		void PlayerEntity::setID(int newID)
		{
			ID = newID;
		}

		int PlayerEntity::getID()
		{
			return ID;
		}

	}
}
