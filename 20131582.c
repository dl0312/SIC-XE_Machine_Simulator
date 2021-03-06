#include "20131582.h"

/*
 * Function: initList
 * 
 * allocate and initialize new list.
 * 
 * return: the address of linked list
 */
LinkedList* initList(void){
	LinkedList *L = (LinkedList *)malloc(sizeof(LinkedList));
	L -> head = NULL;
	L -> size = 0;
	return L;
}

/*
 * Function: appendList
 * 
 * append new element to linked list.
 * 
 * L: the address of linked list
 * new_data: the data of node
 */
void appendList(LinkedList* L, void* new_data) {
	Node *ptr = NULL;
	if (L -> head == NULL) {
		L -> head = (Node*)malloc(sizeof(Node));
		L -> head -> data = new_data;
		L -> head -> link = NULL;
	} else {
		for (ptr = L -> head; ptr -> link != NULL; ptr = ptr -> link);
		ptr -> link = (Node*)malloc(sizeof(Node));
		ptr -> link -> data = new_data;
		ptr -> link -> link = NULL;
	}
	(L -> size)++;
}

/*
 * Function: printHistory
 * 
 * print each nodes on history linked list.
 * 
 * history: history linked list
 */
void printHistory(LinkedList* history)
{
	Node* ptr = NULL;
	int cnt = 0;
	for (ptr = history -> head; ptr != NULL; ptr = ptr -> link) {
		printf("%d\t%s\n", ++cnt, (char*)ptr -> data);
	}
}

/*
 * Function: printExternalSymbolTable
 * 
 * print each nodes on history linked list.
 * 
 * history: history linked list
 */
void printExternalSymbolTable(LinkedList* estab)
{
	Node* ptr = NULL;
	int total_length = 0;
	printf("control\t\tsymbol\t\taddress\t\tlength\n");
	printf("section\t\tname\n");
	printf("--------------------------------------------------------------\n");
	for (ptr = estab -> head; ptr != NULL; ptr = ptr -> link) {
		ExternalSymbol* es_data = ((ExternalSymbol*)(ptr->data));
		if(es_data->is_symbol == 1){
			// symbol
			printf("\t\t%s\t\t%04X\n", es_data->name, es_data->address);
		} else {
			// control section
			printf("%s\t\t\t\t%04X\t\t%04X\n", es_data->name, es_data->address, es_data->length);
			total_length += es_data->length;
		}
	}
	printf("--------------------------------------------------------------\n");
	printf("\t\t\t\ttotal length\t%04X\n", total_length);

}

/*
 * Function: processCmd
 * 
 * process command.
 * 
 * input: the pointer of input string
 * return: result of processing
 * (0: right command, 1: wrong command, -1: quit) 
 */
int processCmd(char* input){ 
	char *cmd = NULL, *tmp_input = NULL, *arg1 = NULL, *arg2 = NULL, *arg3 = NULL, *tmp_arg = NULL;
	int len = 0, flag = 0, wrong_input_flag = 0;
	LinkedList *object_lst = NULL;
	
	if(history == NULL) history = initList();
	len = strlen(input);
	tmp_input = (char*)malloc(sizeof(char) * len);
	strcpy(tmp_input, input);

	// trim tmp input	
	tmp_input[strcspn(tmp_input, "\n")] = '\0';
	
	// flag for input state
	cmd = input;
	
	for (int i = 0; i < len; i++) {
		if (flag == 0) {
			// input cmd
			if (input[i] > 96 && input[i] < 123) {
				continue;
			} else if (input[i] == ' ' || input[i] == '\t') {
				input[i] = '\0';
				flag++;
				continue;
			} else if(input[i] == '\n'){
				input[i] = '\0';
				break;
			}
			else {
				wrong_input_flag = 1;
				break;
			}
		}
		if(strcmp(cmd, "loader") == 0){
			if (flag == 1){
				// empty space
				if (input[i] == ' ' || input[i] == '\t') {
					continue;
				} else if(input[i] == '\n'){
					input[i] = '\0';
					break;
				} else {
					arg1 = &input[i];
					flag++;
					continue;
				} 
			} 
			else if (flag == 2){
				// input arg1
				if (input[i] == ' ' || input[i] == '\t') {
					input[i] = '\0';
					flag++;
					continue;
				} else if (input[i] == '\n'){
					input[i] = '\0';
					break;
				} 
			}
			else if (flag == 3) {
				// empty space
				if (input[i] == ' ' || input[i] == '\t') {
					continue;
				} else if(input[i] == '\n'){
					input[i] = '\0';
					break;
				} else {
					arg2 = &input[i];
					flag++;
					continue;
				}
			}
			else if (flag == 4) {
				// input arg2
				if (input[i] == ' ' || input[i] == '\t') {
					input[i] = '\0';
					flag++;
					continue;
				} else if(input[i] == '\n'){
					input[i] = '\0';
					break;
				}
			}
			else if (flag == 5) {
				// empty space
				if (input[i] == ' ' || input[i] == '\t') {
					continue;
				} else if(input[i] == '\n'){
					input[i] = '\0';
					break;
				} else {
					arg3 = &input[i];
					flag++;
					continue;
				} 
			}
			else if (flag == 6) {
				// input arg3
				if (input[i] == ' ' || input[i] == '\t' || input[i] == '\n') {
					input[i] = '\0';
					flag++;
					continue;
				}
			}
		} else {
			
			if (flag == 1){
				// empty space
				if (input[i] == ' ' || input[i] == '\t') {
					continue;
				}
				else {
					arg1 = &input[i];
					flag++;
					continue;
				}
			} 
			else if (flag == 2){
				// input arg1
				if (input[i] == ' ' || input[i] == '\t') {
					input[i] = '\0';
					flag++;
					continue;
				} else if (input[i] == '\n'){
					input[i] = '\0';
					break;
				} else if (input[i] == ',') {
					input[i] = '\0';
					flag += 2;
				}
			}
			else if (flag == 3) {
				// empty space before comma
				if (input[i] == ' ' || input[i] == '\t') {
					input[i] = '\0';
				}
				else if (input[i] == ',') {
					input[i] = '\0';
					flag ++;
					continue;
				}
				else {
					wrong_input_flag = 1;
					break;
				}

			}
			else if (flag == 4) {
				// empty space after comma
				if (input[i] == ' ' || input[i] == '\t') {
					input[i] = '\0';
				}
				else {
					arg2 = &input[i];
					flag++;
					continue;
				}
			}
			else if (flag == 5) {
				// input arg2
				if (input[i] == ' ' || input[i] == '\t') {
					input[i] = '\0';
					flag++;
					continue;
				} else if (input[i] == ',') {
					input[i] = '\0';
					flag += 2;
					continue;
				} else if(input[i] == '\n'){
					input[i] = '\0';
					break;
				}
			}
			else if (flag == 6) {
				// empty space before comma
				if (input[i] == ' ' || input[i] == '\t') {
					input[i] = '\0';
				}
				else if (input[i] == ',') {
					input[i] = '\0';
					flag++;
					continue;
				}
				else {
					wrong_input_flag = 1;
					break;
				}
			}
			else if (flag == 7) {
				// empty space after comma
				if (input[i] == ' ' || input[i] == '\t') {
					input[i] = '\0';
				}
				else {
					arg3 = &input[i];
					flag++;
					continue;
				}
			}
			else if (flag == 8) {
				// input arg3
				if (input[i] == ' ' || input[i] == '\t' || input[i] == '\n') {
					input[i] = '\0';
					flag++;
					continue;
				}
				else if (input[i] == ',') {
					input[i] = '\0';
					flag += 2;
					continue;
				}
			}
		}
		
	}
	
	// printf("cmd: %s arg1: %s arg2: %s arg3: %s\n", cmd, arg1, arg2, arg3);

	if(wrong_input_flag == 1){
		// wrong command
		printf("wrong command\n");
		printf("if you want to know the commands h[elp]\n");
		return 0;
	}
	else if (strcmp(cmd, "h") == 0 || strcmp(cmd, "help") == 0) {
		// h[elp]
		display_help();
	}
	else if (strcmp(cmd, "d") == 0 || strcmp(cmd, "dir") == 0) {
		// d[ir]
		display_dir();
	}
	else if (strcmp(cmd, "q") == 0 || strcmp(cmd, "quit") == 0) {
		// q[uit]
		printf("quit shell\n");
		return -1;
	}
	else if (strcmp(cmd, "hi") == 0 || strcmp(cmd, "history") == 0) {
		// hi[story]
		appendList(history, (void*)tmp_input);
		printHistory(history);
		return 0;
	}
	else if (strcmp(cmd, "du") == 0 || strcmp(cmd, "dump") == 0) {
		if(arg1==NULL){
			if(last_addr<=0xFFF60){
				// du[mp] 
				last_addr = hexDump(last_addr);
			} else {
				// segmentation fault
				printf("segmentation fault\n");
				last_addr = 0;
			}
		} else {
			int hex_arg1 = (int)strtol(arg1, NULL, 16);
			if(arg2==NULL){
				// du[mp] start
				last_addr = hexDumpWithStart(hex_arg1);
			}else{
				int hex_arg2 = (int)strtol(arg2, NULL, 16);
				if((hex_arg1 <= hex_arg2) && arg3==NULL){
					// du[mp] start, end
					if(hex_arg2 <= 0xFFFFF){
						last_addr = hexDumpWithStartEnd(hex_arg1, hex_arg2);
					} else {
						printf("segmentation fault\n");
					}
				}else{
					printf("end address is lower than start address\n");
					return 1;
				}
			}
		}
	} else if (strcmp(cmd, "e") == 0 || strcmp(cmd, "edit") == 0) {
		// e[dit] address, data
		if(arg1!=NULL && arg2!=NULL){
			edit((int)strtol(arg1, NULL, 16), (int)strtol(arg2, NULL, 16));
		}
	} else if(strcmp(cmd, "f") == 0 || strcmp(cmd, "fill") == 0){
		// f[ill] start, end, data
		if((arg1!=NULL && arg2!=NULL) && arg3!=NULL){
			int hex_arg1 = (int)strtol(arg1, NULL, 16);
			int hex_arg2 = (int)strtol(arg2, NULL, 16);
			int hex_arg3 = (int)strtol(arg3, NULL, 16);
			fill(hex_arg1, hex_arg2, hex_arg3);
		}
	} else if (strcmp(cmd, "reset") == 0){
		// reset
		memset(addr, 0, sizeof(addr));
	} else if (strcmp(cmd, "opcode") == 0){
		if(arg1!=NULL){
			// opcode mnemonic
			int opcode = get_opcode_by_key((unsigned char*)arg1);
			if(opcode != -1){
				printf("opcode is %02X\n", get_opcode_by_key((unsigned char*)arg1));
				appendList(history, (void*)tmp_input);
				return 0;
			} else {
				printf("Can't find %s in opcodelist\n", arg1);
				return 1;
			}
		}
	} else if (strcmp(cmd, "opcodelist") == 0) {
		// opcodelist
		if(opcodelist() != 0){
			printf("Something wrong");
		}
	} else if (strcmp(cmd, "type") == 0) {
		// type filename
		if(arg1!=NULL){
			type_file(arg1);
		}
	} else if (strcmp(cmd, "assemble") == 0){
		// assemble asm file
		if(arg1!=NULL){
			if(assemble_file(arg1) == 0){
				char* filename = strtok(arg1, ".");
				printf("output file : [%s.lst], [%s.obj]", filename, filename);
			};
		}
	} else if (strcmp(cmd, "symbol") == 0){
		// assemble asm file
		print_symbol_table();
	} else if (strcmp(cmd, "progaddr") == 0){
		// progaddr [address]
		if(arg1 != NULL){
			progaddr =  (int)strtol(arg1, NULL, 16);
			if(safe_progaddr(progaddr) == -1){
				printf("the address should be between 0 ~ 2^20\n");
				return 1;
			} else {
				appendList(history, (void*)tmp_input);
				return 0;
			}
		} else {
			printf("address is missing\n");
			return 1;
		}
	} else if (strcmp(cmd, "loader") == 0){
		// loader [object filename1] [object filename2] [...]
		if(safe_progaddr(progaddr) == -1){
			printf("the address is between 0 ~ 2^20\n");
			return 1;
		}
		object_lst = initList();
		if(arg1 != NULL){
			tmp_arg = (char*)calloc(sizeof(char), MAXLEN);
			strcpy(tmp_arg, arg1);
			appendList(object_lst, (void*)tmp_arg);
		}
		if(arg2 != NULL){
			tmp_arg = (char*)calloc(sizeof(char), MAXLEN);
			strcpy(tmp_arg, arg2);
			appendList(object_lst, (void*)tmp_arg);
		}
		if(arg3 != NULL){
			tmp_arg = (char*)calloc(sizeof(char), MAXLEN);
			strcpy(tmp_arg, arg3);
			appendList(object_lst, (void*)tmp_arg);
		}
		linkingLoader(object_lst);
			// type_file(arg1);
			// type_file(arg2);
			// type_file(arg3);
		return 0;
	} else if (strcmp(cmd, "run") == 0){
		int tmp = run();
		if(tmp != -1){
			progaddr = tmp;
		} else {
			return 1;
		}
	} else if (strcmp(cmd, "bp") == 0){
		if(arg1 != NULL){
			if(strcmp(arg1, "clear") == 0){
				clearBreakPoint();
			} else {
				addressBreakPoint((int)strtol(arg1, NULL, 16));
			}
		} else {
			printBreakPoint();
		}
	} else {
		// wrong command
		printf("wrong command\n");
		printf("if you want to know the commands h[elp]\n");
		return 1;
	}
	// create node on history linked list
	appendList(history, (void*)tmp_input);
	return 0;
}

