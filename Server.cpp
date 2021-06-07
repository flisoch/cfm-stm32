#include "mbed.h"
#include <cstdio>

#define PORT 4567
class Server 
{
    public:
        WiFiInterface *wifi;
        TCPSocket *server_socket;
        TCPSocket *client_socket;
        SocketAddress client_address;

        Server(WiFiInterface *wifi) 
        {
            this->wifi = wifi;
            server_socket = new TCPSocket();
            server_socket->open(wifi);
        }

        void start() 
        {   
            server_socket->bind(4567);
            printf("Started Server on port:%d \n", PORT);
                
            server_socket->listen(1);
            nsapi_error_t err;
            client_socket = server_socket->accept(&err);
            if (err == NSAPI_ERROR_OK) {
                printf("Ok, success\n");
            }
            else if(err == NSAPI_ERROR_WOULD_BLOCK) {
                printf("socket set to non-blocking \n");
            }
            else if(err == NSAPI_ERROR_NO_SOCKET) {
                printf("socket was not open\n");
            }
            else {
                printf("error: %d \n", err);
            }
            
            // client_socket->getpeername(&client_address);
            // printf("Accepted %s:%d\n", client_address.get_ip_address(), client_address.get_port());

            while (1) {
            
                char rbuffer[64];            
                int rcount = server_socket->recv(rbuffer, sizeof rbuffer);
                if (rcount > 0) {
                    rbuffer[rcount] = '\0';
                    printf("recv %d [%.*s]\n", rcount, strstr(rbuffer, "\r\n") - rbuffer, rbuffer);
                
                    sprintf(rbuffer, "got your message");
                    client_socket->send(rbuffer, strlen(rbuffer));
                }
                else {
                    printf("No data received\n");
                    ThisThread::sleep_for(3000);
                    break;
                }
            }
            printf("end\n");
            
        }

        void start_client() {
            server_socket->bind(PORT);
            SocketAddress address = SocketAddress("192.168.1.167");
            address.set_port(4567);
            server_socket->connect(address);
            char rbuffer[64]; 
            sprintf(rbuffer, "{\"method\":\"get\", \"topic\": \"/disconnect\"}");
            server_socket->send(rbuffer, strlen(rbuffer));
        }
};