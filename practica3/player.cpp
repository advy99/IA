#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "player.h"
#include "environment.h"

#include <cmath>
#include <algorithm>

using namespace std;

const double masinf=9999999999.0, menosinf=-9999999999.0;


// Constructor
Player::Player(int jug){
    jugador_=jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment & env){
    actual_=env;
}

double Puntuacion(int jugador, const Environment &estado){
    double suma=0;

    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++){
         if (estado.See_Casilla(i,j)==jugador){
            if (j<3)
               suma += j;
            else
               suma += (6-j);
         }
      }

    return suma;
}


// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador){
    int ganador = estado.RevisarTablero();

    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
          return Puntuacion(jugador,estado);
}

// ------------------- Los tres metodos anteriores no se pueden modificar


double ValoracionHorizontal(const int jugador, const Environment & estado){
    double h;

    // contaremos cuantas casillas del jugador y del oponente hay
    int casillas_jugador = 0, casillas_oponente = 0;

    // comprobaremos si la casilla justo anterior es del mismo jugador
    bool seguida_jugador = false;
    bool seguida_oponente = false;

    // contamos cuantas casillas seguidas tiene cada uno
    int n_jugador = 1;
    int n_oponente = 1;

    // recorremos el tablero
    for (int i = 0; i < 7; i++){
       // en principio, en esa fila no hay fichas
        casillas_oponente = 0;
        casillas_jugador = 0;

        // recorremos la fila
        for (int j = 0; j < 7; j++){
            // si en la i,j hay una ficha del jugador (normal o bomba)
            if (estado.See_Casilla(i, j) == jugador || estado.See_Casilla(i, j) == jugador+3){
               // el oponente no tiene fichas seguidas
                seguida_oponente = false;
                n_oponente = 1;

                // si el jugador es
                if (seguida_jugador){
                    n_jugador++;
                    casillas_jugador += 4 * n_jugador;
                } else {
                    casillas_jugador += 4;
                }

                seguida_jugador = true;

            } else if (estado.See_Casilla(i, j) != 0){
                seguida_jugador = false;
                n_jugador = 1;

                if (seguida_oponente){
                    n_oponente++;
                    casillas_oponente += 4 * n_oponente;
                } else{
                    casillas_oponente += 4;
                }

                seguida_oponente = true;

            } else {
                n_oponente = 1;
                n_jugador = 1;
                seguida_jugador = false;
                seguida_oponente = false;
            }


        }

        h -= casillas_jugador;
        h += casillas_oponente;



    }

    return h;
}

double ValoracionVertical(const int jugador, const Environment & estado){
    double h;

    int casillas_jugador = 0, casillas_oponente = 0;
    bool seguida_jugador = false;
    bool seguida_oponente = false;
    int n_jugador = 1;
    int n_oponente = 1;

    for (int i = 0; i < 7; i++){
        casillas_oponente = 0;
        casillas_jugador = 0;


        for (int j = 0; j < 7; j++){
            if (estado.See_Casilla(j, i) == jugador || estado.See_Casilla(j, i) == jugador+3){
                seguida_oponente = false;
                n_oponente = 1;

                if (seguida_jugador){
                    n_jugador++;
                    casillas_jugador += 4 * n_jugador;
                } else {
                    casillas_jugador += 4;
                }

                seguida_jugador = true;

            } else if (estado.See_Casilla(j, i) != 0){
                seguida_jugador = false;
                n_jugador = 1;

                if (seguida_oponente){
                    n_oponente++;
                    casillas_oponente += 4 * n_oponente;
                } else{
                    casillas_oponente += 4;
                }

                seguida_oponente = true;

            } else {
                n_oponente = 1;
                n_jugador = 1;
                seguida_jugador = false;
                seguida_oponente = false;
            }


        }

        h -= casillas_jugador;
        h += casillas_oponente;



    }

    return h;
}