int check_opcode(int opcode) {
	int right_opcode_ary[20] = {0xB4,0x28,0xA0,0x3C,0x30,0x38,0x48,0x00,0x68,0x50,
	0x74,0xD8,0x4C,0x0C,0x54,0x14,0x10,0xE0,0xB8,0xDC};
	for(int i = 0; i < sizeof(right_opcode_ary) / sizeof(int) ; i++){
		if(right_opcode_ary[i] == opcode){
			return 1;
		}
	}
	return 0;
}

int compareRegisters(int reg1, int reg2) {
	return regi_ary[reg1] < regi_ary[reg2] ? -1 : regi_ary[reg1] == regi_ary[reg2] ? 0 : 1;
}

/*
 * Function: safe_progaddr
 * 
 * detect unsafe address reference
 * 
 * addr: address of proaddr
 * return: address or error
 * (-1: error, address(0 ~ 2^20))
 */
int safe_progaddr(int addr){
	if (0 <= addr && addr < (MEMSIZE)) return addr;
	return -1;
}

/*
 * Function: loadOpcodelist
 * 
 * read opcode.txt load on the hash table.
 * 
 * return: Success 
 * (0: success, -1: failure) 
 */
int loadOpcodelist(void){
	// open opcode.txt with readonly.
	FILE *fp_opcode = fopen("opcode.txt", "r");
	if (!fp_opcode){
		printf("Can't read opcode.txt file");
		return -1;
	}

	while (!feof(fp_opcode)) {
		// fill out hash table with opcode.txt.
		fscanf(fp_opcode, "%x %s %s", &inst_record.opcode, inst_record.mnemonic, inst_record.format);
		insert_hash_table(inst_record);
	}
	
	// close file stream.
	fclose(fp_opcode);
	return 0;
}

/*
 * Function: checkRegiNum
 * 
 * check if regi num is right
 * 
 * return: Success 
 * (1: success, 0: failure) 
 */
int checkRegiNum(int reg) {
	return reg < 10 && 0 <= reg && reg != 7;
}

/*
 * Function: getRegiNum
 * 
 * get number of register
 * 
 * return: regi data
 */
int getRegiNum(char *register_name) {
	char *reg[] = {"A", "X", "L", "B", "S", "T", "F", "PC", "SW"};
	int reg_num[] = {0, 1, 2, 3, 4, 5, 6, 8, 9}, i;
	for (i = 0; i < 9; i++)
		if (!strcmp(reg[i], register_name)) return reg_num[i];
	printf("run: undefined register %s is found.\n", register_name);
	return -1;
}

/*
 * Function: printRegisters
 * 
 * print registers
 * 
 */
void printRegisters(int *registers) {
	char *reg[] = {"A", "X", "L", "PC", "B", "S", "T"};
	for (int i = 0; i < 7; i++){
		if(i%2==0 && i != 6){
			printf("  %-2s: %06X\t", reg[i], registers[getRegiNum(reg[i])] & 0xFFFFFF);
		} else {
			printf("  %-2s: %06X\n", reg[i], registers[getRegiNum(reg[i])] & 0xFFFFFF);
		}
	}
}

/*
 * Function: printBreakPoint
 * 
 * print breakpoint
 * 
 * return: Success 
 * (0: success) 
 */
int printBreakPoint(void){
	printf("\tbreakpoints\n");
	printf("\t-----------\n");
	for (int i = 0; i < MEMSIZE; i++) {
		if (bp[i])
			printf("\t%04X\n", i);
	}
	return 0;
}

/*
 * Function: addressBreakPoint
 * 
 * bp [address]
 * 
 * return: Success 
 * (0: success) 
 */
int addressBreakPoint(int addr){
	if (0 > addr || addr >= MEMSIZE){
		printf("bp: please input valid hexadecimal number less than 0x100000\n");
	}
	bp[addr] = 1;
	printf("\t[ok] create breakpoint %04X\n", addr);
	return 0;
}

/*
 * Function: clearBreakPoint
 * 
 * clear breakpoint
 * 
 * return: Success 
 * (0: success) 
 */
int clearBreakPoint(void){
	for (int i = 0; i < MEMSIZE; i++){
		bp[i] = 0;
	}
	printf("\t[ok] clear all breakpoints\n");
	return 0;
}

/*
 * Function: run
 * 
 * run the program that allocated on addresses
 * 
 * return: Success 
 * (0: success, -1: failure) 
 */
