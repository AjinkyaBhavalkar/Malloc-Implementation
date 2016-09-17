//
//  heap.c
//  
//
//  Created by ajinkya bhavalkar on 11/02/16.
//
//

#include <stdio.h>
//#include <unistd.h>

char heap[1000];
char *breaks;

void *sbrk(size_t );
void *brk(void *);

typedef struct s_block *meta_data;
void *base = NULL;

struct s_block{
    size_t size;                //4B
    meta_data next;             //4B
    meta_data prev;             //4B
    int free;                   //4B
};

int main(){
    meta_data b;
    
    breaks = &heap[0];
    for (int i=0;i<20;i++){
        heap[i] = 0;
    }
    
    b = sbrk(0);
    printf("address of b = %x\n",b);
    
    b = sbrk(500);
    printf ("address of b = %x\n",b);
    
    b = brk(b);
    printf ("retruned break position %x\n",b);
    printf ("Break position %x\n",breaks);
    return 0;
}

void *brk(void* temp){
    char *temp2 = temp;
    breaks = temp2;
    return breaks;
}
void *sbrk(size_t x){
    char *temp;
    printf("x = %d\n",x);
    temp = breaks;
    breaks = breaks + x;
    printf("Address of current break = %x\n",breaks);
    return temp;
}

void printy(){
    for (int i=0;i<20;i++){
        printf("\t%d",heap[i]);
    }
}