 /**
 * Implementación en C de la práctica, para que tengáis una
 * versión funcional en alto nivel de todas les funciones que tenéis 
 * que implementar en ensamblador.
 * Desde este código se hacen las llamadas a les subrutinas de ensamblador. 
 * ESTA CÓDIGO NO SE PUEDE MODIFICAR Y HAY QUE ENTREGARLO
 * */

#include <stdlib.h>
#include <stdio.h>
#include <termios.h>     //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>      //STDIN_FILENO

/**
 * Constantes
 */
#define DimMatrix  4     //dimensión de la matriz
#define SizeMatrix DimMatrix*DimMatrix //=16

extern int developer;   //Variable declarada en ensamblador que indica el nombre del programador

/**
 * Definición de variables globales
 */
// Matriz 9x9 donde guardaremos los números del joc
// Accés a les matrius en C: utilitzem fila (0..[DimMatrix-1]) i 
// columna(0..[DimMatrix-1]) (m[fila][columna]).
// Accés a les matrius en assemblador: S'hi accedeix com si fos un vector 
// on indexMat (0..[DimMatrix*DimMatrix-1]). 
// indexMat=((fila*DimMatrix)+(columna))*2 (2 perquè la matriu és de tipus short).
// WORD[M+indexMat] (WORD perquè és de tipus short) 
// (indexMat ha de ser un registre de tipus long/QWORD:RAX,RBX,..,RSI,RDI,..,R15).
short m[DimMatrix][DimMatrix]        = { {    8,    8,    32,    32},
                                         {    4,   32,   128,    64},
                                         {    0,    0,   256,   128},
                                         {    0,    4,   512,  1024} };

short mRotated[DimMatrix][DimMatrix] = { {    2,    0,     2,     0},
                                         {    2,    2,     4,     4},
                                         {    4,    4,     0,     4},
                                         {    4,    2,     2,     4} };

short mAux[DimMatrix][DimMatrix]     = { {    0,    0,     0,     0},
                                         {    0,    0,     0,     0},
                                         {    0,    0,     0,     0},
                                         {    0,    0,     0,     0} };
                                    
short mUndo[DimMatrix][DimMatrix]    = { {    0,    1,     3,     7},
                                         {   15,   31,    63,   127},
                                         {  255,  511,  1023,  2047},
                                         { 4095, 8191, 16383, 32767},};

char state = '1';  // 0: Salir, hemos pulsado la tecla 'ESC' para salir.
                   // 1: Continuamos jugando.
                   // 2: Continuamos jugando pero han habido cambios en la matriz.
			       // 3: Deshacer el último movimento.
				   // 4: Ganado, se ha llegado a la casilla 2048.
                   // 5: Perdido, no se pueden hacer movimientos.

/**
 * Definición de las funciónes de C
 */
void clearscreen_C();
void gotoxyP2_C(int, int);
void printchP2_C(char);
char getchP2_C();

char printMenuP2_C();
void printBoardP2_C();

void  showNumberP2_C(int, int, int);
void  updateBoardP2_C(int);
void  rotateMatrixRP2_C(short [DimMatrix][DimMatrix]);
void  copyMatrixP2_C(short [DimMatrix][DimMatrix], short [DimMatrix][DimMatrix]);
int   shiftNumbersLP2_C(short [DimMatrix][DimMatrix]);
int   addPairsLP2_C(short [DimMatrix][DimMatrix]);

int   readKeyP2_C(int);
void  insertTileP2_C();
void  checkEndP2_C(void);
void  printMessageP2_C(void);
void  playP2_C();


/**
 * Definición de las subrutinas de ensamblador que es llaman desde C.
 */
extern void  showNumberP2(int, int, int);
extern void  updateBoardP2(int);
extern void  rotateMatrixRP2(short [DimMatrix][DimMatrix]);
extern void  copyMatrixP2(short [DimMatrix][DimMatrix], short [DimMatrix][DimMatrix]);
extern int   shiftNumbersRP2(short [DimMatrix][DimMatrix]);
extern int   addPairsRP2(short [DimMatrix][DimMatrix]);
extern int   readKeyP2(int);
extern void  checkEndP2();
extern void  playP2();


/**
 * Borrar la pantalla
 * 
 * Variables globales utilizadas:   
 * Ninguna
 * 
 * Parámetros de entrada: 
 * Ninguno
 *   
 * Parámetros de salida : 
 * Ninguno
 * 
 * Esta función no es llama desde ensamblador
 * y no hay definida una subrutina de ensamblador equivalente.
 */
void clearScreen_C(){
   
    printf("\x1B[2J");
    
}


/**
 * Situar el cursor en una fila y una columna de la pantalla
 * en función de la fila (rowScreen) y de la columna (colScreen) 
 * recibidos como parámetro.
 * 
 * Variables globales utilizadas:   
 * Ninguna
 * 
 * Parámetros de entrada: 
 * rdi(edi): (rowScreen) Fila
 * rsi(esi): (colScreen) Columna
 * 
 * Parámetros de salida : 
 * Ninguno
 * 
 * Se ha definido un subrutina en ensamblador equivalente 'gotoxyP2' 
 * para poder llamar a esta función guardando el estado de los registros 
 * del procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 * El paso de parámetros es equivalente.
 */
