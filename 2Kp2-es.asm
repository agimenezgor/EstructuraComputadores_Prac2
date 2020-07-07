section .data               
;Cambiar Nombre y Apellido por vuestros datos.
developer db "_Nom_ _Cognom1_",0

;Constantes que también están definidas en C.
DimMatrix    equ 4      
SizeMatrix   equ 16

section .text            
                         
;Variables definidas en Ensamblador.
global developer  

;Subrutinas de ensamblador que se llaman desde C.
global showNumberP2, updateBoardP2, rotateMatrixRP2, copyMatrixP2
global shiftNumbersRP2, addPairsRP2
global readKeyP2, checkEndP2, playP2

;Variables definidas en C.
extern m, mRotated, mAux, mUndo, state

;Funciones de C que se llaman desde ensamblador.
extern clearScreen_C,  gotoxyP2_C, getchP2_C, printchP2_C
extern printBoardP2_C, printMessageP2_C, insertTileP2_C   


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; ATENCIÓN: Recordad que en ensamblador las variables y los parámetros 
;;   de tipo 'char' se tienen que asignar a registros de tipo
;;   BYTE (1 byte): al, ah, bl, bh, cl, ch, dl, dh, sil, dil, ..., r15b
;;   las de tipo 'short' se tiene que assignar a registros de tipo 
;;   WORD (2 bytes): ax, bx, cx, dx, si, di, ...., r15w
;;   las de tipo 'int' se tiene que assignar a registros de tipo  
;;   DWORD (4 bytes): eax, ebx, ecx, edx, esi, edi, ...., r15d
;;   las de tipo 'long' se tiene que assignar a registros de tipo 
;;   QWORD (8 bytes): rax, rbx, rcx, rdx, rsi, rdi, ...., r15
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Les subrutinas en ensamblador que hay que modificar para 
;; implementar el paso de parámetros son:
;;   showNumberP2, updateBoardP2, rotateMatrixRP2, copyMatrixP2, 
;;   shiftNumbersRP2, addPairsRP2
;; La subrutina nueva que se tiene que implementar es:
;;   checkEndP2
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Esta subrutina se da hecha. NO LA PODEIS MODIFICAR.
; Situar el cursor en una fila y una columna de la pantalla
; en función de la fila (edi) y de la columna (esi) recibidos como 
; parámetro llamando a la función gotoxyP2_C.
; 
; Variables globales utilizadas:   
; Ninguna
; 
; Parámetros de entrada: 
; rdi(edi): Fila
; rsi(esi): Columna
; 
; Parámetros de salida : 
; Ninguno
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
gotoxyP2:
   push rbp
   mov  rbp, rsp
   ;guardamos el estado de los registros del procesador porque
   ;las funciones de C no mantienen el estado de los registros.
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15

   ; Cuando llamamos a la función gotoxyP2_C(int rowCurScreen, int colCurScreen) 
   ; desde ensamblador el primer parámetro (rowCurScreen) se tiene que 
   ; pasar por el registro rdi(edi), y el segundo  parámetro (colCurScreen)
   ; se tiene que pasar por el registro rsi(esi).
   call gotoxyP2_C
 
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax

   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Esta subrutina se da hecha. NO LA PODEIS MODIFICAR.
