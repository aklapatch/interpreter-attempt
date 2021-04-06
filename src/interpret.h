#pragma once
#include "tokenizer.h"
#include <stdint.h>
#include "../c-data-structures/src/dynarr.h"

typedef union {
    double f;
    uintptr_t u;
    intptr_t i;
    uint8_t bytes[sizeof(uintptr_t)];
    void* ptr;
} stack_val;


static stack_val* stack_ptr = NULL;

stack_val * get_stack_ptr(){
    return stack_ptr;
}

void set_stack_ptr(stack_ptr* input){
    if (input != NULL){
        stack_ptr = input;
    }
}

// TODO:
// - varDecl
// - fnDecl
// - putting values on the stack for operators
// - pulling operators off the stack for operators
// - putting the result in a variable
// - I think I need to make a hash table first.

int interpret(token * tokens){


    return 0;
}
