
#include <stdio.h>
#include <stdlib.h>
// #include "structs.h"
#include "cheackPacket.c"
// #ifndef _STRUCTS_H_
#include "hash_table.c"
// #endif
#include "transaction_functions.c"
#include <pcap.h>
#include "analyze_5_tuple.c"
#include <stdint.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include <sys/types.h>
#include "libconfig.c"
#include <libconfig.h>
#include </usr/include/libconfig.h>
#define FIRST_TRANSACTION_ID 1

FILE *logfile, *staticions;
FILE *fpt;

int main(int argc, char **argv)
{
    config = (configuration *)malloc(sizeof(configuration));

    if (get_config() == EXIT_FAILURE)
        return 0;
    connection *arr_connection = calloc(config->max_number_of_connections, sizeof(connection));
    int index_arr_connection = 0;
    int connection_id = 1;
    five_tuple five_tuple;
    int is_request_packet;
    int requests = 0;
    int max_packet_size_inbound;
    int is_from_server = 0;
    struct timeval request_time;
    const unsigned char *packet;
    struct pcap_pkthdr h;
    int videos_have_been_watched = 0;       //כמה פעמים מצאתי 5 טאפל חדשים
    int average_duration_of_the_videos = 0; // videos_have_been_watchedסכום כל אורכי הוידאו לחלק ל
    int protocol;
    char *temp_key = calloc(sizeof(five_tuple) + 32, sizeof(char));
    char *temp_transaction = calloc(sizeof(transaction_struct) + 100, sizeof(char));
    transaction_struct *transaction = (transaction_struct *)malloc(sizeof(transaction_struct));
    transaction_struct t;
    connection *connect, connection_memory, *temp_connect;
    int num_inbound_packets_in_range;
    char serverIp[18], clientIp[18];
    int size_packet;
    double arrive_time;
    int sum_of_TDR_videos = 0;
    int size_of_the_videos = 0;
    int num_videos = 0;
    int sizeof_udp_payload;
    // int num_connections = 1;
    ht *hash_table = ht_create();
    logfile = fopen("log.txt", "w");
    fpt = fopen("output.csv", "w");
    staticions = fopen("video_statistics.txt", "w");

    if (logfile == NULL || fpt == NULL || staticions == NULL)
    {
        printf("Unable to create file.");
    }
    fprintf(fpt, "connectionId, protocol, clientPort, serverPort, clientIp, serverIp, transactionId, startTime, num_inbound_packets_in_range, num_outbound_packets_in_range, max_packet_size_inbound, min_packet_size_inbound, max_diff_time_inbound, min_diff_time_inbound, SumSquareInboundPacketTimeDiff, RTT\n"); // header
    //* read pcap file
    char error_buffer[PCAP_ERRBUF_SIZE];

    pcap_t *handle = pcap_open_offline("big_capture_file.pcap", error_buffer);
    if (handle == NULL)
    {
        printf("Error opening pcap file.\n");
        exit(1);
    }
    connect = (connection *)malloc(sizeof(connection));
    packet = pcap_next(handle, &h);
    while (packet != NULL)
    {
        arrive_time = (double)h.ts.tv_sec + (double)h.ts.tv_usec / 1000000;

        sizeof_udp_payload = find_sizeof_udp_payload(packet);
        protocol = isYoutubeVideoPacket(packet, config->server_port);
        //  good packet for me
        if (protocol == UDP_PROTOCOL)
        {
            // analyze_5_tuple
            five_tuple = findFiveTuple(packet, protocol, &is_from_server, config->server_port);
            strcpy(serverIp, inet_ntoa(five_tuple.serverIp));
            strcpy(clientIp, inet_ntoa(five_tuple.clientIp));
            sprintf(temp_key, "%u%u%d", five_tuple.serverIp, five_tuple.clientIp, five_tuple.clientPort);
            fprintf(logfile, "\n%s\n", temp_key);
            // sprintf(temp_key, "%d %d %d %s %s", five_tuple.protocol, five_tuple.clientPort, five_tuple.serverPort, clientIp, serverIp);
            //*check if exists in hash table
            connect = (connection *)ht_get(hash_table, temp_key);
            is_request_packet = isRequestPacket(is_from_server, packet, config->request_packet_threshold, sizeof_udp_payload);
            //* if contol packet
            if (is_from_server && (sizeof_udp_payload < config->inbound_packets_in_range_min || sizeof_udp_payload > config->inbound_packets_in_range_max))
            {
                packet = pcap_next(handle, &h);
                continue;
            }
            // can be happen?
            if (!is_from_server && sizeof_udp_payload < config->outbound_packets_in_range_min)
            {
                packet = pcap_next(handle, &h);
                continue;
            }
            //*exists in hash table
            if (connect != (void *)NULL)
            {
                //*time out 20 seconds
                if ((arrive_time - connect->transaction.time_of_arrive) > config->video_connection_timeout)
                {
                    //* check minimum connection size
                    //* enough
                    if (connect->size_video_packets >= config->Minimum_video_connection_size)
                    {
                        //* write to csv file from buffer
                        size_of_the_videos += connect->size_video_packets;
                        num_videos++;
                        videos_have_been_watched++;
                        fprintf(fpt, "%s", connect->buffer);
                    }

                    memset(connect->buffer, 0, 64000);
                    connect->size_video_packets = 0;

                    //* dont enough and enough
                    //* - open new connection - ovveride the prev transaction
                    if (is_request_packet)
                    {
                        requests++;
                        sum_of_TDR_videos++;
                        t = setNewTransaction(connection_id++, h.ts.tv_sec, h.ts.tv_usec, 1, arrive_time);
                        connect->transaction = t;
                        connect->size_video_packets = 0;
                        ht_set(hash_table, temp_key, (void *)connect);
                    }
                    else
                    {
                        //צריך לחכות עד שיהיה ריקווסט - בינתיים ננקה את הטרנזקציה שהייתה כדי שלא יהיו ערכים מוטעים בפעם הבאה
                        ht_set(hash_table, temp_key, (void *)NULL);
                    }
                }
                //*time is oke
                else
                {
                    //*request packet
                    if (is_request_packet)
                    {
                        requests++;
                        sum_of_TDR_videos++;
                        //*close prev transaction - write to buffer
                        sprintf(connect->buffer, "%s %d, %d, %d, %d, %s, %s, %d, %d.%d, %d, %d, %d, %d, %f, %f, %f, %f\n", connect->buffer, connect->transaction.Conn_id, five_tuple.protocol, five_tuple.clientPort, five_tuple.serverPort, clientIp, serverIp, connect->transaction.transaction_id, (int)connect->transaction.start_time_seconds, (int)connect->transaction.start_time_u_seconds, connect->transaction.num_inbound_packets_in_range, connect->transaction.num_outbound_packets_in_range, connect->transaction.max_packet_size_inbound, connect->transaction.min_packet_size_inbound < INT32_MAX ? connect->transaction.min_packet_size_inbound : 0, connect->transaction.max_diff_time_inbound, connect->transaction.min_diff_time_inbound == 1.7976931348623157E+308 ? 0 : connect->transaction.min_diff_time_inbound, (double)connect->transaction.SumSquareInboundPacketTimeDiff, (double)connect->transaction.RTT);
                        if (connect->size_video_packets >= config->Minimum_video_connection_size)
                        {
                            fprintf(fpt, "%s", connect->buffer);
                            strcpy(connect->buffer, "");
                        }

                        //* open new transaction
                        t = setNewTransaction(connect->transaction.Conn_id, h.ts.tv_sec, h.ts.tv_usec, connect->transaction.transaction_id + 1, arrive_time);
                        connect->transaction = t;
                        ht_set(hash_table, temp_key, (void *)connect);
                        fprintf(logfile, "set: %d\n", connect->transaction.Conn_id);
                        temp_connect = (connection *)ht_get(hash_table, temp_key);
                        fprintf(logfile, "get: %d\n", temp_connect->transaction.Conn_id);
                    }

                    //*not request packet
                    else
                    {
                        //* update current transaction
                        t = setExistTransaction(connect->transaction, is_from_server, sizeof_udp_payload, arrive_time);
                        connect->transaction = t;
                        if (is_from_server)
                        {
                            connect->size_video_packets += sizeof_udp_payload;
                        }
                        // connect->size_video_packets += size_packet;
                        ht_set(hash_table, temp_key, (void *)connect);
                    }
                }
            }
            //*dont exists in hash table
            else
            {
                if (is_request_packet)
                {
                    requests++;
                    videos_have_been_watched++;
                    sum_of_TDR_videos++;
                    //* get memory for connection
                    connection_memory = arr_connection[index_arr_connection++];
                    connection_memory.transaction = setNewTransaction(connection_id++, h.ts.tv_sec, h.ts.tv_usec, 1, arrive_time);
                    connection_memory.size_video_packets = 0;
                    ht_set(hash_table, temp_key, (void *)&connection_memory);
                }
            }
        }

        packet = pcap_next(handle, &h);
    }
    // write to csv all transaction that dont saved
    for (int i = 0; i < index_arr_connection; i++)
    {
        connection_memory = arr_connection[i];
        if (connection_memory.size_video_packets >= config->Minimum_video_connection_size)
        {
            //* write to csv file from buffer
            fprintf(fpt, "%s", connection_memory.buffer);
            videos_have_been_watched++;
            // connection_id++;
            // memset(connect->buffer, 0, 64000);
        }
        else
            videos_have_been_watched--;
        // fprintf(fpt, "%s", arr_connection[i].buffer);
    }
    fprintf(staticions, "number of requests: %d\n", requests);
    fprintf(staticions, "number of connections: %d\n", connection_id);
    fprintf(staticions, "videos have been watched: %d\n", videos_have_been_watched);
    fprintf(staticions, "index_arr_connection: %d\n", index_arr_connection);
    // fprintf(staticions, "Average number of TDRs per video: %d\n", sum_of_TDR_videos/num_videos);

    free(hash_table);
    free(arr_connection);
    free(temp_key);
    fclose(logfile);
    fclose(fpt);
}