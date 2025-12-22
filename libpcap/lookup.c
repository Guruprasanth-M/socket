#include<stdio.h>
#include<stdlib.h>
#include<pcap.h>
#include<errno.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int Device_Look(int argc,char *argv[]){
    char error[PCAP_ERRBUF_SIZE];
    pcap_if_t *interfaces, *temp;
    int i = 0;

    if(pcap_findalldevs(&interfaces, error) == -1){
        printf("cannot find devices\n");
        return -1;
    }

    printf("Available Network Interfaces:\n");
    for(temp = interfaces; temp; temp = temp->next){
        printf("%d.%s\n", ++i,temp->name);
    }
    return 0;

}

int main(int argc,char *argv[]){
    if(Device_Look(argc,argv) == -1){
        return -1;
    }
    return 0;
}