void gotoxyP2_C(int rowScreen, int colScreen){
   
   printf("\x1B[%d;%dH",rowScreen,colScreen);
   
}


/**
 * Mostrar un carácter (c) en pantalla, recibido como parámetro, 
 * en la posición donde está el cursor.
 * 
 * Variables globales utilizadas:   
 * Ninguna
 * 
 * Parámetros de entrada: 
 * rdi(dil): (c) Carácter que queremos mostrar.
 * 
 * Parámetros de salida : 
 * Ninguno
 * 
 * Se ha definido un subrutina en ensamblador equivalente 'printchP2' 
 * para llamar a esta función guardando el estado de los registros del 
 * procesador. Esto se hace porque las funciones de C no mantienen 
 * el estado de los registros.
 * El paso de parámetros es equivalente.
 */
void printchP2_C(char c){
   
   printf("%c",c);
   
}


/**
 * Leer una tecla y retornar el carácter asociado 
 * sin mostrarlo en pantalla. 
 * 
 * Variables globales utilizadas:   
 * Ninguna
 * 
 * Parámetros de entrada: 
 * Ninguno
 * 
 * Parámetros de salida : 
 * rax(al): (c) Carácter que leemos de teclado
 * 
 * Se ha definido un subrutina en ensamblador equivalente 'getchP2' para
 * llamar a esta función guardando el estado de los registros del procesador.
 * Esto se hace porque las funciones de C no mantienen el estado de los 
 * registros.
 * El paso de parámetros es equivalente.
 */
char getchP2_C(){

   int c;   

   static struct termios oldt, newt;

   /*tcgetattr obtener los parámetros del terminal
   STDIN_FILENO indica que se escriban los parámetros de la entrada estándard (STDIN) sobre oldt*/
   tcgetattr( STDIN_FILENO, &oldt);
   /*se copian los parámetros*/
   newt = oldt;

    /* ~ICANON para tratar la entrada de teclado carácter a carácter no como línea entera acabada en /n
    ~ECHO para que no se muestre el carácter leído.*/
   newt.c_lflag &= ~(ICANON | ECHO);          

   /*Fijar los nuevos parámetros del terminal para la entrada estándar (STDIN)
   TCSANOW indica a tcsetattr que cambie los parámetros inmediatamente. */
   tcsetattr( STDIN_FILENO, TCSANOW, &newt);

   /*Leer un carácter*/
   c=getchar();                 
    
   /*restaurar los parámetros originales*/
   tcsetattr( STDIN_FILENO, TCSANOW, &oldt);

   /*Retornar el carácter leído*/
   return (char)c;
   
}


/**
 * Mostrar en pantalla el menú del juego y pedir una opción.
 * Sólo acepta una de las opciones correctas del menú ('0'-'9')
 * 
 * Variables globales utilizadas:   
 * developer:((char *)&developer): variable definida en el código ensamblador.
 * 
 * Parámetros de entrada: 
 * Ninguno
 * 
 * Parámetros de salida : 
 * rax(al): (charac) Opción escogida del menú, leída de teclado.
 * 
 * Esta función no se llama desde ensamblador
 * y no hay definida una subrutina de ensamblador equivalente.
 */
char printMenuP2_C(){
   
    clearScreen_C();
    gotoxyP2_C(1,1);
    printf("                                    \n");
    printf("           Developed by:            \n");
	printf("        ( %s )   \n",(char *)&developer);
    printf(" __________________________________ \n");
    printf("|                                  |\n");
    printf("|            MAIN MENU             |\n");
    printf("|__________________________________|\n");
    printf("|                                  |\n");
    printf("|         1. ShowNumber            |\n");
    printf("|         2. UpdateBoard           |\n");
    printf("|         3. CopyMatrix            |\n");
    printf("|         4. RotateMatrix          |\n");
    printf("|         5. ShiftLeft             |\n");
    printf("|         6. AddPairs              |\n");
    printf("|         7. CheckEnd              |\n");
    printf("|         8. Play Game             |\n");
    printf("|         9. Play Game C           |\n");
    printf("|         0. Exit                  |\n");
    printf("|__________________________________|\n");
    printf("|                                  |\n");
    printf("|            OPTION:               |\n");
    printf("|__________________________________|\n"); 

    char charac =' ';
    while (charac < '0' || charac > '9') {
      gotoxyP2_C(21,22);      //Posicionar el cursor
      charac = getchP2_C();   //Leer una opción
      printchP2_C(charac);    //Mostrar opción 
   }
   return charac;

}


/**
 * Mostrar el tablero de juego en pantalla. Las líneas del tablero.
 * 
 * Variables globales utilizadas:   
 * Ninguna
 * 
 * Parámetros de entrada: 
 * Ninguno
 * 
 * Parámetros de salida : 
 * Ninguno
 * 
 * Esta función se llama des de C y desde ensamblador,
 * y no hay definida una subrutina de ensamblador equivalente.
 * No hay paso de parámetros.
 */
