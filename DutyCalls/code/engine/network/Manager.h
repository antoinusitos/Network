#pragma once

#include <string>
#include <SFML/Config.hpp>
#include <enet/enet.h>
#include <list>

struct _ENetHost;
struct _ENetPeer;
typedef struct _ENetHost ENetHost;
typedef struct _ENetPeer ENetPeer;

namespace sf
{
	class Packet;
}

namespace engine
{
	namespace network
	{
		class Client;
		class EventListener;

		class Manager
		{
		public:
			Manager(EventListener *eventListener);

			bool initialize();
			void deinitialize();

			bool createClient(const std::string &host, sf::Uint16 port);
			bool createServer(sf::Uint16 port);

			void broadcast(const sf::Packet &packet);
			void send(const Client &client, const sf::Packet &packet);

			void update();

			ENetHost* GetHost();

			static Manager *instance;

			Client* GetClient();

			void SendPosition(float _x, float _y, int ID);

			void CreatePlayerOnClients(int id);

			void CreateBullet(float X, float Y, float dirX, float dirY, int theID);

			void CreateBulletsOnClients(float X, float Y, float dirX, float dirY, int theID);

		private:
			EventListener *_eventListener;
			ENetHost *_host;
			ENetPeer *_server;
			int _nbPlayer;
			Client *clientLocal;

		};
	}
}
