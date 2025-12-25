#include <stdio.h>
#include <time.h>
#include <netinet/if_ether.h>
#include <pcap.h>
#include <stdlib.h>
#include "arpsniffer.h"

int sniff_arp(char *device) {

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    const u_char *packet;
    struct pcap_pkthdr header;
    struct ether_header *eth;
    arp_hdr *arp;

    int counter = 0;
    time_t ct, lt;
    time(&lt);

    handle = pcap_open_live(device, BUFSIZ, 0, 1000, errbuf);
    if (!handle) {
        fprintf(stderr, "%s\n", errbuf);
        return -1;
    }

    printf("Listening on %s...\n", device);

    while (1) {
        packet = pcap_next(handle, &header);
        if (!packet)
            continue;

        eth = (struct ether_header *)packet;
        if (ntohs(eth->ether_type) != ETHERTYPE_ARP)
            continue;

        ct = time(NULL);
        if (ct - lt > 20)
            counter = 0;

        arp = (arp_hdr *)(packet + ETHER_HDR_LEN);

        char *ip  = get_ip_address(arp->sender_ip);
        char *mac = get_hardware_address(arp->sender_mac);

        counter++;
        if (counter > 10) {
            alert_spoof(ip, mac);
            counter = 0;
        }

        free(ip);
        free(mac);
        lt = ct;
    }

    return 0;
}
