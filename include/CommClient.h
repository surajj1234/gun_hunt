/***************************************************************************
 * Author : Suraj Joseph
 * Available for public use (but not misuse !) 
****************************************************************************/

#ifndef COMMCLIENT_H
#define COMMCLIENT_H

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mutex>

class CommClient
{
    public:
        CommClient(std::string server_address, uint16_t server_port);
        ~CommClient();
        void Run();
        void Quit();
        int SendData(std::string data);
        bool IsConnected();
    private:
        bool terminate, connected;
        std::mutex my_lock;
        struct sockaddr_in sockaddr;
        int socket_descriptor;
        std::string server_address;
        uint16_t server_port;
        void connectToServer();
        int sendWithLocking(const char * buf, int len);
};

#endif
