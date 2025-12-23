#include<stdio.h>
#include<stdlib.h>
#include<pcap.h>
#include<errno.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>


int main(){
    char *device_name, *net_addr, *net_mask;
    int rcode;
    char error[PCAP_ERRBUF_SIZE];

    bpf_u_int32 net_addr_int, net_mask_int; //ip address as unsigned int32
    struct in_addr addr;

    //asks pcap to give a valid eth based device to sniff on it
    device_name = pcap_lookupdev(error);
    if(device_name == NULL){
        printf("Error in getting device name: %s\n", error);
        return -1;
    }else{
        printf("Device Name: %s\n", device_name);
    }


    //with the device place,aquire the ip address and the subnet mask
    rcode = pcap_lookupnet(device_name, &net_addr_int, &net_mask_int, error);
    if(rcode == -1){
        printf("Error in getting details: %s\n", error);
        return -1;
    }


    //convert the ip address to human readable format
    addr.s_addr = net_addr_int;
    net_addr = inet_ntoa(addr);

    if(net_addr == NULL){
        printf("Error in getting ip address\n");
        return -1;
    }else{
        printf("IP Address: %s\n", net_addr);
    }

    addr.s_addr = net_mask_int;
    net_addr = inet_ntoa(addr);

    if(net_addr == NULL){
        printf("Error in getting netmask address\n");
        return -1;
    }else{
        printf("IP Address: %s\n", net_addr);
    }
    return 0;
}