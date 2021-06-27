declare namespace getopt {
  export const optarg: string | null;
  export let optind: number;
  export const optopt: number;
  export let opterr: number;

  export const no_argument: 0;
  export const required_argument: 1
  export const optional_argument: 2

  export function getopt (argc: number, argv: string[], shortopts: string): number;
}

export = getopt;
