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
