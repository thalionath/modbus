
var libmodbus = require('../build/Release/modbus');
var EventEmitter = require('events').EventEmitter;

var modbus = function() {
	var that = new EventEmitter();
	var ctx;

	that.connectTcp = function(options, callback) {

		options.port = options.port || 502;
	
		ctx = libmodbus.new_tcp_context(options.ip, options.port);
		
		libmodbus.connect(ctx, function(error) {

			if( callback ) {
				callback(error)
			}

			if( error ) {
				that.emit('error', error);
			}
			else {
				that.emit('connect');
			}

		});

		return that;
	};

	that.disconnect = function() {
		try {
			libmodbus.close(ctx);
			libmodbus.free_context(ctx);

			that.emit('disconnect');
		}
		catch(e) {
			that.emit('error', e);
		}

		return that;		
	};

	that.readRegisters = function(address, count, callback) {

		libmodbus.read_registers(ctx, address, count, function(error, registers) {

			if( callback ) {
				callback(error, registers);
			}
	
			if( error ) {
				that.emit('error', error);
			}
			else {				
				that.emit('read', registers);
			}
		});

		return that;
	};

	return that;
};

module.exports = {
	create: modbus,
	native: libmodbus
}