int run(void)
{
	int is_break_point = 0, opcode = 0, is_extended = 0, reg1 = 0, reg2 = 0, pc_delta = 0, cc = 0, target_value = 0, target_address = 0;
	Inst *inst = NULL;
	if (last_bp == -1) regi_ary[getRegiNum("L")] = 0xFFFFFF;

	PC = progaddr;
	while (PC < MEMSIZE && PC >= 0) {
		is_break_point = bp[PC];
		pc_delta = 0;
		opcode = addr[PC] & (~3);
		if ((inst = get_inst_by_opcode(opcode)) != NULL && check_opcode(inst -> opcode)) {
		
			if (strcmp(inst->format, "3/4") == 0) {
				is_extended = ((addr[PC + 1] & 0xF0) >> 4) & 1;
				if ((addr[PC] & 3) == 0) {
					pc_delta = 1;
					PC += 1;
					continue;
				} else if (is_extended) { 
					is_break_point = bp[PC] | bp[PC + 1] | bp[PC + 2] | bp[PC + 3];
					BP_FUNC(PC);
					target_address =(((unsigned int)addr[PC + 1] & 0x0F) << 16) | (((unsigned int)addr[PC + 2]) << 8) | (((unsigned int)addr[PC + 3]) << 0);
					target_address &= 0xFFFFFF;
					target_value = (((unsigned int)addr[target_address + 0]) << 16) | (((unsigned int)addr[target_address + 1]) << 8) | (((unsigned int)addr[target_address + 2]) << 0);
					if ((addr[PC + 0] & 3) == 1) target_value = target_address;
					if ((addr[PC + 0] & 3) == 2) {
						target_address = target_value;
						if (0 <= target_value && target_value < MEMSIZE)
							target_value = (((unsigned int)addr[target_value + 0]) << 16) | (((unsigned int)addr[target_value + 1]) << 8) | (((unsigned int)addr[target_value + 2]) << 0);
					}
					pc_delta = 4;
				} else { 
					pc_delta = 3;
					is_break_point = bp[PC] | bp[PC + 1] | bp[PC + 2];
					BP_FUNC(PC);
					target_address = (((unsigned int)addr[PC + 1] & 0x0F) << 8) | (((unsigned int)addr[PC + 2]) << 0);
					if (((addr[PC + 1] & 0xF0) >> 4) & (1 << 3)) target_address += regi_ary[getRegiNum("X")];
					if (((addr[PC + 1] & 0xF0) >> 4) & (1 << 2)) target_address += regi_ary[getRegiNum("B")];
					if (((addr[PC + 1] & 0xF0) >> 4) & (1 << 1)) target_address = ((target_address > 0x7FF ? (0xFFF000 | target_address) : target_address) + regi_ary[getRegiNum("PC")] + pc_delta);
					target_address &= 0xFFFFFF;
					target_value = (((unsigned int)addr[target_address + 0]) << 16) | (((unsigned int)addr[target_address + 1]) << 8) | (((unsigned int)addr[target_address + 2]) << 0);
					if ((addr[PC + 0] & 3) == 1) target_value = target_address;
					if ((addr[PC + 0] & 3) == 2) {
						target_address = target_value;
						if (0 <= target_value && target_value < MEMSIZE)
							target_value = (((unsigned int)addr[target_value + 0]) << 16) | (((unsigned int)addr[target_value + 1]) << 8) | (((unsigned int)addr[target_value + 2]) << 0);
					}
				}
			} else if (strcmp(inst->format,"2") == 0) {
				is_break_point = bp[PC] | bp[PC + 1];
				BP_FUNC(PC);
				reg1 = (addr[PC + 1] & 0xF0) >> 4;
				reg2 = (addr[PC + 1] & 0x0F) >> 0;
				if (!strcmp(inst->mnemonic, "CLEAR")) {
					if (checkRegiNum(reg1)) { 
						pc_delta = 2;
					} else {
						pc_delta = 1;
						continue;
					}
				} else { 
					if (checkRegiNum(reg1) && checkRegiNum(reg2)) {
						pc_delta = 2; // this is valid format 2 instruction.
					} else { // invalid operands.
						pc_delta = 1;
						continue;
					}
				}
			} else {
				is_break_point = bp[PC];
				BP_FUNC(PC);
				pc_delta = 1;
			}
			BP_FUNC(PC);
			switch (opcode) {
				case 0xB4://clear(2)
					regi_ary[reg1] = 0;
					break;
				case 0x28://COMP;
					reg2 = regi_ary[getRegiNum("X")];
					regi_ary[getRegiNum("X")] = target_value;
					cc = compareRegisters(getRegiNum("A"), getRegiNum("X"));
					regi_ary[getRegiNum("X")] = reg2;
					break;
				case 0xA0://COMPR(2)
					cc = compareRegisters(reg1, reg2);
					break;
				case 0x3C://J
					pc_delta = target_address - PC;
					break;
				case 0x30://JEQ
					if (cc != 0) break;
					pc_delta = target_address - PC;
					break;
				case 0x38://JLT
					if (cc != -1) break;
					pc_delta = target_address - PC;
					 break;
				case 0x48://JSUB
					regi_ary[getRegiNum("L")] = PC + pc_delta;
					pc_delta = target_address - PC;
					break;
				case 0x00://LDA
					regi_ary[getRegiNum("A")] = target_value;
					break;
				case 0x68://LDB
					regi_ary[getRegiNum("B")] = target_value;
					break;
				case 0x50://LDCH
					regi_ary[getRegiNum("A")] = (regi_ary[getRegiNum("A")] & 0xFFFF00) | (((target_value & 0xFF0000) >> 16) & 0xFF);
					break;
				case 0x74://LDT
					regi_ary[getRegiNum("T")] = target_value;
					break;
				case 0xD8://RD
					cc = 1;
					break;
				case 0x4C://RSUB
					pc_delta = regi_ary[getRegiNum("L")] - PC;
					break;
				case 0x0C://STA
					addr[target_address + 0] = (regi_ary[getRegiNum("A")] >> 16) & 0xFF;
					addr[target_address + 1] = (regi_ary[getRegiNum("A")] >> 8) & 0xFF;
					addr[target_address + 2] = (regi_ary[getRegiNum("A")] >> 0) & 0xFF;
					break;
				case 0x54://STCH
					addr[target_address + 0] = (regi_ary[getRegiNum("A")] >> 0) & 0xFF;
					break;
				case 0x14://STL
					addr[target_address + 0] = (regi_ary[getRegiNum("L")] >> 16) & 0xFF;
					addr[target_address + 1] = (regi_ary[getRegiNum("L")] >> 8) & 0xFF;
					addr[target_address + 2] = (regi_ary[getRegiNum("L")] >> 0) & 0xFF;
					break;
				case 0x10://STX
					addr[target_address + 0] = (regi_ary[getRegiNum("X")] >> 16) & 0xFF;
					addr[target_address + 1] = (regi_ary[getRegiNum("X")] >> 8) & 0xFF;
					addr[target_address + 2] = (regi_ary[getRegiNum("X")] >> 0) & 0xFF;
					break;
				case 0xE0://TD
					cc = 1;
					break;
				case 0xB8://TIXR
					regi_ary[getRegiNum("X")] += 1;
					cc = compareRegisters(getRegiNum("X"), reg1);
					break;
				case 0xDC://WD
					break;
			}
		} else { // not an instruction.
			PC += 1;
			continue;
		}
		PC += pc_delta;
	}

	last_bp = -1;
	printRegisters(regi_ary);
	printf("End program.\n");
	return PC;
}

/*
 * Function: linkingLoader
 * 
 * Link multiple object files and load it
 * 
 * return: Success 
 * (0: success, -1: failure) 
 */
