# getopt

GNU getopt Node-API binding.

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

```ts
declare namespace mod {
  export const optarg: string | null;
  export let optind: number;
  export const optopt: number;
  export let opterr: number;

  export const no_argument: 0;
  export const required_argument: 1;
  export const optional_argument: 2;

  export type Pointer<T> = { value: T };

  export class Option {
    name: string;
    has_arg: 0 | 1 | 2;
    flag: undefined | null | Pointer<number>;
    val: number;
    constructor (name: string, hasArg?: 0 | 1 | 2, flag?: null | Pointer<number>, val?: string | number);
  }

  export function getopt (argc: number, argv: string[], shortopts: string): number;
  export function getopt_long (argc: number, argv: string[], shortopts: string,
    longopts?: Option[], ind?: null | Pointer<number>): number;
  export function getopt_long_only (argc: number, argv: string[], shortopts: string,
    longopts?: Option[], ind?: null | Pointer<number>): number; 
}

export = mod;
```
