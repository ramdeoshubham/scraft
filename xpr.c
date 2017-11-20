/**
    xpr.c (c) Shubham Ramdeo
    Purpose: A string manipulation utility.
    @author Shubham Ramdeo
    @version 0.2

github.com/ramdeoshubham/scraft

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

  char * str_replace(char * orig, char * rep, char * with) {
    char * result;
    char * ins;
    char * tmp;
    int len_rep; 
    int len_with;
    int len_front;
    int count;

    // sanity checks and initialization
    if (!orig || !rep)
      return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
      return NULL; // empty rep causes infinite loop during count
    if (!with)
      with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
      ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
      return NULL;

    while (count--) {
      ins = strstr(orig, rep);
      len_front = ins - orig;
      tmp = strncpy(tmp, orig, len_front) + len_front;
      tmp = strcpy(tmp, with) + len_with;
      orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
  }

void Usage() {
  printf("xpr version 1.0 \nCopyright Shubham Ramdeo\n");
  printf("Usage: xpr [file] [options]... [pattern] [file]\n");
  printf("Try 'xpr --help' for more information.\n");
}

int output(char * fname) {
  FILE * fp;

  //gcc users
  if ((fp = fopen(fname, "r")) == NULL) {
    return (-1);
  }

  char c;
  int ln = 1;
  if ((fp = fopen(fname, "r")) == NULL) {
    return (-1);
  }
  printf("  1. ");
  while ((c = fgetc(fp)) != EOF) {
    printf("%c", c);
    if (c == '\n') {
      ln++;
      printf("%3d. ", ln);
    }

  }
  printf("\n");
  return (0);
}

int Search_in_File(char * fname, char * str) {
  FILE * fp;
  int line_num = 1;
  int find_result = 0;
  char temp[512];

  //gcc users
  if ((fp = fopen(fname, "r")) == NULL) {
    return (-1);
  }

  int i, total = 0;
  char * x, * t;
  while (fgets(temp, 512, fp) != NULL) {
    x = temp;
    //if(x != NULL){
    while ((x = strstr(x, str)) != NULL) {
      printf("A match found on line: %d col: %d\n ", line_num, x - temp + 1);
      printf("\n%s", temp);
      for (i = 0; i < (x - temp); i++) printf(" ");
      printf("^\n");
      find_result++;
      //total++;
      x++;
    }
    line_num++;
  }

  if (find_result == 0) {
    printf("\nSorry, couldn't find a match.\n");
  }

  //Close the file if still open.
  if (fp) {
    fclose(fp);
  }
  return (0);
}

int replaceinfile(char * fname, char * what, char * with, char * outf) {

  if (!strcmp(fname, outf)) {
    puts("Cannot write output to input file");
    return (-1);
  }
  FILE * fp, * tf;
  int line_num = 1;
  int find_result = 0;
  char temp[512];
  //gcc users
  if ((fp = fopen(fname, "r")) == NULL) {
    return (-1);
  }
  if ((tf = fopen(outf, "w")) == NULL) {
    printf("error");
    return (-1);
  }

  char * x;
  while (fgets(temp, 512, fp) != NULL) {
    x = str_replace(temp, what, with);
    fprintf(tf, "%s", x);
  }

  if (fp) {
    fclose(fp);
  }
  if (fp) {
    fclose(tf);
  }
  return 0;

}

int append(char * fname, char * str) {
  FILE * fptr;

  fptr = fopen(fname, "a+");
  if (fptr == NULL) {
    printf("Error!");
    return (-1);
  }

  fprintf(fptr, "%s\n", str);
  fclose(fptr);

  return 0;
}

int main(int argc, char * argv[]) {
  int result, errno;
  if (argc < 2) {
    Usage();
    exit(1);
  }
  FILE * fp;

  if (!strcmp(argv[1], "--help")) {
    printf("xpr version 0.2 \nCopyright Shubham Ramdeo\n");
    printf("Usage: xpr [file] [options]... [pattern] [file]\n");
    printf("\nOptions\tDescription\n");
    printf("  -o\tThis will show the contents of the file along line numbers.\n");
    printf("\t\tExample: xpr -o file.txt\n");
    printf("  -a\tThis will append the file with a new string at end.\n");
    printf("\t\tExample: xpr -a file.txt \"Another String\"\n");
    printf("  -s\tThis will search for a string match in the file.\n");
    printf("\t\tExample: xpr -s file.txt \"apple\"\n");
    printf("  -r\tThis will replace the file contents with the desired\n\treplacement and will save it in a new file.\n");
    printf("\t\tExample: xpr -r file.txt \"fruit\" \"vegitable\"\n");
    return (0);
  }
  if (!strcmp(argv[1], "--version")) {
    printf("xpr version 0.2 \nCopyright Shubham Ramdeo\n");
    return (0);
  }

  char c;
  int ln;
  char * res;
  switch (argv[2][1]) {
  case 'r':
    printf("replace");
    if (strlen(argv[1]) == 0 || strlen(argv[3]) == 0 || strlen(argv[4]) == 0 || strlen(argv[5]) == 0) {
      Usage();
      break;
      return (0);
    }
    result = replaceinfile(argv[1], argv[3], argv[4], argv[5]);
    break;
  case 'o':
    result = output(argv[1]);
    break;
  case 's':
    result = Search_in_File(argv[1], argv[3]);
    break;
  case 'a':
    result = append(argv[1], argv[3]);
    break;
  default:
    Usage();
  }
  if (result == -1) {
    perror("Error");
    printf("Error number = %d\n", errno);
    exit(1);
  }
  return (0);
}
