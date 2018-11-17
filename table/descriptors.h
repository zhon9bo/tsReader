#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

#include "../mpegts/programs.h"

#define descriptor_tag(cur_service_desc)                        cur_service_desc[0]
#define descriptor_length(cur_service_desc)                     cur_service_desc[1]
#define service_type(cur_service_desc)                          cur_service_desc[2]
#define service_provider_name_length(cur_service_desc)          cur_service_desc[3]
#define SERVICE_DESC_PROVIDERCHAR_HEAD(cur_service_desc)        &cur_service_desc[4]
#define service_name_length(cur_service_desc)                   cur_service_desc[4+service_provider_name_length(cur_service_desc)]
#define SERVICE_DESC_NAMECHAR_HEAD(cur_service_desc)            &cur_service_desc[5+service_provider_name_length(cur_service_desc)]

typedef struct{
    unsigned char descriptor_tag;
    unsigned char descriptor_length;
    unsigned char service_type;
    unsigned char service_provider_name_length;
    unsigned char *service_provider;
    unsigned char service_name_length;
    unsigned char *service_name;
}SERVICE_DESC;

extern int parse_service_descriptor(unsigned char *service_descriptor, SERVICE_DESC *service_desc);
extern int parse_descriptor(DESC **desc, unsigned char *buf, unsigned int length);
#endif