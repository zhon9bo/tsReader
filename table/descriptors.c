#include "descriptors.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../mpegts/programs.h"

int parse_service_descriptor(unsigned char *service_descriptor, SERVICE_DESC *service_desc)
{

    service_desc->descriptor_tag    = descriptor_tag(service_descriptor);
    service_desc->descriptor_length = descriptor_length(service_descriptor);
    if(service_desc->descriptor_tag != 0x48)
        goto not_service_descriptor;
    service_desc->service_type      = service_type(service_descriptor);

    service_desc->service_provider_name_length = service_provider_name_length(service_descriptor);
    service_desc->service_provider = malloc(service_desc->descriptor_length);
    memcpy(service_desc->service_provider,SERVICE_DESC_PROVIDERCHAR_HEAD(service_descriptor),(unsigned int)service_desc->service_provider_name_length);
    
    service_desc->service_name_length = service_name_length(service_descriptor);
    service_desc->service_name = malloc(service_desc->service_name_length);
    memcpy(service_desc->service_name,SERVICE_DESC_NAMECHAR_HEAD(service_descriptor),(unsigned int)service_desc->service_name_length);

    printf("\tservice_provider: %s\n",service_desc->service_provider);
    printf("\tservice_name: %s\n",service_desc->service_name);

not_service_descriptor:
    return (service_desc->descriptor_length + 2);
}


int parse_descriptor(DESC **desc, unsigned char *buf, unsigned int length)
{
    DESC *desc_new;
    unsigned int pos = 0;
    unsigned int descriptor_tag;
    unsigned int descriptor_length;
    int buf_remain;

    while(pos<length)
    {
        descriptor_tag    = buf[pos];
        descriptor_length = buf[pos+1];

        desc_new = DESC_New(descriptor_tag,descriptor_length);

        if(desc_new != NULL)
        {
            
            buf_remain = length - (pos+2);

            if(descriptor_length != 0 && buf_remain >= descriptor_length)
                memcpy(desc_new->descriptor_data, &buf[pos+2], descriptor_length);
            else if(descriptor_length != 0 && buf_remain < descriptor_length)
            {
                printf("需要的长度 > 描述符剩下的长度\n");
                memcpy(desc_new->descriptor_data, &buf[pos+2], buf_remain);
            }

            DESC_Add(desc, desc_new);     
        }
        pos += 2 + descriptor_length;   
    }
}