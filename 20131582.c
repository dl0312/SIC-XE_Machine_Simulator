#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "20131582.h"
#define	MAX	100
#define HASH_TABLE_MAX 20

int main(void) {
	// create hash table
	struct Record *hash_table[HASH_TABLE_MAX];
	struct Inst inst_record;

	// init hash table
	for (int count = 0; count <= HASH_TABLE_MAX - 1; count++)
	{
		hash_table[count] = NULL;
	}

	char input[MAX] = "";

	// save input for history
	char tmp_input[MAX] = "";

	// init dump addresses
	unsigned char addr[16*65536] = { 0 };
	int last_addr = 0;

	// dynamic allocate LinkedList of history
	LinkedList *L = (LinkedList *)malloc(sizeof(LinkedList));
	L->cur = NULL;
	L->head = NULL;
	L->tail = NULL;

	// file input stream
	FILE *ptr_file;

	// open file with readonly
	ptr_file = fopen("opcode.txt", "r");
	if (!ptr_file)
		return 0;

	// read each line and get 
	while (!feof(ptr_file)) {
		// fill out hash table with opcode.txt
		fscanf(ptr_file, "%x %s %s", &inst_record.opcode, inst_record.mnemonic, inst_record.format);
		//printf("%x %s %s\n", inst_record.opcode, inst_record.mnemonic, inst_record.format);
		insert(inst_record, hash_table);
	}
	
	// close file stream
	fclose(ptr_file);

	while (1) {

		// reset cmd, args
		char *cmd;
		char *arg1 = NULL;
		char *arg2 = NULL;
		char *arg3 = NULL;

		// command input
		printf("sicsim> ");
		fgets(input, sizeof(input), stdin);
		strcpy(tmp_input, input);

		// trim tmp input	
		tmp_input[strcspn(tmp_input, "\n")] = '\0';
		
		// flag for input state
		int flag = 0;

		// check wrong command
		int wrong_input_flag = 0;
		int len = strlen(input);
		cmd = input;
		for (int i = 0; i < len; i++) {
			if (flag == 0) {
				// input cmd
				if (input[i] > 96 && input[i] < 123) {
					continue;
				} else if (input[i] == ' ' || input[i] == '\t') {
					input[i] = '\0';
					flag++;
				} else if(input[i] == '\n'){
					input[i] = '\0';
					break;
				}
				else {
					wrong_input_flag = 1;
					break;
				}
			}
			else if (flag == 1){
				// empty space
				if (input[i] == ' ' || input[i] == '\t') {
					continue;
				}
				else {
					arg1 = &input[i];
					flag++;
				}
			}
			else if (flag == 2){
				// input arg1
				if (input[i] == ' ' || input[i] == '\t') {
					input[i] = '\0';
					flag++;
				} else if (input[i] == '\n'){
					input[i] = '\0';
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
				}
			}
			else if (flag == 5) {
				// input arg2
				if (input[i] == ' ' || input[i] == '\t') {
					input[i] = '\0';
					flag++;
				} else if (input[i] == ',') {
					input[i] = '\0';
					flag += 2;
				} else if(input[i] == '\n'){
					input[i] = '\0';
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
				}
			}
			else if (flag == 8) {
				// input arg3
				if (input[i] == ' ' || input[i] == '\t' || input[i] == '\n') {
					input[i] = '\0';
					flag++;
				}
				else if (input[i] == ',') {
					input[i] = '\0';
					flag += 2;
				}
			}
		}
		
		if(wrong_input_flag == 1){
			// wrong command
			printf("wrong command\n");
			printf("if you want to know the commands h[elp]\n");
			continue;
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
			break;
		}
		else if (strcmp(cmd, "hi") == 0 || strcmp(cmd, "history") == 0) {
			// hi[story]
			createNode(L, tmp_input);
			printNodes(L);
			continue;
		}
		else if (strcmp(cmd, "du") == 0 || strcmp(cmd, "dump") == 0) {
			if(arg1==NULL){
				if(last_addr<=0xFFF60){
					// du[mp] 
					last_addr = hexDump(last_addr, &addr);
				} else {
					// segmentation fault
					printf("segmentation fault\n");
					last_addr = 0;
				}
			} else {
				int hex_arg1 = (int)strtol(arg1, NULL, 16);
				if(arg2==NULL){
					// du[mp] start
					last_addr = hexDumpWithStart(hex_arg1,&addr);
				}else{
					int hex_arg2 = (int)strtol(arg2, NULL, 16);
					if((hex_arg1 <= hex_arg2) && arg3==NULL){
						// du[mp] start, end
						if(hex_arg2 <= 0xFFFFF){
							last_addr = hexDumpWithStartEnd(hex_arg1, hex_arg2, &addr);
						} else {
							printf("segmentation fault\n");
						}
					}else{
						printf("end address is lower than start address\n");
						continue;
					}
				}
			}
		} else if (strcmp(cmd, "e") == 0 || strcmp(cmd, "edit") == 0) {
			// e[dit] address, data
			if(arg1!=NULL && arg2!=NULL){
				edit(atoi(arg1), (int)strtol(arg2, NULL, 16), &addr);
			}
		} else if(strcmp(cmd, "f") == 0 || strcmp(cmd, "fill") == 0){
			// f[ill] start, end, data
			if((arg1!=NULL && arg2!=NULL) && arg3!=NULL){
				int hex_arg1 = (int)strtol(arg1, NULL, 16);
				int hex_arg2 = (int)strtol(arg2, NULL, 16);
				int hex_arg3 = (int)strtol(arg3, NULL, 16);
				fill(hex_arg1, hex_arg2, hex_arg3, &addr);
			}
		} else if (strcmp(cmd, "reset") == 0){
			// reset
			memset(addr, 0, sizeof(addr));
		} else if (strcmp(cmd, "opcode") == 0){
			if(arg1!=NULL){
				// opcode mnemonic
				get_opcode_by_key((unsigned char*)arg1, hash_table);
			}
		} else if (strcmp(cmd, "opcodelist") == 0) {
			// opcodelist
			opcodelist(hash_table);
		} else {
			// wrong command
			printf("wrong command\n");
			printf("if you want to know the commands h[elp]\n");
			continue;
		}
		// create node on history linked list
		createNode(L, tmp_input);
	}

	// quit machine
	return 0;
}