; Mostrar un carácter (dil) en pantalla, recibido como parámetro, 
; en la posición donde está el cursor llamando a la función printchP2_C.
; 
; Variables globales utilizadas:   
; Ninguna
; 
; Parámetros de entrada: 
; rdi(dil): carácter que queremos mostrar
; 
; Parámetros de salida : 
; Ninguno
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
printchP2:
   push rbp
   mov  rbp, rsp
   ;guardamos el estado de los registros del procesador porque
   ;las funciones de C no mantienen el estado de los registros.
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15

   ; Cuando llamamos a la función printchP2_C(char c) desde ensamblador, 
   ; el parámetro (c) se tiene que pasar por el registro rdi(dil).
   call printchP2_C
 
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax

   mov rsp, rbp
   pop rbp
   ret
   

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Esta subrutina se da hecha. NO LA PODEIS MODIFICAR.
; Leer una tecla y retornar el carácter asociado (al) sin
; mostrarlo en pantalla, llamando a la función getchP2_C
; 
; Variables globales utilizadas:   
; Ninguna
; 
; Parámetros de entrada: 
; Ninguno
; 
; Parámetros de salida : 
; rax(al): carácter que leemos de teclado
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
getchP2:
   push rbp
   mov  rbp, rsp
   ;guardamos el estado de los registros del procesador porque
   ;las funciones de C no mantienen el estado de los registros.
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15

   mov rax, 0
   ; llamamos a la función getchP2_C(char c) desde ensamblador, 
   ; retorna sobre el registro rax(al) el carácter leído.
   call getchP2_C
 
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   
   mov rsp, rbp
   pop rbp
   ret 



;;;;;
; Convertir el valor recibido como parámetro (edx) de tipo int (DWORD)
; de 6 dígitos (num<=999999) a los caracteres ASCII que representen 
; su valor. 
; Hay que dividir el valor entre 10, de forma iterativa, 
; hasta obtener 6 dígitos.
; En cada iteración, el residuo de la división que es un valor entre
; (0-9) indica el valor del dígito que tenemos que convertir
; a ASCII ('0' - '9') sumando '0' (48 decimal) para poderlo mostrar.
; Cuando el cociente sea 0 mostraremos espacios en la parte no significativa.
; Por ejemplo, si number=103 mostraremos "   103" y no "000103".
; Se tienen que mostrar los dígitos (carácter ASCII) desde la posición 
; indicada por la fila (edi) y la columna (esi) recibidos como 
; parámetro, posición de les unidades, hacia la izquierda.
; Como el primer dígito que obtenemos son las unidades, después las decenas,
; ..., para mostrar el valor se tiene que desplazar el cursor una posición
; a la izquierda en cada iteración.
; Para posicionar el cursor llamar a la subrutina gotoxyP2 y para 
; mostrar los caracteres a la subrutina printchP2 implementando 
; correctament el paso de parámetros.
;
; Variables globales utilizadas:   
; Ninguna.
;
; Parámetros de entrada: 
; rdi (edi): Fila donde lo queremos mostrar en pantalla.
; rsi (esi): Columna donde lo queremos mostrar en pantalla.
; rdx (edx): Valor que queremos mostrar en pantalla.
;
; Parámetros de salida : 
; Ninguno.
;;;;;
showNumberP2:
   push rbp
   mov  rbp, rsp
   
   
   
   mov rsp, rbp
   pop rbp
   ret


;;;;;
; Actualizar el contenido del Tablero de Juego con los datos de 
; la matriz (m) de 4x4 valores de tipo short (WORD) y los puntos del 
; marcador (score) que se han hecho.  
; Se tiene que recorrer toda la matriz (m), y para cada elemento de 
; la matriz posicionar el cursor en pantalla y mostrar el número de 
; esa posición de la matriz.
; Después, mostrar el marcador que recibimos como parámetro (edi) en la 
; parte inferior del tablero, fila 18, columna 26 llamando a la 
; subrutina showNumberP2.
; Finalmente posicionar el cursor en la fila 18, columna 28 llamando a 
; la subrutina gotoxyP2.
;
; Variables globales utilizadas:   
; m        : matriz 4x4 donde hay los números del tablero de Juego.
; 
;
; Parámetros de entrada: 
; rdi (edi): puntos acumulados hasta el momento.
;
; Parámetros de salida : 
; Ninguno.
;;;;;  
updateBoardP2:
   push rbp
   mov  rbp, rsp
   
   
   
   mov rsp, rbp
   pop rbp
   ret


