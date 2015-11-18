#include "STDIO.H"
#include "math.h"
#include "stdlib.h" 
#include "string.h"

#define BIT_POR_BYTE   8

//---------------------------------------------------------------------------------------------------------------
// PROYECTO 2 INFRATEC 2015-2
//---------------------------------------------------------------------------------------------------------------
//- Esteban Dalel R - CÓDIGO: 201227078 - CORREO: e.dalel10@uniandes.edu.co
//- Julio A. Poveda G. - CÓDIGO: 201424179 - CORREO: ja.poveda10@uniandes.edu.co


//En esta estructura se manejaran las pistas de sonido
struct WaveData {
  unsigned int SoundLength;       //Numero de bytes ocupados por la pista
  unsigned int numSamples;        //Numero de muestreos en la pista
  unsigned int bitsPerSample;     //Numero de bits en cada muestreo
  unsigned short *Sample;         //Secuencia de muestreos
};                                //  numSamples muestreos, cada uno de bitsPerSample bits

struct HeaderType {
  int            RIFF;              //RIFF header
  char           relleno1 [18];     //No lo usamos
  unsigned short Canales;           //canales 1 = mono; 2 = estereo
  int            Frecuencia;        //frecuencia
  int            TasaBit;           //Frecuencia * canales * BitRes/8
  short          AlineamientoBloque;//Alineamento de los boques
  unsigned short BitRes;            //bit resolucion 8/16/32 bit
  int            relleno2;          //No lo usamos
  int            subChunckSize;     // numSamples * canales * BitRes/8
} Header;

void cargarWAVE( struct HeaderType *, struct WaveData *, char * );
int escribirWAVE( struct HeaderType *, struct WaveData *, char * );
void escribirMuestreo ( unsigned short * pista, int bitpos, unsigned short muestreo, int bitsPorMuestreo );
unsigned short leerMuestreo( unsigned short * pista, int bitpos, int bitsPorMuestreo );
void unirArchivosWAVE( int, unsigned short *, unsigned short *, unsigned short *, int );
int detectarBitsPorMuestreo( struct WaveData * );
void copiarMuestreo(unsigned short *fuente, int *posEntrada, unsigned short *destino, int *posSalida, int bitsPorMuestreo );
void escribir1bit( unsigned short *, int, unsigned short );
void empaquetar( struct WaveData *, int );
void desempaquetar ( struct WaveData *, int ); 
void corregirHeader( struct HeaderType * );

struct WaveData pistaEntrada1;      //Estructura para la primera pista de entrada
struct WaveData pistaEntrada2;      //Estructura para la segunda pista de entrada
struct WaveData pistaSalida;		    //Estructura para la pista de salida


int main (int argc, char* argv[])
{
	int bitsPorMuestreo;
	
	if ( argc != 4 ){
		printf( "Faltan argumentos - Deben ser 3 archivos:\n" );
		printf( "  - archivo de entrada 1 (monofonico)\n" );
        printf( "  - archivo de entrada 2 (monofonico)\n" );
        printf( "  - archivo de salida (esfonico)\n" );
		system( "pause" );
		return -1;
	}
	
	printf( "Archivo fuente 1 %s\n", argv[1] );	
	printf( "Archivo fuente  2 %s\n", argv[2] );
	printf( "Archivo Destino %s\n", argv[3] );
	system( "pause" );

	cargarWAVE( &Header, &pistaEntrada1, argv[1] );
	cargarWAVE( &Header, &pistaEntrada2, argv[2] );

	bitsPorMuestreo = detectarBitsPorMuestreo( &pistaEntrada1 );
  if ( bitsPorMuestreo != detectarBitsPorMuestreo( &pistaEntrada2 ) ){
    printf( "Los archivos tienen diferente numero de bits por muestreo\n" );
  }
	empaquetar( &pistaEntrada1, bitsPorMuestreo );
	empaquetar( &pistaEntrada2, bitsPorMuestreo );

	pistaSalida.bitsPerSample = bitsPorMuestreo;
	pistaSalida.numSamples = pistaEntrada1.numSamples;
	pistaSalida.SoundLength = 2*pistaEntrada1.SoundLength;
	pistaSalida.Sample = (unsigned short*)malloc( pistaSalida.SoundLength );

	unirArchivosWAVE( pistaEntrada1.numSamples, pistaEntrada1.Sample, pistaEntrada2.Sample, pistaSalida.Sample, bitsPorMuestreo );
	corregirHeader( &Header );
	desempaquetar( &pistaSalida, bitsPorMuestreo );
	escribirWAVE( &Header, &pistaSalida, argv[3] );

	printf ("Concluy� exitosamente.\n");
	system("pause");
	return 0;
}

