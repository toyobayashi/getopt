#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#endif

#define NODE_ADDON_API_DISABLE_DEPRECATED
#include <napi.h>

#include <string>
#include <vector>
#include "getopt.h"

namespace {

Napi::Value optargGetter(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (optarg == nullptr) {
    return env.Null();
  }
  return Napi::String::New(env, optarg);
}

Napi::Value optindGetter(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::Number::New(env, static_cast<double>(optind));
}

void optindSetter(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::Error::New(env, "optind=() arguments.length < 1").ThrowAsJavaScriptException();
    return;
  }
  const Napi::Value& arg0 = info[0];
  if (!arg0.IsNumber()) {
    Napi::Error::New(env, "optind=() Expects a number as first argument.").ThrowAsJavaScriptException();
    return;
  }
  optind = arg0.As<Napi::Number>().Int32Value();
}

Napi::Value optoptGetter(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::Number::New(env, static_cast<double>(optopt));
}

Napi::Value opterrGetter(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::Number::New(env, static_cast<double>(opterr));
}

void opterrSetter(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::Error::New(env, "opterr=() arguments.length < 1").ThrowAsJavaScriptException();
    return;
  }
  const Napi::Value& arg0 = info[0];
  if (!arg0.IsNumber()) {
    Napi::Error::New(env, "opterr=() Expects a number as first argument.").ThrowAsJavaScriptException();
    return;
  }
  opterr = arg0.As<Napi::Number>().Int32Value();
}

void argvFinalizer(napi_env env, void* finalize_data, void* finalize_hint) {
  std::vector<char*> *argv = static_cast<std::vector<char*>*>(finalize_data);
  for (size_t i = 0; i < argv->size(); i++) {
    delete[] (*argv)[i];
  }
  delete argv;
}

Napi::Value jsGetopt(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 3) {
    Napi::Error::New(env, "getopt() arguments.length < 3").ThrowAsJavaScriptException();
    return Napi::Value();
  }
  const Napi::Value& arg0 = info[0];
  const Napi::Value& arg1 = info[1];
  const Napi::Value& arg2 = info[2];
  if (!arg0.IsNumber()) {
    Napi::Error::New(env, "getopt() Expects a number as first argument.").ThrowAsJavaScriptException();
    return Napi::Value();
  }
  if (!arg1.IsArray()) {
    Napi::Error::New(env, "getopt() Expects an Array as second argument.").ThrowAsJavaScriptException();
    return Napi::Value();
  }
  if (!arg2.IsString()) {
    Napi::Error::New(env, "getopt() Expects a string as third argument.").ThrowAsJavaScriptException();
    return Napi::Value();
  }
  int32_t argc = arg0.As<Napi::Number>().Int32Value();
  std::string shortopts = arg2.As<Napi::String>().Utf8Value();
  Napi::Array jsArgv = arg1.As<Napi::Array>();

  std::vector<char*> *argv = nullptr;
  napi_status s = napi_unwrap(env, jsArgv, reinterpret_cast<void**>(&argv));
  if (s != napi_ok || argv == nullptr) {
    uint32_t arrlen = jsArgv.Length();
    argv = new std::vector<char*>(arrlen);
    for (uint32_t i = 0; i < arrlen; i++) {
      Napi::Value el = jsArgv.Get(i);
      if (!el.IsString()) {
        Napi::Error::New(env, "getopt() Expects an Array of string as second argument.").ThrowAsJavaScriptException();
        return Napi::Value();
      }
      std::string argstr = el.As<Napi::String>().Utf8Value();
      (*argv)[i] = new char[argstr.length() + 1];
      strcpy((*argv)[i], argstr.c_str());
    }
    s = napi_wrap(env, arg1, argv, argvFinalizer, nullptr, nullptr);
    if (s != napi_ok) {
      argvFinalizer(env, argv, nullptr);
      Napi::Error::New(env).ThrowAsJavaScriptException();
      return Napi::Value();
    }
  }

  int r = getopt(static_cast<int>(argc), &(*argv)[0], shortopts.c_str());

  for (int32_t x = 0; x < argc; x++) {
    jsArgv.Set(x, Napi::String::New(env, (*argv)[x]));
  }

  return Napi::Number::New(env, static_cast<double>(r));
}

Napi::Object init(Napi::Env env, Napi::Object exports) {
  exports.DefineProperties({
    Napi::PropertyDescriptor::Accessor<optargGetter>("optarg", napi_enumerable),
    Napi::PropertyDescriptor::Accessor<optindGetter, optindSetter>("optind", napi_enumerable),
    Napi::PropertyDescriptor::Accessor<optoptGetter>("optopt", napi_enumerable),
    Napi::PropertyDescriptor::Accessor<opterrGetter, opterrSetter>("opterr", napi_enumerable),
    Napi::PropertyDescriptor({ "getopt", nullptr, nullptr, nullptr, nullptr, Napi::Function::New(env, jsGetopt, "getopt", nullptr), napi_enumerable, nullptr })
  });
  return exports;
}

}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, init)
