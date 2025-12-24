#include<stdio.h>
#include<pcap.h>
#include<stdlib.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include<string.h>
#include<sys/socket.h>

#define _USE_BSD // TO use the BSD IP header
#define _FAVOR_BSD // TO use the BSD TCP header

#define TCPSYN_LEN 20
#define MAXBYTES2CAPTURE 2048

typedef struct pseudo_tcp_header{
    uint32_t src;
    uint32_t dst;
    uint8_t zero;
    uint8_t protocol;
    uint16_t len;
}tcp_pseudohdr;

unsigned short in_cksum(unsigned short *addr,int len){
    register int sum = 0;
    u_short answer = 0;
    register u_short *w = addr;
    register int nleft = len;

    /*
    *Our algorithm is simple, using a 32 bit accumulator (sum), we add
    *up 16 bit words, and at the end, fold back all the carry bits
    *from the top 16 bits into the lower 16 bits.
    */

    while(nleft > 1){
        sum += *w++;
        nleft -= 2;
    }
    // mop up an odd byte, if necessary
    if(nleft == 1){
        *(u_char *)(&answer) = *(u_char *)w;
        sum += answer;
    }
    // add back carry outs from top 16 bits to low 16 bits
    sum = (sum >> 16) + (sum & 0xffff); // add
    sum += (sum >> 16); // add carry
    answer = ~sum;// truncate to 16 bits
    return(answer);
}

int tcp_send_rest(u_int32 seq, u_int32 src_ip, u_int32 dest_ip, u_int32 src_port, u_int32 dest_port){
    
    int rawsocket = 0;
    int one = 1;
    
    char packet[sizeof(struct tcphdr) + sizeof(struct ip) + 1];

     struct ip *ipheader = (struct ip*)packet;
     struct tcphdr *tcpheader = (struct tcphdr*)(packet + sizeof(struct ip));

     tcp_pseudohdr pseudohdr;

     char tcpcsumblock[sizeof(tcp_pseudohdr) + TCPSYN_LEN];
     struct sockaddr_in dstaddress;

     memset(&pseudohdr, 0, sizeof(tcp_pseudohdr));
     memset(&dstaddress, 0, sizeof(dstaddress));
     memset(&packet, 0, sizeof(tcp_packet));

     dstaddress.sin_family = AF_INET;
     dstaddress.sin_port = dest_port;
     dstaddress.sin_addr.s = dest_ip;

     rawsocket = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);

     /*

     TO BE CONTINUE START EXACTLY AT 3:34 or before this ..

     */
}

/*
* usage: ./binary_file <interface> <port>
*/
int main(int argc, char *argv[]){

    pcap_t *desc;
    char error[PCAP_ERRBUF_SIZE];
    char fillter_expression[255] = "((tcp[13] == 0x10) or (tcp[13] == 0x18)) and port 6666";
    struct bpf_program fp;
    // char *device = NULL;
    bpf_u_int32 ip;
    bpf_u_int32 netmask;
    struct ip *iphdr = NULL;
    struct tcphdr *tcphdr = NULL;
    const u_char *packet;
    struct pcap_pkthdr header;
    int count = 0;


    if(argc != 3){
        printf("Usage: %s <interface>\n", argv[0]);
        exit(0);
    }

    desc = pcap_open_live(argv[1], MAXBYTES2CAPTURE, 1, 1, error);
    if(desc == NULL){
        printf("%s\n",error);
        exit(-1);
    }else{
        printf("Listening on %s.......\n", device);
    }

    if(pcap_compile(desc, &fp, fillter_expression, 0, netmask) == -1){
        fprintf(stderr, "Couldn't parse filter %s: %s\n", fillter_expression, pcap_geterr(desc));
        exit(-1);
    }

    if(pcap_setfilter(desc, &fp) == -1){
        fprintf(stderr, "Cannot set filter using pcap_setfilter %s: %s\n", fillter_expression, pcap_geterr(desc));
        exit(-1);
    }

    while(1){
        packet = pcap_next(desc, &header);
        if(packet == NULL){
            printf("Error: Cannot capture packet\n");
            return -1;
        }else{
            iphdr = (struct ip*)(packet + 14);
            tcphdr = (struct tcphdr*)(packet + 14 + 20);
            if(count = 0){
                printf("-----------------------------------------------------------\n");
                printf("Recevied packet #%d:\n",++count);
                printf("\tACK: %u\n",ntohl(tcphdr->th_ack));
                printf("\tSEQ: %u\n",ntohl(tcphdr->th_seq));
                printf("\tDEST IP: %s\n",inet_ntoa(iphdr->ip_dst));
                printf("\tSRC IP: %s\n",inet_ntoa(iphdr->ip_src));
                printf("\tDEST PORT: %s\n",ntohs(tcphdr->th_sport));
                printf("\tSRC PORT: %s\n",ntohs(tcphdr->th_dport));

                tcp_send_reset(tcphdr->th_ack, iphdr->ip_dst.s_addr, iphdr->ip_src.s_addr,
                    tcphdr->th_dport, tcphdr->th_sport);
                
            }
        }
    }
}