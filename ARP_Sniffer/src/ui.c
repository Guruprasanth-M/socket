#include <stdio.h>
#include <pcap.h>
#include "arpsniffer.h"

void print_version(void) {
    printf("\nARP SNIFFER SPOOF DETECTION TOOL v0.1\n");
    printf("Detects suspicious ARP packet bursts.\n\n");
}

void print_help(char *bin) {
    printf("Usage: %s [options]\n\n", bin);
    printf("Options:\n");
    printf("  -h, --help           Show help menu\n");
    printf("  -l, --lookup         List network interfaces\n");
    printf("  -i, --interface IF   Sniff on interface IF\n");
    printf("  -v, --version        Show version\n\n");
}

void alert_spoof(char *ip, char *mac) {
    printf("ALERT: Possible ARP Spoofing Detected | IP: %s | MAC: %s\n",
           ip, mac);
}

int print_available_interfaces(void) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *devs, *d;
    int i = 0;

    if (pcap_findalldevs(&devs, errbuf) == -1) {
        fprintf(stderr, "%s\n", errbuf);
        return -1;
    }

    printf("Available interfaces:\n");
    for (d = devs; d; d = d->next)
        printf("  %d. %s\n", ++i, d->name);

    pcap_freealldevs(devs);
    return 0;
}
