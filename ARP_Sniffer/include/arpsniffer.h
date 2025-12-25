#ifndef ARPSNIFFER_H
#define ARPSNIFFER_H

#include <stdint.h>

/* ARP constants */
#define ARP_REQUEST  1
#define ARP_RESPONSE 2

/* ARP header structure */
typedef struct _arp_hdr {
    uint16_t htype;
    uint16_t ptype;
    uint8_t  hlen;
    uint8_t  plen;
    uint16_t opcode;
    uint8_t  sender_mac[6];
    uint8_t  sender_ip[4];
    uint8_t  target_mac[6];
    uint8_t  target_ip[4];
} arp_hdr;

/* core sniffing */
int sniff_arp(char *device);

/* utilities */
char* get_hardware_address(uint8_t mac[6]);
char* get_ip_address(uint8_t ip[4]);

/* ui / cli */
void alert_spoof(char *ip, char *mac);
int  print_available_interfaces(void);
void print_help(char *bin);
void print_version(void);

#endif