void printBoardP2_C(){

   gotoxyP2_C(1,1);
   printf(" _________________________________________________  \n"); //01
   printf("|                                                  |\n"); //02
   printf("|                  2048 PUZZLE  v2.0               |\n"); //03
   printf("|                                                  |\n"); //04
   printf("|     Join the numbers and get to the 2048 tile!   |\n"); //05   
   printf("|__________________________________________________|\n"); //06
   printf("|                                                  |\n"); //07
   printf("|            0        1        2        3          |\n"); //08
   printf("|        +--------+--------+--------+--------+     |\n"); //09
   printf("|      0 |        |        |        |        |     |\n"); //10
   printf("|        +--------+--------+--------+--------+     |\n"); //11
   printf("|      1 |        |        |        |        |     |\n"); //12
   printf("|        +--------+--------+--------+--------+-    |\n"); //13
   printf("|      2 |        |        |        |        |     |\n"); //14
   printf("|        +--------+--------+--------+--------+     |\n"); //15
   printf("|      3 |        |        |        |        |     |\n"); //16
   printf("|        +--------+--------+--------+--------+     |\n"); //17
   printf("|          Score:   ______                         |\n"); //18
   printf("|__________________________________________________|\n"); //19
   printf("|                                                  |\n"); //20
   printf("| (ESC)Exit (u)Undo (i)Up (j)Left (k)Down (l)Right |\n"); //21
   printf("|__________________________________________________|\n"); //22
   
}


 /**
 * Convierte el valor de la variable (n) de tipo int de 6 dígitos
 * (n <= 999999), recibida como parámetro a caracteres ASCII que
 * representan su valor.
 * Si (n) es maypr que 999999 cambiar el valor a 999999.
 * Hay que dividir(/) el valor entre 10, de forma iterativa, 
 * hasta obtenr 6 dígitos.
 * A cada iteración, el residuo de la división (%) que es un valor
 * entre (0-9) indica el valor del dígito que tenemos que convertir
 * a ASCII ('0' - '9') sumando '0' (48 decimal) para poderlo mostrar.
 * Cuando el cociente sea 0 mostraremos espacios en la parte no significativa.
 * Por ejemplo, si n=103 mostraremos "   103" y no "000103".
 * Se tienen que mostrar los dígitos (carácter ASCII) desde la posición 
 * indicada por las variables (rScreen) y (cScreen), recibidos como 
 * parámetro, posición de les unidades, hacia la izquierda.
 * Como el primer dígito que obtenemos son las unidades, después las decenas,
 * ..., para mostrar el valor se tiene que desplazar el cursor una posición
 * a la izquierda en cada iteración.
 * Para posicionar el cursor se llamada a la función gotoxyP2_C y para 
 * mostrar los caracteres a la función printchP2_C.
 * 
 * Variables globales utilizadas:   
 * Ninguna
 * 
 * Parámetros de entrada: 
 * rScreen : Fila de la pantalla donde posicionamos el cursor.
 * cScreen : Columna de la pantalla donde posicionamos el cursor.
 * n       : Número que queremos mostrar.
 * 
 * Parámetros de salida : 
 * Ninguno
 * 
 * Esta función no se llama desde ensamblador.
 * En la subrutina de ensamblador equivalente 'showNumberP2',  
 * el paso de parámetros es equivalente.
 */
 void showNumberP2_C(int rScreen, int cScreen, int n) {
   
    char charac;
	int  i;

    if (n > 999999) n = 999999;
	for (i=0;i<6;i++){
	  charac = ' ';
	  if (n > 0) {
		charac = n%10;	//residuo
		n = n/10;		//cociente
		charac = charac + '0';
	  } 
	  gotoxyP2_C(rScreen, cScreen);
	  printchP2_C(charac);
	  cScreen--;
	}
   
}


 /**
 * Actualizar el contenido del Tablero de Juego con los datos de 
 * la matriz (m) de 4x4 de tipo short y los puntos del marcador 
 * (score) que se han hecho.  
 * Se tiene que recorrer toda la matriz (m), y para cada elemento de 
 * la matriz posicionar el cursor en pantalla y mostrar el número de 
 * esa posición de la matriz.
 * Después, mostrar el marcador (score) que recibimos como paràmetre 
 * (scr) en la parte inferior del tablero, fila 18, columna 26 llamando
 * a la subrutina showNumberP2_C.
 * Finalmente posicionar el cursor en la fila 18, columna 28 llamando la 
 * funció gotoxyP2_C().
 * 
 * Variables globales utilizadas:   
 * m      : Matriz donde guardamos los números del juego.
 *  
 * Parámetros de entrada: 
 * scr    : Puntos acumulados en el marcador.
 * 
 * Parámetros de salida : 
 * Ninguno
 *  
 * Esta función no se llama desde ensamblador.
 * En la subrutina de ensamblador equivalente 'updateBoardP2', 
 * el paso de parámetros es equivalente.
 */
