#include "engine/gameplay/BulletEntity.h"

#include <cassert>
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
#include <stdlib.h> 

namespace engine
{
	namespace gameplay
	{
		BulletEntity::BulletEntity()
			: _x(0.f)
			, _y(0.f)
		{
			assert(graphics::Manager::instance);
			_shape = graphics::Manager::instance->createShape(graphics::Manager::ShapeType::CIRCLE);

			reinterpret_cast<sf::CircleShape*>(_shape)->setRadius(2.f);
			_shape->setFillColor(sf::Color(150, 50, 250));
			_shape->setOutlineThickness(2.f);
			_shape->setOutlineColor(sf::Color(250, 150, 100));

			ID = rand() * 99999;
		}

		BulletEntity::~BulletEntity()
		{
			assert(graphics::Manager::instance);
			graphics::Manager::instance->destroyShape(_shape);
		}

		void BulletEntity::update()
		{
			assert(Engine::instance);
			float deltaTime = Engine::instance->getDeltaTime();
			float deltaMove = deltaTime * 600; // px/s
			_x += dx * deltaMove;
			_y += dy * deltaMove;

			_shape->setPosition(_x, _y);

			if (ID != -1)
			{
				network::Manager::instance->SendPosition(_x, _y, ID);
			}
		}

		void BulletEntity::updatePosition(float X, float Y, int theID)
		{
			if (theID == ID)
			{
				_x = X;
				_y = Y;
				_shape->setPosition(_x, _y);
			}
		}

		void BulletEntity::setVelocity(float dirX, float dirY)
		{
			dx = dirX;
			dy = dirY;
		}

		void BulletEntity::setPosition(float theX, float theY)
		{
			_x = theX;
			_y = theY;
		}

		void BulletEntity::setID(int newID)
		{
			ID = newID;
		}

		int BulletEntity::getID()
		{
			return ID;
		}

	}
}