/*
*  Funcion que escribe bitsPorMuestreo bits en la pista a partir de la posicion indicada por bitPos
*  pista: apunta a un vector de short que contiene los muestreos de una pista
*  bitpos: posicion a partir de la cual se desea escribir el muestreo
*  muestreo: valor del muestreo que se desea guardar en bitPos
*/
//TODO: IMPLEMENTAR EN ENSAMBLADOR USANDO NOMBRES SIMB�LICOS 
void escribirMuestreo ( unsigned short * pista, int bitpos, unsigned short muestreo, int bitsPorMuestreo )
{
	//----------------------------------------------------------------------------------
	// Implementación C
	//----------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------
	// Implementación Assembler
	//----------------------------------------------------------------------------------
	_asm
	{
		//Se hace push de los registros a usar
		push ebx
        push ecx
		push edx
		push esi
			 
		mov  esi , bitpos //esi se inicializa en bitpos porque se quiere hacer un recorrido desde ahí
		mov  edx , 0	  //se inicializa edx en 0 para llevar las cuentas del número de loop que debe hacer
			 
		inicioCiclo:
		cmp  edx , bitsPorMuestreo //compara la cantidad de ciclos con el contador
		je   finCiclo			   //salta al final al haber dado en número de ciclos
		mov  ebx , ebp+8		   //ebx=*pista
		add  ebx , bitpos		   //lleva el apuntador a la posición desde la cual se debe escribir
		mov [ebx], muestreo		   //escribe muestreo en lo apuntado por ebx
		inc  edx				   //marca un avance en el loop
		jmp  inicioCiclo		   //vuelve al principio del loop


		finEscribir:
		//se hace pop de los registros usados
        pop edx
		pop ecx
        pop ebx
		ret
	}
}

/*
*  Funcion que lee bitsPorMuestreo bits de la pista a partir de la posicion indicada por bitPos
*  pista: apunta a un vector de short que contiene los muestreos de una pista
*  bitpos: posicion a partir de la cual se desea leer el muestreo
*  Retorna el valor del muestreo que se encuentra en la posici�n bitPos
*/
//TODO: IMPLEMENTAR EN ENSAMBLADOR SIN USAR NOMBRES SIMB�LICOS
unsigned short leerMuestreo( unsigned short * pista, int bitpos, int bitsPorMuestreo )
{
	//----------------------------------------------------------------------------------
	// Implementación C
	//----------------------------------------------------------------------------------
	unsigned short mask = 1;

	int shortIntEsta = bitpos / 16;
	int posicionShortInt = bitpos % 16;
	int corrimiento = 15 - posicionShortInt;
	mask = mask << corrimiento;
	int resultado = numero & mask;

	if (resultado == 1)
	{

	}
	else
	{

	}

	//-----------------------------------------------------------------------------------
	// Implementación Assembler
	//-----------------------------------------------------------------------------------
	_asm
	{
		//se hace push de los registros a usar
		push ebx
		push ecx
		push edx

		//se hace push de los parámetros
		push bitsPorMuestreo
		push bitpos
		push pista

		//la variable de respuesta será eax
		xor  eax , eax	   //limpia eax
		pop  ebx		   //captura en ebx el valor de pista
		lea  ecx , ebx     //captura en ecx el valor de *pista (el apuntador)
		pop  ebx           //captura en ebx bitpos (el número de bits a correrse)
		add  ecx , ebx 	   //le suma al apuntador de ecx el valor de ebx para llegar a la posición a cambiar
		mov [ecx], WORD eax//mueve a la posición apuntada por ecx el valor en eax con máscara para que llegue el valor bit a bit
		pop edx			   //guarda en edx bitsPorMuestreo, el número de veces que se debe repetir la operación (hasta 32)

			leerBit:
			//este método recorre los bits y los va poniendo en eax
			cmp  edx , 0	   //compara bitsPorMuestreo con 0 para saber si ya acabó
			je   finLeer	   //va al final del método
			shr  eax , 1       //corre eax una posición a la derecha
			or   eax , [ecx]   //mueve bit a bit los valores de lo apuntado por ecx a eax con or
			dec  edx		   //reduce en 1 edx para marcar un recorrido
			jmp  leerBit       //vuelve al inicio del procedimiento
		
		finleer:
		//se hace pop de los registros usados
		pop edx
		pop ecx
		pop ebx
		ret     //retorna al método que llamo este procedimiento
	}
} 

