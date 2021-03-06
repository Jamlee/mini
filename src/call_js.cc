#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "libplatform/libplatform.h"
#include "v8.h"
#include "call_js.h"

namespace mini
{

  int CallJS::Run(const char *path)
  {
    // Initialize V8.
    v8::V8::InitializeICUDefaultLocation(path);
    v8::V8::InitializeExternalStartupData(path);
    std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();
    // Create a new Isolate and make it the current one.
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator =
        v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    v8::Isolate *isolate = v8::Isolate::New(create_params);
    {
      v8::Isolate::Scope isolate_scope(isolate);
      // Create a stack-allocated handle scope.
      v8::HandleScope handle_scope(isolate);
      // Create a new context.
      v8::Local<v8::Context> context = v8::Context::New(isolate);
      // Enter the context for compiling and running the hello world script.
      v8::Context::Scope context_scope(context);
      // Create a string containing the JavaScript source code.
      v8::Local<v8::String> source =
          v8::String::NewFromUtf8(isolate, "'Hello' + ', World!'",
                                  v8::NewStringType::kNormal)
              .ToLocalChecked();
      // Compile the source code.
      v8::Local<v8::Script> script =
          v8::Script::Compile(context, source).ToLocalChecked();
      // Run the script to get the result.
      v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
      // Convert the result to an UTF8 string and print it.
      v8::String::Utf8Value utf8(isolate, result);
      printf("%s\n", *utf8);

      // 编译字符串
      v8::Local<v8::String> jsSource = v8::String::NewFromUtf8(isolate, "(function(){return 'this is function'})();", v8::NewStringType::kNormal).ToLocalChecked();
      v8::Local<v8::String> name = v8::String::NewFromUtf8(isolate, "hello.js", v8::NewStringType::kNormal).ToLocalChecked();
      bool execute = ExecuteString(isolate, jsSource, name, true, true);
      if (!execute)
      {
        printf("%s\n", *jsSource);
      }
    }

    // Dispose the isolate and tear down V8.
    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    delete create_params.array_buffer_allocator;
    return 0;
  }

  // Executes a string within the current v8 context.
  bool CallJS::ExecuteString(v8::Isolate *isolate, v8::Local<v8::String> source,
                             v8::Local<v8::Value> name, bool print_result,
                             bool report_exceptions)
  {

    v8::HandleScope handle_scope(isolate);
    v8::TryCatch try_catch(isolate);
    v8::ScriptOrigin origin(name);
    v8::Local<v8::Context> context(isolate->GetCurrentContext());
    v8::Local<v8::Script> script;

    if (!v8::Script::Compile(context, source, &origin).ToLocal(&script))
    {
      // Print errors that happened during compilation.
      if (report_exceptions)
        ReportException(isolate, &try_catch);
      return false;
    }
    else
    {
      v8::Local<v8::Value> result;
      if (!script->Run(context).ToLocal(&result))
      {
        assert(try_catch.HasCaught());
        // Print errors that happened during execution.
        if (report_exceptions)
          ReportException(isolate, &try_catch);
        return false;
      }
      else
      {
        assert(!try_catch.HasCaught());
        if (print_result && !result->IsUndefined())
        {
          // If all went well and the result wasn't undefined then print
          // the returned value.
          v8::String::Utf8Value str(isolate, result);
          const char *cstr = ToCString(str);
          printf("%s\n", cstr);
        }
        return true;
      }
    }
  }

  void CallJS::ReportException(v8::Isolate *isolate, v8::TryCatch *try_catch)
  {
    v8::HandleScope handle_scope(isolate);
    v8::String::Utf8Value exception(isolate, try_catch->Exception());
    const char *exception_string = ToCString(exception);
    v8::Local<v8::Message> message = try_catch->Message();
    if (message.IsEmpty())
    {
      // V8 didn't provide any extra information about this error; just
      // print the exception.
      fprintf(stderr, "%s\n", exception_string);
    }
    else
    {
      // Print (filename):(line number): (message).
      v8::String::Utf8Value filename(isolate,
                                     message->GetScriptOrigin().ResourceName());
      v8::Local<v8::Context> context(isolate->GetCurrentContext());
      const char *filename_string = ToCString(filename);
      int linenum = message->GetLineNumber(context).FromJust();
      fprintf(stderr, "%s:%i: %s\n", filename_string, linenum, exception_string);
      // Print line of source code.
      v8::String::Utf8Value sourceline(
          isolate, message->GetSourceLine(context).ToLocalChecked());
      const char *sourceline_string = ToCString(sourceline);
      fprintf(stderr, "%s\n", sourceline_string);
      // Print wavy underline (GetUnderline is deprecated).
      int start = message->GetStartColumn(context).FromJust();
      for (int i = 0; i < start; i++)
      {
        fprintf(stderr, " ");
      }
      int end = message->GetEndColumn(context).FromJust();
      for (int i = start; i < end; i++)
      {
        fprintf(stderr, "^");
      }
      fprintf(stderr, "\n");
      v8::Local<v8::Value> stack_trace_string;
      if (try_catch->StackTrace(context).ToLocal(&stack_trace_string) &&
          stack_trace_string->IsString() &&
          v8::Local<v8::String>::Cast(stack_trace_string)->Length() > 0)
      {
        v8::String::Utf8Value stack_trace(isolate, stack_trace_string);
        const char *stack_trace_string = ToCString(stack_trace);
        fprintf(stderr, "%s\n", stack_trace_string);
      }
    }
  }

  const char *CallJS::ToCString(const v8::String::Utf8Value &value)
  {
    return *value ? *value : "<string conversion failed>";
  }

}