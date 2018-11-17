#include <stdio.h>
#include <stdlib.h>
#include "programs.h"
#include "../opration.h"

int DESC_Del_All(DESC *desc)
{
    if(desc != NULL)
    {
        DESC_Del_All(desc->next);
        if(desc->descriptor_data != NULL)
            free(desc->descriptor_data);
        free(desc);
    }
    return 0;
}

DESC *DESC_New(unsigned int descriptor_tag, unsigned int descriptor_length)
{
    DESC *desc;

    desc = malloc(sizeof(DESC));
    if(desc == NULL)
    {
        printf("malloc desc faild\n");
        return NULL;
    }

    desc->descriptor_tag    = descriptor_tag;
    desc->descriptor_length = descriptor_length;
    desc->descriptor_data   = NULL;
    desc->next              = NULL;

    if(descriptor_length > 0)
    {
        desc->descriptor_data = malloc(descriptor_length);
        if(desc->descriptor_data == NULL)
        {
            printf("malloc descriptor_data faild\n");
            free(desc);
            return NULL;
        }
    }
    
    return desc;
}

int DESC_Add(DESC **descs, DESC *desc_new)
{
    DESC **desc;

    desc = descs;
    while(*desc != NULL)
    {
        desc = &((*desc)->next);
    }

    *desc = desc_new;

    return 0;
}

int DESC_Show(DESC *desc)
{
    if(desc != NULL)
    {
        printf("descriptor_tag:%#X(%d)\ndescriptor_length:%#X(%d)",\
        desc->descriptor_tag,desc->descriptor_tag,desc->descriptor_length,desc->descriptor_length);
        printbufhex(desc->descriptor_data,desc->descriptor_length,16);
        DESC_Show(desc->next);
    }
}

int ELEM_STREM_Del_All(ELEM_STREM *elem_stream)
{
    if(elem_stream != NULL)
    {
        ELEM_STREM_Del_All(elem_stream->next);
        DESC_Del_All(elem_stream->es_desc);
        free(elem_stream);
    }

    return 0;
}

ELEM_STREM *ELEM_STREM_New(unsigned int stream_type, unsigned int elem_pid, unsigned int es_info_length)
{
    ELEM_STREM *elem_stream;

    elem_stream = malloc(sizeof(ELEM_STREM));
    if(elem_stream == NULL)
    {
        printf("malloc elem stream faild\n");
        return NULL;
    }

    elem_stream->stream_type    = stream_type;
    elem_stream->elem_pid       = elem_pid;
    elem_stream->es_info_length = es_info_length;
    elem_stream->es_desc        = NULL;
    elem_stream->next           = NULL;

    return elem_stream;
}

ELEM_STREM *ELEM_STREM_FindBy_PID(ELEM_STREM *elem_stream, unsigned int elem_pid)
{
    if(elem_stream != NULL)
    {
        if(elem_stream->elem_pid == elem_pid)
            return elem_stream;
        else
            return ELEM_STREM_FindBy_PID(elem_stream->next, elem_pid);
    }
    return NULL;
}

int ELEM_STREM_Add(ELEM_STREM **elem_streams, ELEM_STREM *elem_stream_new)
{
    ELEM_STREM **elem_stream;


    elem_stream = elem_streams;
    while(*elem_stream != NULL)
    {
        elem_stream = &((*elem_stream)->next);
    }

    *elem_stream = elem_stream_new;

    return 0;
}

int ELEM_STREM_Show(ELEM_STREM *elem_stream)
{
    if(elem_stream != NULL)
    {
        printf("stream type:%#X(%d)\nelem pid:%#X(%d)\n",elem_stream->stream_type,elem_stream->stream_type,elem_stream->elem_pid,elem_stream->elem_pid);
        DESC_Show(elem_stream->es_desc);
        return ELEM_STREM_Show(elem_stream->next);
    }
    return 0;
}

int PROGRAM_Del_All(PROGRAM *program)
{
    if(program != NULL)
    {
        PROGRAM_Del_All(program->next);
        DESC_Del_All(program->program_desc);
        ELEM_STREM_Del_All(program->elem_streams);
        DESC_Del_All(program->service_descriptor);
        free(program);
    }
    
    return 0;
}

PROGRAM *PROGRAM_New(unsigned int program_number, unsigned int map_pid)
{
    PROGRAM *program;

    if(program_number == 0 || map_pid == 0)//
    {
        return NULL;
    }

    program = malloc(sizeof(PROGRAM));
    if(program == NULL)
    {
        printf("为program %d分配的空间出错\n",program_number);
        return NULL;
    }

    program->program_number = program_number;
    program->pmt_pid        = map_pid;
    program->next           = NULL;

    return program;
}

int PROGRAM_Add(PROGRAM **programs, PROGRAM *program_new)
{
    PROGRAM **program;

    program = programs;
    while(*program != NULL)
    {
        program = &((*program)->next);
    }

    *program = program_new;

    return 0;
}

PROGRAM *PROGRAM_FindBy_PrgNumber(PROGRAM *program, unsigned int program_number)
{
    if(program != NULL)
    {
        if(program->program_number == program_number)
            return program;
        else
            return PROGRAM_FindBy_PrgNumber(program->next, program_number);
    }

    return NULL;    
}


int PROGRAM_Show(PROGRAM *program)
{
    if(program != NULL)
    {
        printf("\n节目[%#X(%d)]    PID:%#X(%d)\n",program->program_number,program->program_number,program->pmt_pid,program->pmt_pid);
        DESC_Show(program->program_desc);
        DESC_Show(program->service_descriptor);
        ELEM_STREM_Show(program->elem_streams);
        
        return PROGRAM_Show(program->next);
    }

    return 0;
}




