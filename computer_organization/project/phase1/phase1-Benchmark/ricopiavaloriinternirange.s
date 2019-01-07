;Dato un vettore di numeri interi {3,40,5,35,6,7,46,35,9,10}, 
;copiare tutti i numeri che si trovano all'interno dell'intervallo [6;30] 
;in un secondo vettore, di cui poi bisogna calcolare la somma.
;le cache separate hanno diversa associativit�
		.data
vett1:	.word 3,40,5,35,6,7,46,35,9,10         
vett2:	.word 0,0,0,0,0,0,0,0,0,0
		.text
		daddi r1,r0,vett1
		daddi r2,r0,vett2
		dsub r3,r2,r1			;r3=sizeof(vett1)
		daddi r1,r0,0			;r1=i=0
		daddi r2,r0,0			;r2=i2=0
		daddi r4,r0,0			;r4=somma=0
while_scanvett1:
		slt r5,r1,r3				;while(i<size)
		beq r5,r0,endwhile_scanvett1
		ld r6,vett1(r1)			;r6=(vett1[i])
		daddi r7,r0,6
		slt r8,r7,r6				;� r8=(vett1[i]>6)
		bne r8,r0,secondcontrol
		j incr
secondcontrol:
		daddi r7,r0,30
		slt r8,r6,r7				;if(!(vett1[i]<30)) goto incr
		beq r8,r0,incr
		sd r6,vett2(r2)			;vett2[i2]=vett1[i]
		dadd r4,r4,r6			;somma+=vett2[i2]
		daddi r2,r2,8
incr:		daddi r1,r1,8
		j while_scanvett1
endwhile_scanvett1: syscall 0			;somma in r4 
		

; public class Compito
; {
	; public static void main(String[] args)
	; {
		; int vett1[]={3,40,5,35,6,7,46,35,9,10};
		; int vett2[]={0,0,0,0,0,0,0,0,0,0};
		; int size=vett1.length;
		; int i=0;
		; int i2=0;
		; int somma=0;
		; while(i<size)
		; {
			; if((vett1[i]>6) && (vett1[i]<30))
			; {
				
				; vett2[i2]=vett1[i];
				; somma+=vett2[i2];
				; i2++;
			; }
			; i++;
		; }
		; System.out.println(somma); //26    7,9,10
	; }
; }