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

void optionsFinalizer(napi_env env, void* finalize_data, void* finalize_hint) {
  std::vector<struct option> *options = static_cast<std::vector<struct option>*>(finalize_data);
  for (size_t i = 0; i < options->size(); i++) {
    const struct option& o = (*options)[i];
    delete[] o.name;
    if (o.flag != nullptr) {
      delete o.flag;
    }
  }
  delete options;
}

std::vector<char*>* tryWrapArgv(const Napi::Env& env, const Napi::Array& jsArgv, uint32_t arrlen) {
  std::vector<char*> *argv = nullptr;
  napi_status s = napi_unwrap(env, jsArgv, reinterpret_cast<void**>(&argv));
  if (s != napi_ok || argv == nullptr) {
    argv = new std::vector<char*>(arrlen);
    for (uint32_t i = 0; i < arrlen; i++) {
      Napi::Value el = jsArgv.Get(i);
      std::string argstr = el.As<Napi::String>().Utf8Value();
      (*argv)[i] = new char[argstr.length() + 1];
      strcpy((*argv)[i], argstr.c_str());
    }
    s = napi_wrap(env, jsArgv, argv, argvFinalizer, nullptr, nullptr);
    if (s != napi_ok) {
      argvFinalizer(env, argv, nullptr);
      Napi::Error::New(env).ThrowAsJavaScriptException();
      return nullptr;
    }
  }
  return argv;
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
    Napi::TypeError::New(env, "getopt() Expects a number as first argument.").ThrowAsJavaScriptException();
    return Napi::Value();
  }
  if (!arg1.IsArray()) {
    Napi::TypeError::New(env, "getopt() Expects an Array as second argument.").ThrowAsJavaScriptException();
    return Napi::Value();
  }
  Napi::Array jsArgv = arg1.As<Napi::Array>();
  uint32_t arrlen = jsArgv.Length();
  for (uint32_t i = 0; i < arrlen; i++) {
    Napi::Value el = jsArgv.Get(i);
    if (!el.IsString()) {
      Napi::TypeError::New(env, "getopt() Expects an Array of string as second argument.").ThrowAsJavaScriptException();
      return Napi::Value();
    }
  }
  if (!arg2.IsString()) {
    Napi::TypeError::New(env, "getopt() Expects a string as third argument.").ThrowAsJavaScriptException();
    return Napi::Value();
  }
  int32_t argc = arg0.As<Napi::Number>().Int32Value();
  std::string shortopts = arg2.As<Napi::String>().Utf8Value();

  std::vector<char*> *argv = tryWrapArgv(env, jsArgv, arrlen);
  if (argv == nullptr) {
    return Napi::Value();
  }

  int r = ::getopt(static_cast<int>(argc), &(*argv)[0], shortopts.c_str());

  for (int32_t x = 0; x < argc; x++) {
    jsArgv.Set(x, Napi::String::New(env, (*argv)[x]));
  }

  return Napi::Number::New(env, static_cast<double>(r));
}

std::vector<struct option>* tryWrapOption(const Napi::Env& env, const Napi::Array& jsOptions, uint32_t optslen) {
  std::vector<struct option> *options = nullptr;
  napi_status s = napi_unwrap(env, jsOptions, reinterpret_cast<void**>(&options));
  if (s != napi_ok || options == nullptr) {
    options = new std::vector<struct option>(optslen + 1);
    for (uint32_t i = 0; i < optslen; i++) {
      Napi::Object el = jsOptions.Get(i).As<Napi::Object>();
      Napi::Value nameValue = el.Get("name");
      if (nameValue.IsString()) {
        std::string name = el.Get("name").As<Napi::String>().Utf8Value();
        char* cname = new char[name.length() + 1];
        strcpy(cname, name.c_str());
        (*options)[i].name = cname;
      } else {

      }

      (*options)[i].has_arg = static_cast<int>(el.Get("has_arg").As<Napi::Number>().Int32Value());
      Napi::Value jsFlag = el.Get("flag");
      (*options)[i].flag = (jsFlag.IsNull() || jsFlag.IsUndefined()) ? nullptr : new int(0);
      (*options)[i].val = static_cast<int>(el.Get("val").As<Napi::Number>().Int32Value());
    }
    (*options)[optslen].name = nullptr;
    (*options)[optslen].has_arg = 0;
    (*options)[optslen].flag = nullptr;
    (*options)[optslen].val = 0;
    s = napi_wrap(env, jsOptions, options, optionsFinalizer, nullptr, nullptr);
    if (s != napi_ok) {
      optionsFinalizer(env, options, nullptr);
      Napi::Error::New(env).ThrowAsJavaScriptException();
      return nullptr;
    }
  }
  return options;
}