/*
*  Funcion para fundir dos pistas monofonicas en una sola estereo
*  numMuestreos: n�mero total de muestreos de cada canal
*  parte1: apunta a un vector de short que contiene los muestreos de una pista
*  parte2: apunta a un vector de short que contiene los muestreos de una pista
*  salida: apunta a un vector de short que contendra la fusion de las dos pistas anteriores
*  bitsPorMuestreo: tamanio en bits de los muestreos
*/
//TODO: IMPLEMENTAR EN ENSAMBLADOR USANDO NOMBRES SIMB�LICOS
void unirArchivosWAVE(int numMuestreos, unsigned short *parte1, unsigned short *parte2, unsigned short *salida, int bitsPorMuestreo )
{
	//----------------------------------------------------------------------------------
	// Implementación C
	//----------------------------------------------------------------------------------

	//Se va a hacer un recorrido para leer los muestreos de cada pista, y luego se escriben intercalados en salida
	for (int i = 0; i < numMuestreos; i++)
	{
		int posicion = i*bitsPorMuestreo;
		unsigned short numero1 = leerMuestreo(*parte1, posicion, bitsPorMuestreo);
		unsigned short numero2 = leerMuestreo(*parte2, posicion, bitsPorMuestreo);

		int posicion2 = i * 2 * bitsPorMuestreo;

		escribirMuestreo(*salida, posicion2, numero1, bitsPorMuestreo);
		escribirMuestreo(*salida, posicion2+bitsPorMuestreo, numero2, bitsPorMuestreo);
	}

	//-----------------------------------------------------------------------------------
	// Implementación Assembler
	//-----------------------------------------------------------------------------------
	_asm
	{
		//Se guarda el espacio para las variables posicion, numero1, numero2 y posicion2
		sub  esp, 16 
		//Se guardan en la pila los registros que serán usados
		push ebx
		push ecx
		push edx
		push esi
        mov  esi, 0
		mov  eax, 0 //Guarda la respuesta

			inicioCiclo:

			cmp esi, numMuestreos   //¿esi es menor a numMuestreos?
			jge finCiclo			//Si esi es mayor o igual a numMuestreos, se sale del while

			//esi es menor a numMuestreos
			mov  ebx      , [ebp + 12]           //ebx=parte1
			mov  bx	 	  , [ebx]                //bx=*parte1
			mov  ecx 	  , [ebp + 16]           //ecx=parte2
			mov  cx	 	  , [ecx]                //cx=*parte2
			mov  edx	  , [ebp + 20]           //edx=salida
			mov  dx		  , [edx]                //dx=*salida  
			mov  [ebp - 4], esi                  //La variable posicion debe ser igual a i*bitsPorMuestreo
			imul [ebp - 4], bitsPorMuestreo

			//Se guardan los parámetros
			push bitsPorMuestreo
			push [ebp - 4]
			push bx
			call leerMuestreo
			add  esp, 12                    //Se sacan los parámetros
			mov  [ebp - 8], eax             //El resultado se asigna a numero1

						                    //Se guardan los parámetros
			push bitsPorMuestreo
			push [ebp - 4]
			push cx
			call leerMuestreo
			add  esp, 12                    //Se sacan los parámetros
			mov  [ebp - 12], eax            //El resultado se asigna a numero2

			mov  [ebp - 16], esi            //La variable posicion2 es igual a i*2*bitsPorMuestreo
			imul [ebp - 16], 2
			imul [ebp - 16], bitsPorMuestreo

			//Se guardan los parámetros
			push bitsPorMuestreo
			push [ebp - 8]                  //Se guarda numero1
			push [ebp - 16]                 //Se guarda posicion2
			push dx                         //Se guarda *salida
			call escribirMuestreo
			add  esp, 16                    //Se sacan los parámetros 

			//Se guardan los parámetros
			push bitsPorMuestreo
			push [ebp - 12]                  //Se guarda numero2
			add  [ebp - 16], bitsPorMuestreo //posicion2+bitsPorMuestreo
			push [ebp - 16]                  //Se guarda posicion2
			push dx                          //Se guarda *salida
			call escribirMuestreo
			add  esp, 16                     //Se sacan los parámetros

			inc  esi
			jmp  inicioCiclo

		finCiclo:
		//Se sacan de la pila los registros utilizados
		add esp, 16
		pop esi
		pop edx
		pop ecx
		pop ebx
		ret
	}
}

