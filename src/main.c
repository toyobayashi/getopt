#include <stdio.h>  /* for printf */
#include <stdlib.h> /* for exit */
#include "getopt.h" /* for getopt_long; POSIX standard getopt is in unistd.h */
/* int main(int argc, char** argv) {
  int c;
  int digit_optind = 0;
  int aopt = 0, bopt = 0;
  char *copt = 0, *dopt = 0;
  // opterr = 0;
  static struct option long_options[] = {
      // NAME       ARGUMENT           FLAG  SHORTNAME
      {"add", required_argument, NULL, 0},
      {"append", no_argument, NULL, 0},
      {"delete", required_argument, NULL, 0},
      {"verbose", no_argument, NULL, 0},
      {"create", required_argument, NULL, 'c'},
      {"file", required_argument, NULL, 0},
      {NULL, 0, NULL, 0}};
  int option_index = 0;
  while ((c = getopt_long(argc, argv, "abc:d:012", long_options,
                          &option_index)) != -1) {
    int this_option_optind = optind ? optind : 1;
    switch (c) {
      case 0:
        printf("option %s", long_options[option_index].name);
        if (optarg)
          printf(" with arg %s", optarg);
        printf("\n");
        break;
      case '0':
      case '1':
      case '2':
        if (digit_optind != 0 && digit_optind != this_option_optind)
          printf("digits occur in two different argv-elements.\n");
        digit_optind = this_option_optind;
        printf("option %c\n", c);
        break;
      case 'a':
        printf("option a\n");
        aopt = 1;
        break;
      case 'b':
        printf("option b\n");
        bopt = 1;
        break;
      case 'c':
        printf("option c with value '%s'\n", optarg);
        copt = optarg;
        break;
      case 'd':
        printf("option d with value '%s'\n", optarg);
        dopt = optarg;
        break;
      case '?':
        break;
      default:
        printf("?? getopt returned character code 0%o ??\n", c);
    }
  }
  if (optind < argc) {
    printf("non-option ARGV-elements: ");
    while (optind < argc)
      printf("%s ", argv[optind++]);
    printf("\n");
  }
  printf("argv: ");
  for (int i = 0; i < argc; i++) {  
    printf("%s ", argv[i]);  
  }
  printf("\n");
  exit(0);
} */

int main(int argc, char** argv) {
  int c;
  int digit_optind = 0;
  int aopt = 0, bopt = 0;
  char *copt = 0, *dopt = 0;
  while ((c = getopt(argc, argv, "abc:d:012")) != -1) {
    int this_option_optind = optind ? optind : 1;
    switch (c) {
      case '0':
      case '1':
      case '2':
        if (digit_optind != 0 && digit_optind != this_option_optind)
          printf("digits occur in two different argv-elements.\n");
        digit_optind = this_option_optind;
        printf("option %c\n", c);
        break;
      case 'a':
        printf("option a\n");
        aopt = 1;
        break;
      case 'b':
        printf("option b\n");
        bopt = 1;
        break;
      case 'c':
        printf("option c with value '%s'\n", optarg);
        copt = optarg;
        break;
      case 'd':
        printf("option d with value '%s'\n", optarg);
        dopt = optarg;
        break;
      case '?':
        break;
      default:
        printf("?? getopt returned character code 0%o ??\n", c);
    }
  }
  if (optind < argc) {
    printf("non-option ARGV-elements: ");
    while (optind < argc)
      printf("%s ", argv[optind++]);
    printf("\n");
  }
  printf("argv: ");
  for (int i = 0; i < argc; i++) {  
    printf("%s ", argv[i]);  
  }
  printf("\n");
  exit(0);
}
