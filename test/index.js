const binding = require('..')
// const binding = require('../build/Debug/getopt.node')

function printf(...args) {
  process.stdout.write(require('util').format(...args))
}

function exit(code) {
  process.exit(code)
}

function main(argc, argv) {
  let c
  let digit_optind = 0
  let aopt = 0, bopt = 0
  let copt = null, dopt = null
  const long_options = [
    new binding.Option('add', binding.required_argument, null, 0),
    new binding.Option('append', binding.no_argument, null, 0),
    new binding.Option('delete', binding.required_argument, null, 0),
    new binding.Option('verbose', binding.no_argument, null, 0),
    new binding.Option('create', binding.required_argument, null, 'c'),
    new binding.Option('add', binding.required_argument, null, 0),
    new binding.Option('file', binding.required_argument, null, 0),
  ]
  let option_index = {
    value: 0
  }
  while ((c = binding.getopt_long(argc, argv, "abc:d:012", long_options, option_index)) != -1) {
    let this_option_optind = binding.optind ? binding.optind : 1
    switch (c) {
      case 0:
        printf("option %s", long_options[option_index.value].name)
        if (binding.optarg)
          printf(" with arg %s", binding.optarg)
        printf("\n")
        break
      case '0'.charCodeAt(0):
      case '1'.charCodeAt(0):
      case '2'.charCodeAt(0):
        if (digit_optind != 0 && digit_optind != this_option_optind)
          printf("digits occur in two different argv-elements.\n")
        digit_optind = this_option_optind
        printf("option %s\n", String.fromCharCode(c))
        break
      case 'a'.charCodeAt(0):
        printf("option a\n")
        aopt = 1
        break
      case 'b'.charCodeAt(0):
        printf("option b\n")
        bopt = 1
        break
      case 'c'.charCodeAt(0):
        printf("option c with value '%s'\n", binding.optarg)
        copt = binding.optarg
        break
      case 'd'.charCodeAt(0):
        printf("option d with value '%s'\n", binding.optarg)
        dopt = binding.optarg
        break
      case '?'.charCodeAt(0):
        break
      default:
        printf("?? getopt returned character code 0%o ??\n", c)
    }
  }
  if (binding.optind < argc) {
    printf("non-option ARGV-elements: ")
    while (binding.optind < argc)
      printf("%s ", argv[binding.optind++])
    printf("\n")
  }
  printf("argv: ")
  for (let i = 0; i < argc; i++) {
    printf("%s ", argv[i])
  }
  printf("\n")
  exit(0)

  /* let c
  let digit_optind = 0
  let aopt = 0, bopt = 0
  let copt = null, dopt = null
  while ((c = binding.getopt(argc, argv, "abc:d:012")) !== -1) {
    let this_option_optind = binding.optind ? binding.optind : 1
    const char = String.fromCharCode(c)
    switch (char) {
      case '0':
      case '1':
      case '2':
        if (digit_optind != 0 && digit_optind != this_option_optind)
          printf("digits occur in two different argv-elements.\n")
        digit_optind = this_option_optind
        printf("option %s\n", char)
        break
      case 'a':
        printf("option a\n")
        aopt = 1
        break
      case 'b':
        printf("option b\n")
        bopt = 1
        break
      case 'c':
        printf("option c with value '%s'\n", binding.optarg === null ? '(null)' : binding.optarg)
        copt = binding.optarg
        break
      case 'd':
        printf("option d with value '%s'\n", binding.optarg === null ? '(null)' : binding.optarg)
        dopt = binding.optarg
        break
      case '?':
        break
      default:
        printf("?? getopt returned character code 0%o ??\n", c)
    }
  }
  if (binding.optind < argc) {
    printf("non-option ARGV-elements: ")
    while (binding.optind < argc)
      printf("%s ", argv[binding.optind++])
    printf("\n")
  }
  printf("argv: ");
  for (let i = 0; i < argc; i++) {  
    printf("%s ", argv[i]);  
  }
  printf("\n");
  exit(0) */
}

main(process.argv.length - 1, process.argv.slice(1))
