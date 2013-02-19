{
 'targets': [
  {
   'target_name': 'modbus',
   'sources': [ 'src/module.cpp', 'src/modbus.cpp', 'src/connect.cpp', 'src/read_registers.cpp' ],
   'link_settings': {
    'libraries': [
     '-lmodbus'
    ]
   },
   'conditions': [
    ['OS=="linux"', {
      'cflags': [
       '-Wall',
       '-std=c++0x'
      ]
     }
    ],
   ],
  }
 ]
}