double ValoracionDiagonal(const int jugador, const Environment & estado){
    double h;

    int casillas_jugador = 0, casillas_oponente = 0;
    bool seguida_jugador = false;
    bool seguida_oponente = false;
    int n_jugador = 1;
    int n_oponente = 1;


    int casillas_jugador_d2 = 0, casillas_oponente_d2 = 0;
    bool seguida_jugador_d2 = false;
    bool seguida_oponente_d2 = false;
    int n_jugador_d2 = 1;
    int n_oponente_d2 = 1;


    casillas_oponente = 0;
    casillas_jugador = 0;

    casillas_oponente_d2 = 0;
    casillas_jugador_d2 = 0;


    for (int i = 0; i < 7; i++){


        if (estado.See_Casilla(i, i) == jugador || estado.See_Casilla(i, i) == jugador+3){
            seguida_oponente = false;
            n_oponente = 1;

            if (seguida_jugador){
                n_jugador++;
                casillas_jugador += 4 * n_jugador;
            } else {
                casillas_jugador += 4;
            }

            seguida_jugador = true;

        } else if (estado.See_Casilla(i, i) != 0){
            seguida_jugador = false;
            n_jugador = 1;

            if (seguida_oponente){
                n_oponente++;
                casillas_oponente += 4 * n_oponente;
            } else{
                casillas_oponente += 4;
            }

            seguida_oponente = true;

        } else {
            n_oponente = 1;
            n_jugador = 1;
            seguida_jugador = false;
            seguida_oponente = false;
        }





        if (estado.See_Casilla(6-i, 6-i) == jugador || estado.See_Casilla(6-i, 6-i) == jugador+3){
            seguida_oponente_d2 = false;
            n_oponente_d2 = 1;

            if (seguida_jugador_d2){
                n_jugador_d2++;
                casillas_jugador_d2 += 4 * n_jugador_d2;
            } else {
                casillas_jugador_d2 += 4;
            }

            seguida_jugador_d2 = true;

        } else if (estado.See_Casilla(6-i, 6-i) != 0){
            seguida_jugador_d2 = false;
            n_jugador_d2 = 1;

            if (seguida_oponente_d2){
                n_oponente_d2++;
                casillas_oponente_d2 += 4 * n_oponente_d2;
            } else{
                casillas_oponente_d2 += 4;
            }

            seguida_oponente_d2 = true;

        } else {
            n_oponente_d2 = 1;
            n_jugador_d2 = 1;
            seguida_jugador_d2 = false;
            seguida_oponente_d2 = false;
        }

        h -= casillas_jugador;
        h += casillas_oponente;


        h -= casillas_jugador_d2;
        h += casillas_oponente_d2;

    }


    for (int i = 1; i < 4; i++){
        casillas_oponente = 0;
        casillas_jugador = 0;

        casillas_oponente_d2 = 0;
        casillas_jugador_d2 = 0;


        for (int j = 0; j < 7 - i; j++){
            if (estado.See_Casilla(i + j, j) == jugador || estado.See_Casilla(i + j, j) == jugador+3){
                seguida_oponente = false;
                n_oponente = 1;

                if (seguida_jugador){
                    n_jugador++;
                    casillas_jugador += 4 * n_jugador;
                } else {
                    casillas_jugador += 4;
                }

                seguida_jugador = true;

            } else if (estado.See_Casilla(i + j, j) != 0){
                seguida_jugador = false;
                n_jugador = 1;

                if (seguida_oponente){
                    n_oponente++;
                    casillas_oponente += 4 * n_oponente;
                } else{
                    casillas_oponente += 4;
                }

                seguida_oponente = true;

            } else {
                n_oponente = 1;
                n_jugador = 1;
                seguida_jugador = false;
                seguida_oponente = false;
            }





            if (estado.See_Casilla(j, i + j) == jugador || estado.See_Casilla(j, i + j) == jugador+3){
                seguida_oponente_d2 = false;
                n_oponente_d2 = 1;

                if (seguida_jugador_d2){
                    n_jugador_d2++;
                    casillas_jugador_d2 += 4 * n_jugador_d2;
                } else {
                    casillas_jugador_d2 += 4;
                }

                seguida_jugador_d2 = true;

            } else if (estado.See_Casilla(j, i + j) != 0){
                seguida_jugador_d2 = false;
                n_jugador_d2 = 1;

                if (seguida_oponente_d2){
                    n_oponente_d2++;
                    casillas_oponente_d2 += 4 * n_oponente_d2;
                } else{
                    casillas_oponente_d2 += 4;
                }

                seguida_oponente_d2 = true;

            } else {
                n_oponente_d2 = 1;
                n_jugador_d2 = 1;
                seguida_jugador_d2 = false;
                seguida_oponente_d2 = false;
            }


        }

        h -= casillas_jugador;
        h += casillas_oponente;


        h -= casillas_jugador_d2;
        h += casillas_oponente_d2;

    }





    for (int i = 1; i < 4; i++){
        casillas_oponente = 0;
        casillas_jugador = 0;

        casillas_oponente_d2 = 0;
        casillas_jugador_d2 = 0;


        for (int j = 0; j < 7 - i; j++){
            if (estado.See_Casilla( 6 - i - j, 6 - j) == jugador || estado.See_Casilla( 6 - i - j, 6 - j) == jugador+3){
                seguida_oponente = false;
                n_oponente = 1;

                if (seguida_jugador){
                    n_jugador++;
                    casillas_jugador += 4 * n_jugador;
                } else {
                    casillas_jugador += 4;
                }

                seguida_jugador = true;

            } else if (estado.See_Casilla(6 - i - j, 6 - j) != 0){
                seguida_jugador = false;
                n_jugador = 1;

                if (seguida_oponente){
                    n_oponente++;
                    casillas_oponente += 4 * n_oponente;
                } else{
                    casillas_oponente += 4;
                }

                seguida_oponente = true;

            } else {
                n_oponente = 1;
                n_jugador = 1;
                seguida_jugador = false;
                seguida_oponente = false;
            }





            if (estado.See_Casilla( 6 - j, 6 - i - j) == jugador || estado.See_Casilla(6 - j, 6 - i - j) == jugador+3){
                seguida_oponente_d2 = false;
                n_oponente_d2 = 1;

                if (seguida_jugador_d2){
                    n_jugador_d2++;
                    casillas_jugador_d2 += 4 * n_jugador_d2;
                } else {
                    casillas_jugador_d2 += 4;
                }

                seguida_jugador_d2 = true;

            } else if (estado.See_Casilla(6 - j, 6 - i - j) != 0){
                seguida_jugador_d2 = false;
                n_jugador_d2 = 1;

                if (seguida_oponente_d2){
                    n_oponente_d2++;
                    casillas_oponente_d2 += 4 * n_oponente_d2;
                } else{
                    casillas_oponente_d2 += 4;
                }

                seguida_oponente_d2 = true;

            } else {
                n_oponente_d2 = 1;
                n_jugador_d2 = 1;
                seguida_jugador_d2 = false;
                seguida_oponente_d2 = false;
            }


        }

        h -= casillas_jugador;
        h += casillas_oponente;


        h -= casillas_jugador_d2;
        h += casillas_oponente_d2;

    }




    return h;
}




