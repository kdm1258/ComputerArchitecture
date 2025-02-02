add x7, x1, x0
sub x8, x7, x1
addi x8, x0, 9
ori x9, x8, 45

LOOP:
add x7, x7, x8
sub x9, x9, x8
bge x9, x8, loop

addi x9, x0, 5
addi x7, x0, 7
bge x7, x9, LAB

add x10, x1, x2
LAB:
sub x11, x3, x4
