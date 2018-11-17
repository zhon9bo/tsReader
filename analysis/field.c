#include "field.h"

int Field_Elem_Insert(FIELF_ELEM *elem, unsigned int count, const char *name, unsigned int size, unsigned int value, unsigned char group)
{
    if(elem == NULL || count == 0 || name == NULL || bit_size == 0)
    {
        FIELD_ERROR("[Error] Inset a invalid element!\n");
        return -1;
    }

    while((elem->name != NULL) && count--)
    {
        elem++;
    }

    if(count == 0)
    {
        FIELD_ERROR("[Error] Not enough space to insert element\n");
        return -1;
    }
    else
    {
        FIELD_MESSAGE("[Message] Remain %d elements!\n");
        elem->name  = name;
        elem->value = value;
        elem->group = group;
        elem->size  = size;
        return 0;
    }

}

#define TYPE_LIST 1

#define GOURP_ELEM(size) struct{\
    FIELF_ELEM elem[size];\
    struct list_head list;\
}

int Field_Elem_Browse(FIELF_ELEM *elem, unsigned int count, unsigned char group)
{
    int i;
    printf("%10s\t%s\t%s\n","name","size","value");
    while((elem->name != NULL) && count--)
    {
        if(elem->type == TYPE_LIST)
        {    
            struct list_head *list = elem->value;
            GOURP_ELEM(elem->size) *group_elem;
            list_for_each_entry(group_elem, list, list)
            {
                for(i = 0; i < sizeof(group_elem->elem); i++)
                {
                    group_elem->elem[i];
                    printf("[%10s]\t%2d\t%#x(%d)\n",group_elem->elem[i]->name,group_elem->elem[i]->size,group_elem->elem[i]->value);
                }
            }
        }
        else   
            printf("[%10s]\t%2d\t%#x(%d)\n",elem->name,elem->size,elem->value);
        elem++;
    }
    return 0;
}