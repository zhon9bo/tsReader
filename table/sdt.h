#ifndef _SDT_H_
#define _SDT_H_
#include "descriptors.h"
#include "../mpegts/programs.h"

extern int parse_sdt(struct list_head *sections_head, CHANNEL *channel);
#endif