void updateBoardP2_C(int scr){

   int i,j;
   int rScreen, cScreen;
   
   rScreen = 10;
   for (i=0;i<DimMatrix;i++){
	  cScreen = 17;
      for (j=0;j<DimMatrix;j++){
         showNumberP2_C(rScreen,cScreen, m[i][j]);
         cScreen = cScreen + 9;
      }
      rScreen = rScreen + 2;
   }
   showNumberP2_C(18, 26, scr);   
   gotoxyP2_C(18,28);

}


 /**
 * Rotar a la derecha la matriz (mToRotate) recibida como parámetro, 
 * sobre la matriz (mRotated). 
 * La primera fila pasa a ser la cuarta columna, la segunda fila pasa 
 * a ser la tercera columna, la tercera fila pasa a ser la segunda
 * columna y la cuarta fila pasa a ser la primer columna.
 * En el enunciado se explica con más detalle como hacer la rotación.
 * NOTA: NO es lo mismo que hacer la matriz traspuesta.
 * La matriz recibida como parámetro no se tiene que modificar, 
 * los cambios se tiene que hacer en la matriz (mRotated).
 * Una vez se ha hecho la rotación, copiar la matriz (mRotated) a la 
 * matriz (m) llamando a la función copyMatrixP2_C().
 * No se tiene que mostrar la matriz.
 * 
 * Variables globales utilizadas:   
 * mRotated  : Matriz donde guardamos los números ya rotados
 * 
 * Parámetros de entrada: 
 * mToRotate : Matriz donde guardamos los números del juego que queremos rotar.
 * 
 * Parámetros de salida : 
 * Ninguno
 * 
 * Esta función no se llama desde ensamblador.
 * En la subrutina de ensamblador equivalente 'rotateMatrixRP2', 
 * el paso de parámetros es equivalente.
 */
void rotateMatrixRP2_C(short mToRotate[DimMatrix][DimMatrix]) {
   
   int i,j;
	
	for (i=0; i<DimMatrix; i++) {
		for (j=0; j<DimMatrix; j++) {	
			mRotated[j][DimMatrix-1-i] = mToRotate[i][j];
		}
	}
	
	copyMatrixP2_C(mToRotate, mRotated);
   
}


 /**
 * Copia la matriz origen (mOrigin) -segundo parámetro- sobre la matriz 
 * destino (mDest) -primer parámetro-.
 * La matriz origen (mOrigin) no se tiene que modificar, 
 * los cambios se tiene que hacer en la matriz destino (mDest).
 * Esto permitirá copiar dos matrices después de una rotación 
 * y gestionar la opción '(u)Undo' del juego.
 * No se muestra la matriz.
 * 
 * Variables globales utilizadas:   
 * Ninguna
 * 
 * Parámetros de entrada: 
 * mOrigin : Matriz donde guardamos los números del juego que queremos copiar.
 * mDest   : Matriz donde guardamos los números del juego que queremos sobreescribir.
 * 
 * Parámetros de salida : 
 * Ninguno
 * 
 * Esta función no se llama desde ensamblador.
 * En la subrutina de ensamblador equivalente 'copyStatusP2', 
 * el paso de parámetros es equivalente.
 */
void copyMatrixP2_C(short mDest[DimMatrix][DimMatrix], short mOrig[DimMatrix][DimMatrix]) {
   
   int i,j;
   
   for (i=0; i<DimMatrix; i++) {
      for (j=0; j<DimMatrix; j++) {
         mDest[i][j]=mOrig[i][j];
      }
   }

}


 /**
 * Desplazar a la derecha los números de cada fila de la matriz 
 * (mShift), recibida como parámetro, manteniendo el orden de los 
 * números y poniendo los ceros a la izquierda.
 * Recorrer la matriz por filas de derecha a izquierda de abajo hacia arriba.   
 * Si se desplaza un número (NO LOS CEROS), se tienen que contar los 
 * desplazamientos incrementando la variable (shifts).
 * En cada fila, si encontramos un 0, mira si hay un número diferentr de cero,
 * en la misma fila para ponerlo en aquella posición.
 * Si una fila de la matriz es: [0,2,0,4] y state = '1', quedará [0,0,2,4] 
 * y state = '2'.
 * Retornaremos el valor de la variable (shifts) para indicar el número
 * de desplazamientos que se han hecho.
 * Los cambios se tienen que hacer sobre la misma  matriz.
 * No se tiene que mostrar la matriz.
 * 
 * Variables globales utilizadas:   
 * Ninguna
 * 
 * Parámetros de entrada: 
 * mShift  : Matriz donde guardamos los números del juego que queremos desplazar.
 * 
 * Parámetros de salida : 
 * shifts   : Desplazamientos que se han hecho a la izquierda.
 * 
 * Esta función no se llama desde ensamblador.
 * En la subrutina de ensamblador equivalente 'ShiftNumbersRP2', 
 * el paso de parámetros es equivalente.
 */
