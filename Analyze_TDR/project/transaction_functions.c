#include <pcap.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/udp.h>      //Provides declarations for udp header
#include <netinet/ip.h>       //Provides declarations for ip header
#include <netinet/if_ether.h> //For ETH_P_ALL
#include <net/ethernet.h>
#ifndef _STRUCTS_H_
#include "structs.h"
#endif
#define MAX_DOUBLE 1.7976931348623157E+308
#define IP_HL(ip) (((ip)->ip_vhl) & 0x0f)

int find_sizeof_udp_payload(const char *packet)
{
    struct udphdr *u_hdr;
    u_hdr = (struct udphdr *)(packet + sizeof(struct ether_header) + sizeof(struct ip));
    int size = ntohs(u_hdr->len);
    return size;
}

int isRequestPacket(int is_from_server, const char *packet, int request_packet_threshold, int sizeof_udp_payload)
{
    // return 1 if isRequestPacket , if not return 0
    if (!is_from_server && sizeof_udp_payload >= request_packet_threshold)
    {
        return 1;
    }
    return 0;
}

transaction_struct setNewTransaction(int connection_id, time_t time_of_request_s, suseconds_t time_of_request_ms, int transaction_id, double time_of_arrive)
{
    transaction_struct transaction;
    transaction.Conn_id = connection_id;
    transaction.transaction_id = transaction_id; 
    transaction.start_time_seconds = time_of_request_s;
    transaction.start_time_u_seconds = time_of_request_ms;
    transaction.num_outbound_packets_in_range = 1;
    transaction.num_inbound_packets_in_range = 0;
    transaction.max_packet_size_inbound = 0;
    transaction.min_packet_size_inbound = UINT32_MAX;
    transaction.max_diff_time_inbound = 0;
    transaction.min_diff_time_inbound = MAX_DOUBLE;
    transaction.SumSquareInboundPacketTimeDiff = 0;
    transaction.time_of_arrive = time_of_arrive;
    transaction.RTT = 0;
    return transaction;
}

transaction_struct setExistTransaction(transaction_struct tr, int is_from_server, bpf_u_int32 packet_size, double time_of_arrive_current)
{
    transaction_struct transaction;
    transaction.time_of_arrive = tr.time_of_arrive;
    double diff_time = time_of_arrive_current - tr.time_of_arrive;
    transaction.Conn_id = tr.Conn_id;
    transaction.transaction_id = tr.transaction_id;
    transaction.start_time_seconds = tr.start_time_seconds;
    transaction.start_time_u_seconds = tr.start_time_u_seconds;
    transaction.SumSquareInboundPacketTimeDiff = tr.SumSquareInboundPacketTimeDiff + diff_time * diff_time;
    if (is_from_server)
    {
        if (diff_time > tr.max_diff_time_inbound)
        {
            transaction.max_diff_time_inbound = diff_time;
            transaction.max_diff_time_inbound = tr.min_diff_time_inbound;
        }
        else
        {
            if (diff_time < tr.min_diff_time_inbound)
            {
                transaction.max_diff_time_inbound = tr.max_diff_time_inbound;
                transaction.min_diff_time_inbound = diff_time;
            }
            else
            {
                transaction.max_diff_time_inbound = tr.max_diff_time_inbound;
                transaction.min_diff_time_inbound = tr.min_diff_time_inbound;
            }
        }
        transaction.num_outbound_packets_in_range = tr.num_outbound_packets_in_range;
        transaction.num_inbound_packets_in_range = (tr.num_inbound_packets_in_range) + 1;
        if (tr.num_inbound_packets_in_range == 0)
        {
            transaction.RTT = time_of_arrive_current - (double)tr.start_time_seconds + (double)tr.start_time_u_seconds / 1000000;
            transaction.min_packet_size_inbound = packet_size;
            transaction.max_packet_size_inbound = packet_size;
            transaction.max_diff_time_inbound = diff_time;
            transaction.max_diff_time_inbound = diff_time;
        }
        else
        {
            transaction.RTT = tr.RTT;
            if (tr.max_packet_size_inbound < packet_size)
            {
                transaction.max_packet_size_inbound = packet_size;
                transaction.min_packet_size_inbound = tr.min_packet_size_inbound;
            }
            else
            {
                if (tr.min_packet_size_inbound > packet_size)
                {
                    transaction.min_packet_size_inbound = packet_size;
                    transaction.max_packet_size_inbound = tr.max_packet_size_inbound;
                }
                else
                {
                    transaction.max_packet_size_inbound = tr.max_packet_size_inbound;
                    transaction.min_packet_size_inbound = tr.min_packet_size_inbound;
                }
            }
        }
    }
    else
    {
        transaction.max_diff_time_inbound = tr.max_diff_time_inbound;
        transaction.min_diff_time_inbound = tr.min_diff_time_inbound;
        transaction.num_outbound_packets_in_range = (tr.num_outbound_packets_in_range) + 1;
        transaction.max_packet_size_inbound = tr.max_packet_size_inbound;
        transaction.min_packet_size_inbound = tr.min_packet_size_inbound;
        transaction.num_inbound_packets_in_range = tr.num_inbound_packets_in_range;
        transaction.RTT = tr.RTT;
    }
    return transaction;
}