int linkingLoader(LinkedList* object_files){
	int sym_addr = 0;
	int cur_size = 0;
	int cur_addr = progaddr;
	int cur_scan_pos = 0;
	int line_len = 0;
	int cur_obj = 0;
	int prog_length[1 << 8] = {};
	int start_addr_array[1 << 8] = {};
	int sym_addr_array[1 << 8] = {};
	int cur_byte = 0;
	int rec_start_addr_array = 0;
	int rec_length = 0;
	FILE *fp = NULL;
	char *pstr = NULL, line_buffer[MAXLEN << 1] = "", prog_name[MAXLEN] = "", sym_name[MAXLEN] = "";
	Node *ptr = NULL;
	LinkedList *symbol_history = initList();
	struct ExternalSymbol *ES = NULL;
	for (ptr = object_files -> head; ptr != NULL; ptr = ptr -> link){
		pstr = (char*)ptr -> data;
		start_addr_array[cur_obj] = cur_addr;
		if (strncmp(pstr + strlen(pstr) - 3, "obj", 3)){
			printf("loader: please open .obj file.\n");
			return -1;
		}
		fp = fopen(pstr, "r");
		cur_size = -1;

		if (fp == NULL){
			printf("loader: cannot open %s.\n", pstr);
			return -1;
		}
		while (fgets(line_buffer, sizeof(line_buffer), fp)){
			line_len = strlen(line_buffer);
			line_buffer[line_len - 1] = ' ';
			if (*line_buffer == 'H') {
				ES = (ExternalSymbol *)malloc(sizeof(ExternalSymbol));
				sscanf(line_buffer, "H%6s%6x%6x", prog_name, start_addr_array + cur_obj, &cur_size);
				start_addr_array[cur_obj] += cur_addr;
				prog_length[cur_obj] = cur_size;
				strcpy(ES->name, prog_name);
				ES->address = cur_addr;
				ES->length = cur_size;
				ES->is_symbol = 0;
				if(insert_external_symbol_table(*ES)){
					printf("external symbol already exist");
					return -1;
				}
				appendList(symbol_history, ES);
			} else if (*line_buffer == 'D') {
				if (cur_size < 0){
					printf("loader: the file is not a valid object file.\n");
					return -1;
				}
				for (cur_scan_pos = 1; cur_scan_pos < line_len; cur_scan_pos += 12) {	
					ES = (ExternalSymbol *)malloc(sizeof(ExternalSymbol));
					sym_name[0] = 0;
					if (sscanf(line_buffer + cur_scan_pos, "%6s%6x", sym_name, &sym_addr) != 2) continue;
					if (search_element_external_symbol_table((unsigned char*)sym_name) != -1){
						printf("loader: repeated symbol %s is found.\n", sym_name);
						return -1;
					}
					strcpy(ES->name, sym_name);
					ES->address = sym_addr + cur_addr;
					ES->length = 0;
					ES->is_symbol = 1;
					if(insert_external_symbol_table(*ES)){
						printf("external symbol already exist");
						return -1;
					}
					appendList(symbol_history, ES);
				}
			} else if (*line_buffer == 'R') {
				if (cur_size < 0){
					printf("loader: the file is not a valid object file.\n");
				}
			} else if (*line_buffer == 'T') { 
				if (cur_size < 0){
					printf("loader: the file is not a valid object file.\n");
				}
			} else if (*line_buffer == 'E') {
				if (cur_size < 0){
					printf("loader: the file is not a valid object file.\n");
				}
				break;
			}
			line_buffer[0] = 0;
		}
		cur_addr += cur_size;
		cur_obj++;

		fclose(fp);
	}

	cur_addr = progaddr;
	cur_obj = 0;

	for (ptr = object_files -> head; ptr != NULL; ptr = ptr -> link) {
		pstr = (char*)ptr -> data;
		if (strncmp(pstr + strlen(pstr) - 3, "obj", 3)){
			printf("loader: please open .obj file.\n");
		}

		fp = fopen(pstr, "r");
		cur_size = -1;

		if (fp == NULL){
			printf("loader: cannot open %s.\n", pstr);
		}

		while (fgets(line_buffer, sizeof(line_buffer), fp)) {
			line_len = strlen(line_buffer);
			if (*line_buffer == 'H') {
				// H Record
				sscanf(line_buffer + 1, "%6s", prog_name);
				sym_addr_array[1] = start_addr_array[cur_obj];
				cur_addr = start_addr_array[cur_obj];
				cur_size = prog_length[cur_obj];
			} else if (*line_buffer == 'R') {
				// R Record
				if (cur_size < 0){
					printf("loader: the file is not a valid object file.\n");
				}
				for (cur_scan_pos = 1; cur_scan_pos < line_len; cur_scan_pos += 8) {
					if (sscanf(line_buffer + cur_scan_pos, "%2x%6s", &sym_addr, sym_name) != 2) continue;
					if (search_element_external_symbol_table((unsigned char*)sym_name) == -1){
						printf("loader: symbol %s is referenced but not found.\n", sym_name);
						return -1;
					}
					sym_addr_array[sym_addr] = search_element_external_symbol_table((unsigned char*)sym_name);
				}
			} else if (*line_buffer == 'T') {
				// T Record
				if (cur_size < 0){
					printf("loader: the file is not a valid object file.\n");
				}
				sscanf(line_buffer + 1, "%6x%2x", &rec_start_addr_array, &rec_length);
				for (cur_scan_pos = 9; cur_scan_pos < line_len; cur_scan_pos+=2) {
					cur_byte = 0;
					sscanf(line_buffer + cur_scan_pos, "%2x", &cur_byte);
					addr[cur_addr + rec_start_addr_array + ((cur_scan_pos - 9) / 2)] = (unsigned char)(cur_byte & 0xFF);
				}
			} else if (*line_buffer == 'M') {
				// M Record
				sscanf(line_buffer + 1, "%6x%2x%1s%2x", &rec_start_addr_array, &rec_length, sym_name, &sym_addr);
				rec_start_addr_array += cur_addr;
				cur_byte = 0;
				if (rec_length == 5) {
					cur_byte = (((unsigned int)(addr[rec_start_addr_array + 0] & 0x0F)) << 16) | (((unsigned int)(addr[rec_start_addr_array + 1] & 0xFF)) << 8) | (((unsigned int)(addr[rec_start_addr_array + 2] & 0xFF)) << 0);
					cur_byte += (*sym_name == '+' ? sym_addr_array[sym_addr] : -sym_addr_array[sym_addr]);
					cur_byte &= 0x0FFFFF;
					addr[rec_start_addr_array + 0] = (unsigned char)((unsigned int)addr[rec_start_addr_array + 0] & 0xF0) | ((unsigned int)(cur_byte >> 16) & 0x0F);
					addr[rec_start_addr_array + 1] = (cur_byte >> 8) & 0xFF;
					addr[rec_start_addr_array + 2] = (cur_byte >> 0) & 0xFF;
				} else {
					cur_byte = (((unsigned int)(addr[rec_start_addr_array + 0] & 0xFF)) << 16) | (((unsigned int)(addr[rec_start_addr_array + 1] & 0xFF)) << 8) | (((unsigned int)(addr[rec_start_addr_array + 2] & 0xFF)) << 0);
					cur_byte += (*sym_name == '+' ? sym_addr_array[sym_addr] : -sym_addr_array[sym_addr]);
					cur_byte &= 0xFFFFFF;
					addr[rec_start_addr_array + 0] = (cur_byte >> 16) & 0xFF;
					addr[rec_start_addr_array + 1] = (cur_byte >> 8) & 0xFF;
					addr[rec_start_addr_array + 2] = (cur_byte >> 0) & 0xFF;
				}
			} else if (*line_buffer == 'E') {
				// E Record
				if (cur_size < 0){
					printf("loader: the file is not a valid object file.\n");
				}
				break;
			}
			line_buffer[0] = 0;
		}
		cur_addr += cur_size;
		cur_obj++;
		fclose(fp);
	}

	cur_addr = progaddr;
	cur_object_file = 0;

	for (ptr = object_files -> head; ptr != NULL; ptr = ptr -> link) {
		pstr = (char*)ptr -> data;
		if (strncmp(pstr + strlen(pstr) - 3, "obj", 3)){
			printf("loader: please open .obj file.\n");
		}

		fp = fopen(pstr, "r");
		cur_size = -1;

		if (fp == NULL){
			printf("loader: cannot open %s.\n", pstr);
		}

		while (fgets(line_buffer, sizeof(line_buffer), fp)) {
			line_len = strlen(line_buffer);
			if (*line_buffer == 'H') {
				sscanf(line_buffer + 1, "%6s", prog_name);
				sym_addr_array[1] = start_addr_array[cur_object_file];
				cur_addr = start_addr_array[cur_object_file];
				cur_size = prog_length[cur_object_file];
			} else if (*line_buffer == 'D') { // Since this is already handled in pass1, ignore this.
			} else if (*line_buffer == 'R') {
				if (cur_size < 0){
					printf("loader: the file is not a valid object file.\n");
				}
				for (cur_scan_pos = 1; cur_scan_pos < line_len; cur_scan_pos += 8) {
					if (sscanf(line_buffer + cur_scan_pos, "%2x%6s", &sym_addr, sym_name) != 2) continue;
					if (search_element_external_symbol_table((unsigned char*)sym_name) == -1){
						printf("loader: symbol %s is referenced but not found.\n", sym_name);
						return -1;
					}
					sym_addr_array[sym_addr] = search_element_external_symbol_table((unsigned char*)sym_name);
					printf("External Symbol Address: %d\n", sym_addr_array[sym_addr]);
				}
			} else if (*line_buffer == 'T') {
				if (cur_size < 0){
					printf("loader: the file is not a valid object file.\n");
				}
				sscanf(line_buffer + 1, "%6x%2x", &rec_start_addr_array, &rec_length);
				for (cur_scan_pos = 9; cur_scan_pos < line_len; cur_scan_pos+=2) {
					cur_byte = 0;
					sscanf(line_buffer + cur_scan_pos, "%2x", &cur_byte);
					addr[cur_addr + rec_start_addr_array + ((cur_scan_pos - 9) / 2)] = (unsigned char)(cur_byte & 0xFF);
				}
			} else if (*line_buffer == 'M') {
				sscanf(line_buffer + 1, "%6x%2x%1s%2x", &rec_start_addr_array, &rec_length, sym_name, &sym_addr);
				rec_start_addr_array += cur_addr;
				cur_byte = 0;
				if (rec_length == 5) {
					cur_byte = (((unsigned int)(addr[rec_start_addr_array + 0] & 0x0F)) << 16) | (((unsigned int)(addr[rec_start_addr_array + 1] & 0xFF)) << 8) | (((unsigned int)(addr[rec_start_addr_array + 2] & 0xFF)) << 0);
					cur_byte += (*sym_name == '+' ? sym_addr_array[sym_addr] : -sym_addr_array[sym_addr]);
					cur_byte &= 0x0FFFFF;
					addr[rec_start_addr_array + 0] = (unsigned char)((unsigned int)addr[rec_start_addr_array + 0] & 0xF0) | ((unsigned int)(cur_byte >> 16) & 0x0F);
					addr[rec_start_addr_array + 1] = (cur_byte >> 8) & 0xFF;
					addr[rec_start_addr_array + 2] = (cur_byte >> 0) & 0xFF;
				} else { // case rec_length = 6
					cur_byte = (((unsigned int)(addr[rec_start_addr_array + 0] & 0xFF)) << 16) | (((unsigned int)(addr[rec_start_addr_array + 1] & 0xFF)) << 8) | (((unsigned int)(addr[rec_start_addr_array + 2] & 0xFF)) << 0);
					cur_byte += (*sym_name == '+' ? sym_addr_array[sym_addr] : -sym_addr_array[sym_addr]);
					cur_byte &= 0xFFFFFF;
					addr[rec_start_addr_array + 0] = (cur_byte >> 16) & 0xFF;
					addr[rec_start_addr_array + 1] = (cur_byte >> 8) & 0xFF;
					addr[rec_start_addr_array + 2] = (cur_byte >> 0) & 0xFF;
				}
			} else if (*line_buffer == 'E') {
				if (cur_size < 0){
					printf("loader: the file is not a valid object file.\n");
				}
				break;
			}
			line_buffer[0] = 0;
		}

		cur_addr += cur_size;
		cur_object_file++;
		fclose(fp);

	}
	
	printExternalSymbolTable(symbol_history);
	return 0;
}

