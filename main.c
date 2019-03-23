#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define	MAX	100
#define HASH_TABLE_MAX 20

typedef struct LinkedList {
	struct Node *cur;
	struct Node *head;
	struct Node *tail;
} LinkedList;

typedef struct Node {
	char *data;
	struct Node *next;
} Node;

typedef struct Inst {
	int opcode;
	char mnemonic[10];
	char format[10];
} Inst;

typedef struct Record {
	struct Inst data;
	struct Record *link;
} Record;


int main(void);

// h[elp] command
void display_help(void);
void display_dir(void);

// history linked list
void createNode(LinkedList * L, char cmd[]);
void printNodes(LinkedList * L);

// hex dump
void hexDump(void * addr);
void hexDumpWithStart(int start, void * addr);
void hexDumpWithStartEnd(int start, int end, void * addr);


void insert(struct Inst inst_record, struct Record *hash_table[]);
int search_element(int key, struct Record *hash_table[]);
void remove_record(int key, struct Record *hash_table[]);
void show(struct Record *hash_table[]);
void opcodelist(struct Record *hash_table[]);
int hash_function(int key);
void edit(int traget_address, int data, void *addr);


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

	char *cmd;
	char *arg1 = NULL;
	char *arg2 = NULL;
	char *arg3;

	// init dump addresses
	char addr[160] = { 0 };
	char *ptr;

	// dynamic allocate LinkedList of history
	LinkedList *L = (LinkedList *)malloc(sizeof(LinkedList));
	L->cur = NULL;
	L->head = NULL;
	L->tail = NULL;

	// file input stream
	FILE *ptr_file;
	int opcode;
	char mnemonic[10];
	char format[10];

	// open file with readonly
	ptr_file = fopen("opcode.txt", "r");
	if (!ptr_file)
		return 0;

	// read each line and get 
	while (!feof(ptr_file)) {
		// fill out hash table with opcode.txt
		fscanf(ptr_file, "%x %s %s", &inst_record.opcode, inst_record.mnemonic, inst_record.format);
		// printf("%x %s %s\n", inst_record.opcode, inst_record.mnemonic, inst_record.format);
		insert(inst_record, hash_table);
	}
	
	// close file stream
	fclose(ptr_file);

	while (1) {

		// command input
		printf("sicsim> ");
		fgets(input, sizeof(input), stdin);
		int flag = 0;
		int len = strlen(input);
		cmd = input;
		for (int i = 0; i < len; i++) {
			if (flag == 0) {
			// 첫 인자
				if (input[i] > 96 && input[i] < 123) {
					continue;
				} else if (input[i] == ' ' || input[i] == '\t') {
					printf("%d", i);
					input[i] = '\0';
					flag++;
				}
				else {
					printf("wrong");
				}
			}
			else if (flag == 1){
			// 빈공간
				if (input[i] == ' ' || input[i] == '\t') {
					continue;
				}
				else {
					printf("%d", i);
					arg1 = &input[i];
					flag++;
				}
			}
			else if (flag == 2){
				// 두번째 인자 
				if (input[i] == ' ' || input[i] == '\t') {
					input[i] = '\0';
					flag++;
				}
				else if (input[i] == ',') {
					input[i] = '\0';
					flag += 2;
				}
			}
			else if (flag == 3) {
			// 빈공간 ','이전
				if (input[i] == ' ' || input[i] == '\t') {
					input[i] = '\0';
				}
				else if (input[i] == ',') {
					input[i] = '\0';
					flag ++;
				}
				else {
					printf("wrong");
				}
			}
			else if (flag == 4) {
				// 빈공간 ','이후
				if (input[i] == ' ' || input[i] == '\t') {
					input[i] = '\0';
				}
				else {
					arg2 = &input[i];
					flag++;
				}
			}
			else if (flag == 5) {
				// 세번째 인자 
				if (input[i] == ' ' || input[i] == '\t') {
				// 세번째 인자 
				if (input[i] == ' ' || input[i] == '\t') {
					input[i] = '\0';
					flag++;
				}
				else if (input[i] == ',') {
					input[i] = '\0';
					flag += 2;
				}
			}
			else if (flag == 6) {
				// 빈공간 ','이전
				if (input[i] == ' ' || input[i] == '\t') {
					input[i] = '\0';
				}
				else if (input[i] == ',') {
					input[i] = '\0';
					flag++;
				}
				else {
					printf("wrong");
				}
			}
			else if (flag == 7) {
				// 빈공간 ','이후
				if (input[i] == ' ' || input[i] == '\t') {
					input[i] = '\0';
				}
				else {
					arg3 = &input[i];
					flag++;
				}
			}
			else if (flag == 8) {
				// 네번째 인자 
				if (input[i] == ' ' || input[i] == '\t') {
					input[i] = '\0';
					flag++;
				}
				else if (input[i] == ',') {
					input[i] = '\0';
					flag += 2;
				}
			}
		}
		
		//printf("%d\n", len);
		printf("command: %s, argument1: %s, argument2: %s\n", cmd, arg1, arg2);
		printf("%s %s strcmp: %d", cmd, "q\n", strcmp(cmd, "q\n"));		
		
		if (strcmp(cmd, "h\n") == 0 || strcmp(cmd, "help\n") == 0) {
			// h[elp]
			display_help();
		}
		else if (strcmp(cmd, "d\n") == 0 || strcmp(cmd, "dir\n") == 0) {
			// d[ir]
			display_dir();
		}
		else if (strcmp(cmd, "q\n") == 0 || strcmp(cmd, "quit\n") == 0) {
			// q[uit]
			printf("quit shell\n");
			break;
		}
		else if (strcmp(cmd, "hi\n") == 0 || strcmp(cmd, "history\n") == 0) {
			// hi[story]
			createNode(L, cmd);
			printNodes(L);
			continue;
		}
		else if (strcmp(cmd, "du\n") == 0 || strcmp(cmd, "dump\n") == 0) {
			// du[mp]
			hexDump(&addr);
		}
		else if (strcmp(cmd, "e\n") == 0 || strcmp(cmd, "edit\n") == 0) {
			// e[dit] address, data
			edit((int)(arg1 - '0'), (int)((arg2[0] - '0') * 16 + (arg2[1] - '0')) , &addr);
		}
		else if (strcmp(cmd, "opcodelist\n") == 0) {
			// opcodelist
			opcodelist(hash_table);
		}
			else {
				// wrong command
				printf("wrong command\n");
				printf("if you want to know the commands h[elp]\n");
				continue;
			}
			createNode(L, cmd);
		}
	}
	return 0;
}

