#include "tokenizer.h"
#include "err_misc.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


// memory allocated by this function needs to be freed by the caller.
char * file_to_buffer(char * fname, uint32_t * out_len){
    FILE * fp = fopen(fname, "r");
    if (!fp){
        PERROR("FAiled to open file");
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    *out_len = ftell(fp);
    rewind(fp);

    char * out = malloc(*out_len + 1);
    if (!out){
        PERROR("Failed to allocate memory for file buffer");
        return NULL;
    }
    size_t bytes_read = fread(out, *out_len, 1, fp);
    if (bytes_read != 1){
        PERROR("Failed to read whole file");
        return NULL;
    }
    out[*out_len] = '\0';
    fclose(fp);
    return out;
}

void print_token(token tok_in){
    printf("token: len=%u type=%s ", tok_in.len, tok_type_str(tok_in.type));
    printf("contents=");
    while (tok_in.len-- >0){
        printf("%c",*tok_in.start);
        tok_in.start++;
    }
    printf("\n");
}

int main(){
    uint32_t f_len = 0;
    char * file = file_to_buffer("lang-sample.txt", &f_len);
    if (!file){
        ERROR("Failed to open file!");
        return EXIT_FAILURE;
    }

    token tok_out;
    char * file_end = file + f_len;
    char * tok_end = file;
    do {
        tok_end = nextToken(&tok_out, tok_end, file_end);
        if (tok_end != NULL){
            print_token(tok_out);
        }
    } while (tok_end != NULL && tok_end < file_end);
    free(file);
    return EXIT_SUCCESS;
}