/*
 * Function: assemble_file
 * 
 * make ${studentID}.lst, ${studentID}.obj file with ${assemble_filename}.asm
 * 
 * filename: assembly filename string
 * return: Success
 * (0: success, 1: failure, -1: error) 
 */
int assemble_file(char * filename){
	struct LinkedList *record_list = NULL;
	FILE *fp_asm, *fp_lst, *fp_obj;
	// int locctr, start, length, opcode;
	int locctr, start, opcode, counter, line;
	char tmp_str[MAX];
	char c, tmp,label[10], mnemonic[10], operand[10], operand2[10];
	char * format_str;
	int base;
	struct Record *record = NULL;
	symbol_ctr = 0;

	char* filename_without_extention = strtok(filename, ".");
	char* list_file = ".lst";
	strcat(filename_without_extention, list_file);
	char* obj_file = ".obj";
	strcat(filename_without_extention, obj_file);
	/* Open assembler file */
	fp_lst = fopen(list_file, "w");
	fp_obj = fopen(obj_file, "w");
	record_list = initList();

	/* pass 1,2 of assembler */
	for(int pass = 1; pass < 3 ; pass++){
		fp_asm = fopen(filename, "r");
		line  = 5;
		if (!fp_asm){
			printf("Cannot open file \n");
			return -1;
		}
		fscanf(fp_asm,"%s%s%s",label, mnemonic, operand);
		strcpy(program_name, label);
		starting_address = (int)strtol(operand, NULL, 16);
		if(strcmp(mnemonic, "START") == 0)
		{
			start = atoi(operand);
			locctr = start;
			if(pass==2){
				fprintf(fp_lst, "%d\t%04X\t%s\t%s\t%s\n", line, locctr, label, mnemonic, operand);
				record = (Record*)malloc(sizeof(Record));
				record->type = HEADER;
				record->start = starting_address;
				record->end = ending_address;
				strcpy(record->content, program_name);
				appendList(record_list, record);
				line += 5;
			}
		} else {
			locctr = 0;
		}
		record = (Record*)malloc(sizeof(Record));
		record->type = TEXT;
		while (strcmp(mnemonic,"END") != 0) {
			char ni [2] = {'0', '0'};
			char xbpe [4] = {'0', '0', '0', '0'};
			char tmp_content [MAXLEN << 1];
			counter = 0;
			memset(label, 0, sizeof(label));
			memset(mnemonic, 0, sizeof(mnemonic));
			memset(operand, 0, sizeof(operand));
			format_str = NULL;
			c = getc(fp_asm);
			if(pass == 2){
				line += 5;
			}
			if(c == ' '){
				fscanf(fp_asm,"%s\n", mnemonic);
				if(strcmp(mnemonic, "END") == 0){
					// return to caller
					fscanf(fp_asm,"%s%c", operand, &tmp);
					if(pass == 2){
						fprintf(fp_lst, "%d\t\t\t\t\t%s \t%s\n", line, mnemonic, operand);
					}
				} else if (strcmp(mnemonic, "RSUB") == 0){
					// return to caller
					if(pass == 2){
						fprintf(fp_lst, "%d\t%04X\t\t\t%s\t\t", line, locctr, mnemonic);
					}
				} else if(strcmp(mnemonic, "LDCH") == 0 || strcmp(mnemonic, "STCH") == 0){
					// LDCH or STCH
					fscanf(fp_asm,"%s%s", operand, operand2);
					operand[strlen(operand)-1] = '\0';
					if(pass == 2){
						fprintf(fp_lst, "%d\t%04X\t\t\t%s \t%s\t%s\t", line, locctr, mnemonic, operand, operand2);
					}
					if(strlen(operand2) == 1 && strcmp(operand2, "X") == 0){
						// STCH or LDCH
						ungetc(' ', fp_asm);
						xbpe[0] = '1';
					} else {
						printf("line %d: %s is not register", line, operand2);
					}
				} else if(strcmp(mnemonic, "COMPR") == 0){
					// COMPR
					fscanf(fp_asm,"%s%s", operand, operand2);
					operand[strlen(operand)-1] = '\0';
					if(pass == 2){
						fprintf(fp_lst, "%d\t%04X\t\t\t%s \t%s\t%s\t", line, locctr, mnemonic, operand, operand2);
					}
					if(strlen(operand2) == 1){
						ungetc(' ', fp_asm);
					} else {
						printf("line %d: %s is not register", line, operand2);
					}
				} else if(strcmp(mnemonic, "BASE") == 0){
					// BASE
					fscanf(fp_asm,"%s%c", operand, &tmp);
					base = get_loc_by_symbol((unsigned char*)operand);
					if(pass == 2){
						fprintf(fp_lst, "%d\t\t\t\t\t%s \t%s\n", line, mnemonic, operand);
					}
				} else{
					// without label
					fscanf(fp_asm,"%s%c", operand, &tmp);
					if(pass == 2) fprintf(fp_lst, "%d\t%04X\t\t\t%s \t%s\t", line, locctr, mnemonic, operand);
				}
			} else if(c == '.'){
				// comment
				fgets(tmp_str, sizeof(tmp_str), fp_asm);
				if(pass == 2) fprintf(fp_lst, "%d\t%s", line, tmp_str);
			} else {
				// with label
				ungetc(c, fp_asm);
				fscanf(fp_asm, "%s%s%s%c", label, mnemonic, operand, &tmp);
				if(pass == 2) fprintf(fp_lst, "%d\t%04X\t%s \t%s \t%s\t", line, locctr, label, mnemonic, operand);
				if(pass == 1){
					struct Symbol symbol;
					symbol.loc = locctr;
					strcpy(symbol.symbol, label);
					insert_symbol_table(symbol);
				}
			}

			if((opcode = get_opcode_by_key((unsigned char*)mnemonic)) != -1){
				format_str = get_format_by_key((unsigned char*)mnemonic);
				if(strcmp(format_str,"2") == 0){
					counter = 2;
				} else if(strcmp(format_str,"3/4") == 0){
					counter = 3;
				}
				if(pass == 2){
					if(operand[0] == '#'){
						// immediate addressing
						ni[1] = '1';
						if(get_loc_by_symbol((unsigned char*)operand + sizeof(unsigned char)) != -1){
							ni[1] = '1';
							int symbol_loc = get_loc_by_symbol((unsigned char*)operand  + sizeof(unsigned char));
							int pc = locctr + counter;
							int disp = symbol_loc - pc;
							if(disp < 0x1000 && disp > -1*0x1000){
								// pc
								xbpe[2] = '1';
								if(pass == 2){
									fprintf(fp_lst,"%02X%01X%03X\n", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), disp);
									if(record->len == 0) record->start = locctr;
									sprintf(tmp_content, "%02X%01X%03X", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), disp);
									strcat(record->content, tmp_content);
								}
							} else {
								// base
								xbpe[1] = '1';
								disp = symbol_loc - base;
								if(pass == 2){
									fprintf(fp_lst, "%02X%01X%03X\n", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), disp);
									if(record->len == 0) record->start = locctr;
									sprintf(tmp_content, "%02X%01X%03X", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), disp);
									strcat(record->content, tmp_content);
								}
							}
						} else {
							int address = (int)strtol(operand + sizeof(unsigned char), NULL, 16);
							if(pass == 2){
								fprintf(fp_lst, "%02X%01X%03X\n", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), address);
								if(record->len == 0) record->start = locctr;
								sprintf(tmp_content, "%02X%01X%03X", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), address);
								strcat(record->content, tmp_content);
							}
						}
					} else if(operand[0] == '@'){
						// indirect addressing
						ni[0] = '1';
						if(get_loc_by_symbol((unsigned char*)operand + sizeof(unsigned char)) != -1){
							int symbol_loc = get_loc_by_symbol((unsigned char*)operand  + sizeof(unsigned char));
							int pc = locctr + counter;
							int disp = symbol_loc - pc;
							if(disp < 0x1000 && disp > -1*0x1000){
								// pc
								xbpe[2] = '1';
								if(pass == 2){
									fprintf(fp_lst, "%02X%01X%03X\n", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), disp);
									if(record->len == 0) record->start = locctr;
									sprintf(tmp_content, "%02X%01X%03X", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), disp);
									strcat(record->content, tmp_content);
								}
							} else {
								// base
								xbpe[1] = '1';
								disp = symbol_loc - base;
								if(disp>=0){
									if(pass == 2){
										fprintf(fp_lst, "%02X%01X%03X\n", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), disp);
										if(record->len == 0) record->start = locctr;
										sprintf(tmp_content, "%02X%01X%03X", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), disp);
										strcat(record->content, tmp_content);
									}
								} else {
									printf("line %d: cannot object code with either pc register and base register\n", line);
								}
							}
						}
					} else {
						// simple addressing
						if(get_loc_by_symbol((unsigned char*)operand) != -1){
							ni[0] = '1';
							ni[1] = '1';
							int symbol_loc = get_loc_by_symbol((unsigned char*)operand);
							int pc = locctr + counter;
							int disp = symbol_loc - pc;
							if(disp < 0x1000 && disp > -1*0x1000){
								// pc
								xbpe[2] = '1';
								if(disp >= 0){
									if(pass == 2){
										fprintf(fp_lst, "%02X%01X%03X \n", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), (uint16_t)(disp));
										if(record->len == 0) record->start = *(&locctr);
										sprintf(tmp_content, "%02X%01X%03X", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), (uint16_t)(disp));
										strcat(record->content, tmp_content);
									}
								} else {
									if(pass == 2){
										fprintf(fp_lst, "%02X%01X%03X \n", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), (uint16_t)(disp + 0x1000));
										if(record->len == 0) record->start = locctr;
										sprintf(tmp_content, "%02X%01X%03X", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), (uint16_t)(disp + 0x1000));
										strcat(record->content, tmp_content);
									}
								}
							} else {
								// base
								xbpe[1] = '1';
								disp = symbol_loc - base;
								if(disp >= 0){
									if(pass == 2){
										fprintf(fp_lst, "%02X%01X%03X \n", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), (uint16_t)(disp));
										if(record->len == 0) record->start = locctr;
										sprintf(tmp_content, "%02X%01X%03X", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), (uint16_t)(disp));
										strcat(record->content, tmp_content);
									}
								} else {
									if(pass == 2){
										fprintf(fp_lst, "%02X%01X%03X \n", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), (uint16_t)(disp + 0x1000));
										if(record->len == 0) record->start = locctr;
										sprintf(tmp_content, "%02X%01X%03X", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), (uint16_t)(disp));
										strcat(record->content, tmp_content);
									}
								}							
							}
						} else if (strlen(operand) == 1 && strcmp(format_str, "2") == 0){
							if(strcmp(mnemonic, "COMPR") == 0){
								int regi_first, regi_second;
								if(strcmp(operand, "X") == 0){
									regi_first = regi_x;
								} else if(strcmp(operand, "A") == 0){
									regi_first = regi_a;
								} else if(strcmp(operand, "S") == 0){
									regi_first = regi_s;
								} else if(strcmp(operand, "T") == 0){
									regi_first = regi_t;
								}

								if(strcmp(operand2, "X") == 0){
									regi_second = regi_x;
								} else if(strcmp(operand2, "A") == 0){
									regi_second = regi_a;
								} else if(strcmp(operand2, "S") == 0){
									regi_second = regi_s;
								} else if(strcmp(operand2, "T") == 0){
									regi_second = regi_t;
								}
								if(pass == 2) {
									fprintf(fp_lst, "%02X%01X%01X\n", opcode+(int)strtol(ni, NULL, 2), regi_first, regi_second);
									if(record->len == 0) record->start = locctr;
									sprintf(tmp_content, "%02X%01X%01X", opcode+(int)strtol(ni, NULL, 2), regi_first, regi_second);
									strcat(record->content, tmp_content);
								}
							} else {
								if(strcmp(operand, "X") == 0){
									if(pass == 2){
										fprintf(fp_lst, "%02X%01X%01X\n", opcode, regi_x, 0x0);
										if(record->len == 0) record->start = locctr;
										sprintf(tmp_content, "%02X%01X%01X", opcode, regi_x, 0x0);
										strcat(record->content, tmp_content);
									}
								} else if(strcmp(operand, "A") == 0){
									if(pass == 2){
										fprintf(fp_lst, "%02X%01X%01X\n", opcode, regi_a, 0x0);
										if(record->len == 0) record->start = locctr;
										sprintf(tmp_content, "%02X%01X%01X", opcode, regi_a, 0x0);
										strcat(record->content, tmp_content);
									}
								} else if(strcmp(operand, "S") == 0){
									if(pass == 2){
										fprintf(fp_lst, "%02X%01X%01X\n", opcode, regi_s, 0x0);
										if(record->len == 0) record->start = locctr;
										sprintf(tmp_content, "%02X%01X%01X", opcode, regi_s, 0x0);
										strcat(record->content, tmp_content);
									}
								} else if(strcmp(operand, "T") == 0){
									if(pass == 2){
										fprintf(fp_lst, "%02X%01X%01X\n", opcode, regi_t, 0x0);
										if(record->len == 0) record->start = locctr;
										sprintf(tmp_content, "%02X%01X%01X", opcode, regi_t, 0x0);
										strcat(record->content, tmp_content);
									}
								}
							}
						} else if(strcmp(mnemonic, "RSUB") == 0){
							ni[0] = '1';
							ni[1] = '1';
							int disp = 0;
							if(pass == 2){
								fprintf(fp_lst, "%02X%01X%03X\n", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), (uint16_t)(disp));
								if(record->len == 0) record->start = locctr;
								sprintf(tmp_content, "%02X%01X%03X", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), (uint16_t)(disp));
								strcat(record->content, tmp_content);
							}
						} 
					}
				}
			} else if(mnemonic[0] == '+' && (opcode = get_opcode_by_key((unsigned char*)mnemonic + sizeof(unsigned char))) != -1){
				// format 4
				counter = 4;
				xbpe[3] = '1';
				if(pass == 2){
					if(operand[0] == '#'){
						// immediate addressing
						ni[1] = '1';
						if(get_loc_by_symbol((unsigned char*)operand + sizeof(unsigned char)) != -1){
							int address = get_loc_by_symbol((unsigned char*)operand);
							if(pass == 2){
								fprintf(fp_lst, "%02X%01X%05X\n", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), address);
								if(record->len == 0) record->start = locctr;
								sprintf(tmp_content, "%02X%01X%05X", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), address);
								strcat(record->content, tmp_content);
							}
						} else {
							// with const value
							int address = (int)strtol(operand + sizeof(unsigned char), NULL, 10);
							if(pass == 2){
								fprintf(fp_lst, "%02X%01X%05X\n", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), address);
								if(record->len == 0) record->start = locctr;
								sprintf(tmp_content, "%02X%01X%05X", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), address);
								strcat(record->content, tmp_content);
							}
						}
					} else if(operand[0] == '@'){
						// indirect addressing
						ni[0] = '1';
						if(get_loc_by_symbol((unsigned char*)operand + sizeof(unsigned char)) != -1){
							int address = get_loc_by_symbol((unsigned char*)operand);
							if(pass == 2){
								fprintf(fp_lst, "%02X%01X%05X\n", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), address);
								if(record->len == 0) record->start = locctr;
								sprintf(tmp_content, "%02X%01X%05X", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), address);
								strcat(record->content, tmp_content);
							}
						}
					} else {
						if(get_loc_by_symbol((unsigned char*)operand) != -1){
							ni[0] = '1';
							ni[1] = '1';
							int address = get_loc_by_symbol((unsigned char*)operand);
							if(pass == 2) {
								modified_addr_ary = (int *)realloc(modified_addr_ary, (m_size + 1)*sizeof(int));
								modified_addr_ary[m_size] = locctr + 0x1;
								m_size += 1; 
								fprintf(fp_lst, "%02X%01X%05X\n", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), address);
								if(record->len == 0) record->start = locctr;
								sprintf(tmp_content, "%02X%01X%05X", opcode+(int)strtol(ni, NULL, 2), (int)strtol(xbpe, NULL, 2), address);
								strcat(record->content, tmp_content);
							}
						}
					}
				}
			} else if(strcmp(mnemonic, "WORD") == 0){
				counter = 3;
			} else if(strcmp(mnemonic, "RESW") == 0){
				counter = (3*(atoi(operand)));
				if(pass == 2){
					fprintf(fp_lst, "\n");
					if(record->len){
						record->end = locctr + counter;
						record->len = strlen(record->content)/2;
						appendList(record_list, (void*)record);
						record = (Record*)malloc(sizeof(Record));
					}
				}
			} else if(strcmp(mnemonic, "RESB") == 0){
				counter = atoi(operand);
				if(pass == 2) fprintf(fp_lst, "\n");
			} else if(strcmp(mnemonic, "BYTE") == 0){
				char type = operand[0];
				char * byte_ary;
				int operand_len = strlen(operand);
				int pass_quote_flag = 0;
				for(int idx=1; idx<operand_len; idx++){
					if(operand[idx] == '\''){
						operand[idx] = '\0';
						if(pass_quote_flag == 0){
							pass_quote_flag = 1;
							byte_ary = &operand[idx+1];
						}
					}
				}
				if(type == 'C'){
					counter = strlen(byte_ary);
					if(pass == 2){
						for(int i = 0; i < strlen(byte_ary); i++){
							if(pass == 2) {
								fprintf(fp_lst, "%02X", byte_ary[i]);
								if(record->len == 0) record->start = locctr;
								sprintf(tmp_content, "%02X", byte_ary[i]);
								strcat(record->content, tmp_content);
							}
						}
						if(pass == 2) fprintf(fp_lst, "\n");
					}
				} else if(type == 'X'){
					counter = strlen(byte_ary)/2;
					if(pass == 2){
						if(counter == 1){
							if(pass == 2) {
								fprintf(fp_lst, "%02X\n", (int)strtol(byte_ary, NULL, 16));
								if(record->len == 0) record->start = locctr;
								sprintf(tmp_content, "%02X", (int)strtol(byte_ary, NULL, 16));
								strcat(record->content, tmp_content);
							}
						} else if(counter == 2){
							if(pass == 2) {
								fprintf(fp_lst, "%04X\n", (int)strtol(byte_ary, NULL, 16));
								if(record->len == 0) record->start = locctr;
								sprintf(tmp_content, "%04X", (int)strtol(byte_ary, NULL, 16));
								strcat(record->content, tmp_content);
							}			
						} else if(counter == 3){
							if(pass == 2) {
								fprintf(fp_lst, "%06X\n", (int)strtol(byte_ary, NULL, 16));
								if(record->len == 0) record->start = locctr;
								sprintf(tmp_content, "%06X", (int)strtol(byte_ary, NULL, 16));
								strcat(record->content, tmp_content);
							}
						}
					}
				} else {
					printf("line %d: Can't understand the type of BYTE", line);
					return 1;
				}
			}
			record->len = strlen(record->content)/2;
			if(record->len >= 0x1D){
				record->end = locctr + counter;
				appendList(record_list, record);
				record = (Record*)malloc(sizeof(Record));
			}
			locctr += counter;
		}
		fclose(fp_asm);
		if(pass == 1) ending_address = locctr;
	}
	record->end = locctr + counter;
	appendList(record_list, record);
	record = (Record*)malloc(sizeof(Record));			
	
	Node* ptr = NULL;
	for (ptr = record_list -> head; ptr != NULL; ptr = ptr -> link) {
		switch(((Record*)(ptr->data))->type){
			case HEADER:
				fprintf(fp_obj, "H%s\t%06X%06X\n", ((Record*)(ptr->data))->content, ((Record*)(ptr->data))->start, ((Record*)(ptr->data))->end);
				break;
			case TEXT:
				fprintf(fp_obj, "T%06X%02X%s\n", ((Record*)(ptr->data))->start, ((Record*)(ptr->data))->len, ((Record*)(ptr->data))->content);
				break;
		}
	}

	for(int idx = 0 ; idx < m_size ; idx++){
		fprintf(fp_obj, "M%06X05\n", modified_addr_ary[idx]);
	}
	fprintf(fp_obj, "E%06X\n", starting_address);
	fclose(fp_lst);
	fclose(fp_obj);
	
	return 0;	
}

