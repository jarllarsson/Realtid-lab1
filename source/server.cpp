
#include <enet/enet.h>

#include <iostream>
#include <boost/thread.hpp>

int main()
{
	std::cout << "Hello, world!" << std::endl;

    if (enet_initialize () != 0)
    {
		std::cerr << "Could not initialize ENet!" << std::endl;
		return 0;
    }
    atexit (enet_deinitialize);

	ENetAddress address;
	ENetHost* server;

	address.host = ENET_HOST_ANY;
	address.port = 1234;

	server = enet_host_create(&address, 32, 2, 0, 0);

	if (server == NULL)
	{
		std::cerr << "Could not create ENet host!" << std::endl;
		return 0;
	}
	
	bool running = true;

	while (running)
	{
		ENetEvent event;
		while (enet_host_service (server, & event, 1000) > 0)
		{
			switch (event.type)
			{
				case ENET_EVENT_TYPE_CONNECT:
					printf ("A new client connected from %x:%u.\n", 
					event.peer -> address.host,
					event.peer -> address.port);

					/* Store any relevant client information here. */
					event.peer -> data = "Client information";

					break;

				case ENET_EVENT_TYPE_RECEIVE:
					printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
					event.packet -> dataLength,
					event.packet -> data,
					event.peer -> data,
					event.channelID);

					/* Clean up the packet now that we're done using it. */
					enet_packet_destroy (event.packet);
            
					break;
           
				case ENET_EVENT_TYPE_DISCONNECT:
					printf ("%s disconected.\n", event.peer -> data);

					/* Reset the peer's client information. */

					event.peer -> data = NULL;
			}
		}
	}

	enet_host_destroy(server);

	return 0;
}