/* display commands */
void display_help(void) {
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
}

/* display directory files */
void display_dir(void) {
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
}

/* create node on history linked list */
void createNode(LinkedList *L, char cmd[]) {
	Node *newNode = (Node *)malloc(sizeof(Node));
	newNode->data = strdup(cmd);
	newNode->next = NULL;
	if (L->head == NULL && L->tail == NULL)
		L->head = L->tail = newNode;
	else {
		L->tail->next = newNode;
		L->tail = newNode;
	}
}

/* print each nodes on history linked list */
void printNodes(LinkedList *L) {
	Node *p = L->head;
	int cnt = 1;
	while (p != NULL) {
		printf("%d\t", cnt);
		printf("%s\n", p->data);
		p = p->next;
		cnt++;
	}
}

/* dump 10 lines */
int hexDump(int last_addr, void *addr)
{
	int i;
	unsigned char buff[17];
	unsigned char *pc = (unsigned char*)addr;
	for(i= last_addr - last_addr%16; i < last_addr; i++){
		if((i%16) == 0){
			printf("  %05X ", i);
		}
		buff[i%16] = '.';
		printf("   ");
	}	
	// process every byte in the data
	for (i = last_addr; i < 160 + last_addr; i++) {
		// new line 
		if ((i % 16) == 0) {
			// don't print ASCII code for the zeroth line
			if (i != last_addr)
				printf(" ; %s\n", buff);

			// output the offset
			printf("  %05X ", i);
		}

		// the hex code for the specific character
		printf(" %02X", pc[i]);
		
		// store a printable ASCII character(0x20 ~ 0x7E)
		if ((pc[i] < 0x20) || (pc[i] > 0x7E)) {
			buff[i % 16] = '.';
		}
		else {
			buff[i % 16] = pc[i];
		}

		buff[(i % 16) + 1] = '\0';
	}
	if(i%16 != 0){	
		for(int j = last_addr%16; j < 16; j++){
			buff[j%16] = '.';
			printf("   ");
		}	
	}

	// print the final ASCII bit
	printf(" ; %s\n", buff);
	return i;
}

