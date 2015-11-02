#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int a, int b)
{
	char sprimero[10] = "";
	char ssegundo[10] = "";
	int primero;
	int segundo;
	int c;

	printf("El siguiente programa calcula la suma de dos cadenas de caractéres ASCII convertidas a enteros\n");

	printf("Ingrese el primer valor:");
	scanf("%s", sprimero);

	printf("Ingrese el segundo valor:");
	scanf("%s", ssegundo);

	primero = stringAnumero(sprimero);
	segundo = stringAnumero(ssegundo);

	c = suma(primero, segundo);

	printf("La suma es: %d\n", c);
	system("pause");
}

int stringAnumero(char *s)
{
	int i = strlen(s);
	__asm
	{
		push ebx
		push ecx
		push esi
		push edx
		mov eax, 0	//guarda la respuesta
		mov ebx, s	//guarda los apuntadores a lo caracteres
		mov esi, 1	//contador de iteraciones para el cálculo de las potencias de 10
		mov edx, [i]	//contador de la longitud de la cadena
		to_end:		//ciclo que lleva el apuntador al final de la cadena
		cmp[ebx],0	//mientras no haya llegado al final de la cadena
			je fin_to_end
			inc ebx		//se avanza el apuntador
			jmp to_end
		fin_to_end:
		dec ebx		//Vuelve al último caracter de la cadena
		while_strlen:	//ciclo que recorre la cadena al contrario
		cmp edx, 0		//Mientras el contador no llegue a cero
		je fin_while_strlen
			mov ecx, 0		//se limpia ecx
			mov cl, [ebx]	//Se pasa el caracter a la parte baja de ecx
			sub ecx, 48		//Se sustrae '0'
			imul ecx, esi	//Se multiplica por la potencia actual
			add eax, ecx	//Se añade al número que se tiene hasta ahora
			imul esi, 10	//Se pasa la siguiente potencia
			dec ebx		//El apuntador pasa al anterior caracter
			dec edx		//Se decrece el contador
			jmp while_strlen
		fin_while_strlen:
		pop edx		//Se restablecen los registros 
		pop esi
		pop ecx
		pop ebx
	}

}

int suma(int a, int b)
{
	__asm
	{
		push ebx //Salvaguarda de registro
		mov ebx, [ebp + 8] //Se guarda el parámetro a en el registro ebx
		add ebx, [ebp + 12]  //Se suma el parámetro b al registro ebx
		mov eax, ebx //Se pasa al registro de retorno
		pop ebx //Se restablecen los registros
	}
}
