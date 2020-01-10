/**
 * RMS-to-Text Conversion tool For VMS
 */

#include <stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <rms.h>

static void
convert (char* input_file, char* output_file)
{
  FILE* in_fp    = fopen(input_file, "r");
  FILE* out_fp   = fopen(output_file, "w");
  char* buffer   = NULL;
  int recl       = 0;
  int rec_total  = 0;
  int rec_ok     = 0;
  int written    = 0;
  struct stat file_info;

  stat(input_file, &file_info);

  buffer = (char*) malloc(file_info.st_fab_mrs);

  do
  {
    recl = decc$record_read(in_fp, (void*) buffer, file_info.st_fab_mrs);
    ++rec_total;
    
    if (recl > 0)
    {
      written = fwrite(&recl, sizeof(recl), 1, out_fp);


      if (written < 1)
      {
        printf("fwrite() wrote fewer items than expected (%d) at record %d while writing prefix\n", written, rec_total);
      }
      
      written = fwrite(buffer, recl, 1, out_fp);

      if (written < 1)
      {
        printf("fwrite() wrote fewer items than expected (%d) at record %d\n", written, rec_total);
      }
      
      ++rec_ok;
    }
  }
  while (recl > 0);

  printf("Copied %d records from %s to %s (binary, length prefixed)\n", rec_ok, input_file, output_file);
  
  free(buffer);
  buffer = NULL;
  
  fclose(in_fp);
  fclose(out_fp);
}

int
main (int argc, char** argv)
{
  if (argc < 3)
  {
    printf("usage: %s <record file> <output file>\n", argv[0]);
    return 22;
  } else {
    convert(argv[1], argv[2]);
    return 0;
  }
}
