ldr w21 gpio21
ldr w20 #0x3f200008
str w21 [x20] 

high:
ldr w20 bit21
str w0 [x20]
ldr wzr #0x3f200028
ldr w0 #0x3f20001c

delay:
add x28, x28, #1000
delay_outer_loop:
    add x29, x29, #1000000
delay_inner_loop:
    subs x29, x29, #1
    b.ne delay_inner_loop
subs x28, x28, #1
b.ne delay_outer_loop

low:
ldr w20 bit21
str w0 [x20]
ldr wzr #0x3f20001c
ldr w0 #0x3f200028

delay:
add x28, x28, #1000
delay_outer_loop:
    add x29, x29, #1000000
delay_inner_loop:
    subs x29, x29, #1
    b.ne delay_inner_loop
subs x28, x28, #1
b.ne delay_outer_loop

and x0, x0, x0
b high

gpio21:
  .int 0x8
bit21:
  .int 0x200000
