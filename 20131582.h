#define _CRT_SECURE_NO_WARNINGS
#define	MAX	100
#define HASH_TABLE_MAX 20
#define SYMBOL_TABLE_MAX 5

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

typedef struct Symbol {
	int loc;
	char symbol[10];
} Symbol;

typedef struct HashRecord {
    struct Inst data;
    struct HashRecord *link;
} HashRecord;

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


int main(void);

// assemble asm file
int assemble_file(char * filename);

// type filename
int type_file(char * filename);

// h[elp] command
int display_help(void);
int display_dir(void);

// history linked list
int createNode(char cmd[]);
int printNodes(void);

// hex dump
int hexDump(int last_addr);
int hexDumpWithStart(int start);
int hexDumpWithStartEnd(int start, int end);

int insert_symbol_table(struct Symbol symbol);
int insert_hash_table(struct Inst inst_record);
int search_element_symbol_table(unsigned char * key);
int search_element_hash_table(unsigned char * key);
int get_opcode_by_key(unsigned char * key);
char* get_format_by_key(unsigned char * key);
int opcodelist(void);
int print_symbol_table(void);
int hash_function(unsigned char * key, int length);
int edit(int target_address, int data);
int fill(int start, int end, int data);
int myCompare(const void* a,const void* b);
int get_loc_by_symbol(unsigned char * key);


// symbol table
struct SymbolRecord *symbol_table[SYMBOL_TABLE_MAX];

// hash table of opcode list
struct HashRecord *hash_table[HASH_TABLE_MAX];

struct Inst inst_record;
// init dump addresses
unsigned char addr[16*65536] = { 0 };
int regi_x = 0x10, regi_a = 0x00, regi_s = 0x40;
int last_addr = 0;

int symbol_ctr = 0;

struct LinkedList *L;
struct Symbol * S;

// file input stream
FILE *fp_opcode;
