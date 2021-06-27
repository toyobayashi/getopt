module.exports = {
  project: 'getopt',
  targets: [
    {
      name: 'getopt',
      type: 'dll',
      sources: [
        './src/getopt.c',
        './src/getopt1.c',
      ],
      defines: ['GETOPT_BUILD_DLL'],
      
      staticVCRuntime: false
    },
    {
      name: 'test',
      type: 'exe',
      sources: [
        './src/main.c'
      ],
      defines: ['GETOPT_USE_DLL'],
      libs: ['getopt'],
      staticVCRuntime: false
    }
  ]
}
