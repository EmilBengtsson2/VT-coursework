; stores some data, small code and few stores should make
; this program fit in quite small caches and result in few cache misses
; does the equivalent of this code:
; int array[11];
; for (int i = 0; i <= 10; i++) {
;     array[i] = i
; }

        .data
array:  .word 0
        .text
        daddi   r1,r0,0
        daddi   r4,r0,80
        daddi   r5,r0,0
L1:     daddi   r1,r1,8
        sd      r5,array(r1)
        daddi   r5,r5,1
        dsub    r2,r1,r4
        bnez    r2,L1
        syscall 0