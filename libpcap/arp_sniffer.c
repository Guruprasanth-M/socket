#include<stdio.h>
#include<pcap.h>
#include<string.h>
#include <stdlib.h>
#include<errno.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netinet/if_ether.h>
#include<time.h>

int print_available_interfaces(){
    char error[PCAP_ERRBUF_SIZE];
    pcap_if_t *interfaces, *temp;
    int i = 0;

    if(pcap_findalldevs(&interfaces, error) == -1){
        fprintf(stderr, "Error finding devices: %s\n", error);
        return -1;
    }

    printf("Available Network Interfaces:\n");
    for(temp = interfaces; temp != NULL; temp = temp->next){
        printf("#%d. %s\n", ++i, temp->name);
    }
    return 0;
}


void print_version(){
    printf("       __  __                    \n");
    printf("   /\\ |__)|__)                   \n");
    printf("  /--\\| \\ |                      \n");
    printf("                                 \n");
    printf("   __      __ __ __ __           \n");
    printf("  (_ ||\\ ||_ |_ |_ |__)          \n");
    printf("  __)|| \\||  |  |__| \\           \n");
    printf("                                 \n");
    printf("   __  _____ __ _____  __        \n");
    printf("  |  \\|_  | |_ /   | |/  \\|\\ |   \n");
    printf("  |__/|__ | |__\\__ | |\\__/| \\|   \n");
    printf("                                 \n");
    printf("  ___ __  __            __       \n");
    printf("   | /  \\/  \\|    \\  / /  \\    /|\n");
    printf("   | \\__/\\__/|__   \\/  \\__/.    |\n");
    printf("                                 \n");
    printf("                                 \n");                                                          
    printf("\nARP SNIFFER SPOOF DETECTION TOOL V0.1\n");
    printf("This tool will sniff for ARP packets in the interface and can possibly detect if there is an ongoing ARP spoofing attack. This tool is still in a beta stage. \n");
}


void print_help(char *bin){
    printf("\nAvailable options:\n");
    printf("----------------------------\n");
    printf("-h or --help:       \t\tprint this help text.\n");
    printf("-l or --lookup:     \t\tprint the available interfaces.\n");
    printf("-i or --interface:\t\tProvide the interface to sniff on.\n");
    printf("-v or --version:\t\tprint the version information.\n");
    printf("---------------------------------------------------------\n");
    printf("\nUsage: %s -i<interface> [You can look for the available interfaces using -l option]\n", bin);
    exit(1);
}


int sniff_arp(char *device_name){
    int i;
    char error[PCAP_ERRBUF_SIZE];
    pcap_t *pack_desc;
    const u_char *packet;
    struct pcap_pkthdr header;
    struct ether_header *eptr;
    u_char *hard_ptr;
}


int main(int argc, char *argv[]){
    // for(int i = 0; i < argc;i++){
    //     printf("Argument %d: %s\n", i, argv[i]);
    // }
    if(argc < 2 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
        print_version();
        print_help(argv[0]);
    }
    else if(strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--lookup") == 0){
        print_available_interfaces();
        exit(1);
    }
    else if(strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0){
        print_version();
    }
    else if(strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "--interface") == 0){
        if(argc < 3){
            printf("ERROR: Please provide an interface to sniff on.\n");
            printf("----------------------------------\n");
            print_available_interfaces();
            printf("\nUsage: %s -i<interface> [You can look for the available interfaces using -l option]\n", argv[0]);

        }else{
            sniff_arp(argv[2]);
        }
    }else{
        printf("Invalid argument.\n");
        print_help(argv[0]);
    }
    return 0;
}