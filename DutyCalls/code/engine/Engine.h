#pragma once

#include <list>
#include "engine/graphics/EventListener.h"
#include "engine/network/EventListener.h"
#include "engine/gameplay/PlayerEntity.h"
#include "engine/gameplay/BulletEntity.h"


namespace engine
{

	namespace graphics
	{
		class Manager;
	}

	namespace network
	{
		class Manager;
	}

	class Engine : public graphics::EventListener, public network::EventListener
	{
	public:
		enum class InstanceType
		{
			CLIENT,
			SERVER,
		};

		bool initialize();
		void deinitialize();

		bool createClient(const std::string &host, sf::Uint16 port);
		bool createServer(sf::Uint16 port);

		void run();
		float getDeltaTime() const;

		// graphics::EventListener
		virtual void onWindowClosed();

		// network::EventListener
		virtual void onConnected(network::Client &client);
		virtual void onDisconnected(network::Client &client);
		virtual void onMessageReceived(network::Client &client, sf::Packet &packet);

		static Engine *instance;

		gameplay::PlayerEntity * GetEntityLocal();

		InstanceType getInstanceType();

		std::list<gameplay::PlayerEntity*> getEntities();

		void AddBullet(gameplay::BulletEntity* theBullet);

	private:
		InstanceType _instanceType;
		bool _running;
		float _deltaTime;
		gameplay::PlayerEntity *entityLocal;
		std::list<gameplay::PlayerEntity*> _entities;
		std::list<gameplay::BulletEntity*> _bullets;
	};
}
