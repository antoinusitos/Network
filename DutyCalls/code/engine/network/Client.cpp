#include "engine/network/Client.h"

#include <enet/enet.h>

namespace engine
{
	namespace network
	{
		Client::Client(ENetPeer *peer)
			: _peer(peer)
		{
			// empty
			ID = -1;
			
		}

		ENetPeer *Client::getPeer() const
		{
			return _peer;
		}

		void Client::setID(int IdPlayer)
		{
			ID = IdPlayer;
		}

		int Client::getID()
		{
			return ID;
		}
	}
}
