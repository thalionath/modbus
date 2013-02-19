
#ifndef __NPM_MODBUS_MODBUS_H_INCLUDED__
#define __NPM_MODBUS_MODBUS_H_INCLUDED__

#include <node.h>
#include <modbus/modbus.h>

extern v8::Handle<v8::Value> new_tcp_context(const v8::Arguments& args);
extern v8::Handle<v8::Value> free_context(const v8::Arguments& args);

extern v8::Handle<v8::Value> connect(const v8::Arguments& args);
extern v8::Handle<v8::Value> close(const v8::Arguments& args);

extern v8::Handle<v8::Value> read_registers(const v8::Arguments& args);

#endif // __NPM_MODBUS_MODBUS_H_INCLUDED__
