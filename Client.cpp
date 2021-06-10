#include "mbed.h"
#include <cstdio>

#define PORT 4567
class Client {
public:
  WiFiInterface *wifi;
  TCPSocket *server_socket;
  SocketAddress server_address;
  bool connected;

  Client(WiFiInterface *wifi) {
    this->wifi = wifi;
    server_socket = new TCPSocket();
    int err;
    err = server_socket->open(wifi);
    printf("Open socket, err: %d \n", err);

    server_address = SocketAddress("192.168.1.2");
    server_address.set_port(4567);
    connected = false;
  }

  void connect() {
    nsapi_error_t conn_err;

    while (!connected) {
      conn_err = server_socket->connect(server_address);
      printf("Connection: err=  %d \n", conn_err);
      if (conn_err < 0) {
        printf("Failed to connect\n");
        ThisThread::sleep_for(5000);
      } else {
        connected = true;
      }
    }
  }

  int receive_message(char *rbuffer) {
    int rcount;
    rcount = server_socket->recv(rbuffer, strlen(rbuffer));
    rbuffer[rcount] = '\0';
    printf("recv %d [%.*s]\n", rcount, strstr(rbuffer, "\r\n") - rbuffer,
           rbuffer);
    return rcount;
  }

  void generate_message(char *rbuffer, int *left) {
    char topic = rbuffer[0];
    *left = (*left) - 1;
    if (topic == 'x') {
      // disconnect
    } else if (topic == 'g') {
      // gauss
      printf("TOPIC G");
      sprintf(rbuffer, "n1b-1.790667E+01r-5.701844E-02i-5.636006E-02");

    } else if (topic == 'c') {
      // connect
    } else if (topic == 'm') {
      // mask
    } else if (topic == 'd') {
      // demagnetize
    }
  }

  void send_message(char *rbuffer) {
    server_socket->send(rbuffer, strlen(rbuffer));
  }

  void handle_commands() {
    char rbuffer[128];
    int rcount;
    while (connected) {
      rcount = receive_message(rbuffer);
      if (rcount > 0) {
        int messages_left = 10000;
        while (messages_left > 0) {
          generate_message(rbuffer, &messages_left);
          send_message(rbuffer);
        }
      }
    }
  }

  void start_client() {
    while (1) {
      connect();
      printf("Connection Established\n");
    //   connected = false;
      handle_commands();

      ThisThread::sleep_for(10000);
    }
  }
};