void display_help(void) {
	printf("\nh[elp]");
	printf("\nd[ir]");
	printf("\nq[uit]");
	printf("\nhi[story]");
	printf("\ndu[mp] [start, end]");
	printf("\ne[dit] address, value");
	printf("\nf[ill] start, end, value");
	printf("\nreset");
	printf("\nopcode mnemonic");
	printf("\nopcodelist\n");
}

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
				printf("%s/\n", dir->d_name);
			} else if (buf.st_mode & S_IXUSR) {
				printf("%s*\n", dir->d_name);
			}
			else {
				printf("%s\n", dir->d_name);
			}
		}
		closedir(d);
	}
	return(0);
}

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

// pirnt linked list history
void printNodes(LinkedList *L) {
	Node *p = L->head;
	int cnt = 1;
	while (p != NULL) {
		printf("%d\t", cnt);
		printf("%s", p->data);
		p = p->next;
		cnt++;
	}
}

void hexDump(void *addr)
{
	int i;
	unsigned char buff[17];
	unsigned char *pc = (unsigned char*)addr;
	// Process every byte in the data.
	for (i = 0; i < 160; i++) {
		// Multiple of 16 means new line (with line offset).

		if ((i % 16) == 0) {
			// Just don't print ASCII for the zeroth line.
			if (i != 0)
				printf("  %s\n", buff);

			// Output the offset.
			printf("  %04X ", i);
		}

		// Now the hex code for the specific character.
		printf(" %02X", 0);
		//printf(" %02x", pc + sizeof(char));
		//printf(" %x", pc + sizeof(char));
		// And store a printable ASCII character for later.
		if ((pc[i] < 0x20) || (pc[i] > 0x7e)) {
			buff[i % 16] = '.';
		}
		else {
			buff[i % 16] = pc[i];
		}

		buff[(i % 16) + 1] = '\0';
	}

	// Pad out last line if not exactly 16 characters.
	while ((i % 16) != 0) {
		printf("   ");
		i++;
	}

	// And print the final ASCII bit.
	printf("  %s\n", buff);
}

void hexDumpWithStart(int start, void *addr)
{
	int i;
	unsigned char buff[17];
	unsigned char *pc = (unsigned char*)addr;
	// Process every byte in the data.
	for (i = 0; i < 160; i++) {
		// Multiple of 16 means new line (with line offset).

		if ((i % 16) == 0) {
			// Just don't print ASCII for the zeroth line.
			if (i != 0)
				printf("  %s\n", buff);

			// Output the offset.
			printf("  %04X ", i);
		}

		// Now the hex code for the specific character.
		printf(" %02X", pc + sizeof(char));

		// And store a printable ASCII character for later.
		if ((pc[i] < 0x20) || (pc[i] > 0x7e)) {
			buff[i % 16] = '.';
		}
		else {
			buff[i % 16] = pc[i];
		}

		buff[(i % 16) + 1] = '\0';
	}

	// Pad out last line if not exactly 16 characters.
	while ((i % 16) != 0) {
		printf("   ");
		i++;
	}

	// And print the final ASCII bit.
	printf("  %s\n", buff);
}