double F_Valoracion(const int jugador, const Environment & estado){
    double h = 0;

    // valoramos el estado comprobando las piezas en vertical
    h += ValoracionVertical(jugador, estado);
    // valoramos el estado comprobando las piezas en horizontal
    h += ValoracionHorizontal(jugador, estado);
    // valoramos el estado comprobando las piezas en diagonal
    h += ValoracionDiagonal(jugador, estado);

    return h;

}


// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
double Valoracion(const Environment &estado, int jugador){

    // comprobamos si en el estado dado gana algun jugador
    int ganador = estado.RevisarTablero();

    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
        return F_Valoracion(jugador,estado);// valoramos el estado para ver como de bueno es


}





// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
void JuegoAleatorio(bool aplicables[], int opciones[], int &j){
    j=0;
    for (int i=0; i<8; i++){
        if (aplicables[i]){
           opciones[j]=i;
           j++;
        }
    }
}






// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think(){
    const int PROFUNDIDAD_MINIMAX = 6;  // Umbral maximo de profundidad para el metodo MiniMax
    const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

    Environment::ActionType accion; // acci�n que se va a devolver
    bool aplicables[8]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                        // aplicables[0]==true si PUT1 es aplicable
                        // aplicables[1]==true si PUT2 es aplicable
                        // aplicables[2]==true si PUT3 es aplicable
                        // aplicables[3]==true si PUT4 es aplicable
                        // aplicables[4]==true si PUT5 es aplicable
                        // aplicables[5]==true si PUT6 es aplicable
                        // aplicables[6]==true si PUT7 es aplicable
                        // aplicables[7]==true si BOOM es aplicable



    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha, beta; // Cotas de la poda AlfaBeta

    int n_act; //Acciones posibles en el estado actual


    n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
    int opciones[10];

    // Muestra por la consola las acciones aplicable para el jugador activo
    //actual_.PintaTablero();
    cout << " Acciones aplicables ";
    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
    for (int t=0; t<8; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t)  );
    cout << endl;


    //--------------------- COMENTAR Desde aqui
    /*
    cout << "\n\t";
    int n_opciones=0;
    JuegoAleatorio(aplicables, opciones, n_opciones);

    if (n_act==0){
      (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
      cout << " No puede realizar ninguna accion!!!\n";
      //accion = Environment::actIDLE;
    }
    else if (n_act==1){
           (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
            cout << " Solo se puede realizar la accion "
                 << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[0])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[0]);

         }
         else { // Hay que elegir entre varias posibles acciones
            int aleatorio = rand()%n_opciones;
            cout << " -> " << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[aleatorio])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[aleatorio]);
         }

    //--------------------- COMENTAR Hasta aqui
    */



    //--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------

    // establecemos alpha y beta a menos infinito e infinito respectivamente
    alpha = menosinf;//-INFINITY;
    beta = masinf;//INFINITY;

    // si no podemos jugar, avisamos de empate
    if (n_act==0){
      (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
      cout << " No puede realizar ninguna accion!!!\n";
      //accion = Environment::actIDLE;
      valor = 0;
    }
    else{

       // si podemos jugar, generamos los 8 primeros hijos como jugador
        Environment acciones[8];
        int n_pos = actual_.GenerateAllMoves(acciones);

        // establecemos el valor de la accion a menos infinito y declaramos
        // una variable auxiliar v que almacenara el valor de cada hijo del
        // estado actual
        double v;
        valor = -INFINITY;



        // para todos los hijos
        for (int i = 0; i < n_pos; i++){

            // almacenamos el valor de realizar la poda, como ya hemos generado
            // el nivel del jugador, ahora le toca al oponente jugar, como vemos
            // en el tercer parametro
            v = Poda_AlfaBeta(acciones[i], jugador_, 1, PROFUNDIDAD_ALFABETA-1, alpha, beta);
            cout << "Accion: " << acciones[i].Last_Action(jugador_) << " valor:  " << v << endl;

            // si el resultado de acciones[i] lleva a una mejor situacion
            // cambiamos el valor y la accion a realizar
            if ( v > valor || (v == 99999999.0 && acciones[i].Last_Action(jugador_) == 7) ){
                accion = static_cast< Environment::ActionType > (acciones[i].Last_Action(jugador_) );
                valor = v;
            }

        }

    }


    //valor = Poda_AlfaBeta(actual_, jugador_, 0, PROFUNDIDAD_ALFABETA, accion, alpha, beta);

    // Opcion: Poda AlfaBeta
    // NOTA: La parametrizacion es solo orientativa
    // valor = Poda_AlfaBeta(actual_, jugador_, 0, PROFUNDIDAD_ALFABETA, accion, alpha, beta);
    cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;

    return accion;
}



