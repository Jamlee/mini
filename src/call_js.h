#include "v8.h"

namespace mini
{
  class CallJS
  {
  public:
    int Run(const char *path);
    
    bool ExecuteString(v8::Isolate *isolate, v8::Local<v8::String> source,
                       v8::Local<v8::Value> name, bool print_result,
                       bool report_exceptions);
    void ReportException(v8::Isolate *isolate, v8::TryCatch *try_catch);
    const char *ToCString(const v8::String::Utf8Value &value);
  };
}
