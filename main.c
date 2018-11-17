#include <stdio.h>
#include <string.h>
#include "opration.h"
#include "table/pat.h"
#include "table/pmt.h"
#include "table/sdt.h"
#include "input/input.h"
#include "table/table.h"
#include "mpegts/programs.h"

int main()
{
	struct list_head section_list;
	CHANNEL channel;
	PROGRAM *program;
	int i = 0;

	INPUT_Init();
	INPUT_AllChannel_Init();
	
	TABLE_Packets2Section("tsfile", 1, 0, 0, &section_list);
	//memset(&channel, 0, sizeof(CHANNEL));
	channel.programs = NULL;
	parse_pat(&section_list,&channel);
	del_sections(&section_list);

	TABLE_Packets2Section("tsfile", 1, 0x11, 0x42, &section_list);
	parse_sdt(&section_list,&channel);
	del_sections(&section_list);

	program = channel.programs;
	while(program != NULL)
	{
		i++;
		printf("\n节目[%#X(%d)]    PID:%#X(%d)\n",program->program_number,program->program_number,program->pmt_pid,program->pmt_pid);
		TABLE_Packets2Section("tsfile", 1, program->pmt_pid, 0x2, &section_list);
		parse_pmt(&section_list,program);
		del_sections(&section_list);

		program = program->next;
	}

	PROGRAM_Show(channel.programs);
	
	return 0;

}