/*
 * Function: type_file
 * 
 * read file line by line
 * 
 * filename: filename string
 * return: Success
 * (0: success, -1: failure) 
 */
int type_file(char * filename){
	printf("%s\n", filename);
	FILE *fptr; 
 	char c; 
 	// Open file 
	fptr = fopen(filename, "r"); 
	if (fptr == NULL) 
	{ 
		printf("Cannot open file \n"); 
		return 1;
	} 

	// Read contents from file 
	c = fgetc(fptr); 
	while (c != EOF) 
	{ 
		printf ("%c", c); 
		c = fgetc(fptr); 
	}
	fclose(fptr); 
	return 0; 
}

/*
 * Function: display_help
 * 
 * display commands
 * 
 * return: Success (not that important)
 * (0: success) 
 */
int display_help(void) {
	printf("h[elp]\n");
	printf("d[ir]\n");
	printf("q[uit]\n");
	printf("hi[story]\n");
	printf("du[mp] [start, end]\n");
	printf("e[dit] address, value\n");
	printf("f[ill] start, end, value\n");
	printf("reset\n");
	printf("opcode mnemonic\n");
	printf("opcodelist\n");
	printf("assemble filename\n");
	printf("type filename\n");
	printf("symbol\n");
	return 0;
}

/*
 * Function: display_dir
 * 
 * display directory files
 * 
 * return: Success (not that important)
 * (0: success) 
 */
