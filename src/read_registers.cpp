
#include "async_modbus_task.h"

using namespace v8;

class ReadRegistersTask : public AsyncModbusTask
{
private:

	const int address_;

	const int register_count_;

	//! The result of reading.
	uint16_t* const results_;

	int registers_read_;

public:

	ReadRegistersTask(modbus_t* context, int address, int register_count, const Persistent<Function>& callback)
	: AsyncModbusTask(context, callback),
	  address_(address), 
	  register_count_(register_count),
	  results_(new uint16_t[register_count])
	{
	}

	virtual ~ReadRegistersTask()
	{
		delete [] results_;
	}

	virtual void work()
	{
		const int r = modbus_read_registers(context_, address_, register_count_, results_);

		if( -1 == r )
		{
			setModbusError(errno);
		}
		else
		{
			registers_read_ = r;
		}
	}

	virtual void after()
	{
		HandleScope scope;

		Local<Value> error = error_flag_ ? Exception::Error(String::New(error_message_.c_str())) : Local<Value>::New(Null());

		v8::Local<v8::Array> register_data = v8::Array::New(registers_read_);
		
		for( int i=0; i<registers_read_; ++i )
		{
			register_data->Set(i, Integer::New(results_[i]));
		}

		const unsigned argc = 2;

		Local<Value> argv[argc] = {error, register_data};

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

//! Shedules int modbus_read_registers(modbus_t *ctx, int addr, int nb, uint16_t *dest)
Handle<Value> read_registers(const Arguments& args)
{
	HandleScope scope;

	if( args.Length() < 4 )
	{
		return ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
	}

	if( !args[0]->IsExternal() || !args[1]->IsNumber() || !args[2]->IsNumber() || !args[3]->IsFunction() )
	{
		return ThrowException(Exception::TypeError(String::New("Wrong arguments")));
	}

	modbus_t* const context = static_cast<modbus_t*>(External::Cast(*args[0])->Value());

	if( !context )
	{
		return ThrowException(Exception::Error(String::New("Unable to cast argument to libmodbus context")));		
	}

	Local<Function> callback = Local<Function>::Cast(args[3]);

	AsyncModbusTask* const task = new ReadRegistersTask(
					context,				// ctx
					args[1]->Int32Value(),	// addr
					args[2]->Int32Value(),	// nb
					Persistent<Function>::New(callback)
					);

	if( ! task->queue() )
	{
		delete task;

		// TODO: really throw exeception ?
		return ThrowException(Exception::Error(String::New("Unable to queue work request")));		
	}

	return Undefined();
}