;;;;;      
; Rotar a la derecha la matriz recibida como parámetro (rdi), sobre 
; la matriz (mRotated). 
; La primera fila pasa a ser la cuarta columna, la segunda fila pasa 
; a ser la tercera columna, la tercera fila pasa a ser la segunda
; columna y la cuarta fila pasa a ser la primer columna.
; En el enunciado se explica con más detalle como hacer la rotación.
; NOTA: NO es lo mismo que fer la matriz traspuesta.
; La matriz recibida como parámetro no se tiene que modificar, 
; los cambios se tiene que hacer en la matriz (mRotated).
; Para recorrer la matriz en ensamblador el indice va de 0 (posición [0][0])
; a 30 (posición [3][3]) con incrementos de 2 porque los datos son de 
; tipo short(WORD) 2 bytes.
; Para acceder a una posición concreta de la matriz desde ensamblador 
; hay que tener en cuenta que el índice es:(index=(fila*DimMatrix+columna)*2),
; multiplicamos por 2 porque los datos son de tipo short(WORD) 2 bytes.
; Una vez se ha hecho la rotación, copiar la matriz (mRotated) a la 
; matriz (m) llamando a la subrtuina copyMatrixP2.
; No se tiene que mostrar la matriz.
;
; Variables globales utilizadas:   
; mToRotate : Matriz donde guardamos los números del juego que queremos rotar.
;
; Parámetros de entrada: 
; rdi       : Dirección de la matriz que queremos rotar.
;
; Parámetros de salida : 
; Ninguno.
;;;;;  
rotateMatrixRP2:
   push rbp
   mov  rbp, rsp
   
   
   
   mov rsp, rbp
   pop rbp
   ret


;;;;;  
; Copia la matriz, que recibimos como segundo parámetro (rsi), en la 
; matriz destino, que recibimos como primer parámetro (rdi). 
; La matriz origen no se tiene que modificar, 
; los cambios se tiene que hacer en la matriz destino.
; Esto permitirá copiar dos matrices después de una rotación 
; y gestionar la opción '(u)Undo' del juego.
; Para recorrer la matriz en ensamblador el índice va de 0 (posición [0][0])
; a 30 (posición [3][3]) con incrementos de 2 porque los datos son de 
; tipo short(WORD) 2 bytes.
; No se muestra la matriz. 
;
; Variables globales utilizadas:   
; Ninguna.
;
; Parámetros de entrada: 
; rdi : Dirección de la matriz destino.
; rsi : Dirección de la matriz origen.
;
; Parámetros de salida : 
; Ninguno.
;;;;;  
copyMatrixP2:
   push rbp
   mov  rbp, rsp

             
   
   mov rsp, rbp
   pop rbp
   ret   
   

;;;;;  
; Desplazar a la izquierda los números de cada fila de la matriz 
; recibida como parámetro (rdi), manteniendo el orden de los 
; números y poniendo los ceros a la izquierda.
; Recorrer la matriz por filas de derecha a izquierda de abajo hacia arriba.   
; Si se desplaza un número (NO LOS CEROS) se tienen que contar los 
; desplazamientos.
; Retornaremos el número de desplazamientos hechos sobre (eax).
; Si una fila de la matriz es: [0,2,0,4] y (shift=0)
; quedará [0,0,2,4] y (shift=2).
; En cada fila, si encuentra un 0, mira si hay un número distinto de zero,
; en la misma fila para ponerlo en aquella posición.
; Para recorrer la matriz en ensamblador, en este caso, el índice va de la
; posición 30 (posición [3][3]) a la 0 (posición [0][0]) con decrementos de
; 2 porque los datos son de tipo short(WORD) 2 bytes.
; Per a acceder a una posición concreta de la matriz desde ensamblador 
; hay que tener en cuenta que el índice es:(index=(fila*DimMatrix+columna)*2),
; multiplicamos por 2 porque los datos son de tipo short(WORD) 2 bytes.
; Los cambios se tienen que hacer sobre la misma  matriz.
; No se tiene que mostrar la matriz.
;
; Variables globales utilizadas:   
; Ninguna.
;
; Parámetros de entrada: 
; rdi : Dirección de la matriz que queremos desplazar los números.
;
; Parámetros de salida : 
; rax(eax) : Número de desplazamientos hechos.
;;;;;  
shiftNumbersRP2:
   push rbp
   mov  rbp, rsp

           
   
   mov rsp, rbp
   pop rbp
   ret
      

