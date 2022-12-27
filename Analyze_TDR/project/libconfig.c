#include "libconfig.h"

int get_config()
{
    config_t cfg;
    config_setting_t *setting;

    config_init(&cfg);

    /* Read the file. If there is an error, report it and exit. */
    if (!config_read_file(&cfg, "ini_file.ini"))
    {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        return (EXIT_FAILURE);
    }

    if (!config_lookup_int(&cfg, "request_packet_threshold", &config->request_packet_threshold))
        // printf("request_packet_threshold: %d\n\n", config->request_packet_threshold);
        // else
        fprintf(stderr, "No 'request_packet_threshold' setting in configuration file.\n");
    if (!config_lookup_int(&cfg, "Minimum_video_connection_size", &config->Minimum_video_connection_size))
        // printf("Minimum_video_connection_size: %d\n\n", config->Minimum_video_connection_size);
        // else
        fprintf(stderr, "No 'Minimum_video_connection_size' setting in configuration file.\n");
    if (!config_lookup_int(&cfg, "inbound_packets_in_range_min", &config->inbound_packets_in_range_min))
        // printf("inbound_packets_in_range_min: %d\n\n", config->inbound_packets_in_range_min);
        // else
        fprintf(stderr, "No 'inbound_packets_in_range_min' setting in configuration file.\n");
    if (!config_lookup_int(&cfg, "inbound_packets_in_range_max", &config->inbound_packets_in_range_max))
        // printf("inbound_packets_in_range_max: %d\n\n", config->inbound_packets_in_range_max);
        // else
        fprintf(stderr, "No 'inbound_packets_in_range_max' setting in configuration file.\n");
    if (!config_lookup_int(&cfg, "outbound_packets_in_range_min", &config->outbound_packets_in_range_min))
        // printf("outbound_packets_in_range_min: %d\n\n", config->outbound_packets_in_range_min);
        // else
        fprintf(stderr, "No 'outbound_packets_in_range_min' setting in configuration file.\n");
    if (!config_lookup_int(&cfg, "outbound_packets_in_range_max", &config->outbound_packets_in_range_max))
        // printf("outbound_packets_in_range_max: %d\n\n", config->outbound_packets_in_range_max);
        // else
        fprintf(stderr, "No 'max_diff_time_inbound_threshold' setting in configuration file.\n");
    if (!config_lookup_int(&cfg, "max_diff_time_inbound_threshold", &config->max_diff_time_inbound_threshold))
        // printf("max_diff_time_inbound_threshold: %d\n\n", config->max_diff_time_inbound_threshold);
        // else
        fprintf(stderr, "No 'min_diff_time_inbound_threshold' setting in configuration file.\n");
    if (!config_lookup_int(&cfg, "min_diff_time_inbound_threshold", &config->min_diff_time_inbound_threshold))
        // printf("min_diff_time_inbound_threshold: %d\n\n", config->min_diff_time_inbound_threshold);
        // else
        fprintf(stderr, "No 'number_of_videos_to_output_statistics_per_video' setting in configuration file.\n");
    if (!config_lookup_int(&cfg, "number_of_videos_to_output_statistics_per_video", &config->number_of_videos_to_output_statistics_per_video))
        // printf("number_of_videos_to_output_statistics_per_video: %d\n\n", config->number_of_videos_to_output_statistics_per_video);
        // else
        fprintf(stderr, "No 'max_number_of_connections' setting in configuration file.\n");
    if (!config_lookup_int(&cfg, "max_number_of_connections", &config->max_number_of_connections))
        // printf("max_number_of_connections: %d\n\n", config->max_number_of_connections);
        // else
        fprintf(stderr, "No 'max_number_of_transaction_per_video' setting in configuration file.\n");
    if (!config_lookup_int(&cfg, "max_number_of_transaction_per_video", &config->max_number_of_transaction_per_video))
        // printf("max_number_of_transaction_per_video: %d\n\n", config->max_number_of_transaction_per_video);
        // else
        fprintf(stderr, "No 'video_connection_timeout' setting in configuration file.\n");
    if (!config_lookup_int(&cfg, "video_connection_timeout", &config->video_connection_timeout))
        // printf("video_connection_timeout: %d\n\n", config->video_connection_timeout);
        // else
        fprintf(stderr, "No 'server_port' setting in configuration file.\n");
    if (!config_lookup_int(&cfg, "server_port", &config->server_port))
        // printf("server_port: %d\n\n", config->server_port);
        // else
        fprintf(stderr, "No 'server_port' setting in configuration file.\n");
    if (!config_lookup_int(&cfg, "udp_protocol", &config->udp_protocol))
        // printf("udp_protocol: %d\n\n", config->udp_protocol);
        // else
        fprintf(stderr, "No 'udp_protocol' setting in configuration file.\n");
    if (!config_lookup_int(&cfg, "ethernet_header_length", &config->ethernet_header_length))
        // printf("ethernet_header_length: %d\n\n", config->ethernet_header_length);
        // else
        fprintf(stderr, "No 'ethernet_header_length' setting in configuration file.\n");
    config_destroy(&cfg);

    return (EXIT_SUCCESS);
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// static int handler(void *user, const char *section, const char *name, const char *value)
// {
//     configuration *pconfig = (configuration *)user;

// #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
//     if (MATCH("from_file", "request_packet_threshold"))
//     {
//         pconfig->request_packet_threshold = atoi(value);
//     }
//     else if (MATCH("from_file", "Minimum_video_connection_size"))
//     {
//         pconfig->Minimum_video_connection_size = atoi(value);
//     }
//     else if (MATCH("from_file", "inbound_packets_in_range_min"))
//     {
//         pconfig->inbound_packets_in_range_min = atoi(value);
//     }
//     else if (MATCH("from_file", "inbound_packets_in_range_max"))
//     {
//         pconfig->inbound_packets_in_range_max = atoi(value);
//     }
//     else if (MATCH("from_file", "outbound_packets_in_range_min"))
//     {
//         pconfig->outbound_packets_in_range_min = atoi(value);
//     }
//     else if (MATCH("from_file", "outbound_packets_in_range_max"))
//     {
//         pconfig->outbound_packets_in_range_max = atoi(value);
//     }
//     else if (MATCH("from_file", "max_diff_time_inbound_threshold"))
//     {
//         pconfig->max_diff_time_inbound_threshold = atoi(value);
//     }
//     else if (MATCH("from_file", "min_diff_time_inbound_threshold"))
//     {
//         pconfig->min_diff_time_inbound_threshold = atoi(value);
//     }
//     else if (MATCH("from_file", "number_of_videos_to_output_statistics_per_video"))
//     {
//         pconfig->number_of_videos_to_output_statistics_per_video = atoi(value);
//     }
//     else if (MATCH("from_file", "max_number_of_connections"))
//     {
//         pconfig->max_number_of_connections = atoi(value);
//     }
//     else if (MATCH("from_file", "max_number_of_transaction_per_video"))
//     {
//         pconfig->max_number_of_transaction_per_video = atoi(value);
//     }
//     else if (MATCH("from_file", "video_connection_timeout"))
//     {
//         pconfig->video_connection_timeout = atoi(value);
//     }
//     else if (MATCH("from_me", "protocol_udp_number"))
//     {
//         pconfig->protocol_udp_number = atoi(value);
//     }
//     else if (MATCH("from_me", "server_port_video"))
//     {
//         pconfig->server_port_video = atoi(value);
//     }
//     else if (MATCH("from_me", "ethernet_header_length"))
//     {
//         pconfig->ethernet_header_length = atoi(value);
//     }
//     else
//     {
//         return 0; /* unknown section/name, error */
//     }
//     return 1;
// }

// int readFromIniFile()
// {
//     configuration config;

//     if (ini_parse("ini_file.ini", handler, &config) < 0)
//     {
//         printf("Can't load 'ini_file.ini'\n");
//         return 1;
//     }
//     // printf("Config loaded from 'ini_file.ini': version=%d, name=%s, email=%s\n",
//     //        config.version, config.name, config.email);
//     return 0;
// }