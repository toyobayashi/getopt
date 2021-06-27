# getopt

[WIP] GNU getopt Node-API binding.

```js
const binding = require('@tybys/getopt') // not published yet

;(function main (argc, argv) {
  let c
  while ((c = binding.getopt(argc, argv, 'abc:')) !== -1) {
    const char = String.fromCharCode(c)
    switch (char) {
      case 'a':
        console.log('option a')
        break
      case 'b':
        console.log('option b')
        break
      case 'c':
        console.log("option c with value '%s'", binding.optarg === null ? '(null)' : binding.optarg)
        break
      case '?':
        process.exit(1)
      default:
        console.log(`?? getopt returned character code ${c} ??`)
    }
  }
  if (binding.optind < argc) {
    process.stdout.write('non-option ARGV-elements: ')
    while (binding.optind < argc)
      process.stdout.write('%s ', argv[binding.optind++])
    process.stdout.write('\n')
  }
  process.exit(0)
})(process.argv.length - 1, process.argv.slice(1))
```