/* dump 10 lines from start address */
int hexDumpWithStart(int start, void *addr)
{
	int i;
	unsigned char buff[17] = {"."};
	unsigned char *pc = (unsigned char*)addr;
	for(i= start - start%16; i < start; i++){
		if((i%16) == 0){
			printf("  %05X ", i);
		}
		buff[i%16] = '.';
		printf("   ");
	
	}
	// process every byte in the data
	for (i = start; i < 160 + start; i++) {
		// address can't exceed 0xFFFFF
		if(i>0xFFFFF){
			break;
		}
		// new line
		if ((i % 16) == 0) {
			// don't print ASCII code for the zeroth line
			if (i != start)
				printf(" ; %s\n", buff);

			// output the offset
			printf("  %05X ", i);
		}

		// the hex code for the specific character
		printf(" %02X", pc[i]);

		// store a printable ASCII character(0x20 ~ 0x7E)
		if ((pc[i] < 0x20) || (pc[i] > 0x7e)) {
			buff[i % 16] = '.';
		}
		else {
			buff[i % 16] = pc[i];
		}

		buff[(i % 16) + 1] = '\0';
	}
	if(i%16!=0){
		for(int j = start%16; j < 16; j++){
			buff[j%16] = '.';
			printf("   ");
		}	
	}
	// print the final ASCII bit
	printf(" ; %s\n", buff);
	return i;
}

/* dump from start address to end address */
int hexDumpWithStartEnd(int start, int end, void *addr)
{
	int i;
	unsigned char buff[17] = {"."};
	unsigned char *pc = (unsigned char*)addr;
	for(i= start - start%16; i < start; i++){
		if((i%16) == 0){
			printf("  %05X ", i);
		}
		buff[i%16] = '.';
		printf("   ");
	
	}
	// process every byte in the data
	for (i = start; i <= end; i++) {
		// new line

		if ((i % 16) == 0) {
			// don't print ASCII code for the zeroth line.
			if (i != 0)
				printf(" ; %s\n", buff);

			// output the offset
			printf("  %05X ", i);
		}

		// the hex code for the specific character
		printf(" %02X", pc[i]);

		// store a printable ASCII character(0x20 ~ 0x7E)
		if ((pc[i] < 0x20) || (pc[i] > 0x7e)) {
			buff[i % 16] = '.';
		}
		else {
			buff[i % 16] = pc[i];
		}

		buff[(i % 16) + 1] = '\0';
	}
	for(int j = end%16 +1; j < 16; j++){
		buff[j%16] = '.';
		printf("   ");
	}	

	// print the final ASCII bit
	printf(" ; %s\n", buff);
	return i;
}

/* edit the data of target address */
void edit(int target_address, int data, void *addr) {
	//unsigned char *pc = (unsigned char*)addr;
	((unsigned char*)addr)[target_address] = data;
}

/* fill the data from start address to end address */
void fill(int start, int end, int data, void *addr){
	unsigned char *pc = (unsigned char*)addr;	
	for (int i = start; i <= end; i++) {
		pc[i] = data;
	}
}

/* insert to hash table */
void insert(struct Inst inst_record, struct Record *hash_table[])
{
	unsigned char *key, h;
	struct Record *temp;
	key = (unsigned char*)inst_record.mnemonic;

	// skip it if inst already exist 
	if (search_element(key, hash_table) != -1)
	{
		return;
	}
	h = hash_function(key);
	temp = malloc(sizeof(struct Record));
	temp->data = inst_record;
	temp->link = hash_table[h];
	hash_table[h] = temp;
}

/* search position of element */
int search_element(unsigned char * key, struct Record *hash_table[])
{
	int h;
	struct Record *ptr;
	h = hash_function(key);
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

/* get opcode by key */
void get_opcode_by_key(unsigned char * key, struct Record *hash_table[]){
	int h;
	struct Record *ptr;
	h = hash_function(key);
	ptr = hash_table[h];
	while (ptr != NULL){
		if(strcmp(ptr->data.mnemonic, (char *)key) == 0){
			printf("opcode is %02X\n", ptr->data.opcode);
		}
		ptr = ptr->link;
	}
}

/* print hash table of opcode list */
void opcodelist(struct Record *hash_table[]) {
	for (int count = 0; count < HASH_TABLE_MAX; count++) {
		printf("%02d : ", count);
		struct Record *ptr = hash_table[count];
		int start_flag = 1;
		while (ptr != NULL && ptr->link != NULL) {
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
}

/* calculate hash function of string by djb2 algorithm of Dan Bernstein */
int hash_function(unsigned char * key) {
	unsigned long hash = 5381;
	int c;
    	while ((c = *key++)){
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	return hash%HASH_TABLE_MAX;
}
