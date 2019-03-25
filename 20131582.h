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

// type filename
int type_file(char * filename);

// h[elp] command
void display_help(void);
void display_dir(void);

// history linked list
void createNode(LinkedList * L, char cmd[]);
void printNodes(LinkedList * L);

// hex dump
int hexDump(int last_addr, void * addr);
int hexDumpWithStart(int start, void * addr);
int hexDumpWithStartEnd(int start, int end, void * addr);


void insert(struct Inst inst_record, struct Record *hash_table[]);
int search_element(unsigned char * key, struct Record *hash_table[]);
void get_opcode_by_key(unsigned char * key, struct Record *hash_table[]);
void opcodelist(struct Record *hash_table[]);
int hash_function(unsigned char * key);
void edit(int target_address, int data, void *addr);
void fill(int start, int end, int data, void *addr);