int shiftNumbersRP2_C(short mShift[DimMatrix][DimMatrix]) {
   
    int i,j,k;
	int shifts=0;
	
	for (i=DimMatrix-1; i>=0; i--) {
      for (j=DimMatrix-1; j>0; j--) {
        if (mShift[i][j] == 0) {
          k = j-1;           
          while (k>=0 && mShift[i][k]==0) k--;
          if (k==-1) {
             j=0;                
          } else {
              mShift[i][j]=mShift[i][k];
              mShift[i][k]= 0; 
              shifts++;         
          }
        }      
      }
    }

    return shifts;
   
}
   

 /**
 * Emparejar números iguales des la derecha de la matriz (mPairs), 
 * recibida como parámetro, y acumular los puntos de 
 * las parejas que se hagan.
 * Recorrer la matriz por filas de derecha a izquierda y de abajo hacia arriba. 
 * Cuando se encuentre una pareja, dos casillas consecutivas con el mismo 
 * número, juntamos la pareja poniendo la suma de los números de la 
 * pareja en la casilla de la derecha y un 0 en la casilla de la izquierda y 
 * acumularemos esta suma en la variable (p) (puntos que se ganan).
 * Si una fila de la matriz es: [8,4,4,2] y p = 0, quedará [8,0,8,2] , 
 * p = p + (4+4).
 * Retornamos los puntos obtenidos de hacer la parejas.
 * No se tiene que mostrar la matriz.
 * 
 * Variables globales utilizadas:   
 * Ninguna
 * 
 * Parámetros de entrada: 
 * mPairs  : Matriz donde guardamos los números del juego que queremos hacer parejas.
 * 
 * Parámetros de salida : 
 * p       : puntos  de las parejas que se hayan hecho.
 * 
 * Esta función no se llama desde ensamblador.
 * En la subrutina de ensamblador equivalente 'addPairsRP2', 
 * el paso de parámetros es equivalente.
 */
int addPairsRP2_C(short mPairs[DimMatrix][DimMatrix]) {
   
    int i,j;
	int p = 0;
	
	for (i=DimMatrix-1; i>=0; i--) {
      for (j=DimMatrix-1; j>0; j--) {
			if ((mPairs[i][j]!=0) && (mPairs[i][j]==mPairs[i][j-1])) {
				mPairs[i][j]  = mPairs[i][j]*2;
				mPairs[i][j-1]= 0;
				p = p + mPairs[i][j];
			}		
		}
	}
	
	return p;
   
}


/**
 * Leer una tecla. Guardar la tecla leída en la variable (c).
 * Segun la tecla leida llamaremos a las funciones que corresponda.
 *    ['i' (arriba),'j'(izquierda),'k' (a bajo) o 'l'(derecha)] 
 * Desplazar los números y hacer las parejas según la dirección escogida.
 * Según la tecla pulsada, rotar la matriz (m) llamando (rotateMatrixRP2_C),
 * para poder hacer los desplazamientos de los números hacia la derechara
 * llamando (shiftNumbersRP2_C), hacer las parejas hacia la derecha
 * (addPairsRP2_C) que retorna los puntos que tenemos que sumar al 
 * marcador (actualScore) recibido como parámetro y que retornaremos 
 * para actualizar el marcador (score).
 * Volver a desplazar los números hacia la izquierda (shiftNumbersLP2_C)
 * con las parejas hechas.
 * Si se ha hecho algún desplazamiento o alguna pareja
 * indicarlo poniendo (state='2').
 * Después seguir rotando llamando (rotateMatrixRP2_C)
 * hasta dejar la matriz en la posición inicial. 
 * Para la tecla 'l' (derecha) no hay que hacer rotaciones, para el
 * resto se tienen que hacer 4 rotaciones.
 *     'u'                Poner (state = '3') para recuperar el estado anterior.
 *    '<ESC>' (ASCII 27)  Poner (state = '0') para salir del juego.
 * Si no es ninguna de estas teclea no hacer nada.
 * Los cambios producidos por estas funciones no se tiene que mostrar en 
 * pantalla, por lo tanto, hay que actualizar después el tablero llamando 
 * la subrutina UpdateBoardP2_C.
 * 
 * Variables globales utilizadas:   
 * m        : Matriz donde guardamos los números del juego.
 * mRotated : Matriz donde guardamos los números ya rotados.
 * state    : Estado del juego.
 * 
 * Parámetros de entrada: 
 * actualScore : Puntos acumulados en el marcador.
 * 
 * Parámetros de salida : 
 * actualScore : Puntos acumulados en el marcador actualizados.
 * 
 * Esta función no se llama desde ensamblador.
 * En la subrutina de ensamblador equivalente 'readKeyP2', 
 * el paso de parámetros es equivalente.
 */
