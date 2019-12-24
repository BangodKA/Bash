all:
	gcc -Wall int.c 0_general/proc_manager.c 0_general/functions.c 1_first_step/reallocs.c 1_first_step/command_prompt.c 1_first_step/get_command.c 2_second_step/process_command.c 2_second_step/cd_command.c 4_fourth_step/open_create_files.c 5_fifth_step/pipeline.c -o int 
	./int