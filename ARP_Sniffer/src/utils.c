#include <stdio.h>
#include <stdlib.h>
#include "arpsniffer.h"

char* get_hardware_address(uint8_t mac[6]) {
    char *m = malloc(18);
    sprintf(m, "%02X:%02X:%02X:%02X:%02X:%02X",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return m;
}

char* get_ip_address(uint8_t ip[4]) {
    char *m = malloc(16);
    sprintf(m, "%d.%d.%d.%d",
            ip[0], ip[1], ip[2], ip[3]);
    return m;
}
