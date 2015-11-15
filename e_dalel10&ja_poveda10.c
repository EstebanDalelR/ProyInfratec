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
void unirArchivosWAVE( unsigned short *, unsigned short *, unsigned short *, int );
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

	unirArchivosWAVE( pistaEntrada1.Sample, pistaEntrada2.Sample, pistaSalida.Sample, bitsPorMuestreo );
	corregirHeader( &Header );
	desempaquetar( &pistaSalida, bitsPorMuestreo );
	escribirWAVE( &Header, &pistaSalida, argv[3] );

	printf ("Concluyó exitosamente.\n");
	system("pause");
	return 0;
}

/*
*  Funcion que escribe bitsPorMuestreo bits en la pista a partir de la posicion indicada por bitPos
*  pista: apunta a un vector de short que contiene los muestreos de una pista
*  bitpos: posicion a partir de la cual se desea escribir el muestreo
*  muestreo: valor del muestreo que se desea guardar en bitPos
*/
//TODO: DESARROLLAR COMPLETAMENTE ESTA FUNCION
void escribirMuestreo ( unsigned short * pista, int bitpos, unsigned short muestreo, int bitsPorMuestreo )
{
	//se declaran las variables a usar
	//lleva la posicíón en el for y la pista a modificar
	int i;
	//lleva la posición en el rchivo de entrada
	int j = 0;
	//toma unos bits en muestreo, deben ser bitsPorMuestreo bits
	//va hasta bitpos de pista, agrega esos bits
	
	//se itera sobre los bits que le siguen a bitpos bitsPorMuestreo veces
	for ( i = bitpos; i < bitsPorMuestreo; i++)
	{	
		//se reeemplaza el bit actual por el bit que llega en muestreo
		* pista[i] = muestreo[j];
		//se avanza a la siguente posición de muestreo
		j++;
		//repite el proceso
	}	
}

/*
*  Funcion que lee bitsPorMuestreo bits de la pista a partir de la posicion indicada por bitPos
*  pista: apunta a un vector de short que contiene los muestreos de una pista
*  bitpos: posicion a partir de la cual se desea leer el muestreo
*  Retorna el valor del muestreo que se encuentra en la posición bitPos
*/
//TODO: DESARROLLAR COMPLETAMENTE ESTA FUNCION
unsigned short leerMuestreo( unsigned short * pista, int bitpos, int bitsPorMuestreo )
{
	//se declaran las variables a usar
	unsigned short respuesta[15] = 0;
	//IMPORTANTE 
	// PISTA APUNTA AL VECTOR
	//LOS CAMBIOS SE DEBEN HACER EN EL VECTOR EN MEMORIA

	int numTotalBits = numSamples*bitsPorMuestreo;
	int m=0;
	int i=0;
	int n=0;
>>>>>>> 4a285f9894210af2260e127fa2e7d9b4540f05ad
	
	//Recorremos el numero total de bits 
	for (int i = 0; i < numTotalBits; i++)
	{
	    //¿Llegó a la posicion desde donde se quiere empezar a leer el muestreo?
		if (i == bitpos)
		{
			//Este for nos permite manejar la posicion de los bits desde bitpos hasta completar un muestreo
			for (m = bitpos; m < bitsPorMuestreo; m++)
			{
				respuesta[i] = pista[m];
			}
			//j no es una variable válida
			//la cambié por i
			
		}
	}

	return respuesta;
} 

/*
*  Funcion para fundir dos pistas monofonicas en una sola estereo
*  parte1: apunta a un vector de short que contiene los muestreos de una pista
*  parte2: apunta a un vector de short que contiene los muestreos de una pista
*  salida: apunta a un vector de short que contendra la fusion de las dos pistas anteriores
*  bitsPorMuestreo: tamanio en bits de los muestreos
*/
//TODO: DESARROLLAR COMPLETAMENTE ESTA FUNCION
void unirArchivosWAVE( unsigned short *parte1, unsigned short *parte2, unsigned short *salida, int bitsPorMuestreo )
{
	//se declaran las variables a usar
	int i;
	//lleva la posición en el vector de salida
	int m = 0;
	//leva la bitpos en cada parte
	int n = 0;

	//el vector de salida debe tener el largo de entrada1+entrada2
	//se van agregando bitsPorMuestreo bits de cada archivo de entrada hasta llegar al fin

	for (i = 0; i < sizeof(*salida); i++)
	{
		//escribe los bits de la parte1
		escribirMuestreo(*salida, m, leerMuestreo(parte1, n, bitsPorMuestreo), bitsPorMuestreo);
		//avanza m hasta la siguente posición disponible
		m += bitsPorMuestreo;
		//escribe los bits de la parte2
		escribirMuestreo(*salida, m, leerMuestreo(parte2, n, bitsPorMuestreo), bitsPorMuestreo);
		//avanza m hasta la siguente posición disponible
		m += bitsPorMuestreo;
		//avanza n
		n += 2 * bitsPorMuestreo;
	}
}

/*
* Función que detecta el numero de bits por muestreo
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
*  bit: vale 1 o 0, indicando cuál es el valor que se desea asignar al bit
*/
void escribir1bit( unsigned short * pista, int bitpos, unsigned short bit ){
	escribirMuestreo ( pista, bitpos, bit, 1 );
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

  //Check resolución bits
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
* Función que corrige el header: cambio de monofónico a estereofónico
* NO MODIFICAR
*/
void corregirHeader ( struct HeaderType * header ){
	header->Canales = 2;
	header->TasaBit = header->Frecuencia * header->BitRes / 8 * header->Canales;
	header->AlineamientoBloque = header->BitRes / 8 * header->Canales;
	header->subChunckSize = 2*header->subChunckSize;
}

//A partir de este punto se escribe en lenguaje ensamblador

_asm{
leerDatos
//se guarda es stream en pila
//los datos se guardan en registro
//se leeen num sample veces
//se escriben bitsPerSample bits
//estos datos van en un registro y 
//se van restando en cada loop
mov   numSamples    , eax   ;         //Numero de muestreos en la pista queda en eax
mov   bitsPerSample , ebx   ;         //Numero de bit por cada muestreo
//en cada loop de lectura se reduce 
//eax en 1
//por cada bit leído se reduce ebx

escribirDatos
//se escribe el stream desde la pila
//sucede numSamples veces
mov   numSamples    , eax  ;         //Numero de muestreos en la pista queda en eax
mov   bitsPerSample , ebx  ;         //Numero de bit por cada muestreo
}
