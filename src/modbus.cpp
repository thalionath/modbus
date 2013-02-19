
#include "module.h"

using namespace v8;

Handle<Value> new_tcp_context(const Arguments& args)
{
	if( args.Length() < 2 )
	{
		return ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
	}

	if( !args[0]->IsString() || !args[1]->IsNumber() )
	{
    		return ThrowException(Exception::TypeError(String::New("Wrong arguments")));
	}

	modbus_t* const context = modbus_new_tcp(*String::Utf8Value(args[0]), args[1]->Int32Value());

	if( nullptr == context )
	{
		return ThrowException(Exception::Error(String::New("Unable to allocate libmodbus context")));
	}

	return External::New(context);
}

Handle<Value> free_context(const Arguments& args)
{
	if( args.Length() < 1 )
	{
		return ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
	}

	modbus_t* const context = static_cast<modbus_t*>(External::Cast(*args[0])->Value());

	if( !context )
	{
		return ThrowException(Exception::Error(String::New("Unable to cast argument to libmodbus context")));		
	}

	modbus_free(context);

	return Undefined();
}

Handle<Value> close(const Arguments& args)
{
	if( args.Length() < 1 )
	{
		return ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
	}

	if( ! args[0]->IsExternal() )
	{
		return ThrowException(Exception::TypeError(String::New("Wrong arguments")));
	}

	modbus_t* const context = static_cast<modbus_t*>(External::Cast(*args[0])->Value());

	if( ! context )
	{
		return ThrowException(Exception::Error(String::New("Unable to cast argument to libmodbus context")));		
	}

	modbus_close(context);

	return Undefined();
}
