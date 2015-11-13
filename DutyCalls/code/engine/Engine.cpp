#include "engine/Engine.h"

#include <cassert>
#include <iostream>
#include <SFML/System.hpp>

#include "engine/graphics/Manager.h"
#include "engine/network/Manager.h"
#include "engine/network/Client.h"
#include "engine/gameplay/PlayerEntity.h"

namespace engine
{
	Engine *Engine::instance = nullptr;

	bool Engine::initialize()
	{
		network::Manager::instance = new network::Manager(this);
		if (!network::Manager::instance->initialize())
			return false;

		graphics::Manager::instance = new graphics::Manager(this);
		if (!graphics::Manager::instance->initialize())
			return false;

		return true;
	}

	void Engine::deinitialize()
	{
		for (gameplay::PlayerEntity *entity : _entities)
			delete entity;

		_entities.clear();

		//assert(_entities.size() == 0);

		assert(graphics::Manager::instance);
		assert(network::Manager::instance);
		graphics::Manager::instance->deinitialize();
		network::Manager::instance->deinitialize();
		delete graphics::Manager::instance;
		delete network::Manager::instance;
	}

	bool Engine::createClient(const std::string &host, sf::Uint16 port)
	{
		entityLocal = new gameplay::PlayerEntity();
		entityLocal->setOriginal(true);
		_entities.push_back(entityLocal);
		_instanceType = InstanceType::CLIENT;
		assert(graphics::Manager::instance);
		assert(network::Manager::instance);
		graphics::Manager::instance->setWindowTitle("Client");
		return network::Manager::instance->createClient(host, port);
	}

	bool Engine::createServer(sf::Uint16 port)
	{
		_instanceType = InstanceType::SERVER;
		assert(graphics::Manager::instance);
		assert(network::Manager::instance);
		graphics::Manager::instance->setWindowTitle("Server");
		return network::Manager::instance->createServer(port);
	}

	void Engine::run()
	{
		_running = true;

		sf::Clock clock;
		while (_running)
		{

			_deltaTime = clock.restart().asSeconds();

			network::Manager::instance->update();

			for (gameplay::PlayerEntity *entity : _entities)
				entity->update();

			for (gameplay::BulletEntity *bullet : _bullets)
			{
				bullet->update();
			}

			graphics::Manager::instance->update();
		}
	}

	float Engine::getDeltaTime() const
	{
		return _deltaTime;
	}

	void Engine::onWindowClosed()
	{
		_running = false;
	}

	void Engine::onConnected(network::Client &client)
	{
		if (_instanceType == InstanceType::CLIENT)
		{

			int theID = client.getID();

			/*std::cout
				<< "message received ID: "
				<< std::to_string(theID)
				<< std::endl;*/
		}
		else
		{
			/*std::cout
				<< "cree un nouveau client ! "
				<< std::endl;*/
			gameplay::PlayerEntity *entity = new gameplay::PlayerEntity();
			entity->setOriginal(false);
			entity->setID(network::Manager::instance->GetClient()->getID());
			_entities.push_back(entity);
		}
	}

	void Engine::onDisconnected(network::Client &client)
	{

	}

	void Engine::onMessageReceived(network::Client &client, sf::Packet &packet)
	{

		if (_instanceType == InstanceType::SERVER)
		{
			std::string s;
			int id;
			float x, y, dx, dy;
			packet >> s >> id >> x >> y >> dx >> dy;

			if (s == "move")
			{
				for (std::list<gameplay::PlayerEntity*>::iterator it = _entities.begin(); it != _entities.end(); ++it)
				{
					reinterpret_cast<gameplay::PlayerEntity*>(*it)->updatePosition(x, y, id);
				}

				sf::Packet movePacket;
				movePacket << "movePlayer" << id << x << y;
				network::Manager::instance->broadcast(movePacket);
			}
			else if (s == "CreateBullet")
			{
				gameplay::BulletEntity *bullet = new gameplay::BulletEntity();
				bullet->setPosition(x, y);
				bullet->setVelocity(dx, dy);
				_bullets.push_back(bullet);
				network::Manager::instance->CreateBulletsOnClients(x, y, dx, dy, id);
			}
		}
		else
		{
			std::string s;
			int id;
			float x, y, dx, dy;
			packet >> s >> id >> x >> y >> dx >> dy;

			if (s == "id")
			{
				network::Manager::instance->GetClient()->setID(id);
				entityLocal->setID(id);
			}
			else if (s == "createPlayer")
			{
				if (id != entityLocal->getID())
				{
					gameplay::PlayerEntity *entity = new gameplay::PlayerEntity();
					entity->setOriginal(false);
					entity->setID(id);
					_entities.push_back(entity);
				}
			}
			else if (s == "movePlayer")
			{
				if (id != entityLocal->getID())
				{
					for (std::list<gameplay::PlayerEntity*>::iterator it = _entities.begin(); it != _entities.end(); ++it)
					{
						reinterpret_cast<gameplay::PlayerEntity*>(*it)->updatePosition(x, y, id);
					}
				}
			}
			else if (s == "CreateBullet")
			{
				gameplay::BulletEntity *bullet = new gameplay::BulletEntity();
				bullet->setPosition(x, y);
				bullet->setVelocity(dx, dy);
				_bullets.push_back(bullet);
			}
		}
	}

	gameplay::PlayerEntity * Engine::GetEntityLocal()
	{
		return entityLocal;
	}

	Engine::InstanceType Engine::getInstanceType()
	{
		return _instanceType;
	}

	std::list<gameplay::PlayerEntity*> Engine::getEntities()
	{
		return _entities;
	}

	void Engine::AddBullet(gameplay::BulletEntity* theBullet)
	{
		_bullets.push_back(theBullet);
	}
}
