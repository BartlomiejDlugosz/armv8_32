ldr w21, gpio21
ldr w20, address
str w21, [x20]
and x0, x0, x0

gpio21:
 .int 0x8
 .int 0
address:
 .int 0x1008
 .int 0
