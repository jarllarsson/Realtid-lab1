
#include <enet/enet.h>

#include <iostream>
#include <boost/thread.hpp>

using namespace std;

int main()
{
	std::cout << "Hello, world!" << std::endl;

    if (enet_initialize () != 0)
    {
        fprintf (stderr, "An error occurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }
    atexit (enet_deinitialize);



	/////////////////////////////////////////////////////////////////////////

	ENetHost * client;

	client = enet_host_create (NULL /* create a client host */,
									1 /* only allow 1 outgoing connection */,
									2 /* allow up 2 channels to be used, 0 and 1 */,
									57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
									14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);

	if (client == NULL)
	{
		fprintf (stderr, 
			"An error occurred while trying to create an ENet client host.\n");
		exit (EXIT_FAILURE);
	}

	// do stuff here
	// =============================================================
	ENetAddress address;
	ENetEvent event;
	ENetPeer *peer;


	enet_address_set_host (&address, "194.47.147.51");
	address.port = 1234;

	/* Initiate the connection, allocating the two channels 0 and 1. */
	peer = enet_host_connect (client, &address, 2, 0);    

	if (peer == NULL)
	{
		fprintf (stderr, 
			"No available peers for initiating an ENet connection.\n");
		exit (EXIT_FAILURE);
	}

	/* Wait up to 5 seconds for the connection attempt to succeed. */
	if (enet_host_service (client, & event, 5000) > 0 &&
		event.type == ENET_EVENT_TYPE_CONNECT)
	{
		cout<<"Connection to some.server.net:1234 succeeded.";


		/* Create a reliable packet of size 7 containing "packet\0" */
		ENetPacket * packet = enet_packet_create ("packet", 
			strlen ("packet") + 1, 
			ENET_PACKET_FLAG_RELIABLE);

		/* Extend the packet so and append the string "foo", so it now */
		/* contains "packetfoo\0"                                      */
		/*enet_packet_resize (packet, strlen ("packetfoo") + 1);
		strcpy (&packet -> data [strlen ("packet")], "foo");*/

		/* Send the packet to the peer over channel id 0. */
		/* One could also broadcast the packet by         */
		/* enet_host_broadcast (host, 0, packet);         */
		enet_peer_send (peer, 0, packet);

		/* One could just use enet_host_service() instead. */
		enet_host_flush(client);


	}
	else
	{
		/* Either the 5 seconds are up or a disconnect event was */
		/* received. Reset the peer in the event the 5 seconds   */
		/* had run out without any significant event.            */
		enet_peer_reset (peer);

		cout<<"Connection to some.server.net:1234 failed.";
	}




	cin.get();
	// =============================================================
	// deallocate enet

	enet_host_destroy(client);


	return 0;
}
