//
//  pattern.c
//  
//
//  Created by ajinkya bhavalkar on 11/02/16.
//
//

#include <stdio.h>
#include <stdlib.h>
//bit pattern 11011

#define MAX 1024

//Global variables
char* base;
char array[MAX];

//Prototypes
void fill_pattern(char*, char*);
void init_zeros(char*,char*);
char* check_pattern(char*);
char* decimal_to_binary(int n);
void waitkey(int);

//Main function-----------------------
int main(){
    char *start_addr,*end_addr;
    char *k,*z;
    base = &array[0];
    int option = 1;
    //Acquiring Address
    
    start_addr = &array[0];
    end_addr = &array[MAX];
    
    printf ("start_addr %p end_array %p\n",start_addr,end_addr);
    init_zeros(start_addr,end_addr);                                //Init function call.
    
    fill_pattern(start_addr,end_addr);                              //fill pattern function call.
    

    
    while (option != 0){
            printf ("\nMENU\nPlease choose from below : \n1. pattern is untouched. call check_pattern.\n2. pattern is disrupted. Call check_pattern.\n Press 0 to exit.");
        scanf("%d",&option);
    if (option == 1)  {
        printf("Pattern is filled again--->\n \n");
        fill_pattern(start_addr, end_addr);
        k = check_pattern(start_addr);
    }
    else if (option == 2){
        *(end_addr - 2) = 0;
        
        z = check_pattern(start_addr);
    }
    else if (option == 0) break;
    else printf("Option Invalid\n");
    
    }
    return 0;
}

//Init_Zeros fills up the space in address space with zeros.
void init_zeros(char *start_addr,char *end_addr){
    printf("start_addr = %p end_addr = %p\n",start_addr,end_addr);
    char *temp;
    if (start_addr < base || end_addr > (base + MAX)) printf("Error\n");
    else{
        temp = start_addr;
        while (temp != end_addr){
            *(temp) = 0;
            
            temp++;
        }
    }
    
}

//Fill_Pattern fills up the space with a continious pattern of 11011. This function is designed specifically for this pattern.
void fill_pattern(char *start_addr,char *end_addr){
    int pat[5];
    char *pointer;
    int  i = 0;
    if (start_addr < base || end_addr > (base + MAX)) printf("Error\n");
    else{

    //pattern generator
    pat[0] = 1;
    pat[1] = 1;
    pat[2] = 0;
    pat[3] = 1;
    pat[4] = 1;
    
    int mask;                       //mask for anding operations
    char result = 0;
    int count = 0;                  //count to keep circular pattern generator working.
    char temp;
    char *p;
    p = start_addr;
    mask = 0b00000001;
    //pat = "11011";

    while(p != end_addr){
        result = 0;
    for (i = 0 ; i < 8; i++){
        //Masking with pattern generator and shifting as per iteration.
        temp = (pat[count] & mask) << (7 - i);
        
        //Decimal to binary conversion.
        pointer = decimal_to_binary(temp);
       // printf("pat[%d]=%d ==> %s\n",count,pat[count], pointer);


        count++;
        
        if (count == 5) count = 0;
        result = result | temp;
        
        
    }
        //Update Pointer to point to next char.
        *p++ = result;
    }
    

    printf("Pattern filled...\n \n");
    p = start_addr;
    while(p != end_addr){
    pointer = decimal_to_binary(p[0]);
    printf("%s",pointer);
            p++;
        }
        
        printf("\n----------------------\n");
        
}

}

//Check_Pattern is implemented for checking the function 11011.
char *check_pattern(char* start_addr){
    char* b;
    char temp;
    char mask = 0b10000000;
    int buf;
    int  i = 0,j = 0;
    b = start_addr;
    int pat[5];
    
    //Pattern Generator
    pat[0] = 1;
    pat[1] = 1;
    pat[2] = 0;
    pat[3] = 1;
    pat[4] = 1;

    if (start_addr < base) printf("Error\n");
    else{
        
        b = start_addr;
        
        while (b < (start_addr + MAX)){
            temp = b[0];
            for (j = 0;j < 8; j++){
            buf = temp & mask;
                
                buf = (buf>>7) & 0b00000001;              //Maintain a buf to check with pattern.
                
            printf("temp %d buf %d\n",temp,buf);
                if ((buf != pat[i++])) {
                    //waitkey(0);
                    printf("\n Error found at address %p\n",b);
                    return b;
                }
            if (i == 5) i = 0;
            
            temp = temp << 1;                           //Checks for every bit.
            }
            b++;
        }
        printf("Pattern is correct until the end of array.\n");
        return (start_addr + MAX);
    }
    
    return 0;
}

//Using this function to display in binary. Not my function.
char *decimal_to_binary(int n)
{
    int c, d, count;
    char *pointer;
    
    count = 0;
    pointer = (char*)malloc(32+1);
    
    if ( pointer == NULL )
        exit(EXIT_FAILURE);
    
    for ( c = 7 ; c >= 0 ; c-- )
    {
        d = n >> c;
        
        if ( d & 1 )
            *(pointer+count) = 1 + '0';
        else
            *(pointer+count) = 0 + '0';
        
        count++;
    }
    *(pointer+count) = '\0';
    
    return  pointer;
}

//Waits untill user enters 0
void waitkey(int x){
        int t = 0;
        while(1){
            scanf("%d",&t);
            if (x == t ) break;
            
        }
    }
