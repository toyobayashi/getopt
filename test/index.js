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
  printf("option ? with value '%s'\n", binding.optarg === null ? '(null)' : binding.optarg)
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
        printf("option %c\n", c)
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
  exit(0)
}

main(process.argv.length - 1, process.argv.slice(1))