int readKeyP2_C(int actualScore){

   int s1, s2;
   int p;
   
   char c;
   c = getchP2_C();	//lleer una tecla y guardarla en la variable c.
 
   switch(c){
      case 'i': //i:(105) arriba
         rotateMatrixRP2_C(m);
		 
         s1 = shiftNumbersRP2_C(m);
         p  = addPairsRP2_C(m);
         s2 = shiftNumbersRP2_C(m);
         actualScore = actualScore + p;
         if ( (s1>0) || (p>0) || (s2>0) ) state = '2';
         
         rotateMatrixRP2_C(m);
         rotateMatrixRP2_C(m);
         rotateMatrixRP2_C(m);
		 
      break;
      case 'j': //j:(106) izquierda
         rotateMatrixRP2_C(m);
         rotateMatrixRP2_C(m);
         
         s1 = shiftNumbersRP2_C(m);
         p  = addPairsRP2_C(m);
         s2 = shiftNumbersRP2_C(m);
         actualScore = actualScore + p;
         if ( (s1>0) || (p>0) || (s2>0) ) state = '2';
         
         rotateMatrixRP2_C(m);
         rotateMatrixRP2_C(m);
      break;
      case 'k': //k:(107) abajo
         rotateMatrixRP2_C(m);
         rotateMatrixRP2_C(m);
         rotateMatrixRP2_C(m);
             
         s1 = shiftNumbersRP2_C(m);
         p  = addPairsRP2_C(m);
         s2 = shiftNumbersRP2_C(m);
         actualScore = actualScore + p;
         if ( (s1>0) || (p>0) || (s2>0) ) state = '2';
         
		 rotateMatrixRP2_C(m);
      break;
      case 'l': //l:(108) derecha
         s1 = shiftNumbersRP2_C(m);
         p  = addPairsRP2_C(m);
         s2 = shiftNumbersRP2_C(m);
         actualScore = actualScore + p;
         if ( (s1>0) || (p>0) || (s2>0) ) state = '2';
      break;  
	  case 'u': //u:(117) Deshacer jugada
         state = '3';
	  break;
	  case 27: //ESC:(27) Salir del programa
		 state = '0';
	  break;
	}
   
   return actualScore;
   
}


/**
 * Generar nueva ficha de forma aleatoria.
 * Si hay como mínimo una casilla vacía en la matriz (m) generar una  
 * fila y una columna de forma aleatoria hasta que sea una de les casillas 
 * vacías. 
 * A continuación generar un número aleatorio para decidir si la nueva
 * ficha tiene que de ser un 2 (90% de los casos) o un 4 (10% de los casos).
 * 
 * Variables globales utilizadas:   
 * m        : Matriz donde guardamos los números del juego.
 * 
 * Parámetros de entrada: 
 * Ninguno
 * 
 * Parámetros de salida : 
 * Ninguno
 * 
 * Esta función se llama desde C y desde ensamblador,
 * y no hay definida una subrutina de ensamblador equivalente.
 * No hay paso de parámetros.
 */
void insertTileP2_C() {
   
   int i,j,k,l,r;
   
   i=DimMatrix; // Miramos si hay una casilla vácia.
   do {
      i--;
      j=DimMatrix;
      do {
         j--;   
      } while ((j>=0) && (m[i][j]!=0));
   } while ((i>=0) && (m[i][j]!=0));
   
   //Insertamos el 2 o el 4 si hay como a mínimo una casilla vacía.
   if (m[i][j]==0) { 
      do { // Generar filas y columnas aleatoriamente hasta que 
           // encuentra una posición vacía.
         k = rand() % 4; l = rand() % 4; }
      while( m[k][l] != 0 );
      
      //Decidimos si ponemos un 2 o un 4
      r = rand() % 100;
      if( r < 90 ) {
         m[k][l] = 2;
      } else {
         m[k][l] = 4;
      }
   }

}


/**
 * Verificar si se ha llegado a 2048 o si no se puede hacer algún movimiento.
 * Si hay el número 2048 en la matriz (m), cambiar el estado a 4 
 * (status='4') para indicar que se ha ganado (WIN!).
 * Si no hemos ganado, mirar si se puede hacer algun movimento, 
 * Si no se puede hacer ningún movimiento cambiar el estado a 5 
 * (status='5') para indicar que se ha perdido (GAME OVER!!!).
 * Recorrer la matriz (m) por filas de derecha a izquierda y de abajo hacia 
 * arriba contando las casillas vacías y mirando si hay el número 2048. 
 * Si hay el número 2048 poner (status='4') y acabar.
 * Si no hay el número 2048 y no hay casillas vacías, mirar si se puede
 * hacer alguna pareja en horizontal o en vertical. Par hacerlo 
 * hay que copiar la matriz (m) sobre la matriz (mAux) llamando 
 * (copyMatrixP2_C), hacer parejas en la matriz (mAux) para mirar si se
 * pueden hacer parejas en horizontal llamando (addPairsRP2_C) y guardar
 * los puntos obtenidos, rotar la matriz (mAux) llamando (rotateMatrixRP2_C)
 * y volver a hacer parejas en la matriz (mAux) para mirar si se pueden 
 * hacer parejas en vertical llamando (addPairsRP2_C) y acumular los 
 * puntos obtenidos con los puntos obtenidos antes, si los puntos 
 * acumulados son 0, quiere decir que no se pueden hacer parejas y se 
 * tiene que poner (status='5').
 * No se puede modificar ni la matriz (m), ni la matriz (mUndo).
 * 
 * Variables globales utilizadas:   
 * m        : Matriz donde guardamos los números del juego.
 * mRotated : Matriz donde guardamos los números ya rotados.
 * mAux     : Matriz donde copiamos la matriz (m) para comprobarla
 * state    : Estado del juego.
 * 
 * Parámetros de entrada: 
 * Ninguno
 * 
 * Parámetros de salida : 
 * Ninguno
 * 
 * Esta función se llama desde C y desde ensamblador,
 * En la subrutina de ensamblador equivalente 'checkEndP2', 
 * el paso de parámetros es equivalente.
 */
