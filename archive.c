#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


/** 
 *  * Packs a single file or directory recursively
 *   *
 *    * @param fn The filename to pack
 *     * @param outfp The file to write encoded output to
 *      */
void
pack(char * const fn, FILE *outfp)
{
    struct stat st;
      stat(fn, &st);
        if (S_ISDIR(st.st_mode))
            {
               fprintf(stderr, "Recursing `%s'\n", fn);
               fprintf(outfp, "%zu:%s/", (strlen(fn)+1), fn);
               DIR* folder = opendir(fn);
               chdir(fn);
               // Error check directory opening
               if (folder == NULL) {
                 err(1, "Couldn't open directory");
               }
               // Create directory structure and read it
               struct dirent *dt;
               dt = readdir(folder);
               // Start loopin through directory (every call to readdir() points to next in dir
               while(dt = readdir(folder)) {
                  struct stat dir;
                  stat(dt->d_name, &dir);

                 if (strcmp(dt->d_name, ".") && strcmp(dt->d_name, "..")) {
                   pack(dt->d_name, outfp);
                  }
                }
               fprintf(outfp, "0:");
               chdir("..");
            }
          else if (S_ISREG(st.st_mode))
              {
                    fprintf(stderr, "Packing `%s'\n", fn);
                    fprintf(stderr, "Here is the filesize %zu", st.st_size);
                    fprintf(outfp, "%zu:%s%zu:", strlen(fn), fn, st.st_size);
                    // Now we pack the contents
                    // Open the file
                    FILE* file_to_open = fopen(fn, "r");
                    // Create Buffer
                    uint8_t* contents;
                    contents = calloc(st.st_size, sizeof(uint8_t));
                    // Check if sufficient memory is there
                    if (contents == NULL) {
                       fprintf(stderr, "Memory Error, not enough to allocate\n");
                       exit(1);
                    }
                    // Read the contents of the file
                    size_t file_to_read = fread(contents, sizeof(uint8_t), st.st_size, file_to_open);
                    fclose(file_to_open);
                    fprintf(stderr, "File size output : %zu\n", file_to_read);
                    // Write the contents of the file to output
                    size_t written_file = fwrite(contents, sizeof(uint8_t), st.st_size, outfp);
                    free(contents);
                    if ((written_file < st.st_size-1) && ferror(outfp)) {
                      err(1, "Write Failed on file : %s\n", fn);
                    }
                      }
            else
                {
                      fprintf(stderr, "Skipping non-regular file `%s'.\n", fn);
                        }
}


int
main(int argc, char *argv[])
{
    if (argc < 2) {
          fprintf(stderr, "Usage: %s FILE... OUTFILE\n"
                                  "       %s INFILE\n", argv[0], argv[0]);
              exit(1);
     }
    // Then we set first input file/directory as the name of our archive
    char *file_name = argv[argc-1];
    if (argc > 2)
        { /* Packing files */
        printf("Unpacking Files now\n");
        FILE *fp = fopen(file_name, "w");
        for (int argind = 1; argind < argc - 1; ++argind)
          {
            printf("It's going\n");
            pack(argv[argind], fp);
          }
        }
}
