
#include "async_modbus_task.h"

using namespace v8;

class ConnectTask : public AsyncModbusTask
{
public:

	ConnectTask(modbus_t* modbus_context, const Persistent<Function>& callback)
	: AsyncModbusTask(modbus_context, callback)
	{}

	virtual ~ConnectTask()
	{
	}

	virtual void work()
	{
		if( -1 == modbus_connect(context_) )
		{
			setModbusError(errno);
		}
	}

	virtual void after()
	{
		HandleScope scope;

		const unsigned argc = 1;

		Local<Value> error = error_flag_ ? Exception::Error(String::New(error_message_.c_str())) : Local<Value>::New(Null());

		Local<Value> argv[argc] = {error};

		TryCatch try_catch;
		{
			callback_->Call(Context::GetCurrent()->Global(), argc, argv);

			if( try_catch.HasCaught() )
			{
				node::FatalException(try_catch);
			}
		}
	}
};

Handle<Value> connect(const Arguments& args)
{
	HandleScope scope;

	if( args.Length() < 2 )
	{
		return ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
	}

	if( !args[0]->IsExternal() || !args[1]->IsFunction() )
	{
		return ThrowException(Exception::TypeError(String::New("Wrong arguments")));
	}

	modbus_t* const context = static_cast<modbus_t*>(External::Cast(*args[0])->Value());

	if( !context )
	{
		return ThrowException(Exception::Error(String::New("Unable to cast argument to libmodbus context")));		
	}

	Local<Function> callback = Local<Function>::Cast(args[1]);

	AsyncModbusTask* const task = new ConnectTask(context, Persistent<Function>::New(callback));

	if( ! task->queue() )
	{
		delete task;	

		// TODO: really throw exeception ?
		return ThrowException(Exception::Error(String::New("Unable to queue work request")));
	}

	return Undefined();
}