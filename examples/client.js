
var modbus = require('modbus');

var device = modbus.create();

console.log('connecting...');

device.on('connect', function() {
	console.log('event: connect');
}).on('disconnect', function() {
	console.log('event: disconnect');
}).on('read', function(registers) {
	console.log('event: read', registers);
}).on('error', function(e) {
	console.log('event: error', e);
}).connectTcp({ip: '10.0.0.1', port: 502}, function(error) {

	if( error )
	{
		console.error(error);
	}
	else
	{
		console.log('connected');

		device.readRegisters(0, 2).readRegisters(0, 2, function(error, registers) {
			
			if( error ) {
				console.error('read failed', error);
			}
			else {
				console.error('read succeeded', registers);
			}

			device.disconnect();
		});

	}

});