void checkEndP2_C() {
   
   int i,j;
	int zeros=0;
	int pairs=0;
		
	i=DimMatrix; // Contar casillas vacías y mirar si hay un 2048.
	do {
		i--;
		j=DimMatrix;
		do {
			j--;
			if (m[i][j] == 0 ) zeros++;
			if (m[i][j] == 2048) state='4';
		} while ((j>0) && (m[i][j]!=2048));
		
	} while ((i>0) && (m[i][j]!=2048));

	if ((state!= '4') && (zeros == 0)) {
		copyMatrixP2_C(mAux,m);
		pairs = addPairsRP2_C(mAux);
		rotateMatrixRP2_C(mAux);
		pairs = pairs + addPairsRP2_C(mAux);
		if (pairs==0) state = '5';	
	}
   
} 


 /**
 * Muestrar un mensaje debajo del tablero según el valor de la variable 
 * (state).
 * state:  0: Salir, hemos pulsado la tecla 'ESC' para salir.
 *         1: Continuamos jugando.
 *         2: Continuamos jugando pero han habido cambios en la matriz.
 *         3: Deshacer el último movimento.
 *         4: Ganado, se ha llegado a la casilla 2048.
 *         5: Perdido, no se pueden hacer movimientos.
 * Espera que se pulse una tecla para continuar.
 * 
 * Si se ha perdido (status='5') se da la opción de hacer un último 'Undo'
 * pulsando la tecla 'u' para recuperar el estado anterior y probar de 
 * continuar jugando. Si se pulsa otra tecla se sale del juego.
 * 
 * Variables globales utilizadas:   
 * state   :  Estado del juego.
 * 
 * Parámetros de entrada: 
 * Ninguno
 * 
 * Parámetros de salida : 
 * Ninguno
 * 
 * Esta función se llama desde C y desde ensamblador,
 * y no hay definida una subrutina de ensamblador equivalente.
 */
void printMessageP2_C() {

    switch(state){
      case '0':
		 gotoxyP2_C(23,12);
         printf("<<<<<< EXIT: (ESC) Pressed >>>>>>");
         getchP2_C();
        break;
      case '4':
		 gotoxyP2_C(23,12);
         printf("++++++ 2048!!!  YOU  W I N ++++++");
         getchP2_C();
      break;
      case '5':
		 gotoxyP2_C(23,12);
         printf("---- G A M E   O V E R ! ! ! ----");
         gotoxyP2_C(24,12);
         printf("---- (u)Undo  (Any key) EXIT ----");
         char c;
		 c = getchP2_C();
		 if (c == 'u') { //u:(117) Deshacer jugada
			gotoxyP2_C(23,12);
			printf("                                  ");
			gotoxyP2_C(24,12);
            printf("                                  ");
            state='3';
		 }
      break;
   }

}
 

/**
 * Juego del 2048
 * Función principal del juego
 * Permite jugar al juego del 2048 llamando todas las funcionalidades.
 *
 * Pseudo-código:
 * Inicializar estado del juego, (state='1')
 * Borrar pantalla (llamar a la función clearScreen_C).
 * Mostrar el tablero de juego (llamar a la función PrintBoardP2_C).
 * Actualizar el contenido del Tablero de Juego y los puntos que se han 
 * hecho (llamar a la función updateBoardP2_C).
 * Mientras (state=='1') hacer:
 *   Copiar la matriz (m) sobre la matriz (mAux) (llamando la función 
 *   copyMatrixP2_C) y copiar los puntos (score) sobre (scoreAux).
 *   Leer una tecla (llamar a la función readKeyP2_C). Según la tecla 
 *     leída llamar a las funciones que corresponda.
 *     - ['i','j','k' o 'l'] desplazar los números y hacer las parejas 
 *                           según la dirección escogida.
 *     - 'u'                 Recuperar la jugada anterior copiando (mUndo) 
 *                           sobre (m) (llamando a la función copyMatrixP2_C)
 *                           y copiando (scoreUndo) sobre (score).
 *     - '<ESC>'  (código ASCII 27) poner (state = '0') para salir.   
 *   Si hemos movido algún número al hacer los desplazamientos o al hacer
 *   las parejas (state=='2'), copiar el estado del juego que hemos guardado
 *   antes (mAux i scoreAux) a (mUndo i scoreUndo) para poder hacer el Undo 
 *   (recuperar estado anterior) copiando (mAux) sobre (mUndo) (llamando a 
 *   la función copyMatrixP2_C) y copiando (scoreAux) sobre (scoreUndo).
 *   Generar una nueva ficha (llamando a la función  insertTileP2_C) y 
 *   poner la variable state a 1 (state='1').
 *   Si tenemos que recuperar el estado anterior (state='3'), copiar el estatdo
 *   anterior del juego que tenemos en (mUndu y scoreUndu) sobre (m y score)
 *   (llamando a la función copyMatrixP2_C) y copiando (scoreUndu) sobre 
 *   (score) y poner la variable state a '1' (state='1').
 *   Actualizar el contenido del Taublero de Juego y los puntos que se han
 *   hecho (llamar a la función updateBoardP2_C).
 *   Verificar si se ha llegado a 2048 o si no se puede hacer ningún 
 *   movimiento (llamar a la función CheckEndP2_C).
 *   Mostrar un mensaje debajo del tablero según el valor de la variable 
 *   (state). (llamar a la función printMessageP2_C()).
 * Fin mientras.
 * Salir: 
 * Se ha terminado el juego.
 */
