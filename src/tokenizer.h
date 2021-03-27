#pragma once
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum tok_type {
    tok_unknown = 0,
    tok_comment,
    tok_doc_comment,
    tok_macro,
    tok_struct,
    tok_enum,
    tok_identifier,
    tok_sint,
    tok_float,
    tok_uint,
    tok_def,
    tok_r_brack,
    tok_l_brack,
    tok_r_paren,
    tok_l_paren,
    tok_r_brace,
    tok_l_brace,
    tok_comma,
    tok_semicolon,
    tok_typedec,
}tok_type;

// TODO Evaluate whether the multi line comment carachter should be changed to ` from $
bool isReservedChar(char input){
    return strchr("#,$(){}[];:", input) != NULL;
}

typedef struct token{
    tok_type type;
    char * start;
    uint32_t len, line_num, col_num;
} token;

// end should be the last character in the string?
// it could also be the NULL terminator, but I'm not sure
char* nextToken(token * output, char * str, char * str_end){
    if (str == NULL || output == NULL){
        return NULL;
    }
    while(isspace(*str)){
        ++str;
    }

    // comments
    if (*str == '#'){
        tok_type out_type = tok_comment;
        if (str + 1 < str_end && str[1] == 'd'){
            out_type = tok_doc_comment;
        }
        char * comment_end = strchr(str, '\n') + 1;
        if (comment_end == NULL){
            comment_end = str_end;
        }
        output->type = out_type;
        output->start = str;
        output->len = (uint32_t)(comment_end - str);
        return comment_end;
    }

    // TODO increment comment_end by 1 before getting the length
    if (*str == '$'){
        tok_type out_type = tok_comment;
        char * cursor = str + 1;
        if (cursor >= str_end){

            return NULL;
        }
        if (*cursor == 'd'){
            out_type = tok_doc_comment;
        }
        char * comment_end = strchr(cursor, '$') + 1;
        if (comment_end == NULL){
            return NULL;
        }
        output->type = out_type;
        output->len = (uint32_t)(comment_end - str);
        output->start = str;
        return comment_end;
    }

    if (isalpha(*str)){
        char * cursor = str;
        // go until you find another space or a reserved char.
        while(!isspace(*cursor) && !isReservedChar(*cursor)){
            ++cursor;
        }

        tok_type out_type = tok_identifier;
        // once you get there, get the length
        output->len = (uint32_t)(cursor - str);
        output->start = str;
        return cursor;
    }



    return NULL;
}

int tokenize(char * input, token * output){

    return 0;
}
