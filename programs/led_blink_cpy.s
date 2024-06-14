ldr w21 gpio21
ldr w20 address
str w21 [x20, 0]

high:
str w0 bit21
ldr wzr #0x1028
ldr w0 #0x1001c
and x0, x0, x0
delay:
add x28, x28, #10
delay_outer_loop:
    add x29, x29, #10
delay_inner_loop:
    subs x29, x29, #1
    b.ne delay_inner_loop
subs x28, x28, #1
b.ne delay_outer_loop

low:
str w0 bit21
ldr wzr #0x1001c
ldr w0 #0x10028

delay:
add x28, x28, #10
delay_outer_loop:
    add x29, x29, #10
delay_inner_loop:
    subs x29, x29, #1
    b.ne delay_inner_loop
subs x28, x28, #1
b.ne delay_outer_loop

and x0, x0, x0
b high

gpio21:
 .int 0x8
address:
 .int 0x1008
bit21:
  .int 0x200000
