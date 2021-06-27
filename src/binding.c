#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <node_api.h>
#include "getopt.h"
#include "binding.h"

static napi_value js_optarg_getter(napi_env env, napi_callback_info info) {
  napi_value nil;
  napi_value optarg_str;
#ifdef _WIN32
  napi_status s;
  int len;
  unsigned short* optargw;
  char* optarga;
#endif

  if (optarg == NULL) {
    NAPI_CALL(env, napi_get_null(env, &nil));
    return nil;
  }

// #ifdef _WIN32
//   len = MultiByteToWideChar(CP_ACP, 0, optarg, -1, NULL, 0);
//   if (len == 0) return NULL;
//   optargw = (unsigned short*)malloc(sizeof(unsigned short) * len);
//   if (optargw == NULL) return NULL;
//   MultiByteToWideChar(CP_ACP, 0, optarg, -1, optargw, len);

//   len = WideCharToMultiByte(CP_UTF8, 0, optargw, -1, NULL, 0, NULL, NULL);
//   if (len == 0) {
//     free(optargw);
//     return NULL;
//   }
//   optarga = (char*)malloc(sizeof(char) * len);
//   if (optarga == NULL) {
//     free(optargw);
//     return NULL;
//   }
//   WideCharToMultiByte(CP_UTF8, 0, optargw, -1, optarga, len, NULL, NULL);
//   free(optargw);
//   s = napi_create_string_utf8(env, optarga, NAPI_AUTO_LENGTH, &optarg_str);
//   if (s != napi_ok) {
//     free(optarga);
//     GET_AND_THROW_LAST_ERROR((env));
//     return NULL;
//   }
//   free(optarga);
// #else
  NAPI_CALL(env, napi_create_string_utf8(env, optarg, NAPI_AUTO_LENGTH, &optarg_str));
// #endif
  return optarg_str;
}

static napi_value js_optind_getter(napi_env env, napi_callback_info info) {
  napi_value js_optind;
  NAPI_CALL(env, napi_create_int32(env, optind, &js_optind));
  return js_optind;
}

static napi_value js_optind_setter(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value argv[1];
  int32_t arg0 = 0;
  napi_valuetype valuetype0;

  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, NULL, NULL));
  if (argc < 1) {
    NAPI_CALL(env, napi_throw_error(env, NULL, "optind=() arguments.length < 1"));
    return NULL;
  }

  NAPI_CALL(env, napi_typeof(env, argv[0], &valuetype0));
  NAPI_ASSERT(env, valuetype0 == napi_number,
      "optind=() Expects a number as first argument.");

  NAPI_CALL(env, napi_get_value_int32(env, argv[0], &arg0));
  optind = (int)arg0;
  return NULL;
}

static napi_value js_optopt_getter(napi_env env, napi_callback_info info) {
  napi_value js_optopt;
  NAPI_CALL(env, napi_create_int32(env, optopt, &js_optopt));
  return js_optopt;
}

static napi_value js_opterr_getter(napi_env env, napi_callback_info info) {
  napi_value js_opterr;
  NAPI_CALL(env, napi_create_int32(env, opterr, &js_opterr));
  return js_opterr;
}

static napi_value js_opterr_setter(napi_env env, napi_callback_info info) {
  size_t argc = 1;
  napi_value argv[1];
  int32_t arg0 = 0;
  napi_valuetype valuetype0;

  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, NULL, NULL));
  if (argc < 1) {
    NAPI_CALL(env, napi_throw_error(env, NULL, "opterr=() arguments.length < 1"));
    return NULL;
  }

  NAPI_CALL(env, napi_typeof(env, argv[0], &valuetype0));
  NAPI_ASSERT(env, valuetype0 == napi_number,
      "opterr=() Expects a number as first argument.");

  NAPI_CALL(env, napi_get_value_int32(env, argv[0], &arg0));
  opterr = (int)arg0;
  return NULL;
}

static void argv_finalizer(napi_env env, void* finalize_data, void* finalize_hint) {
  uint32_t i;
  uint32_t* p = (uint32_t*)finalize_hint;
  char** argv = (char**)finalize_data;
  uint32_t arrlen = *p;
  for (i = 0; i <= arrlen; i++) { free(*(argv + i)); }
  free(argv);
  free(p);
}