int display_dir(void) {
	DIR *d;
	struct stat buf;
	struct dirent *dir;
	d = opendir(".");
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			stat(dir->d_name, &buf);
			if (S_ISDIR(buf.st_mode)) {
				// directory
				printf("%s/\n", dir->d_name);
			} else if (buf.st_mode & S_IXUSR) {
				// executable file
				printf("%s*\n", dir->d_name);
			}
			else {
				printf("%s\n", dir->d_name);
			}
		}
		closedir(d);
	}
	return 0;
}

/*
 * Function: hexDump
 * 
 * dump 10 lines
 * 
 * last_addr: last address
 * return: current address
 */
int hexDump(int last_addr)
{
	int cur_addr;
	unsigned char buff[17];
	unsigned char *pc = (unsigned char*)addr;
	for(cur_addr= last_addr - last_addr%16; cur_addr < last_addr; cur_addr++){
		if((cur_addr%16) == 0){
			printf("  %05X ", cur_addr);
		}
		buff[cur_addr%16] = '.';
		printf("   ");
	}	
	// process every byte in the data
	for (cur_addr = last_addr; cur_addr < 160 + last_addr; cur_addr++) {
		// new line 
		if ((cur_addr % 16) == 0) {
			// don't print ASCII code for the zeroth line
			if (cur_addr != last_addr)
				printf(" ; %s\n", buff);

			// output the offset
			printf("  %05X ", cur_addr);
		}

		// the hex code for the specific character
		printf(" %02X", pc[cur_addr]);
		
		// store a printable ASCII character(0x20 ~ 0x7E)
		if ((pc[cur_addr] < 0x20) || (pc[cur_addr] > 0x7E)) {
			buff[cur_addr % 16] = '.';
		}
		else {
			buff[cur_addr % 16] = pc[cur_addr];
		}

		buff[(cur_addr % 16) + 1] = '\0';
	}
	if(cur_addr%16 != 0){	
		for(int j = last_addr%16; j < 16; j++){
			buff[j%16] = '.';
			printf("   ");
		}	
	}

	// print the final ASCII bit
	printf(" ; %s\n", buff);
	return cur_addr;
}

/*
 * Function: hexDumpWithStart
 * 
 * dump 10 lines from start address
 * 
 * start: start address
 * return: current address
 */
int hexDumpWithStart(int start)
{
	int cur_addr;
	unsigned char buff[17] = {"."};
	unsigned char *pc = (unsigned char*)addr;
	for(cur_addr= start - start%16; cur_addr < start; cur_addr++){
		if((cur_addr%16) == 0){
			printf("  %05X ", cur_addr);
		}
		buff[cur_addr%16] = '.';
		printf("   ");
	
	}
	// process every byte in the data
	for (cur_addr = start; cur_addr < 160 + start; cur_addr++) {
		// address can't exceed 0xFFFFF
		if(cur_addr>0xFFFFF){
			break;
		}
		// new line
		if ((cur_addr % 16) == 0) {
			// don't print ASCII code for the zeroth line
			if (cur_addr != start)
				printf(" ; %s\n", buff);

			// output the offset
			printf("  %05X ", cur_addr);
		}

		// the hex code for the specific character
		printf(" %02X", pc[cur_addr]);

		// store a printable ASCII character(0x20 ~ 0x7E)
		if ((pc[cur_addr] < 0x20) || (pc[cur_addr] > 0x7e)) {
			buff[cur_addr % 16] = '.';
		}
		else {
			buff[cur_addr % 16] = pc[cur_addr];
		}

		buff[(cur_addr % 16) + 1] = '\0';
	}
	if(cur_addr%16!=0){
		for(int j = start%16; j < 16; j++){
			buff[j%16] = '.';
			printf("   ");
		}	
	}
	// print the final ASCII bit
	printf(" ; %s\n", buff);
	return cur_addr;
}

/*
 * Function: hexDumpWithStart
 * 
 * dump from start address to end address 
 * 
 * start: start address
 * end: end address
 * return: current address
 */
int hexDumpWithStartEnd(int start, int end)
{
	int cur_addr;
	unsigned char buff[17] = {"."};
	unsigned char *pc = (unsigned char*)addr;
	for(cur_addr= start - start%16; cur_addr < start; cur_addr++){
		if((cur_addr%16) == 0){
			printf("  %05X ", cur_addr);
		}
		buff[cur_addr%16] = '.';
		printf("   ");
	
	}
	// process every byte in the data
	for (cur_addr = start; cur_addr < end; cur_addr++) {
		// new line

		if ((cur_addr % 16) == 0) {
			// don't print ASCII code for the zeroth line.
			if (cur_addr != start){
				printf(" ; %s\n", buff);
			}

			// output the offset
			printf("  %05X ", cur_addr);
		}

		// the hex code for the specific character
		printf(" %02X", pc[cur_addr]);

		// store a printable ASCII character(0x20 ~ 0x7E)
		if ((pc[cur_addr] < 0x20) || (pc[cur_addr] > 0x7e)) {
			buff[cur_addr % 16] = '.';
		}
		else {
			buff[cur_addr % 16] = pc[cur_addr];
		}

		buff[(cur_addr % 16) + 1] = '\0';
	}
	for(int j = end%16; j < 16; j++){
		buff[j%16] = '.';
		printf("   ");
	}	

	// print the final ASCII bit
	printf(" ; %s\n", buff);
	return cur_addr;
}

/*
 * Function: edit
 * 
 * edit the data of target address
 * 
 * target_address: target address
 * data: data editing 
 * return: success ( 0: success )
 */
int edit(int target_address, int data) {
	((unsigned char*)addr)[target_address] = data;
	return 0;
}

/*
 * Function: fill
 * 
 * fill out from start address to end address with the data 
 * 
 * start: start address
 * end: end address
 * data: data editing
 * return: success
 * (0: success)
 */
