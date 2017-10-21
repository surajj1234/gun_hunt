/***************************************************************************
 * Author : Suraj Joseph
 * Available for public use (but not misuse !) 
****************************************************************************/

#include "CommClient.h"
#include <string.h>
#include <unistd.h>

CommClient::CommClient(std::string address, uint16_t port)
{
    server_address = address;
    server_port = port;
    terminate = false;
    connected = false;
    socket_descriptor = -1;
}

CommClient::~CommClient()
{
    if (connected)
    {
        close(socket_descriptor);
    }
}

void CommClient::Run()
{
    while(terminate == false)
    {
        if (connected)
        {
            sleep(1);

            // Sneaky way to verify that the connection is still open,
            // do a write with a carriage return and catch disconnects
            // if it fails
            char heartbeat = '\r';
            int ret = sendWithLocking(&heartbeat, 1);
		    if(ret == -1)
            {
                connected = false;
            }
        }
        else
        {
            // Try reconnecting if connection was broken
            sleep(1);
            close(socket_descriptor);
            connectToServer();
        }
    }
}

int CommClient::SendData(std::string data)
{
    // On success, returns number of bytes sent, -1 on failure
    return sendWithLocking(data.c_str(), data.length());
}

void CommClient::Quit()
{
    terminate = true;
}

void CommClient::connectToServer()
{
    // Construct socket address
	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = inet_addr(server_address.c_str());
	sockaddr.sin_port = htons(server_port);

    connected = false;
    socket_descriptor = socket(PF_INET, SOCK_STREAM, 0);

	if (socket_descriptor >= 0)
    {
        if (connect(socket_descriptor, (struct sockaddr *)&sockaddr,
                    sizeof(sockaddr)) >= 0)
        {
            connected = true;
        }
    }
}

bool CommClient::IsConnected()
{
    // Critical section
    std::lock_guard<std::mutex> lk(my_lock);
    return connected;
}

int CommClient::sendWithLocking(const char * buf, int len)
{
    // critical section
    std::lock_guard<std::mutex> lk(my_lock);
    return send(socket_descriptor, buf, len, MSG_NOSIGNAL);
}