void playP2_C(){
   
   int score      = 290500;   // Puntos acumulados en el marcador.
   int scoreAux   = 0;
   int scoreUndo  = 1;        // Puntos que se han hecho en la jugada anterior.  
           
   state = '1';	   			   
   clearScreen_C();
   printBoardP2_C();
   updateBoardP2_C(score);
          
   while (state == '1') {       //Bucle principal.
    copyMatrixP2_C(mAux,m);   //Para hacer el Undo si se hacen movimentos
    scoreAux = score;
    score = readKeyP2_C(score);
    if (state == '2') {	      //Si se ha hecho algún movimiento, guardamos estado.
		copyMatrixP2_C(mUndo,mAux);
		scoreUndo = scoreAux;
		insertTileP2_C();	  //Añadir ficha (2 o 4)
		state = '1';
	 }
	 if (state == '3') {      //Deshacer último movimiento
		 copyMatrixP2_C(m,mUndo);
         score = scoreUndo;
         state = '1';
     }
     updateBoardP2_C(score);
	 checkEndP2_C();
	 printMessageP2_C();      //Mostra el mensaje para indicar com acaba.
	 if (state == '3') {      //Deshacer último movimiento
		 copyMatrixP2_C(m,mUndo);
         score = scoreUndo;
         state = '1';
         updateBoardP2_C(score);
     }
   }
   
}


/**
 * Programa Principal
 * 
 * ATENCIÓN: En cada opción se llama a una subrutina de ensamblador.
 * Debajo hay comentada la función en C equivalente que os damos hecha 
 * por si queréis ver como funciona, para el juego completo hay una 
 * opción para cada caso.
 */
int main(){   

   char op=' ';
   char c;
   int score = 123456;
     
   while (op!='0') {
      clearScreen_C();
      op = printMenuP2_C();   //Mostrar menú y leer opción
      
      switch(op){
         case '1':// Mostrar puntos
            clearScreen_C();  
            printBoardP2_C(); 
            gotoxyP2_C(18, 30);
            printf(" Press any key ");
            //=======================================================
            showNumberP2(18, 26, score);       
            //showNumberP2_C(18, 26, score);   
            //=======================================================
            getchP2_C();
         break;
         case '2': //Actualizar el contenido del tablero.
            clearScreen_C();  
            printBoardP2_C(); 
            //=======================================================
            updateBoardP2(score);
            //updateBoardP2_C(score); 
            //=======================================================
            gotoxyP2_C(18, 30);
            printf("Press any key ");
            getchP2_C();
         break;
         case '3': //Copiar matrices y marcador
            clearScreen_C();  
            printBoardP2_C(); 
            int scoreUndu = 500;
            //=======================================================
            copyMatrixP2(m, mUndo);
            //copyMatrixP2_C(m, mUndo);
            //=======================================================
            updateBoardP2_C(scoreUndu);
            gotoxyP2_C(18, 30);
			printf("Press any key ");
	        getchP2_C();
         break;
         case '4': //Rotar matriz a la derecha
            clearScreen_C();  
            printBoardP2_C(); 
            //===================================================
            rotateMatrixRP2(m);
            //rotateMatrixRP2_C(m);
            //===================================================
            updateBoardP2_C(score);
            gotoxyP2_C(18, 30);
            printf("Press any key ");
            getchP2_C();
          break;
          case '5': //Desplazar números a la derecha
            clearScreen_C();  
            printBoardP2_C(); 
            //===================================================
            shiftNumbersRP2(m);
			//shiftNumbersRP2_C(m);
			//===================================================
            updateBoardP2_C(score);
            gotoxyP2_C(18, 30);
			printf("Press any key ");
	        getchP2_C();
          break;
          case '6': //Sumar parejas
            clearScreen_C();  
            printBoardP2_C(); 
            score = 1000;
            //===================================================
			score = score + addPairsRP2(m);
		    //score = score + addPairsRP2_C(m);
			//===================================================
			updateBoardP2_C(score);
			gotoxyP2_C(18, 30);
			printf("Press any key ");
	        getchP2_C();
         break;
         case '7': //Verificar si hay el 2048 o si no se puede hacer algún movimiento.
            clearScreen_C();  
            printBoardP2_C(); 
            updateBoardP2_C(score);
            //===================================================
            checkEndP2();
            //checkEndP2_C();
            //===================================================
            printMessageP2_C();//Mostra mensaje segun state
            if ((state!='4') && (state!='5')) {
			   gotoxyP2_C(18, 30);
			   printf("Press any key ");
	           getchP2_C();
	        }
         break;
         case '8': //Juego completo Ensamblador
            //=======================================================
            playP2();
            //=======================================================
         break;
         case '9': //Juego completo C 
            //=======================================================
            playP2_C();
            //=======================================================
         break;
     }
   }
   printf("\n\n");
   
   return 0;
   
}
