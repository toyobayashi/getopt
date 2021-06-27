class Option {
  constructor (name, hasArg, flag, val) {
    /** @type {string} */
    this.name = name || ''
    /**
     * has_arg can't be an enum because some compilers complain about
     * type mismatches in all the code that assumes it is an int.
     * @type {0 | 1 | 2}
     */
    this.has_arg = hasArg || 0
    /** @type {null | { value: number }} */
    this.flag = flag
    /** @type {number} */
    this.val = val
  }
};


module.exports = require('./dist/getopt.node')

Object.defineProperties(module.exports, {
  no_argument: { enumerable: true, value: 0 },
  required_argument: { enumerable: true, value: 1 },
  optional_argument: { enumerable: true, value: 2 },
  Option: Option
})
