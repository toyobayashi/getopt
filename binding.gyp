{
  "targets": [
    {
      "target_name": "getopt",
      'includes': [
        './common.gypi'
      ],
      "sources": [
        "src/binding.cpp",
        "src/getopt.c"
      ]
    },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "getopt" ],
      "copies": [
        {
          "files": [ "<(PRODUCT_DIR)/getopt.node" ],
          "destination": "./dist"
        }
      ]
    }
  ]
}
