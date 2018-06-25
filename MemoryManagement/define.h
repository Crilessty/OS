#ifndef Memory_Management_H
#define Memory_Management_H

#define MEM_SIZE 2000

#define free false
#define busy true

typedef struct {
	int pid;
	int start;
	int length;
}process;

struct Node{
	int num;
	bool status;
	Node *next;
}Node;

typedef struct {
	int start;
	int length;
}free_block;

void init();
void show();
void create_process(int );
void free_process(int );







#endif // !Memory_Management_H