double Player::Poda_AlfaBeta(const Environment & actual_, int jugador_, const bool oponente,
                  const int PROFUNDIDAD_ALFABETA,
                  double alfa, double beta){



   // si llegamos a un nodo terminal, o al limite de profundidad
   // devolvemos el valor de ese estado
   if (PROFUNDIDAD_ALFABETA == 0 || actual_.JuegoTerminado()){
      return Valoracion(actual_, jugador_);
   }
   // si no, pasamos a generar sus hijos

   Environment acciones[8];

   int n_act = actual_.GenerateAllMoves(acciones);


   // si le toca a nuestro jugador, estamos en un nodo MAX
   if (!oponente){

      // para todos los hijos, modificamos alfa si es mayor del que ya tenemos
      for(int i = 0; i < n_act; i++){

         // como en este nodo juega el jugador, en el siguiente juega el contrario
         // por eso pasamos !oponente, ya que en este oponente = false
        alfa = max(alfa, Poda_AlfaBeta(acciones[i], jugador_, !oponente, PROFUNDIDAD_ALFABETA-1, alfa, beta ));

        // si en algun momento alfa supera a beta, quiere decir que ya hemos
        // encontrado el mejor valor para estos hijos, y podemos parar de
        //explorar
        if (alfa >= beta){
           return beta;
        }

      }

      //como estamos en un nodo MAX, devolvemos el valor de alfa;
      return alfa;

   } else{
      // le toca al oponente, estamos en un nodo MIN

      // para todos los hijos, modificamos beta si es menor del que ya tenemos
      for(int i = 0; i < n_act; i++){

         // como en este nodo juega el oponente, en el siguiente juega el jugador
         // por eso pasamos !oponente, ya que en este oponente = true
        beta = min(beta, Poda_AlfaBeta(acciones[i], jugador_, !oponente, PROFUNDIDAD_ALFABETA-1, alfa, beta ));

        // si en algun momento alfa supera a beta, quiere decir que ya hemos
        // encontrado el mejor valor para estos hijos, y podemos parar de
        //explorar
        if (alfa >= beta){
           return alfa;
        }

      }

      // como estamos en un nodo MIN, devolvemos el valor de beta
      return beta;

   }



}
