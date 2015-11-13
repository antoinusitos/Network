#pragma once

struct _ENetPeer;
typedef struct _ENetPeer ENetPeer;

namespace engine
{
	namespace network
	{
		class Client
		{
		public:
			Client(ENetPeer *peer);

			ENetPeer *getPeer() const;

			void setID(int IdPlayer);

			int getID();

		private:
			ENetPeer *_peer;

			int ID;
		};
	}
}
