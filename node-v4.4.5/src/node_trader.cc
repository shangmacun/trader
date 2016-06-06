#include "node.h"
#include "node_buffer.h"

#include "async-wrap.h"
#include "async-wrap-inl.h"
#include "env.h"
#include "env-inl.h"
#include "util.h"
#include "util-inl.h"

#include "v8.h"

#include "ThostFtdcMdApi.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

namespace node {

using v8::Array;
using v8::Context;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::HandleScope;
using v8::Integer;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;

void InitThostFtdcMdApi(v8::Local<v8::Object> target);

/*
* ThostFtdcMdApi
*/
class ThostFtdcMdApiCtx : public AsyncWrap {
  public:
    ThostFtdcMdApiCtx(Environment* env, Local<Object> wrap)
       : AsyncWrap(env, wrap, AsyncWrap::PROVIDER_THOSTFTDCMDAPIWRAP) {
       MakeWeak<ThostFtdcMdApiCtx>(this);
       m_instance = CThostFtdcMdApi::CreateFtdcMdApi("",false,false);
    }

    static void New(const FunctionCallbackInfo<Value>& args) {
      Environment* env = Environment::GetCurrent(args);
/*      if (args.Length() != 3) {
         return env->ThrowTypeError("Bad argument count or type");
      }
*/
      new ThostFtdcMdApiCtx(env,args.This());
    }

    void Release() {
      if (m_instance) {
        m_instance->Release();
      }
    }

    static void Release(const FunctionCallbackInfo<Value> &args) {
      ThostFtdcMdApiCtx* ctx = Unwrap<ThostFtdcMdApiCtx>(args.Holder());
      ctx->Release();
    }

    size_t self_size() const override { return sizeof(*this); }

  private: 
    CThostFtdcMdApi* m_instance;

};


void InitThostFtdcMdApi(Local<Object> target,
              Local<Value> unused,
              Local<Context> context,
              void* priv) {
  Environment* env = Environment::GetCurrent(context);
  Local<FunctionTemplate> z = env->NewFunctionTemplate(ThostFtdcMdApiCtx::New);

  z->InstanceTemplate()->SetInternalFieldCount(1);

  env->SetProtoMethod(z, "Release", ThostFtdcMdApiCtx::Release);
/*
  env->SetProtoMethod(z, "Init", CThostFtdcMdApi::Init);
  env->SetProtoMethod(z, "Join", CThostFtdcMdApi::Join);
  env->SetProtoMethod(z, "GetTradingDay", CThostFtdcMdApi::GetTradingDay);
  env->SetProtoMethod(z, "RegisterFront", CThostFtdcMdApi::RegisterFront);
  env->SetProtoMethod(z, "RegisterNameServer", CThostFtdcMdApi::RegisterNameServer);
  */
  z->SetClassName(FIXED_ONE_BYTE_STRING(env->isolate(), "ThostFtdcMdApi"));
  target->Set(FIXED_ONE_BYTE_STRING(env->isolate(), "ThostFtdcMdApi"), z->GetFunction());
}

}  // namespace node

NODE_MODULE_CONTEXT_AWARE_BUILTIN(ThostFtdcMdApi, node::InitThostFtdcMdApi)
