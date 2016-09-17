//
//  my_malloc.h
//  
//
//  Created by ajinkya bhavalkar on 12/02/16.
//
//
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_VAL 1000

typedef struct s_block *meta_data;


//Structure of meta data : {size of data in the chunk,
//                          pointer to next meta-data,
//                          pointer to prev meta-data,
//                          flag to check if chunk is available}

struct s_block{
    size_t size;
    meta_data next;
    meta_data prev;
    int free;
};



//base address
extern void *base;

//last visited chunk to avoid traversing.
extern meta_data last;

//heap management
extern char heap[MAX_VAL];             //global heap
extern char *breaks;            //Break called by my_sbrk,my_brk functions.


//Function Declarations (prototypes)
meta_data heap_extend(int );
meta_data find_block(size_t );
void* my_malloc(size_t);
void print_x();
void my_free(void * );
void waitkey(int );
void* my_sbrk(size_t );
void my_brk(void* );
void *my_calloc(size_t, size_t);
void *my_realloc(void* ,size_t );
int validate(meta_data );
size_t freeSizeHeap(void);

//MACROS
#define BLOCK_SIZE sizeof (struct s_block)
#define PRINT_MAIN printf("\nThis is main -> \n")
#define PRINT_HEAP printf("Current break position --> %p\n",breaks)
#define EOI printf("-------------------\n")


