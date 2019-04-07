#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <dirent.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
 * Constants and Macros.
 */
#define _CRT_SECURE_NO_WARNINGS    // prevent compile error because of sprintf security alert 
#define MEMSIZE		            (1 << 20)
#define	MAX	                    100
#define	MAXLEN		            80
#define HASH_TABLE_MAX          20
#define SYMBOL_TABLE_MAX        5

/*
 * Linked List Structure.
 */
typedef struct Node {
    void *data;
    struct Node *link;
} Node;

typedef struct LinkedList {
    struct Node *head;
    int size;
} LinkedList;

LinkedList* initList(void);
void appendList(LinkedList*, void*);
void printHistory(LinkedList*);

/*
 * Record Structure.
 */
typedef enum RecordType {
    // type of records.
    TEXT, HEADER
} RecordType;

typedef struct Record {
    int start, end, len;
    RecordType type;
    char content[MAXLEN << 1];
} Record;

/*
 * Insturction Structure.
 */
typedef struct Inst {
    int opcode;
    char mnemonic[10];
    char format[10];
} Inst;

typedef struct HashRecord {
    struct Inst data;
    struct HashRecord *link;
} HashRecord;

/*
 * Symbol Structure.
 */
typedef struct Symbol {
	int loc;
	char symbol[10];
} Symbol;

typedef struct SymbolRecord {
        struct Symbol data;
        struct SymbolRecord *link;
} SymbolRecord;

typedef struct Statement {
	int loc;
	char * label;
	char * opcode;
	char * operand;
} Statement;

/*
 * Functions.
 */
int main(void);
int processCmd(char*);
int loadOpcodelist(void);
int loadOpcode(void);
int assemble_file(char*);
int type_file(char*);
int display_help(void);
int display_dir(void);
int hexDump(int);
int hexDumpWithStart(int);
int hexDumpWithStartEnd(int, int);
int insert_symbol_table(struct Symbol);
int insert_hash_table(struct Inst);
int search_element_symbol_table(unsigned char*);
int search_element_hash_table(unsigned char*);
int get_opcode_by_key(unsigned char*);
char* get_format_by_key(unsigned char*);
int opcodelist(void);
int print_symbol_table(void);
int hash_function(unsigned char*, int);
int edit(int, int);
int fill(int, int, int);
int myCompare(const void*,const void*);
int get_loc_by_symbol(unsigned char*);

/*
 * Global variable.
 */
struct SymbolRecord *symbol_table[SYMBOL_TABLE_MAX];
struct HashRecord *hash_table[HASH_TABLE_MAX];
struct Inst inst_record;
struct LinkedList *history = NULL;
struct Symbol * S = NULL;

unsigned char addr[MEMSIZE] = { 0 };
char program_name[10];

int * modified_addr_ary;

int regi_a = 0x0;
int regi_x = 0x1;
int regi_l = 0x2;
int regi_b = 0x3;
int regi_s = 0x4;
int regi_t = 0x5;
int regi_f = 0x6;
int regi_pc = 0x8;
int regi_sw = 0x9;

int last_addr = 0;
int object_length = 0;
int text_record_length_ary[100];

int symbol_ctr;
int starting_address;
int ending_address;
int m_size = 0;