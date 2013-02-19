
var modbus = require('./build/Release/modbus');

/*
modbus.async(function(err, result) {
    console.warn(result);
});
*/

try {
	




}
catch(e) {
	console.error(e.name, e.message);
}


var ctx = modbus.new_tcp_context("130.92.145.203", 502);

modbus.connect( ctx, function(error) {

	if( error )
	{
		console.error('connect failed', error);
	}
	else
	{
		console.error('connect succeeded');

		modbus.read_registers(ctx, 0, 2, function(error, registers) {

			if( error )
			{
				console.error('read failed', error);
			}
			else
			{
				console.error('read succeeded', registers);
			}

			modbus.close(ctx);
			modbus.free_context(ctx);
		});
	}

	
});

