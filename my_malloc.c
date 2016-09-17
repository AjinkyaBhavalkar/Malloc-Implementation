//  ASSIGNMENT-1 | RTOS | HEAP MANAGEMENT SYSTEM | MT2015505
//  my_malloc.c
//  
//
//  Created by ajinkya bhavalkar.
//  Heap Management System.
//  Functions : my_malloc, my_calloc, my_free, my_freeSizeHeap.
//

#include "my_malloc.h"

void* base = NULL;
char *breaks = heap;



int main(){
    return 0;
}
//Heap-extend
//Heap is extended when called to increment of (BLOCK_SIZE + size) demanded by the user. Returns address of new block. returns NULL if heap cannot be further extended.
meta_data heap_extend(int s){
    meta_data b;
    
    b = my_sbrk(0);
    if (my_sbrk(BLOCK_SIZE + s) == (void*) -1) return NULL;
    //printf("***Address of b = %p\n",b);
    b->next = NULL;
    //printf("***Address of b->next = %p\n",b->next);
    if(last) {
        last->next = b;             //Connecting the meta-data linked list.
        b->prev = last;
        last = last->next;
    }
    b->size = s;
    b->free = 0;                    //Chunk assigned.
    return(b);
    
}

//Finds the available block that can fit the requested size. Returns the base address of that block. returns NULL if no block is available in the present heap.
meta_data find_block(size_t s){
    meta_data b;
    b = base;
    printf ("Inside Find_block--> \n");
    print_x();
    while (b && !(b->free && b->size >= s)){
        last = b;                                   //update last.
        b = b->next;
    }
    
    if (b==NULL) printf("Block not present. heap will be extended.\n");
    else printf("Block found at %p \n",b);
    
    print_x();
    return (b);
    
    
}

//My_Malloc : allocates size requested size by the user in a heap. returns its base address. returns NULL if block cannot be assigned, heap is full.
void* my_malloc(size_t s){
    meta_data b;
    char *tmp;
    if(s == 0) return NULL;
    
    if (base){
        printf ("Base found at address %p\n",base);
        b = find_block(s);                              //find block that can fit size s.
        
        if (b == NULL) {
            b = heap_extend(s);                         //if not found try to extend heap
            if (!b) {
                printf("\nError : Heap cannot be extended.\n ");
                return NULL;                            //Error: Heap cannot be extended.
            }
        }
        
    }
    else                                                //base initializing..
    {
        printf ("base initializing \n");
        b = heap_extend(s);                             //extend heap for the first time.
        if (!b) {
            printf ("\nError : Heap cannot be extended.\n");
            return NULL;
        }
        b->prev = NULL;
        base = b;                                       //base
        last = b;
        print_x();
        
    }
    
    b->size = s;
    b->free = 0;
    printf ("For b = %p, b->free = %d\n",b,b->free);
    printf("b->next = %p\n",b->next);
    
    //Pointer packing: converting it to pointer to data in the chunk
    tmp = (char *)b;
    tmp += BLOCK_SIZE;
    printf("Address to your chunk is %p, its meta-data is at %p\n",tmp,b);
    return (tmp);
}

//free: Releases memory chunk which is requested. memory has to be allocated by my_malloc. silently does nothing if pointer is invalid.
void my_free(void *q){
    meta_data b;
    //Bring the pointer back to meta-data
    char *tmp;
    tmp = q;
    tmp -= BLOCK_SIZE;
    b = (meta_data)tmp;
    
    if (validate(b) == 1){
    printf("b to be freed is %p, b-> next = %p\n",b,b->next);
    // Check if previous block is free
    if (b->prev && b->prev->free == 1){
        printf("-----Previous block is free\n");
        if (b->next){
        b->prev->next = b->next;
        b->next->prev = b->prev;
        b = b->prev;
        }
        else{
            b->prev->next = NULL;
        }
//        b->free = 1;
    }
    
    //check if next block exists, and is free
    if (b->next && (b->next->free == 1)){
        printf("-----Next block exists and is free\n");
        if (b->next->next){
            b->next = b->next->next;
            b->next->next->prev = b;
            
        }
        // if next block is last, release memory
        else {
            if (b->prev){
                b->prev->next = NULL;
                if (b->prev->prev) last = b->prev->prev;
                else last = b->prev;
            }
            else base = NULL;
            my_brk(b);
        }
    }
    // If block is last block, release memory
    else if(b->next == NULL)
    {
        printf("Last block\n");
        if(b->prev){
            printf("releasing memory...\n");
            last = b->prev;
            printf("last = %p\n",last);
            my_brk(b);
            last->next = NULL;
  
        }        else base = NULL;
    }
    printf("Block freed with address %p\n",b);
    b->free = 1;
    }
    else{
        printf("Error: Address returned is not obtained from malloc. Block cannot be freed. Permission denied.\n");
    }
}

