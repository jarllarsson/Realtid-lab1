
#include <enet/enet.h>

#include <iostream>
#include <boost/thread.hpp>

int main()
{
	std::cout << "Hello, world!" << std::endl;

    if (enet_initialize () != 0)
    {
        fprintf (stderr, "An error occurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }
    atexit (enet_deinitialize);

	return 0;
}