void hexDumpWithStartEnd(int start, int end, void *addr)
{
	int i;
	unsigned char buff[17];
	unsigned char *pc = (unsigned char*)addr;
	// Process every byte in the data.
	for (i = 0; i < 160; i++) {
		// Multiple of 16 means new line (with line offset).

		if ((i % 16) == 0) {
			// Just don't print ASCII for the zeroth line.
			if (i != 0)
				printf("  %s\n", buff);

			// Output the offset.
			printf("  %04X ", i);
		}

		// Now the hex code for the specific character.
		printf(" %02X", pc + sizeof(char));

		// And store a printable ASCII character for later.
		if ((pc[i] < 0x20) || (pc[i] > 0x7e)) {
			buff[i % 16] = '.';
		}
		else {
			buff[i % 16] = pc[i];
		}

		buff[(i % 16) + 1] = '\0';
	}

	// Pad out last line if not exactly 16 characters.
	while ((i % 16) != 0) {
		printf("   ");
		i++;
	}

	// And print the final ASCII bit.
	printf("  %s\n", buff);
}

void edit(int traget_address, int data, void *addr) {
	unsigned char *pc = (unsigned char*)addr;
	pc[traget_address] = data;
}

void insert(struct Inst inst_record, struct Record *hash_table[])
{
	int key, h;
	struct Record *temp;
	key = inst_record.opcode;
	if (search_element(key, hash_table) != -1)
	{
		printf("Duplicate Key\n");
		return;
	}
	h = hash_function(key);
	temp = malloc(sizeof(struct Record));
	temp->data = inst_record;
	temp->link = hash_table[h];
	hash_table[h] = temp;
}

void show(struct Record *hash_table[])
{
	int count;
	struct Record *ptr;
	for (count = 0; count < HASH_TABLE_MAX; count++)
	{
		printf("\n[%3d]", count);
		if (hash_table[count] != NULL)
		{
			ptr = hash_table[count];
			while (ptr != NULL)
			{
				printf("%d %s %d\t", ptr->data.opcode, ptr->data.mnemonic);
				ptr = ptr->link;
			}
		}
	}
	printf("\n");
}

int search_element(int key, struct Record *hash_table[])
{
	int h;
	struct Record *ptr;
	h = hash_function(key);
	ptr = hash_table[h];
	while (ptr != NULL)
	{
		if (ptr->data.opcode == key)
		{
			return h;
		}
		ptr = ptr->link;
	}
	return -1;
}

void remove_record(int key, struct Record *hash_table[])
{
	int h;
	struct Record *temp, *ptr;
	h = hash_function(key);
	if (hash_table[h] == NULL)
	{
		printf("Key %d Not Found\n", key);
		return;
	}
	if (hash_table[h]->data.opcode == key)
	{
		temp = hash_table[h];
		hash_table[h] = hash_table[h]->link;
		free(temp);
		return;
	}
	ptr = hash_table[h];
	while (ptr->link != NULL)
	{
		if (ptr->link->data.opcode == key)
		{
			temp = ptr->link;
			ptr->link = temp->link;
			free(temp);
			return;
		}
		ptr = ptr->link;
	}
	printf("Key %d Not Found\n", key);
}

void opcodelist(struct Record *hash_table[]) {
	for (int count = 0; count < 20; count++) {
		printf("%d : ", count);
		struct Record *ptr = hash_table[count];
		int start_flag = 1;
		while (ptr != NULL && ptr->link != NULL) {
			if (start_flag) {
				printf("[%s, %X]", ptr->data.mnemonic, ptr->data.opcode);
				start_flag = 0;
			}
			else {
				printf(" -> [%s, %X]", ptr->data.mnemonic, ptr->data.opcode);
			}
			ptr = ptr->link;
		}
		printf("\n");
	}
}

// Based on an original suggestion on Robert Jenkin's part in 1997
// https://gist.github.com/badboy/6267743
int hash_function(int key) {
	key = ~key + (key << 15); // key = (key << 15) - key - 1;
	key = key ^ (key >> 12);
	key = key + (key << 2);
	key = key ^ (key >> 4);
	key = key * 2057; // key = (key + (key << 3)) + (key << 11);
	key = key ^ (key >> 16);	
	return (key % HASH_TABLE_MAX);
}