using getopt_lont_t = int (*)(int, char *__getopt_argv_const *, const char *, const struct option *, int *);

Napi::Value getoptLong(const Napi::CallbackInfo& info, getopt_lont_t callback) {
  Napi::Env env = info.Env();
  if (info.Length() < 3) {
    Napi::Error::New(env, "getopt_long() arguments.length < 3").ThrowAsJavaScriptException();
    return Napi::Value();
  }
  const Napi::Value& arg0 = info[0];
  const Napi::Value& arg1 = info[1];
  const Napi::Value& arg2 = info[2];
  if (!arg0.IsNumber()) {
    Napi::TypeError::New(env, "getopt_long() Expects a number as first argument.").ThrowAsJavaScriptException();
    return Napi::Value();
  }
  if (!arg1.IsArray()) {
    Napi::TypeError::New(env, "getopt_long() Expects an Array as second argument.").ThrowAsJavaScriptException();
    return Napi::Value();
  }
  Napi::Array jsArgv = arg1.As<Napi::Array>();
  uint32_t arrlen = jsArgv.Length();
  for (uint32_t i = 0; i < arrlen; i++) {
    Napi::Value el = jsArgv.Get(i);
    if (!el.IsString()) {
      Napi::TypeError::New(env, "getopt_long() Expects an Array of string as second argument.").ThrowAsJavaScriptException();
      return Napi::Value();
    }
  }
  if (!arg2.IsString()) {
    Napi::TypeError::New(env, "getopt() Expects a string as third argument.").ThrowAsJavaScriptException();
    return Napi::Value();
  }
  int32_t argc = arg0.As<Napi::Number>().Int32Value();
  std::string shortopts = arg2.As<Napi::String>().Utf8Value();

  std::vector<struct option>* options = nullptr;

  if (info.Length() >= 4) {
    const Napi::Value& arg3 = info[3];
    if (!arg3.IsUndefined() && !arg3.IsNull() && !arg3.IsArray()) {
      Napi::TypeError::New(env, "getopt_long() Expects an Array or null as fourth argument.").ThrowAsJavaScriptException();
      return Napi::Value();
    }
    if (arg3.IsArray()) {
      Napi::Array jsOptions = arg3.As<Napi::Array>();
      uint32_t optslen = jsOptions.Length();
      for (uint32_t i = 0; i < optslen; i++) {
        Napi::Value el = jsOptions.Get(i);
        if (!el.IsObject()) {
          Napi::TypeError::New(env, "getopt_long() Expects an Array of object as fourth argument.").ThrowAsJavaScriptException();
          return Napi::Value();
        }
        Napi::Object elobj = el.As<Napi::Object>();
        Napi::Value nameValue = elobj.Get("name");
        if (!nameValue.IsString()) {
          Napi::TypeError::New(env, "getopt_long() typeof option.name !== 'string'").ThrowAsJavaScriptException();
          return Napi::Value();
        }
        Napi::Value hasArgValue = elobj.Get("has_arg");
        if (!hasArgValue.IsNumber()) {
          Napi::TypeError::New(env, "getopt_long() typeof option.has_arg !== 'number'").ThrowAsJavaScriptException();
          return Napi::Value();
        }
        int32_t hasArg = hasArgValue.As<Napi::Number>().Int32Value();
        if (hasArg != no_argument && hasArg != required_argument && hasArg != optional_argument) {
          Napi::TypeError::New(env, "getopt_long() typeof option.has_arg !== 0 | 1 | 2").ThrowAsJavaScriptException();
          return Napi::Value();
        }
        Napi::Value flagValue = elobj.Get("flag");
        if (!flagValue.IsUndefined() && !flagValue.IsNull() && !flagValue.IsObject()) {
          Napi::TypeError::New(env, "getopt_long() option.flag must be null or object").ThrowAsJavaScriptException();
          return Napi::Value();
        }
        Napi::Value valValue = elobj.Get("val");
        if (!valValue.IsNumber()) {
          Napi::TypeError::New(env, "getopt_long() typeof option.val !== 'number'").ThrowAsJavaScriptException();
          return Napi::Value();
        }
      }
      options = tryWrapOption(env, jsOptions, optslen);
      if (options == nullptr) {
        return Napi::Value();
      }
    }
  }

  if (info.Length() >= 5) {
    const Napi::Value& arg4 = info[4];
    if (!arg4.IsUndefined() && !arg4.IsNull() && !arg4.IsObject()) {
      Napi::TypeError::New(env, "getopt_long() Expects an object or null as fifth argument.").ThrowAsJavaScriptException();
      return Napi::Value();
    }
  }

  std::vector<char*> *argv = tryWrapArgv(env, jsArgv, arrlen);
  if (argv == nullptr) {
    return Napi::Value();
  }

  int ind = 0;
  int r = callback(static_cast<int>(argc), &(*argv)[0], shortopts.c_str(), options == nullptr ? nullptr : &(*options)[0], &ind);

  if (r == 0 && options != nullptr) {
    Napi::Object jsOpt = info[3].As<Napi::Array>().Get(static_cast<uint32_t>(ind)).As<Napi::Object>();
    jsOpt.Get("flag").As<Napi::Object>().Set("value", Napi::Number::New(env, static_cast<double>(*((*options)[ind].flag))));
  }

  if (info.Length() >= 5 && info[4].IsObject()) {
    info[4].As<Napi::Object>().Set("value", Napi::Number::New(env, static_cast<double>(ind)));
  }

  for (int32_t x = 0; x < argc; x++) {
    jsArgv.Set(x, Napi::String::New(env, (*argv)[x]));
  }

  return Napi::Number::New(env, static_cast<double>(r));
}

Napi::Value jsGetoptLong(const Napi::CallbackInfo& info) {
  return getoptLong(info, ::getopt_long);
}

Napi::Value jsGetoptLongOnly(const Napi::CallbackInfo& info) {
  return getoptLong(info, ::getopt_long_only);
}

Napi::Object init(Napi::Env env, Napi::Object exports) {
  exports.DefineProperties({
    Napi::PropertyDescriptor::Accessor<optargGetter>("optarg", napi_enumerable),
    Napi::PropertyDescriptor::Accessor<optindGetter, optindSetter>("optind", napi_enumerable),
    Napi::PropertyDescriptor::Accessor<optoptGetter>("optopt", napi_enumerable),
    Napi::PropertyDescriptor::Accessor<opterrGetter, opterrSetter>("opterr", napi_enumerable),
    Napi::PropertyDescriptor::Function(env, exports, "getopt", jsGetopt, napi_enumerable),
    Napi::PropertyDescriptor::Function(env, exports, "getopt_long", jsGetoptLong, napi_enumerable),
    Napi::PropertyDescriptor::Function(env, exports, "getopt_long_only", jsGetoptLongOnly, napi_enumerable)
  });
  return exports;
}

}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, init)
