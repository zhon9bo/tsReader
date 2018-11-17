#ifndef _FIELD_H_
#define _FIELD_H_

#define FIELD_ERROR     printf
#define FIELD_WARNING   printf
#define FIELD_MESSAGE   printf

typedef struct{
    char          *name;
    unsigned int  size;
    unsigned int  value;
    unsigned char type;
    unsigned char group;
}FIELF_ELEM;

extern int Field_Elem_Insert(FIELF_ELEM *elem, unsigned int count, const char *name, unsigned int size, unsigned int value, unsigned char group);
extern int Field_Elem_Browse(FIELF_ELEM *elem, unsigned int count, unsigned char group);
#endif