;;;;;  
; Emparejar números iguales des la derecha de la matriz recibida como 
; parámetro (rdi) y acumular los puntos sumando los puntos de las 
; parejas que se hayan hecho.
; Recorrer la matriz por filas dde derecha a izquierda y de abajo hacia arriba.
; Cuando se encuentre una pareja, dos casillas consecutivas con el mismo 
; número, juntamos la pareja poniendo la suma de los números de la 
; pareja en la casilla de la derecha y un 0 en la casilla de la izquierda
; y acumularemos esta suma (puntos que se ganan).
; Si una fila de la matriz es: [8,4,4,2] y (state='1'), quedará [8,0,8,2], 
; y puntos = (4+4)= 8.
; Para recorrer la matriz en ensamblador, en este caso, el índice va de la
; posición 30 (posición [3][3]) a la 0 (posición [0][0]) con decrementos de
; 2 porque los datos son de tipo short(WORD) 2 bytes.
; Per a acceder a una posición concreta de la matriz desde ensamblador 
; hay que tener en cuenta que el índice es:(index=(fila*DimMatrix+columna)*2),
; multiplicamos por 2 porque los datos son de tipo short(WORD) 2 bytes.
; Los cambios se tienen que hacer sobre la misma  matriz.
; No se tiene que mostrar la matriz.
;
; Variables globales utilizadas:   
; Ninguna.
;
; Parámetros de entrada: 
; rdi      : Dirección de la matriz que queremos hacer las parejas.
;
; Parámetros de salida : 
; rax(eax) : Puntos obtenidos de hacer las parejas.
;;;;;  
addPairsRP2:
   push rbp
   mov  rbp, rsp

   
   
   mov rsp, rbp
   pop rbp
   ret


