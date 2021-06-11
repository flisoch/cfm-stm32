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
    int err;
    err = server_socket->open(wifi);
    while (!connected) {
      conn_err = server_socket->connect(server_address);
      printf("Connection: err=  %d \n", conn_err);
      if (conn_err < 0) {
        printf("Failed to connect\n");
        ThisThread::sleep_for(1000);
      } else {
        connected = true;
      }
    }
  }

  int receive_message(char *rbuffer) {
    int rcount;
    rcount = server_socket->recv(rbuffer, strlen(rbuffer));
    rbuffer[rcount] = '\0';
    if (rcount > 0) {
      printf("recv %d [%.*s]\n", rcount, strstr(rbuffer, "\r\n") - rbuffer,
             rbuffer);
    }
    return rcount;
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
        char topic = rbuffer[0];

        if (topic == 'x') {
          // disconnect
          server_socket->close();
          connected = false;
        } else if (topic == 'g') {
          // gauss
          printf("TOPIC G\n");
          send_gauss_points();

        } else if (topic == 'c') {
          // connect
        } else if (topic == 'm') {
          // mask
        } else if (topic == 'd') {
          // demagnetize
        }
        // }
      }
    }
  }

  void start_client() {
    while (1) {
      connect();
      printf("Connection Established\n");
      //   connected = false;
      handle_commands();
      printf("Connection closed");
    }
  }

  void send_gauss_points() {
    char rbuffer[255];
    sprintf(rbuffer, "g 1 -1.790667E+01 -5.701844E-02 -5.636006E-02");
    send_message(rbuffer);
    sprintf(rbuffer, "g 2 -1.808751E+01 -5.712836E-02 -5.623527E-02");
    send_message(rbuffer);
    sprintf(rbuffer, "g 3 -1.790298E+01 -5.710830E-02 -5.647627E-02");
    send_message(rbuffer);
    sprintf(rbuffer, "g 4 -1.784763E+01 -5.717870E-02 -5.647685E-02");
    send_message(rbuffer);
    sprintf(rbuffer, "g 5 -1.805061E+01 -5.727426E-02 -5.658544E-02");
    send_message(rbuffer);
    sprintf(rbuffer, "g 6 -1.801001E+01 -5.724437E-02 -5.642178E-02");
    send_message(rbuffer);
    sprintf(rbuffer, "g 7 -1.798049E+01 -5.731123E-02 -5.671356E-02");
    send_message(rbuffer);
    sprintf(rbuffer, "g 8 -1.814656E+01 -5.738969E-02 -5.678269E-02");
    send_message(rbuffer);
  }
};