/*
* Funci�n que detecta el numero de bits por muestreo
* NO MODIFICAR
*/
int detectarBitsPorMuestreo( struct WaveData * voice ){
	int posiciones = 0;
	unsigned short sample = voice->Sample[0];

	while ( sample ){
		posiciones++;
		sample <<= 1;
	}

	return posiciones;
}

/*
*  Funcion que copia un muestreo (bitsPorMuestreo) a partir del bit posEntrada de fuente
*  a los bits a partir de la posicion posSalida de destino
*  fuente: apunta a un vector de short que contiene los muestreos de una pista
*  destino: apunta a un vector de short que contiene los muestreos de una pista
*  posEntrada: posicion de fuente (en bits) desde donde se copiara el muestreo
*  posSalida: posicion de destino (en bits) a donde se copiara el muestreo
*  bitsPorMuestreo: tamanio en bits de los muestreos
*/
void copiarMuestreo(unsigned short *fuente, int *posEntrada, unsigned short *destino, int *posSalida, int bitsPorMuestreo )
{	
	unsigned short muestreo = leerMuestreo( fuente, *posEntrada, bitsPorMuestreo );
	escribirMuestreo ( destino, *posSalida, muestreo, bitsPorMuestreo );
	*posEntrada += bitsPorMuestreo;
	*posSalida += bitsPorMuestreo;
}

/*
* Funcion para empaquetar los muestreos de una pista
* NO MODIFICAR
*/
void empaquetar ( struct WaveData *pista, int bitsPorMuestreo ){
	int i;
	int posEntrada = 0;
	int posSalida = 0;
    unsigned short * nuevoSample;

	nuevoSample = (unsigned short *)malloc( pista->SoundLength );

	for (i = 0; i < pista->numSamples; i++){
		copiarMuestreo( pista->Sample, &posEntrada, nuevoSample, &posSalida, bitsPorMuestreo );
		posEntrada += ( 2*BIT_POR_BYTE - bitsPorMuestreo ); 
	}
	pista->bitsPerSample = bitsPorMuestreo;
	free(pista->Sample);
	pista->Sample = nuevoSample;
}

