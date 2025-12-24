#include<stdio.h>
#include<pcap.h>
#include<string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#define MAXBYTES2CAPTURE 2048

void print_packet(u_char *count,const struct pcap_pkthdr *h,const u_char *bytes){
    int i, *counter = (int *)count;

    printf("--------------------------------------\n");
    printf("Packet count : %d\n", ++(*counter));
    printf("Received a packet with length of [%d]\n", h->len);
    printf("Received time : %s\n", ctime((const time_t *)&h->ts.tv_sec));
    printf("Payload:\n");
    for(i = 0; i < h->len; i++){
        if(isprint(bytes[i])){
            printf("%c ", bytes[i]);
        }else{
            printf(". ");
        }
        if(i % 32 == 0 && i!= 0 || i == h->len -1){
            printf("\n");
        }
    }
    return;
}

int main(int argc, char *argv[]){
    char *device = NULL;
    int count = 0;
    char error[PCAP_ERRBUF_SIZE];
    pcap_t *desc;
    char fillter_expression[] = "port 6666";
    struct bpf_program fp;
    bpf_u_int32 subnet_mask, ip;

    if(argc > 1){
        device = argv[1];
    }else{
        printf("Usage: %s <interface>\n", argv[0]);
        exit(0);
    }
    
    if(pcap_lookupnet(device, &ip, &subnet_mask, error) == -1){
        printf("Couldn't get netmask for device %s: %s\n", device, error);
        exit(-1);
    }

    printf("Opening device %s for sniffing...\n", device);
    desc = pcap_open_live(device, MAXBYTES2CAPTURE, 1, 1, error);
    if(desc == NULL){
        printf("%s\n",error);
        exit(-1);
    }else{
        printf("Listening on %s.......\n", device);
    }

    if(pcap_compile(desc, &fp, fillter_expression, 0, subnet_mask) == -1){
        printf("Couldn't parse filter %s: %s\n", fillter_expression, pcap_geterr(desc));
        exit(-2);
    }

    if(pcap_setfilter(desc, &fp) == -1){
        printf("Cannot set filter using pcap_setfilter(): %s: %s\n", fillter_expression, pcap_geterr(desc));
        exit(-2);
    }

    if(pcap_loop(desc, -1,print_packet,(u_char *)&count) == -1){
        printf("Error in pcap_loop: %s\n", pcap_geterr(desc));
        exit(-2);
    }
    return 0;
}