#include "mbed.h"
#include "Client.cpp"
#include <cstdio>

Thread thread;

void print_start_info() 
{
    printf("CFM Wi-Fi module started\n");
#ifdef MBED_MAJOR_VERSION
    printf("Mbed OS version %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
#endif
}

int scan_demo(WiFiInterface *wifi)
{
    WiFiAccessPoint *ap;

    printf("Scan:\n");

    int count = wifi->scan(NULL,0);

    if (count <= 0) {
        printf("scan() failed with return value: %d\n", count);
        return 0;
    }

    /* Limit number of network arbitrary to 15 */
    count = count < 15 ? count : 15;

    ap = new WiFiAccessPoint[count];
    count = wifi->scan(ap, count);

    if (count <= 0) {
        printf("scan() failed with return value: %d\n", count);
        return 0;
    }

    printf("%d networks available.\n", count);

    delete[] ap;
    return count;
}

int main()
{   
    WiFiInterface *wifi;
    print_start_info();
    wifi = WiFiInterface::get_default_instance();
    if (!wifi) {
        printf("ERROR: No WiFiInterface found.\n");
        return -1;
    }
    int count = scan_demo(wifi);
    if (count == 0) {
        printf("No WIFI APs found - can't continue further.\n");
        return -1;
    }

    printf("\nConnecting to %s...\n", MBED_CONF_APP_WIFI_SSID);
    int ret = wifi->connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
    if (ret != 0) {
        printf("\nConnection error: %d\n", ret);
        return -1;
    }
    SocketAddress address = SocketAddress();
    wifi->get_ip_address(&address);
    printf("CFM IP:%s\n", address.get_ip_address());

   
    // Thread thread(wifi, start, osPriorityNormal, OS_STACK_SIZE, NULL);
    Client client = Client(wifi);
    thread.start(&client, &Client::start_client);
    while (1) {
        // printf("main\n");
        // ThisThread::sleep_for(10000);
    }
    return 0;
}


