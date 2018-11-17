#ifndef _PMT_H___
#define _PMT_H___
#include "../mpegts/programs.h"

extern int parse_pmt(struct list_head *sections_head, PROGRAM *programs);
#endif