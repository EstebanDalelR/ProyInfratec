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

	 printf ("La suma es: %d\n", c);
	 system("pause");
 }

 int stringAnumero(char *s)
 {
	 int n=0;
	 int i;
	 int j = 1;

	 int len = strlen(s);
	 for (i = (len-1); i >=0; i--){
		 char m = s[i];
		 int k = m - '0';
		 n = n + k*j;
		 j=j*10;
	 }

	 return n;
 }

 int suma(int a, int b)
 {
	 return a + b;
 }
