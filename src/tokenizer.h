#pragma once
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include "err_misc.h"

typedef enum {
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
    tok_colon,
    tok_typedec,
    tok_if,
    tok_elif,
    tok_else,
    tok_import,
    tok_bool,
}tok_type;

#define SWITCH_STR(val) case val: return #val

char * tok_type_str(tok_type in){
    switch(in){
        SWITCH_STR(tok_unknown);
        SWITCH_STR(tok_comment);
        SWITCH_STR(tok_doc_comment);
        SWITCH_STR(tok_macro);
        SWITCH_STR(tok_struct);
        SWITCH_STR(tok_enum);
        SWITCH_STR(tok_identifier);
        SWITCH_STR(tok_sint);
        SWITCH_STR(tok_uint);
        SWITCH_STR(tok_def);
        SWITCH_STR(tok_r_brack);
        SWITCH_STR(tok_l_brack);
        SWITCH_STR(tok_l_brace);
        SWITCH_STR(tok_r_brace);
        SWITCH_STR(tok_r_paren);
        SWITCH_STR(tok_l_paren);
        SWITCH_STR(tok_semicolon);
        SWITCH_STR(tok_comma);
        SWITCH_STR(tok_colon);
        SWITCH_STR(tok_typedec);
        SWITCH_STR(tok_if);
        SWITCH_STR(tok_elif);
        SWITCH_STR(tok_else);
        SWITCH_STR(tok_import);
        SWITCH_STR(tok_bool);
        default:
            printf("Unrecognized token type %u\n", in);
            return NULL;
    }
}

// TODO Evaluate whether the multi line comment carachter should be changed to ` from $
bool isReservedChar(char input){
    if (input == '\0'){
        return false;
    }
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
    if (str == NULL ||
        output == NULL ||
        str >= str_end ||
        *str == '\0'){
        return NULL;
    }
    while(str < str_end && isspace(*str)){
        ++str;
    }
    if (str >= str_end){
        return NULL;
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

    // handle reserved characters
    if (isReservedChar(*str)){
        output->start = str;
        output->len = 1;

        struct {
            char tok_char;
            tok_type type;
        } tok_pairs[] = {
            { '(', tok_l_paren},
            { ')', tok_r_paren},
            { ']', tok_r_brack},
            { '[', tok_l_brack},
            { '{', tok_l_brace},
            { '}', tok_r_brace},
            { ';', tok_semicolon},
            { ',', tok_comma},
            { ':', tok_colon},
        };
        output->type = tok_unknown;
        for (uint16_t i = 0;
             i < sizeof(tok_pairs)/sizeof(tok_pairs[0]);
             ++i){
            if (*str == tok_pairs[i].tok_char){
                output->type = tok_pairs[i].type;
                break;
            }
        }
        if (output->type == tok_unknown){
            ERROR("Unknown reserved char");
            printf("char = %c", *str);
            return NULL;
        }

        return str + 1;
    }

    // check for numbers, this includes - signs
    if (isdigit(*str) || *str == '-' || *str == '.'){
        // don't do anything about a - without a number behind it
        if (str + 1 < str_end && isdigit(*str)){
            bool has_dot = false;
            if (*str == '.'){
                has_dot = true;
            }
            bool has_neg = false;
            if (*str == '-'){
                has_neg = true;
            }
            char * cursor = str;
            do {
                cursor++;
                if (cursor < str_end && *cursor == '.'){
                    if (has_dot){
                        ERROR("Got second dot!");
                        return NULL;
                    }
                    bool has_dot = true;
                    cursor++;
                }
            } while (cursor < str_end && isdigit(*cursor));
            // if we have something following the number that is a letter that is a problem

            if (!isReservedChar(*cursor) && !isspace(*cursor)){
                ERROR("Found something unexpected after a number!");
                return NULL;
            }
            output->start = str;
            if (has_dot){
                output->type = tok_float;
            } else if (has_neg){
                output->type = tok_sint;
            } else {
                output->type = tok_uint;
            }
            output->len = (uint32_t)(cursor - str);
            return cursor;
        }
    }

    if (isalpha(*str) || !isspace(*str)){
        char * cursor = str;
        // go until you find another space or a reserved char.
        while(cursor < str_end && !isspace(*cursor) && !isReservedChar(*cursor)){
            ++cursor;
        }

        // once you get there, get the length
        output->len = (uint32_t)(cursor - str);
        output->start = str;
        output->type = tok_identifier;
        struct {
            const char * word;
            tok_type type;
        } word_pairs[] = {
            { "def", tok_def},
            { "enum", tok_enum},
            {"struct",tok_struct},
            {"macro",tok_macro},
            {"if",tok_if},
            {"else",tok_else},
            {"elif", tok_elif},
            {"true", tok_bool},
            {"false", tok_bool},
        };
        for (uint16_t i = 0; i < sizeof(word_pairs)/sizeof(word_pairs[0]); ++i){
            if (strlen(word_pairs[i].word) == output->len){
                if (strncmp(word_pairs[i].word, str, output->len) == 0){

                    output->type = word_pairs[i].type;
                    break;
                }
            }
        }
        return cursor;
    }

    // no token found
    return NULL;
}

int tokenize(char * input, token * output){

    return 0;
}
