#include <stdint.h>
#include <pcap.h>

five_tuple findFiveTuple(const unsigned char *packet, int protocol, int *is_from_server, int server_port)
{
    struct sockaddr_in source, dest;
    unsigned short iphdrlen;
    struct iphdr *iph = (struct iphdr *)(packet + sizeof(struct ethhdr));
    iphdrlen = iph->ihl * 4;
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;

    struct udphdr *udph = (struct udphdr *)(packet + iphdrlen + sizeof(struct ethhdr));
    five_tuple tuple;
    tuple.protocol = protocol;
    tuple.serverPort = server_port;
    int port = ntohs(udph->source);
    struct in_addr sourceIp = source.sin_addr;
    struct in_addr destIp =dest.sin_addr;
    // char *sourceIp = inet_ntoa(source.sin_addr);
    // char *destIp = inet_ntoa(dest.sin_addr);
    if (port == 443)
    {
        //* the packet from server to client
        *is_from_server=1;
        tuple.clientPort = ntohs(udph->dest);
        tuple.clientIp = destIp;
        tuple.serverIp = sourceIp;
    }
    else
    {
        //* the packet from client to server
        *is_from_server=0;
        tuple.clientPort = port;
        tuple.clientIp = sourceIp;
        tuple.serverIp = destIp;
    }
    return tuple;
}

// tuple.protocol = findProtocol();
// tuple.clientPort = findClientPort();
// tuple.serverPort = findServerPort(udph);
// tuple.clientIp = findClientIp();
// tuple.serverIp = findServerIp();

// char *findClientIp()
// {
// }

// char *findServerIp()
// {
// }

// int findClientPort()
// {
// }

// int findServerPort(struct udphdr *udph)
// {
//     return ntohs(udph->source);
// }

// int findProtocol()
// {
//     return 17;//udp
// TODO: return from configuration file
// }

// void packetHandler(u_char *userData, const struct pcap_pkthdr *pkthdr, const u_char *packet)
// {
//     const struct ether_header *ethernetHeader;
//     const struct ip *ipHeader;
//     const struct tcphdr *tcpHeader;
//     const struct udphdr *udpHeader;
//     char sourceIP[INET_ADDRSTRLEN];
//     char destIP[INET_ADDRSTRLEN];
//     u_int sourcePort, destPort;
//     u_char *data;
//     int dataLength = 0;
//     int i;
//     ethernetHeader = (struct ether_header *)packet;
//     if (ntohs(ethernetHeader->ether_type) == ETHERTYPE_IP)
//     {
//         ipHeader = (struct ip *)(packet + sizeof(struct ether_header));
//         inet_ntop(AF_INET, &(ipHeader->ip_src), sourceIP, INET_ADDRSTRLEN);
//         inet_ntop(AF_INET, &(ipHeader->ip_dst), destIP, INET_ADDRSTRLEN);
//         if (ipHeader->ip_p == IPPROTO_TCP)
//         {
//             tcpCount = tcpCount + 1;
//             tcpHeader = (struct tcphdr *)(packet + sizeof(struct ether_header) + sizeof(struct ip));
//             sourcePort = ntohs(tcpHeader->source);
//             destPort = ntohs(tcpHeader->dest);
//             data = (u_char *)(packet + sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct tcphdr));
//             dataLength = pkthdr->len - (sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct tcphdr));
//             if (sourcePort == 80 || sourcePort == 443 || destPort == 80 || destPort == 443)
//             {
//                 for (i = 0; i < httpIdx; i++)
//                 {
//                     if (strcmp(destIP, httpIP[i]) == 0)
//                     {
//                         httpCount[i] = httpCount[i] + dataLength;
//                     }
//                 }
//                 strcpy(httpIP[httpIdx], destIP);
//                 httpCount[httpIdx] = dataLength;
//                 httpIdx = httpIdx + 1;
//             }
//             if (tcpHeader->th_flags & TH_SYN)
//             {
//                 for (i = 0; i < synIdx; i++)
//                 {
//                     if (strcmp(sourceIP, synIP[i]) == 0)
//                     {
//                         synCount[i] = synCount[i] + 1;
//                     }
//                 }
//                 strcpy(synIP[synIdx], sourceIP);
//                 synCount[synIdx] = 1;
//                 synIdx = synIdx + 1;
//             }
//         }
//         else if (ipHeader->ip_p == IPPROTO_UDP)
//         {
//             udpCount = udpCount + 1;
//             udpHeader = (struct udphdr *)(packet + sizeof(struct ether_header) + sizeof(struct ip));
//             sourcePort = ntohs(udpHeader->source);
//             destPort = ntohs(udpHeader->dest);
//             if (sourcePort == 53 || destPort == 53)
//             {
//                 dnsCount = dnsCount + 1;
//             }
//         }
//         else if (ipHeader->ip_p == IPPROTO_ICMP)
//         {
//             icmpCount = icmpCount + 1;
//         }
//     }
// }