;;;;;
; Esta subrutina se da hecha. NO LA PODÉIS MODIFICAR.
; Llegir una tecla. llamando a la subrutina getchP2 que la guarda en el 
; el registro al.
; Segons la tecla llegida cridarem a les funcions corresponents.
;    ['i' (amunt),'j'(esquerra),'k' (avall) o 'l'(dreta)] 
; Desplazar los números y hacer las parejas según la dirección escogida.
; Según la tecla pulsada, rotar la matriz (m) llamando (rotateMatrixRP2_C),
; para poder hacer los desplazamientos de los números hacia la derechara
; llamando (shiftNumbersRP2_C), hacer las parejas hacia la derecha
; (addPairsRP2_C) que retorna los puntos que tenemos que sumar al 
; para actualizar el marcador (score).
; Volver a desplazar los números hacia la izquierda (shiftNumbersLP2_C)
; con las parejas hechas.
; Si se ha hecho algún desplazamiento o alguna pareja
; indicarlo poniendo (state='2').
; Después seguir rotando llamando (rotateMatrixRP2_C)
; hasta dejar la matriz en la posición inicial. 
; Para la tecla 'l' (derecha) no hay que hacer rotaciones, para el resto 
; se tiene que hacer 4 rotaciones.
;    'u'                Poner (state = '3') para recuperar el estado anterior.
;    '<ESC>' (ASCII 27) Poner (state = '0') para salir del juego.
; Si no es ninguna de estas teclas no hacer nada.
; Los cambios producidos por estas subrutinas no se tienen que mostraran en 
; pantalla.
;
; Variables globales utilizadas:
; m        : Dirección de la matriz m.
; mRotated : Dirección de la matriz mRotated.
; state    : Estado del juego.
;
; Parámetros de entrada: 
; rdi(edi) : Puntos acumulados en el marcador.
;
; Parámetros de salida : 
; rax(eax) : Puntos acumulados en el marcador actualizados.
;;;;;  
readKeyP2:
   push rbp
   mov  rbp, rsp

   push rbx
   push rdx
   push rsi
   push rdi
   push r8          ;s1
   push r9          ;s2
   push r10         ;p
   push r11         ;actualscore
   
   mov  r11d, edi
   mov  rdi, m      
   mov  rsi, mRotated
   
   call getchP2     ;getchP2_C();
      
   readKeyP2_i:
   cmp al, 'i'                ;i:(105) amunt
   jne  readKeyP2_j
      call rotateMatrixRP2    ;rotateMatrixRP2_C(m);
      
      call shiftNumbersRP2    ;s1 = shiftNumbersRP2_C(m);
      mov  r8d, eax
      call addPairsRP2        ;p  = addPairsRP2_C(m);
      mov  r10d, eax
      call shiftNumbersRP2    ;s2 = shiftNumbersRP2_C(m);
      mov  r9d, eax           
      
      call rotateMatrixRP2    ;rotateMatrixRP2_C(m);
      call rotateMatrixRP2    ;rotateMatrixRP2_C(m);
      call rotateMatrixRP2    ;rotateMatrixRP2_C(m);
      jmp  readKeyP2_moved
      
   readKeyP2_j:
   cmp al, 'j'                ;j:(106) esquerra
   jne  readKeyP2_k
      call rotateMatrixRP2    ;rotateMatrixRP2_C(m);
      call rotateMatrixRP2    ;rotateMatrixRP2_C(m);
      
      call shiftNumbersRP2    ;s1 = shiftNumbersRP2_C(m);
      mov  r8d, eax
      call addPairsRP2        ;actualScore = actualScore + p;
      mov  r10d, eax
      call shiftNumbersRP2    ;s2 = shiftNumbersRP2_C(m);
      mov  r9d, eax          
      
      call rotateMatrixRP2    ;rotateMatrixRP2_C(m);
      call rotateMatrixRP2    ;rotateMatrixRP2_C(m);
      jmp  readKeyP2_moved
   
   readKeyP2_k:
   cmp al, 'k'                ;k:(107) dreta
   jne  readKeyP2_l
      call rotateMatrixRP2    ;rotateMatrixRP2_C(m);
      call rotateMatrixRP2    ;rotateMatrixRP2_C(m);
      call rotateMatrixRP2    ;rotateMatrixRP2_C(m);
     
      call shiftNumbersRP2    ;s1 = shiftNumbersRP2_C(m);
      mov  r8d, eax
      call addPairsRP2        ;p  = addPairsRP2_C(m);
      mov  r10d, eax
      call shiftNumbersRP2    ;s2 = shiftNumbersRP2_C(m);
      mov  r9d, eax           
      
      call rotateMatrixRP2    ;rotateMatrixRP2_C(m);
      jmp  readKeyP2_moved
      
   readKeyP2_l:
   cmp al, 'l'                ;l:(108) avall
   jne  readKeyP2_u
      
      call shiftNumbersRP2    ;s1 = shiftNumbersRP2_C(m);
      mov  r8d, eax
      call addPairsRP2        ;p  = addPairsRP2_C(m);
      mov  r10d, eax
      call shiftNumbersRP2    ;s2 = shiftNumbersRP2_C(m);
      mov  r9d, eax           
      jmp readKeyP2_moved
      
   readKeyP2_u:
   cmp al, 'u'                ; Undo
   jne  readKeyP2_ESC
      mov BYTE[state], '3'    ;state = '3';
      jmp  readKeyP2_End
   
   readKeyP2_ESC:
   cmp al, 27                 ; Sortir del programa
   jne readKeyP2_End
      mov BYTE[state], '0'    ;state='0';
   jmp readKeyP2_End 

   readKeyP2_moved:
   add  r11d, r10d            ;actualScore = actualScore + p;
   cmp  r8d, 0                ;if ( (s1>0) || 
   jg  readKeyP2_status2
      cmp  r10d, 0            ;(p>0) || 
      jg  readKeyP2_status2
         cmp r9d, 0           ;(s2>0) ) 
         jg  readKeyP2_status2
            jmp readKeyP2_End
   readKeyP2_status2:         ;state = '2';
   mov  BYTE[state], '2'
      
   readKeyP2_End:
   mov eax, r11d              ;return actualScore;
   
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rbx
      
   mov rsp, rbp
   pop rbp
   ret


