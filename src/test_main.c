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
    fread(out, *out_len, 1, fp);
    out[*out_len] = '\0';
    fclose(fp);
    return out;
}

int main(){
    uint32_t f_len = 0;
    char * file = file_to_buffer("lang-sample.txt", &f_len);
    if (!file){
        ERROR("Failed to open file!");
        return EXIT_FAILURE;
    }
    printf("%s", file);
    free(file);
    return EXIT_SUCCESS;
}
