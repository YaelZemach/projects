#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>

typedef struct
{
    int request_packet_threshold;
    int Minimum_video_connection_size;
    int inbound_packets_in_range_min;
    int inbound_packets_in_range_max;
    int outbound_packets_in_range_min;
    int outbound_packets_in_range_max;
    int max_diff_time_inbound_threshold;
    int min_diff_time_inbound_threshold;
    int number_of_videos_to_output_statistics_per_video;
    int max_number_of_connections;
    int max_number_of_transaction_per_video;
    int video_connection_timeout;
    int protocol_udp_number;
    int server_port_video;
    int ethernet_header_length;
    int server_port;
    int udp_protocol;
} configuration;

configuration *config;

int get_config();