#ifndef _PAT_H___
#define _PAT_H___

#include "../mpegts/programs.h"
#include "../list/list.h"

extern int parse_pat(struct list_head *sections_head, CHANNEL *channel);
#endif