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
#include<unistd.h>

#define ARP_REQUEST 1   //ARP Request
#define ARP_RESPONSE 2  //ARP Response

typedef struct _arp_hdr arp_hdr;
struct _arp_hdr{  
    uint16_t htype;        //Hardware type
    uint16_t ptype;        //Protocol type
    uint8_t hlen;          //Hardware address
    uint8_t plen;          //Protocol address
    uint16_t opcode;       //Operation code(request or respone)
    uint8_t sender_mac[6]; //Sender hardware address
    uint8_t sender_ip[4];  //Sender ip address
    uint8_t target_mac[6]; //Target mac address
    uint8_t target_ip[4];  //Target ip address
};

void aleart_spoof(char *ip, char *mac){
    char cmd[256];
    sprintf(cmd,"/usr/bin/notify-send -t 5000 -i face-angry \"Possible ARP Spoofing Detected. IP: %s and MAC: %s.\"",ip,mac);
    system(cmd);
    printf("ALERT: Possible ARP Spoofing Detected. IP: %s and MAC: %s.\n",ip,mac);
}

void aleart_welcome(){
    system("/usr/bin/notify-send -t 5000 -i face-angle \"I am watching for ARP Spoofing.Sit back and relax!\"");
}

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
    printf("--------------------------------------------------------------\n");
    printf("-h or --help:\t\t\tprint this help text.\n");
    printf("-l or --lookup:\t\t\tprint the available interfaces.\n");
    printf("-i or --interface:\t\tProvide the interface to sniff on.\n");
    printf("-v or --version:\t\tprint the version information.\n");
    printf("--------------------------------------------------------------\n");
    printf("\nUsage: %s -i<interface> [You can look for the available interfaces using -l option]\n", bin);
    exit(1);
}


char* get_hardware_address(uint8_t mac[6]){
    char *m = (char*)malloc(20*sizeof(char));
    sprintf(m,"%02X:%02X:%02X:%02X:%02X:%02X",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
    return m;
}

char* get_ip_address(uint8_t ip[4]){
    char *m = (char*)malloc(20*sizeof(char));
    sprintf(m,"%d.%d.%d.%d",ip[0],ip[1],ip[2],ip[3]);
    return m;
}

int sniff_arp(char *device_name){
    char error[PCAP_ERRBUF_SIZE];
    pcap_t *pack_desc;
    const u_char *packet;
    struct pcap_pkthdr header;
    struct ether_header *eptr;
    arp_hdr *arpheader = NULL;
    int i;
    u_char *hard_ptr;
    char *t_mac, *t_ip, *s_mac, *s_ip;
    int counter = 0;
    time_t ct,lt;
    long int diff;

    pack_desc = pcap_open_live(device_name,BUFSIZ,0,1,error);
    if(pack_desc == NULL){
        printf("%s\n",error);
        print_available_interfaces();
        return -1;
    }else{
        printf("Listening on %s......\n",device_name);
    }
    while(1){
        packet = pcap_next(pack_desc,&header);
        if(packet == NULL){
            printf("ERROR: Cannot capture packet\n");
            return -1;
        }else{
            eptr = (struct ether_header*)packet;
            if(ntohs(eptr->ether_type) == ETHERTYPE_ARP){
                ct = time(NULL);
                diff = ct - lt;
                if(diff > 20){
                    counter = 0;
                }
                printf("ct: %ld; diff: %ld; Counter: %d\n",ct,diff,counter);
                arpheader = (arp_hdr*)(packet+14);
                printf("\nReceived an ARP packet with length %d\n",header.len);
                printf("Received at %s",ctime((const time_t*) &header.ts.tv_sec));
                printf("Ethernet header length: %d\n",ETHER_HDR_LEN);
                printf("Operation type: %s\n", ntohs(arpheader->opcode) == ARP_REQUEST ? "ARP Request" : "ARP Response");
                t_ip = get_ip_address(arpheader->target_ip);
                t_mac = get_hardware_address(arpheader->target_mac);
                s_ip = get_ip_address(arpheader->sender_ip);
                s_mac = get_hardware_address(arpheader->sender_mac);
                printf("Sender MAC: %s\n", s_mac);
                printf("Sender IP: %s\n", s_ip);
                printf("Target MAC: %s\n", t_mac);
                printf("Target IP: %s\n", t_ip);
                printf("--------------------------------------------------------\n");
                lt = time(NULL);
                counter++;
                if(counter > 10){
                    aleart_spoof(s_ip, s_mac);
                }else{

                }
            }
        }
    }
    return 0;
}


int main(int argc, char *argv[]){

    if(access("/usr/bin/notify-send",F_OK) == -1){
        printf("Missing dependency: notify-send\n");
        printf("Please run: sudo apt-get install libnotify-bin");
        printf("\n");
        print_version();
    }else{
        aleart_welcome();
    }

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