//Sbrk allocates chunk on the heap of size requested and returns its base address.
void* my_sbrk(size_t s){
    char *temp;
    
    printf("Requested size = %zu\n",s);
    temp = breaks;
    
    breaks = breaks + s;                            //shift break, allocates memory.
    if (breaks > &heap[MAX_VAL]){
        breaks = breaks - s;
        return (void*) -1;
    }
    else{
    
    printf("Address of current break = %p\n",breaks);
    return temp;
    }
}

//brk: brk releases allocated memory and returns new break position.
void my_brk(void* temp){
    char *temp2 = temp;
    if ((void*)temp2 < base) printf("Error : Incorrect pointer");
    else breaks = temp2;
    
}

//Calloc: Allocates space for an array of nmemb objects each having size s. returns pointer where data is allocated. returns NULL if allocation fails.
void *my_calloc(size_t nmemb, size_t s){
    void* b;
    char *t;
    int i;
    b = (void *)my_malloc(nmemb * s);
    
    if(b) {
        
        printf("address of space given to calloc: %p\n",b);
        t = (char*)b;
        for (i = 0;i < (nmemb * s);i++){
            *(t + i) = 0;
        }
    }
    return b;
    
}

//Realloc will change size of object pointed by the pointer. If ptr is null, realloc behaves like malloc. If size is zero and ptr not null, object is freed. Otherwise, it reallocates the memory of size wherever possible and copies data to the new object.
void* my_realloc(void *ptr,size_t s){
    meta_data b;
    //Bring the pointer back to meta-data
    char *tmp,*tmp2;
    void *temp_ptr;
    char *copy;
    size_t i = 0;
    
    tmp = ptr;
    tmp -= BLOCK_SIZE;
    b = (meta_data)tmp;
    if (validate(b) == 1){
    
    printf("address to meta-data you want is %p , address is %p\n",b,ptr);
    
    if(ptr) {
        if (s == 0) my_free(ptr);   //If Ptr in not NULL but size is.it frees the memory
        else{
            if(b->size >= s) return ptr; //If the pointed chunk can occupy requested size
            
            // If next block is free
                if(b->next && b->next->free){
                    printf("Next block is free\n");
                    if((b->size + b->next->size + BLOCK_SIZE) >= s) {
                        printf("Both blocks will be merged\n");
                        b->size = b->size + b->next->size;
                        if (b->next->next) {

                            b->next->next->prev = b;
                            b->next = b->next->next;
                            
                        }
                        else{

                            if (b->prev) b->next = NULL;
                            else base = NULL;
                        }
                    }
                    else{
                        printf("New position allocated using malloc\n");
                        temp_ptr = (void*)my_malloc(s);
                        //copying data from previous location
                        copy = (char*)ptr;
                        tmp2 = (char*)temp_ptr;
                        for (i = 0;i < (b->size);i ++){
                            *tmp2++ = *copy++;
                        }
                        my_free(ptr);
                            
                        return temp_ptr;
                    }
                }
                else{
                    //last block: extend heap..
                    printf("Heap needs to extended by %zu\n",s - b->size);
                    b = my_sbrk(0);
                    if (my_sbrk(s - b->size) == (void*) -1) {
                        printf("Heap cannot be extended.\n");
                        return NULL;
                        }
                    b->size += s - b->size;
                    printf("New b->size = %zu\n",b->size);
                }
            
        }
        
        tmp = (char *)b;
        tmp += BLOCK_SIZE;
        printf("Address to your chunk is %p, its meta-data is at %p\n",tmp,b);
        return tmp;
        
    }
    else{
        temp_ptr = (void *)my_malloc(s);          //Acts as malloc with NULL ptr.
        return temp_ptr;
    }
    }
    else {
        printf("Error: Address you requested is not obtained from malloc. Permission denied.\n");
        return NULL;
    }
}

//size of free space in heap: Calculates the size which is available and returns it in no. of bytes
size_t freeSizeHeap(){
    char *temp;
    meta_data b;
    size_t s = 0;
    if(base) {
        b = base;
        while (b && (b->free = 1)){
            s += b->size;
            b = b->next;
        }
        temp = &heap[MAX_VAL];
        s += (temp - breaks);
        return s;
    }
    else{
        return MAX_VAL;
    }
    
    
    
}

int validate(meta_data b)
{
    meta_data p = base;
    while (p != NULL){
        if (p == b) return 1;
        p = p->next;
    }
    return 0;
    
}

//------------Testing functions
void print_x(){
    printf("Base address = %p\n",base);
    printf("Last address = %p\n",last);
    
    
}

void waitkey(int x){
    int t = 0;
    while(1){
    scanf("%d",&t);
        if (x == t ) break;

    }
}




