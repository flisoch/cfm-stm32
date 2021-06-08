#include "mbed.h"
#include <cstdio>

#define PORT 4567
class Client {
public:
  WiFiInterface *wifi;
  TCPSocket *server_socket;
  // TCPSocket *client_socket;
  SocketAddress server_address;

  Client(WiFiInterface *wifi) {
    this->wifi = wifi;
    server_socket = new TCPSocket();
    int err;
    err = server_socket->open(wifi);
    printf("Open socket, err: %d \n", err);

    server_address = SocketAddress("192.168.1.2");
    server_address.set_port(4567);
  }

  void start() {
    // printf("Started Server on port:%d \n", PORT);
    // nsapi_error_t err;
    // if (err == NSAPI_ERROR_OK) {
    //     printf("Ok, success\n");
    // }
    // else if(err == NSAPI_ERROR_WOULD_BLOCK) {
    //     printf("socket set to non-blocking \n");
    // }
    // else if(err == NSAPI_ERROR_NO_SOCKET) {
    //     printf("socket was not open\n");
    // }
    // else {
    //     printf("error: %d \n", err);
    // }

    // client_socket->getpeername(&client_address);
    // printf("Accepted %s:%d\n", client_address.get_ip_address(),
    // client_address.get_port());

    // while (1) {

    //     char rbuffer[64];
    //     int rcount;

    //     sprintf(rbuffer, "{\"method\":\"get\", \"topic\": \"/disconnect\"}");
    //     server_socket->connect(client_address);
    //     rcount = server_socket->send(rbuffer, strlen(rbuffer));

    //     rcount = server_socket->recvfrom(&client_address, rbuffer, sizeof
    //     rbuffer);

    //     if (rcount > 0) {
    //         rbuffer[rcount] = '\0';
    //         printf("recv %d [%.*s]\n", rcount, strstr(rbuffer, "\r\n") -
    //         rbuffer, rbuffer);

    //         sprintf(rbuffer, "got your message");
    //         client_socket->send(rbuffer, strlen(rbuffer));
    //     }
    //     else {
    //         printf("No data received\n");
    //         ThisThread::sleep_for(3000);
    //         break;
    //     }
    // }
    // printf("end\n");
  }

  void start_client() {

    nsapi_error_t conn_err;
    while (1) {

      conn_err = server_socket->connect(server_address);
      printf("Connection: err=  %d \n", conn_err);
      if (conn_err < 0) {
        printf("Failed to connect\n");
        ThisThread::sleep_for(5000);
        continue;
      } else {
        char rbuffer[256];
        int rcount;
        sprintf(rbuffer, "{\"topic\": \"connect\", \"data\": \"hello\"}");
        server_socket->send(rbuffer, strlen(rbuffer));
        rcount = server_socket->recv(rbuffer, sizeof rbuffer);
        rbuffer[rcount] = '\0';
        printf("recv %d [%.*s]\n", rcount, strstr(rbuffer, "\r\n") - rbuffer,
               rbuffer);
        break;
      }
    }
  }
};