int fill(int start, int end, int data){
	unsigned char *pc = (unsigned char*)addr;	
	for (int i = start; i <= end; i++) {
		pc[i] = data;
	}
	return 0;
}

/*
 * Function: insert_symbol_table
 * 
 * insert to symbol table
 * 
 * symbol: the symbol to insert on symbol table
 * return: success
 * (0: insert success, 1: symbol already exist)
 */
int insert_external_symbol_table(struct ExternalSymbol external_symbol){
	// insert to symbol linked list
	symbol_ctr ++;

	// insert to symbol table
	unsigned char *key, h;
	struct ExternalSymbolRecord *temp;
	key = (unsigned char*)external_symbol.name;

	// skip it if inst already exist 
	if (search_element_symbol_table(key) != -1)
	{
		return 1;
	}
	h = hash_function(key, HASH_TABLE_MAX);
	printf("hash: %d", h);
	temp = malloc(sizeof(struct ExternalSymbolRecord));
	temp->data = external_symbol;
	temp->link = external_symbol_table[h];
	external_symbol_table[h] = temp;
	return 0;
}

/*
 * Function: insert_symbol_table
 * 
 * insert to symbol table
 * 
 * symbol: the symbol to insert on symbol table
 * return: success
 * (0: insert success, 1: symbol already exist)
 */
int insert_symbol_table(struct Symbol symbol){
	// insert to symbol linked list
	symbol_ctr ++;

	// insert to symbol table
	unsigned char *key, h;
	struct SymbolRecord *temp;
	key = (unsigned char*)symbol.symbol;

	// skip it if inst already exist 
	if (search_element_symbol_table(key) != -1)
	{
		return 1;
	}
	h = hash_function(key, SYMBOL_TABLE_MAX);
	temp = malloc(sizeof(struct SymbolRecord));
	temp->data = symbol;
	temp->link = symbol_table[h];
	symbol_table[h] = temp;

	S = (Symbol *)realloc(S, sizeof(Symbol) * (symbol_ctr));
	strcpy(S[symbol_ctr-1].symbol, temp->data.symbol);
	S[symbol_ctr-1].loc = temp->data.loc;
	return 0;
}

/*
 * Function: insert_hash_table
 * 
 * insert to hash table
 * 
 * inst: Instruction
 * return: success
 * (0: insert success, 1: instruction already exist)
 */
int insert_hash_table(struct Inst inst)
{
	unsigned char *key, h;
	struct HashRecord *temp;
	key = (unsigned char*)inst.mnemonic;

	// skip it if inst already exist 
	if (search_element_hash_table(key) != -1)
	{
		return 1;
	}
	h = hash_function(key, HASH_TABLE_MAX);
	temp = malloc(sizeof(struct HashRecord));
	temp->data = inst;
	temp->link = hash_table[h];
	hash_table[h] = temp;
	return 0;
}

/*
 * Function: search_element_external_symbol_table
 * 
 * search position of element
 * 
 * key: symbol name
 * return: position on symbol table or error(-1)
 */
int search_element_external_symbol_table(unsigned char * key)
{
	int h;
	struct ExternalSymbolRecord *ptr;
	h = hash_function(key, HASH_TABLE_MAX);
	ptr = external_symbol_table[h];
	while (ptr != NULL)
	{
		if (strcmp(ptr->data.name, (char *)key) == 0)
		{
			return ptr->data.address;
		}
		ptr = ptr->link;
	}
	return -1;
}

/*
 * Function: search_element_symbol_table
 * 
 * search position of element
 * 
 * key: symbol name
 * return: position on symbol table or error(-1)
 */
int search_element_symbol_table(unsigned char * key)
{
	int h;
	struct SymbolRecord *ptr;
	h = hash_function(key, SYMBOL_TABLE_MAX);
	ptr = symbol_table[h];
	while (ptr != NULL)
	{
		if (strcmp(ptr->data.symbol, (char *)key) == 0)
		{
			return h;
		}
		ptr = ptr->link;
	}
	return -1;
}

/*
 * Function: search_element_hash_table
 * 
 * search position of element
 * 
 * key: mnemonic
 * return: position on hash table or error(-1)
 */
int search_element_hash_table(unsigned char * key)
{
	int h;
	struct HashRecord *ptr;
	h = hash_function(key, HASH_TABLE_MAX);
	ptr = hash_table[h];
	while (ptr != NULL)
	{
		if (strcmp(ptr->data.mnemonic, (char *)key) == 0)
		{
			return h;
		}
		ptr = ptr->link;
	}
	return -1;
}

/*
 * Function: get_loc_by_symbol
 * 
 * get location counter by symbol
 * 
 * key: symbol
 * return: location counter or error(-1)
 */
int get_loc_by_symbol(unsigned char * key){
	int h;
	struct SymbolRecord *ptr;
	h = hash_function(key, SYMBOL_TABLE_MAX);
	ptr = symbol_table[h];
	while (ptr != NULL){
		if(strcmp(ptr->data.symbol, (char *)key) == 0){
			return ptr->data.loc;
		}
		ptr = ptr->link;
	}
	return -1;
}

Inst* get_inst_by_opcode(int opcode){
	struct HashRecord *ptr;
	for(int h=0; h < HASH_TABLE_MAX ; h++){
		ptr = hash_table[h];
		while (ptr != NULL)
		{
			if (ptr->data.opcode == opcode)
			{
				return &(ptr->data);
			}
			ptr = ptr->link;
		}
	}
	return NULL;
}
/*
 * Function: get_loc_by_symbol
 * 
 * get opcode by key
 * 
 * key: mnemonic
 * return: opcode or error(-1)
 */
int get_opcode_by_key(unsigned char * key){
	int h;
	struct HashRecord *ptr;
	h = hash_function(key, HASH_TABLE_MAX);
	ptr = hash_table[h];
	while (ptr != NULL){
		if(strcmp(ptr->data.mnemonic, (char *)key) == 0){
			return ptr->data.opcode;
		}
		ptr = ptr->link;
	}
	return -1;
}

/*
 * Function: get_format_by_key
 * 
 * get format by key
 * 
 * key: mnemonic
 * return: format or error("")
 */
char* get_format_by_key(unsigned char * key){
	int h;
	struct HashRecord *ptr;
	h = hash_function(key, HASH_TABLE_MAX);
	ptr = hash_table[h];
	while (ptr != NULL){
		if(strcmp(ptr->data.mnemonic, (char *)key) == 0){
			return ptr->data.format;
		}
		ptr = ptr->link;
	}
	return "";
}

/*
 * Function: opcodelist
 * 
 * print hash table of opcode list
 * 
 * return: success (not that important)
 * (0: success)
 */
int opcodelist() {
	for (int count = 0; count < HASH_TABLE_MAX; count++) {
		printf("%02d : ", count);
		struct HashRecord *ptr = hash_table[count];
		int start_flag = 1;
		while (ptr != NULL) {
			if (start_flag) {
				printf("[%s,%02X]", ptr->data.mnemonic, ptr->data.opcode);
				start_flag = 0;
			}
			else {
				printf(" -> [%s,%02X]", ptr->data.mnemonic, ptr->data.opcode);
			}
			ptr = ptr->link;
		}
		printf("\n");
	}
	return 0;
}

/*
 * Function: print_symbol_table
 * 
 * print symbol table 
 * 
 * return: success (not that important)
 * (0: success)
 */
int print_symbol_table(void) {

    qsort(S, symbol_ctr, sizeof(struct Symbol), myCompare); 

	for(int count = 0; count < symbol_ctr; count++) {
		printf("\t%s\t%04X\n", S[count].symbol, S[count].loc);
	}
	return 0;
}

/*
 * Function: myCompare
 * 
 * define comparator function
 * 
 * a: Symbol a
 * b: Symbol b
 * return: comparison result
 * (0: same, 1: a.symbol > b.symbol, -1: a.symbol < b.symbol)
 */
int myCompare(const void* a,const void* b) 
{ 
	struct Symbol * symbol_a = (struct Symbol *)a;
	struct Symbol * symbol_b = (struct Symbol *)b;
    // setting up rules for comparison 
    return strcmp((*symbol_b).symbol, (*symbol_a).symbol); 
} 

/*
 * Function: hash_function
 * 
 * calculate hash function of string by djb2 algorithm of Dan Bernstein
 * 
 * key: key string
 * length: maximun length of hash table
 * return: hash
 */
int hash_function(unsigned char * key, int length) {
	unsigned long hash = 5381;
	int c;
    	while ((c = *key++)){
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	return hash%length;
}

/*
 * Function: main
 * 
 * sic/xe machine simulator
 * 
 * return: program exit (not that important)
 * (0: success) 
 */
int main(void) {
	char input[MAX] = "";
	// init symbol table
	for (int count = 0; count <= SYMBOL_TABLE_MAX - 1; count++)
	{
		symbol_table[count] = NULL;
	}
	// init hash table
	for (int count = 0; count <= HASH_TABLE_MAX - 1; count++)
	{
		hash_table[count] = NULL;
	}
	// init external symbol table
	for (int count = 0; count <= HASH_TABLE_MAX - 1; count++){
		external_symbol_table[count] = NULL;
	}
	if(loadOpcodelist() == -1){
		return 0;
	}

	// start sic/xe machine simulator
	while (1) {
		printf("sicsim> ");
		fgets(input, sizeof(input), stdin);
		if(processCmd(input) == -1) break;	
	}

	// quit sic/xe machine
	return 0;
}