static napi_value js_getopt(napi_env env, napi_callback_info info) {
  size_t argc = 3;
  napi_value argv[3];
  napi_status s;
  napi_valuetype valuetype0;
  bool is_array;
  int32_t c_argc = 0;
  char** c_argv = NULL;
  void* args = NULL;
  char* shortopts = NULL;
  size_t strlength = 0;
  uint32_t arrlen = 0;
  uint32_t i, j;
  uint32_t* hint;
  int r;
  napi_value el;
  napi_value getopt_rs;

  NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, NULL, NULL));
  if (argc < 3) {
    NAPI_CALL(env, napi_throw_error(env, NULL, "getopt() arguments.length < 3"));
    return NULL;
  }

  NAPI_CALL(env, napi_typeof(env, argv[0], &valuetype0));
  NAPI_ASSERT(env, valuetype0 == napi_number, "getopt() Expects a number as first argument.");
  NAPI_CALL(env, napi_is_array(env, argv[1], &is_array));
  NAPI_ASSERT(env, is_array, "getopt() Expects an Array as second argument.");
  NAPI_CALL(env, napi_typeof(env, argv[2], &valuetype0));
  NAPI_ASSERT(env, valuetype0 == napi_string, "getopt() Expects a string as third argument.");

  NAPI_CALL(env, napi_get_value_int32(env, argv[0], &c_argc));
  NAPI_CALL(env, napi_get_value_string_utf8(env, argv[2], NULL, 0, &strlength));
  shortopts = (char*)malloc(sizeof(char) * (strlength + 1));
  if (shortopts == NULL) return NULL;
  s = napi_get_value_string_utf8(env, argv[2], shortopts, strlength + 1, &strlength);
  if (s != napi_ok) { free(shortopts); GET_AND_THROW_LAST_ERROR((env)); return NULL; }

  s = napi_get_array_length(env, argv[1], &arrlen);
  if (s != napi_ok) { free(shortopts); GET_AND_THROW_LAST_ERROR((env)); return NULL; }

  
  s = napi_unwrap(env, argv[1], &args);
  if (s != napi_ok || args == NULL) {
    c_argv = (char**)malloc(sizeof(char*) * (arrlen + 1));
    if (c_argv == NULL) {
      free(shortopts);
      return NULL;
    }
    memset(c_argv, 0, sizeof(char*) * (arrlen + 1));

    for (i = 0; i < arrlen; i++) {
      s = napi_get_element(env, argv[1], i, &el);
      if (s != napi_ok) {
        free(shortopts);
        argv_finalizer(env, c_argv, &i);
        GET_AND_THROW_LAST_ERROR((env));
        return NULL;
      }

      s = napi_typeof(env, el, &valuetype0);
      if (s != napi_ok) {
        free(shortopts);
        argv_finalizer(env, c_argv, &i);
        GET_AND_THROW_LAST_ERROR((env));
        return NULL;
      }
      if (valuetype0 != napi_string) {
        free(shortopts);
        argv_finalizer(env, c_argv, &i);
        NAPI_CALL(env, napi_throw_error(env, NULL, "getopt() Expects an Array of string as second argument."));
        return NULL;
      }

      s = napi_get_value_string_utf8(env, el, NULL, 0, &strlength);
      if (s != napi_ok) {
        free(shortopts);
        argv_finalizer(env, c_argv, &i);
        GET_AND_THROW_LAST_ERROR((env));
        return NULL;
      }
      *(c_argv + i) = malloc(sizeof(char) * (strlength + 1));
      if (NULL == *(c_argv + i)) {
        free(shortopts);
        argv_finalizer(env, c_argv, &i);
        NAPI_CALL(env, napi_throw_error(env, NULL, "getopt() malloc failed."));
        return NULL;
      }
      s = napi_get_value_string_utf8(env, el, *(c_argv + i), strlength + 1, &strlength);
      if (s != napi_ok) {
        free(shortopts);
        j = i + 1;
        argv_finalizer(env, c_argv, &j);
        GET_AND_THROW_LAST_ERROR((env));
        return NULL;
      }
    }
    hint = (uint32_t*)malloc(sizeof(uint32_t));
    *hint = arrlen;
    s = napi_wrap(env, argv[1], c_argv, argv_finalizer, hint, NULL);
    if (s != napi_ok) {
      free(shortopts);
      argv_finalizer(env, c_argv, &arrlen);
      free(hint);
      GET_AND_THROW_LAST_ERROR((env));
      return NULL;
    }
  } else {
    c_argv = (char**)args;
  }

  r = getopt((int)c_argc, c_argv, shortopts);
  free(shortopts);
  NAPI_CALL(env, napi_create_int32(env, (int32_t)r, &getopt_rs));
  return getopt_rs;
}

NAPI_MODULE_EXPORT napi_value NAPI_MODULE_INITIALIZER(napi_env env, napi_value exports);
napi_value NAPI_MODULE_INITIALIZER(napi_env env, napi_value exports) {
  napi_property_descriptor desc[] = {
    { "optarg", NULL, NULL, js_optarg_getter, NULL, NULL, napi_enumerable, NULL },
    { "optind", NULL, NULL, js_optind_getter, js_optind_setter, NULL, napi_enumerable, NULL },
    { "optopt", NULL, NULL, js_optopt_getter, NULL, NULL, napi_enumerable, NULL },
    { "opterr", NULL, NULL, js_opterr_getter, js_opterr_setter, NULL, napi_enumerable, NULL },
    { "getopt", NULL, js_getopt, NULL, NULL, NULL, napi_enumerable, NULL },
  };
  NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(napi_property_descriptor), desc));
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, NAPI_MODULE_INITIALIZER)
