#include <cassert>
#include <iostream>
#include <string>

#include <enet/enet.h>
#include <windows.h>

int main()
{
	if (enet_initialize() != 0)
	{
		std::cerr << "An error occurred while initializing ENet." << std::endl;
		return EXIT_FAILURE;
	}
	
	ENetHost *client;
	client = enet_host_create(
		NULL, // client host
		1, // one outgoing connection
		1, // one channel
		0, // automatic downstream bandwidth
		0 // automatic upstream bandwidth
	);
	if (client == nullptr)
	{
		std::cerr << "An error occurred while trying to create an ENet client host." << std::endl;
		return EXIT_FAILURE;
	}

	const char* serverHost = "10.51.0.24";
	//const char* serverHost = "127.0.0.1";
	const int serverPort = 3000;
	
	ENetAddress address;
	enet_address_set_host(&address, serverHost);
	address.port = serverPort;
	
	ENetPeer *server = enet_host_connect(client, &address, 2, 0);
	if (server == nullptr)
	{
		std::cerr << "No available peers for initiating an ENet connection." << std::endl;
		return EXIT_FAILURE;
	}

	ENetEvent networkEvent;

	// Wait up to 1 second for the connection attempt to succeed.
	if (enet_host_service(client, &networkEvent, 1000) > 0 &&
		networkEvent.type == ENET_EVENT_TYPE_CONNECT)
	{
		std::cerr << "Connection to " << serverHost << ":" << serverPort << " successful." << std::endl;
	}
	else
	{
		enet_peer_reset(server);
		std::cerr << "Connection to " << serverHost << ":" << serverPort << " failed." << std::endl;
		return EXIT_FAILURE;
	}

	std::string message("fourbe");
	ENetPacket *packet = enet_packet_create(message.c_str(), message.length(), 0);

	int nbPackageToSend = 1000;

	SYSTEMTIME before, after;
	GetSystemTime(&before);

	for (int i = 0; i < nbPackageToSend - 1; ++i)
	{
		enet_peer_send(server, 0, packet);
		Sleep(1);
	}
	
	const char* messageFin = "end";
	packet = enet_packet_create(messageFin, strlen(messageFin), 0);
	enet_peer_send(server, 0, packet);

	//même chose que au dessus mais avec un string
	//std::string message("hello");
	//ENetPacket *packet = enet_packet_create(message.c_str(), message.length(), 0);
	//enet_peer_send(server, 0, packet);

	int receivedPackage = 0;
	std::string retour = "";
	bool connected = true;
	while (connected)
	{
		int serviceResponse = enet_host_service(client, &networkEvent, 0);
		if (serviceResponse < 0) {
			std::cerr << "Event handling failed." << std::endl;
		}

		if (serviceResponse > 0)
		{
			switch (networkEvent.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
				receivedPackage++;
				std::cout
					<< "Received "
					<< networkEvent.packet->dataLength
					<< " bytes on channel "
					<< static_cast<int>(networkEvent.channelID)
					<< ": "
					<< std::string(reinterpret_cast<char*>(networkEvent.packet->data), networkEvent.packet->dataLength)
					<< "."
					<< std::endl;

				retour = std::string(reinterpret_cast<char*>(networkEvent.packet->data), networkEvent.packet->dataLength);

				if (retour.find("end") != std::string::npos)
				{
					GetSystemTime(&after);
					std::cout
					<< "number package send :"
					<< std::to_string(nbPackageToSend)
					<< ", number package received :"
					<< std::to_string(receivedPackage)
					<< "time to arrive : "
					<< std::to_string(after.wMinute - before.wMinute)
					<< " m "
					<< std::to_string(after.wSecond - before.wSecond)
					<< " s "
					<< std::to_string(after.wMilliseconds - before.wMilliseconds)
					<< " ms "
					<< std::endl;
				}
				// Clean up the packet!
				enet_packet_destroy(networkEvent.packet);

				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				std::cout << "Disconnected." << std::endl;
				connected = false;
				break;
			}
		}
	}

	enet_host_destroy(client);
	enet_deinitialize();
	
	return EXIT_SUCCESS;
}