/*
*  Funcion que escribe 1 bit en la pista en la posicion indicada por bitPos
*  pista: apunta a un vector de short que contiene los muestreos de una pista
*  bitpos: posicion del bit de la pista que se desea modificar
*  bit: vale 1 o 0, indicando cu�l es el valor que se desea asignar al bit
*/
void escribir1bit(unsigned short * pista, int bitpos, unsigned short bit) {

	escribirMuestreo(pista, bitpos, bit, 1);
	//----------------------------------------------------------------------------------
	// Implementación Assembler
	//----------------------------------------------------------------------------------
	_asm {
		//Se guardan en la pila los registros que serán usados
		push  eax
		push  ebx
		push  ecx

		mov  ah  , bit	   //captura en ah el valor de bit
		mov  ebx , bitpos  //captura en ebx el valor de bitpos
		mov  ecx , pista   //captura en ecx el valor de pista (el apuntador)
		add  ecx , ebx 	   //le suma al apuntador de ecx el valor de ebx para llegar a la posición a cambiar
		mov [ecx], ah	   //mueve a la posición apuntada por ecx el valor en ah

		//se hace pop de los registros usados
		pop  ecx
		pop  ebx
		pop  eax
	}
}

/*
* Funcion para desempaquetar los muestreos de una pista
* NO MODIFICAR
*/
void desempaquetar ( struct WaveData * pista, int bitsPorMuestreo ){
	int i;
	int j;
	int posEntrada = 0;
	int posSalida = 0;
  unsigned short * nuevoSample;

	nuevoSample = (unsigned short*)malloc( pista->SoundLength );
	for ( i = 0; i < 2*pista->numSamples; i++ ){
		copiarMuestreo( pista->Sample, &posEntrada, nuevoSample, &posSalida, bitsPorMuestreo );
		for ( j = 0; j < 16 - bitsPorMuestreo; j++ ){
			escribir1bit( nuevoSample, posSalida, 0 );
			posSalida++;
		}
	}
	
	pista->bitsPerSample = 16;
	free( pista->Sample );
	pista->Sample = nuevoSample;

}

/*
*  Carga el archivo WAVE en memoria.
*  NO MODIFICAR.
*/
void cargarWAVE ( struct HeaderType * header, struct WaveData * pista, char * FileName ){
  FILE * WAVFile;

  WAVFile = fopen( FileName, "rb" );
  if ( WAVFile == NULL ){
    printf( "No se puede abrir el archivo (%s)\n", FileName );
    exit( 0 );
  }

  //Cargar el encabezado
  fread( header, 44, 1, WAVFile );

  pista->SoundLength = header->subChunckSize;
  pista->Sample = (unsigned short *)malloc( pista->SoundLength ); //Asignar memoria
  if ( pista->Sample == NULL ){
    printf( "No hay memoria para cargar el archivo (%s)\n", FileName );
    exit( 0 );
  }

  //Check RIFF header
  if ( header->RIFF != 0x46464952 ){
    printf( "El archivo no es de tipo wav (%s)\n", FileName );
    exit( 0 );
  }

  //Check canales
  if ( header->Canales != 1 ){
    printf( "El archivo no es monofonico (%s)\n", FileName );
    exit( 0 );
  }

  //Check resoluci�n bits
  if ( header->BitRes != 16 ){
    printf( "El archivo no es de 16 bits (%s)\n", FileName );
    exit( 0 );
  }

  //Carga los muestreos
  fread( pista->Sample, pista->SoundLength, 1, WAVFile );
  
  fclose( WAVFile ); 

  pista->numSamples = header->subChunckSize / 2;

}

/*
* Funcion que escribe un archivo WAVE en su totalidad
* NO MODIFICAR
*/
int escribirWAVE ( struct HeaderType * header, struct WaveData * pista, char * FileName ){
  FILE * WAVFile;

  WAVFile = fopen( FileName, "wb" );

  if ( WAVFile == NULL ){
    printf( "No se puede crear el archivo (%s)\n", FileName );
    return (0);
  }

  fwrite( header,44,1, WAVFile );
  fwrite( pista->Sample, header->subChunckSize, 1, WAVFile );
  return 1;
}

/*
* Funci�n que corrige el header: cambio de monof�nico a estereof�nico
* NO MODIFICAR
*/
void corregirHeader ( struct HeaderType * header ){
	header->Canales = 2;
	header->TasaBit = header->Frecuencia * header->BitRes / 8 * header->Canales;
	header->AlineamientoBloque = header->BitRes / 8 * header->Canales;
	header->subChunckSize = 2*header->subChunckSize;
}
