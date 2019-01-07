; This program has a lot of instuctions, and also a lot of jumps.
; This should result in more cache misses I think.

		.data
array:	.word 0
		.text
		daddi	r1,r0,1
		daddi	r2,r0,5
		daddi	r4,r0,0
		daddi	r5,r0,0
		daddi	r6,r0,0
		daddi	r7,r0,0
l1:		dsub	r2,r2,r1
		daddi	r3,r3,1
		daddi	r3,r3,1
		j		l2
lr4:	daddi	r4,r0,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
l2:		daddi	r3,r3,1
		daddi	r3,r3,1
		j		l3
lr5:	daddi	r5,r0,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
l3:		j		l4
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
l4:		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		j		l5
lr6:	daddi	r6,r0,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
l5:		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		j		l6
lr7:	daddi	r7,r0,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
l6:		daddi	r3,r3,1
		daddi	r3,r3,1
		j		l7
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
l7:		j		l8
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
l8:		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		j		l9
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
l9:		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		j		l10
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
l10:	daddi	r3,r3,1
		daddi	r3,r3,1
		j		l11
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
l11:	j		l12
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
l12:	daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		j		l13
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
l13:	daddi	r3,r3,1
		j		l14
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
l14:	daddi	r3,r3,1
		daddi	r3,r3,1
		j		l15
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
l15:	j		l16
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
l16:	daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		j		l17
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
l17:	daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		j		l18
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
		daddi	r3,r3,1
l18:	daddi	r3,r3,1
		beqz	r4,lr4
		beqz	r5,lr5
		beqz	r6,lr6
		beqz	r7,lr7
		bnez	r2,l1
		syscall 0