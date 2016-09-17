//
//  try.c
//  
//
//  Created by ajinkya bhavalkar on 11/02/16.
//
//

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    char temp = 0b10000000;
    printf("%d\n",temp>>7);
    return 0;
}