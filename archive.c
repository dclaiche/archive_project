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
               
                    }
          else if (S_ISREG(st.st_mode))
              {
                    fprintf(stderr, "Packing `%s'\n", fn);
                    fprintf(outfp, "%zu:%s%zu:", strlen(fn), fn, st.st_size);


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
    if (argc >= 2)
        { /* Packing files */
        printf("Unpacking Files now\n");
        FILE *fp = fopen(file_name, "w");
        for (int argind = 1; argind <= argc - 1; ++argind)
          {
            printf("It's going");
            pack(argv[argind], fp);
          }
        }

}
