#include<stdio.h>
#include<stdlib.h>
#include<pcap.h>
#include<errno.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netinet/if_ether.h>
#include<time.h>

int main(){
    int i;
    char *device_name;
    char error[PCAP_ERRBUF_SIZE];
    pcap_t *pack_desc;
    const u_char *packet;
    struct pcap_pkthdr header;
    struct ether_header *eptr;
    u_char *hard_ptr;

    // Lookup device
    device_name = pcap_lookupdev(error);
    if (device_name == NULL) {
        printf("Error in getting device name: %s\n", error);
        return -1;
    } else {
        printf("Device Name: %s\n", device_name);
    }

    // Open device for live capture
    pack_desc = pcap_open_live(device_name, BUFSIZ, 0, 1000, error);
    if (pack_desc == NULL) {
        printf("Error in opening device: %s\n", error);
        return -1;
    }
    printf("-----------------\n");
    // Capture one packet
    packet = pcap_next(pack_desc, &header);
    if (packet == NULL) {
        printf("No packet captured. Try again.\n");
        pcap_close(pack_desc); // Close handle before exiting
        return -1;
    } else {
        printf("Received a packet with length %d\n", header.len);
        printf("Received at %s\n", ctime((const time_t*) &header.ts.tv_sec));
        printf("Ethernet header length: %d\n", ETHER_ADDR_LEN);

        eptr = (struct ether_header*) packet;
        if(ntohs(eptr->ether_type) == ETHERTYPE_IP){
            printf("Ethernet type dex : 0x%x; dec : %d is an ip packet\n",ETHERTYPE_IP,ETHERTYPE_IP);
        }else if((ntohs(eptr->ether_type) == ETHERTYPE_ARP)){
            printf("Ethernet type dex : 0x%x; dec : %d is an ip packet\n",ETHERTYPE_ARP,ETHERTYPE_ARP);
        }else{
            printf("Ethernet type hex: 0x%x; dec: %d is not an ip or arp packet, so quiting...\n",ntohs(eptr->ether_type),ntohs(eptr->ether_type));
            return -1;
        }

        hard_ptr = eptr->ether_dhost;
        i = ETHER_ADDR_LEN;
        printf("Destination address:");
        do{
            printf("%s%x",(i== ETHER_ADDR_LEN)?"":":",*hard_ptr++);
        }while(--i>0);
        printf("\n");

        hard_ptr = eptr->ether_dhost;
        i = ETHER_ADDR_LEN;
        printf("source address:");
        do{
            printf("%s%x",(i== ETHER_ADDR_LEN)?"":":",*hard_ptr++);
        }while(--i>0);
        printf("\n");
        printf("-----------------\n");
    }
    
    // Close the packet descriptor to free resources
    pcap_close(pack_desc);
    return 0;
}