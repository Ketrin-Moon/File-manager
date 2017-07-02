#include <stdio.h>
#include <stdlib.h>
 
#define COPY_BLOCK_SIZE 64
#define PROGRAM_NAME argv[0]
#define INPUT_FILE_NAME argv[1]
#define OUTPUT_FILE_NAME argv[2]
 
int main(int argc, char ** argv) {
    unsigned char buf[COPY_BLOCK_SIZE];
    FILE * fin, * fout;
    size_t bytes;
    
    if ( argc < 3 ) {
        fprintf(stderr, "Usage: %s input_file output_file\n", PROGRAM_NAME);
        exit(1);
    }
    
    if ( ! ( fin = fopen(INPUT_FILE_NAME, "rb") ) ) {
        fprintf(stderr, "%s: can't open %s for input!\n", PROGRAM_NAME, INPUT_FILE_NAME);
        exit(1);
    }
    if ( ! ( fout = fopen(OUTPUT_FILE_NAME, "wb") ) ) {
        fprintf(stderr, "%s: can't open %s for output!\n", PROGRAM_NAME, OUTPUT_FILE_NAME);
        if ( fclose(fin) )
            fprintf(stderr, "%s: can't properly close %s!\n", PROGRAM_NAME, INPUT_FILE_NAME);
        exit(1);
    }
    
    while ( ( bytes = fread(buf, sizeof(char), COPY_BLOCK_SIZE, fin) ) != (size_t)EOF && bytes > 0 ) {
        if ( fwrite(buf, sizeof(char), bytes, fout) != bytes ) {
            fprintf(stderr, "%s: can't write to %s!\n", PROGRAM_NAME, OUTPUT_FILE_NAME);
            if ( fclose(fin) )
                fprintf(stderr, "%s: can't properly close %s!\n", PROGRAM_NAME, INPUT_FILE_NAME);
            if ( fclose(fout) )
                fprintf(stderr, "%s: can't properly close %s!\n", PROGRAM_NAME, OUTPUT_FILE_NAME);
            exit(1);
        }
    }
    
    if ( ferror(fin) ) {
        fprintf(stderr, "%s: can't readf rom %s!\n", PROGRAM_NAME, INPUT_FILE_NAME);
        if ( fclose(fin) )
            fprintf(stderr, "%s: can't properly close %s!\n", PROGRAM_NAME, INPUT_FILE_NAME);
        if ( fclose(fout) )
            fprintf(stderr, "%s: can't properly close %s!\n", PROGRAM_NAME, OUTPUT_FILE_NAME);
        exit(1);
    }
    
    if ( fclose(fin) ) {
        fprintf(stderr, "%s: can't properly close %s!\n", PROGRAM_NAME, INPUT_FILE_NAME);
        if ( fclose(fout) )
            fprintf(stderr, "%s: can't properly close %s!\n", PROGRAM_NAME, OUTPUT_FILE_NAME);
        exit(1);
    }
    
    if ( fclose(fout) ) {
        fprintf(stderr, "%s: can't properly close %s!\n", PROGRAM_NAME, OUTPUT_FILE_NAME);
        exit(1);
    }
    
    fprintf(stderr, "Done.\n");
    exit(0);
}
