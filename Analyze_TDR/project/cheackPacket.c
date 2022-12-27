#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <pcap.h>
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define UDP_PROTOCOL 17

int isYoutubeVideoPacket(const unsigned char *packet, int server_port)
{
    struct iphdr *iph = (struct iphdr *)(packet + sizeof(struct ethhdr));
    // unsigned short iphdrlen = iph->ihl * 4;
    struct udphdr *udph = (struct udphdr *)(packet + (iph->ihl * 4) + sizeof(struct ethhdr));
    if (ntohs(udph->source) != server_port&& ntohs(udph->dest) != server_port)
        return 0;
    // protocol
    if (iph->protocol == UDP_PROTOCOL)
        return UDP_PROTOCOL;
    return 0;
}

