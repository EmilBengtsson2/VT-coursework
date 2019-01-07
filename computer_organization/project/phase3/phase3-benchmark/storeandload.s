; stores and loads data
; does the equivalent of this code:
; int array[151];
; array[0] = 1;
; for (int i = 1; i <= 150; i++) {
;     array[i] = i + array[i-1];
; }

        .data
array:  .word 0
        .text
        daddi   r1,r0,0
        daddi   r2,r0,1
        daddi   r4,r0,1200
        daddi   r5,r0,1
        sd      r2,array(r1)
L1:     ld      r3,array(r1)
        daddi   r5,r5,1
        daddi   r1,r1,8
        dadd    r2,r5,r3
        sd      r2,array(r1)
        dsub    r2,r1,r4
        bnez    r2,L1
        syscall 0