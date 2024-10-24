#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

#define TODO()\
do{\
    extern int printf(char *, ...);\
    printf("Add your code here: file %s, line %d\n", __FILE__, __LINE__);\
}while(0)




#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#include "send_icmp.h"

unsigned char sendbuff[BUFFSIZE] = { '\0' };
unsigned char recvbuff[BUFFSIZE] = { '\0' };

tunnel* tnel;
int sckt;
struct sockaddr_in dest_addr;
unsigned short total_len;

int main() {
    sckt = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);

    char myname[NAMESIZE];
    printf("Enter your name: ");
    scanf("%s", myname);

    char whoname[NAMESIZE];
    printf("Who you want to communicate: ");
    scanf("%s", whoname);

    char dst_ip[16];
    printf("Enter the dest IP: ");
    scanf("%s", dst_ip);

    init_icmp_packet(myname, whoname, dst_ip);

    pid_t pid = fork();
    if (pid == 0) {
        while (1) {
            int n = recvfrom(sckt, recvbuff, BUFFSIZE, 0, NULL, NULL);
            struct iphdr* iph = (struct iphdr*)(recvbuff);
            struct icmphdr* icmph = (struct icmphdr*)
                (recvbuff + sizeof(struct iphdr));
            tunnel* recv = (tunnel*)(recvbuff + sizeof(struct iphdr)
                + sizeof(struct icmphdr));
            decrypt();
            if (filter(icmph)) {
                printf("\t%s : ", recv->sname);
                printf("%s\n", recv->data);
            }
            memset(recvbuff, 0, BUFFSIZE);
        }
    }
    while (1) {
        struct icmphdr* icmph = (struct icmphdr*)(sendbuff);
        icmph->type = ICMP_ECHO;
        char* databuff = tnel->data;
        fgets(databuff, sizeof(tnel->data), stdin);
        databuff[strcspn(databuff, "\n")] = '\0';
        if (strlen(databuff) == 0)
            continue;
        senddata(databuff);
    }

    close(sckt);
    return 0;
}