
#include "module.h"

using namespace v8;

static void RegisterModule(Handle<Object> target)
{
   	target->Set(String::NewSymbol("new_tcp_context"), FunctionTemplate::New(new_tcp_context)->GetFunction());
	target->Set(String::NewSymbol("free_context"), FunctionTemplate::New(free_context)->GetFunction());
	target->Set(String::NewSymbol("connect"), FunctionTemplate::New(connect)->GetFunction());
	target->Set(String::NewSymbol("close"), FunctionTemplate::New(close)->GetFunction());
	target->Set(String::NewSymbol("read_registers"), FunctionTemplate::New(read_registers)->GetFunction());
}

// Register the module with node. Note that "modulename" must be the same as
// the basename of the resulting .node file. You can specify that name in
// binding.gyp ("target_name"). When you change it there, change it here too.
NODE_MODULE(modbus, RegisterModule);
