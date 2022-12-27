#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#define _STRUCTS_H_
//#define _STRUCTS_H_

typedef struct
{
    struct in_addr clientIp;
    struct in_addr serverIp;
    int protocol;
    int clientPort;
    int serverPort;
} five_tuple;

typedef struct
{
    int Conn_id;               // Connection id of the transaction
    int transaction_id;        // The ID of the transaction in the connection
    time_t start_time_seconds; // The time of the request of the transaction
    time_t start_time_u_seconds;
    unsigned int num_inbound_packets_in_range;    // Number of packets from the server to the client with size in each range
    unsigned int num_outbound_packets_in_range;   // Number of packets from the client to the server with size less than request size
    unsigned int max_packet_size_inbound;         // Max packet size in inbound direction.
    unsigned int min_packet_size_inbound;         // Min packet size in inbound direction
    double max_diff_time_inbound;        // Max diff time between two consecutive packets (size more than predefine size) in the inbound direction
    double min_diff_time_inbound;        // Min diff time between two consecutive packets (size more than predefined size) in the inbound direction
    double SumSquareInboundPacketTimeDiff; // Sum of squared diff time between two consecutive inbound packets (for variance calculation)
    double RTT;                          // Time between the request until the first packet from the server
    double time_of_arrive;
    
} transaction_struct;

typedef struct
{
    char buffer[640000];
    int size_video_packets;
    transaction_struct transaction;
} connection;