;;;;;  
; Verificar si se ha llegado a 2048 o si no se puede hacer algún movimiento.
; Si hay el número 2048 en la matriz (m), cambiar el estado a 4 
; (status='4') para indicar que se ha ganado (WIN!).
; Si no hemos ganado, mirar si se puede hacer algun movimento, 
; Si no se puede hacer ningún movimiento cambiar el estado a 5 
; (status='5') para indicar que se ha perdido (GAME OVER!!!).
; Recorrer la matriz (m) por filas de derecha a izquierda y de abajo hacia 
; arriba contando las casillas vacías y mirando si hay el número 2048.
; Para recorrer la matriz en ensamblador, en este caso, el índice va de la
; posición 30 (posición [3][3]) a la 0 (posición [0][0]) con decrementos de
; 2 porque los datos son de tipo short(WORD) 2 bytes.
; Per a acceder a una posición concreta de la matriz desde ensamblador 
; hay que tener en cuenta que el índice es:(index=(fila*DimMatrix+columna)*2),
; multiplicamos por 2 porque los datos son de tipo short(WORD) 2 bytes.  
; Si hay el número 2048 poner el estado en 4 (status='4') y acabar.
; Si no hay el número 2048 y no hay casillas vacías, mirar si se puede
; hacer alguna pareja en horizontal o en vertical. Par hacerlo 
; hay que copiar la matriz (m) sobre la matriz (mAux) llamando a la
; subrutina (copyMAtrixP2), hacer parejas en la matriz (mAux) para mirar
; si se pueden hacer parejas en horizontal llamando a la subrutina 
; (addPairsRP2) y guardar los puntos obtenidos, rotar la matriz (mAux) 
; llamando a la subrutina (rotateMatrixRP2) y copiar la matriz rotada 
; (mRotated) en la matriz (mAux) llamando a la subrutina (copyMatrixP2),
; volver a hacer parejas en la matriz (mAux) para mirar si se pueden 
; hacer parejas en vertical llamando a la subrutina (addPairsRP2)
; y acumular los puntos obtenidos con los puntos obtenidos antes. 
; Si los puntos acumulados son cero, quiere decir que no se pueden hacer
; parejas y se tiene que poner el estado del juego en 5 (status='5').
; No se puede modificar ni la matriz (m), ni la matriz (mUndo).
;
; Variables globales utilizadas:   
; m        : Dirección de la matriz que queremos verificar.
; mAux     : Dirección de la matriz donde copiaremos los datos para hacer verificaciones.
; mRotated : Dirección de la matriz donde se hace la rotación.
; state    : Estado del juego.
; 
; Parámetros de entrada: 
; Ninguno.
;
; Parámetros de salida : 
; Ninguno.
;;;;;  
checkEndP2:
   push rbp
   mov  rbp, rsp

   
   
   mov rsp, rbp
   pop rbp
   ret   


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Esta subrutina se da hecha. NO LA PODÉIS MODIFICAR.
; Muestrar un mensaje debajo del tablero según el valor de la variable 
; (state) llamando a la función printMessageP2_C().
; 
; Variables globales utilizadas:   
; Ninguno.
; 
; Parámetros de entrada: 
; Ninguno.
;
; Parámetros de salida : 
; Ninguno.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
printMessageP2:
   push rbp
   mov  rbp, rsp
   ;guardamos el estado de los registros del procesador porque
   ;las funciones de C no mantienen el estado de los registros.
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15
   push rbp

   ;Llamamos a la función printMessageP2_C() desde ensamblador.
   call printMessageP2_C
 
   pop rbp
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax
   
   mov rsp, rbp
   pop rbp
   ret 
   
   
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Esta subrutina se da hecha. NO LA PODÉIS MODIFICAR.
; Generar nueva ficha de forma aleatoria.
; Si hay como mínimo una casilla vacía en la matriz (m) generar una  
; fila y una columna de forma aleatoria hasta que sea una de les casillas 
; vacías. A continuación generar un número aleatorio para decidir si la 
; nueva ficha tiene que de ser un 2 (90% de los casos) o un 4 (10% de 
; los casos) llamando a la función insertTileP2_C().
; 
; Variables globales utilizadas:   
; Ninguno.
; 
; Parámetros de entrada: 
; Ninguno.
;
; Parámetros de salida : 
; Ninguno.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
insertTileP2:
   push rbp
   mov  rbp, rsp
   ;guardamos el estado de los registros del procesador porque
   ;las funciones de C no mantienen el estado de los registros.
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15

   ;Llamamos a la función insertTileP2_CC() desde ensamblador.
   call insertTileP2_C
 
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax
   
   mov rsp, rbp
   pop rbp
   ret
   
   
