
#ifndef __NPM_MODBUS_ASYNC_MODBUS_TASK_H_INCLUDED__
#define __NPM_MODBUS_ASYNC_MODBUS_TASK_H_INCLUDED__

#include "module.h"
#include <string>
#include <sstream>

class AsyncModbusTask
{
protected:

	modbus_t* const context_;

	// This handle holds the callback function we'll call after the work request
	// has been completed in a threadpool thread. It's persistent so that V8
	// doesn't garbage collect it away while our request waits to be processed.
	// This means that we'll have to dispose of it later ourselves.
	v8::Persistent<v8::Function> callback_;

	// Tracking errors that happened in the worker function. You can use any
	// variables you want. E.g. in some cases, it might be useful to report
	// an error number.
	bool error_flag_;

	std::string error_message_;

public:

	AsyncModbusTask(modbus_t* modbus_context, const v8::Persistent<v8::Function>& callback) 
	: context_(modbus_context), callback_(callback), error_flag_(false)
	{}

	virtual ~AsyncModbusTask()
	{
		// The callback is a permanent handle, so we have to dispose of it manually.
		callback_.Dispose();
	}

	static void work_callback(uv_work_t* work_request)
	{
		AsyncModbusTask* const task = static_cast<AsyncModbusTask*>(work_request->data);

		assert(task);

		if(task)
		{
			task->work();
		}
	}

	virtual void work() = 0;

	static void after_callback(uv_work_t* work_request)
	{
		AsyncModbusTask* const task = static_cast<AsyncModbusTask*>(work_request->data);

		assert(task);

		if(task)
		{
			task->after();
		}
	}

	virtual void after() = 0;

	//! Schedules the task for execution.
	bool queue()
	{
		uv_work_t* const work_request = new uv_work_t();
		{
			work_request->data = this;
		}

		const int status = uv_queue_work(uv_default_loop(), work_request, work_callback, (uv_after_work_cb)after_callback);

		if( -1 == status )
		{
			delete work_request;
			return false;	
		}

		return true;
	}

	void setModbusError(int error)
	{
		error_flag_ = true;

		std::stringstream stream;
		{
			stream << modbus_strerror(error) << " (" << error << ")";
		}
		error_message_ = stream.str();
	}
};

#endif // __NPM_MODBUS_ASYNC_MODBUS_TASK_H_INCLUDED__