;;;;;
; Juego del 2048
; Función principal del juego
; Permite jugar al juego del 2048 llamando a todas las funcionalidades.
; Esta subrutina se da hecha. NO LA PODÉIS MODIFICAR.
;
; Pseudo-código:
; Inicializar estado del juego, (state='1').
; Borrar pantalla (llamar a la función clearScreen_C).
; Mostrar el tablero de juego (llamar a la función PrintBoardP2_C).
; Actualizar el contenido del Tablero de Juego y los puntos que se han hecho
; (llamar a la subrutina updateBoardP2).
; Mientras (state=='1') hacer
;   Copiar el estado del juego (m y score) a (mAux  y scoreAux) 
;   (llamando a la subrutina copyMAtrixP2) y copiando (score) en (scoreAux).
;   Leer una tecla (llamar a la subrutina readKeyP2). Según la tecla 
;     leída llamaremos a las funciones que correspondan.
;     - ['i','j','k' o 'l'] desplazar los números y hacer las parejas 
;                           según la dirección escogida.
;     - 'u'                 Recupera la jugada anterior (mUndo y 
;                           scoreUndo) sobre (m y score) (llamando a la 
;                           subrutna copyStatusP2) y copiando (scoreAux)
;                           en (score).
;     - '<ESC>'  (codi ASCII 27) poner (state = '0') para salir.   
;   Si hemos movido algún número al hacer los desplazamientos o al hacer las 
;   parejas (state=='2'), copiar el estado del juego que hemos guardado antes
;   (mAux y scoreAux) sobre (mUndo y scoreUndo) para poder hacer el Undo 
;   (recuperar estado anterior) copiando (mAux) sobre (mUndo) (llamando a 
;   la funció copyMatrixP2_C) y copiando (scoreAux) sobre (scoreUndo).
;   generar una nueva ficha (llamando a la subrutina insertTileP2_C) y 
;   pponer la variable state a '1' (state='1').
;   Si tenemos que recuperar el estado anterior (state='3'), copiar el estatdo
;   anterior del juego que tenemos en (mUndu y scoreUndu) sobre (m y score)
;   (llamando a  la funció copyMatrixP2_C) y copiando (scoreUndu) sobre 
;   (score)y poner la variable state a '1' (state='1').
;   Actualiza el contenido del Tablero de Juego y los puntos que se han
;   hecho (llamando a la subrutina updateBoardP2).
;   Verificar si se ha llegado a 2048 o si no se puede hacer ningún
;   movimiento (llamar a la subrutina CheckEndP2).
;   Mostrar un mensaje debajo del tablero según el valor de la variable 
;   (state). (llamar la subrutina printMessageP2).
; Fin mientras.
; Salir: 
; Se acaba el juego.
;
; Variables globales utilizadas:
; state    : Estado del juego.
; m        : Dirección de la matriz que queremos verificar.
; mAux     : Dirección de la matriz donde copiaremos los datos para hacer verificaciones.
; mUndo    : Dirección de la matriz donde copiaremos la jugada anterior del juego.
; score    : marcador de puntos.
; scoreAux : marcador de puntos auxiliar.
; scoreUndo: marcador de puntos anterior.
; state    : Estado del juego.
;
; Parámetros de entrada: 
; Ninguno
;
; Parámetros de salida : 
; Ninguno
;;;;;  
playP2:
   push rbp
   mov  rbp, rsp
   
   push rax
   push rbx
   push rdx
   push rsi
   push rdi
   
   call clearScreen_C
   call printBoardP2_C
   
   mov  BYTE[state], '1' ;state: 0: Salir, hemos pulsado la tecla 'ESC' para salir.
                         ;       1: Continuamos jugando.
                         ;       2: Continuamos jugando pero han habido cambios en la matriz.
                         ;       3: Deshacer el último movimento.
                         ;       4: Ganado, se ha llegado a la casilla 2048.
                         ;       5: Perdido, no se pueden hacer movimientos.
   
   mov  r10d, 290500          ;int score     = 290500;
   mov  r11d, 0               ;int scoreAux  = 0;
   mov  r12d, 1               ;int scoreUndu = 1;
   
   mov  edi, r10d
   call updateBoardP2

   playP2_Loop:                    ;while  {  
   cmp  BYTE[state], '1'           ;(state == 1)
   jne  playP2_End
      
      mov edi, mAux
      mov esi, m   
      call copyMatrixP2            ;copyMatrixP2_C(mAux,m);
      mov r11d, r10d               ;scoreAux = score
      mov edi,  r10d                        
      call readKeyP2               ;readKeyP2_C();
      mov r10d, eax
      cmp BYTE[state], '2'         ;(state == '2') //Se tiene que hacer algun movimento,
      jne playP2_Next 
         mov edi, mUndo
         mov esi, mAux
         call copyMatrixP2         ;copyMatrixP2_C(mUndo,mAux);guardamos estado
         mov  r12d, r11d           ;scoreUndo = scoreAux
         call insertTileP2         ;insertTileP2_C(); //añadir ficha (2 o 4)
         mov BYTE[state],'1'       ;state = '1';
         jmp playP2_Next
      cmp BYTE[state], '3'         ;(state == '3') //Deshacer último movimento
      jne playP2_Next  
         mov  edi, m 
		 mov  esi, mUndo
		 call copyMatrixP2         ;copyMatrixP2_C(m,mUndo);
		 mov  r10d, r12d           ;score = scoreUndo;
		 mov  BYTE[state], '1'     ;state = '1';
      playP2_Next:
      mov  edi, r10d
      call updateBoardP2           ;updateBoardP2_C(score);
      call checkEndP2              ;checkEndP2_C();  
      call printMessageP2          ;printMessageP2_C(); 
      cmp BYTE[state], '3'         ;(state == '3') //Deshacer último movimento
      jne playP2_Loop
         mov edi, m
		 mov  esi, mUndo
		 call copyMatrixP2        ;copyMatrixP2_C(m,mUndo);
		 mov  r10d, r12d          ;score = scoreUndo;
		 mov  BYTE[state], '1'    ;state = '1';
		 mov  edi, r10d
		 call updateBoardP2
   jmp playP2_Loop
   
   playP2_End:
           
   
   pop rdi
   pop rsi
   pop rdx
   pop rbx
   pop rax  
   
   mov rsp, rbp
   pop rbp
   ret
