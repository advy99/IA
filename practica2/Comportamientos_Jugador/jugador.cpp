
#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>
#include <map>
#include <vector>

using namespace std;

bool EsObstaculo(unsigned char casilla){
	if (casilla=='P' or casilla=='M' or casilla =='D')
		return true;
	else
	  return false;
}


// Este es el método principal que debe contener los 4 Comportamientos_Jugador
// que se piden en la práctica. Tiene como entrada la información de los
// sensores y devuelve la acción a realizar.
Action ComportamientoJugador::think(Sensores sensores) {



	Action sigAccion;

	//Capturar valores de filas y columnas


	if (sensores.nivel != 4){
		//nivel 1

		if (sensores.mensajeF != -1){
			fil = sensores.mensajeF;
			col = sensores.mensajeC;
			ultimaAccion = actIDLE;
		}


		//Actualizar el efecto de la ultima accion
		switch (ultimaAccion){
			case actTURN_R: brujula = (brujula+1)%4; break;
			case actTURN_L: brujula = (brujula+3)%4; break;
			case actFORWARD:
				switch (brujula){
					case 0: fil--; break;
					case 1: col++; break;
					case 2: fil++; break;
					case 3: col--; break;
				}
				break;
		}

		//Comprobamos si ha cambiado la posición del objetivo
		if (sensores.destinoF != destino.fila or sensores.destinoC != destino.columna){
			destino.fila = sensores.destinoF;
			destino.columna = sensores.destinoC;
			hayPlan = false;
		}

		// Si no hay plan, establecemos la fila,  columna y orientacion del
		//  estado actual a la fila y columna obtenida por los sensores
		//  y buscamos un plan
		if (!hayPlan){
			actual.fila = fil;
			actual.columna = col;
			actual.orientacion = brujula;
			hayPlan =  pathFinding(sensores.nivel, actual, destino, plan);
		}


		// Si tenemos un plan, y es viable (la longitud es mayor que 0):
		if (hayPlan and plan.size()>0){

			sigAccion = actIDLE;

			// Comprobamos si la casilla de delante es un obstaculo, y si lo
			// es, establecemos hayPlan a falso, y recalculamos el plan
			if (EsObstaculo(sensores.terreno[2]) and plan.front() == actFORWARD ){
				hayPlan = false;
			} else if (sensores.superficie[2] != 'a' || plan.front() != actFORWARD){
				//en caso de que no sea un obstaculo o la accion sea un giro
				// si no hay un aldeano delante, o si es un giro realizamos el plan
				sigAccion = plan.front();
				plan.erase(plan.begin());
			}
			// en caso de que tengamos un aldeano delante y la accion del plan sea
			// avanzar, simplemente esperamos a que el aldeano se mueva

			//Esta situacion en principio no puede darse en el nivel 1 ya que
			// no hay aldeanos, aun asi, he decidido mantener esta comprobación
			// de cara a algún cambio imprevisto

		}
		else{
			//en caso de no encontrar plan, realizamos un comportamiento reactivo,
			// comprobamos si lo que tenemos delante es un problema a
			// la hora de moverse hacia delante
			if (sensores.terreno[2] == 'P' || sensores.terreno[2] == 'M' ||
				 sensores.terreno[2] == 'D' || sensores.superficie[2] == 'a'){
			   //si lo es, giramos
				sigAccion = actTURN_R;
			}
			else{
				//si no, avanzamos
				sigAccion = actFORWARD;
			}


		}



	} else{
		//nivel 2

		// si estamos sobre un PK guardamos los datos de fila y columna
		if (sensores.mensajeF != -1){
			fil = sensores.mensajeF;
			col = sensores.mensajeC;
			ultimaAccion = actIDLE;
			estoyBienSituado = true;
		}


		// si estoy bien situado, es decir, he encontrado un PK
		if (estoyBienSituado){

			//actualizo el estado conforme a la ultima accion
			switch (ultimaAccion){
				case actTURN_R: brujula = (brujula+1)%4; break;
				case actTURN_L: brujula = (brujula+3)%4; break;
				case actFORWARD:
					switch (brujula){
						case 0: fil--; break;
						case 1: col++; break;
						case 2: fil++; break;
						case 3: col--; break;
					}
					break;
			}

			actual.orientacion = brujula;


			// pintamos la parte del mapa que el personaje es capaz de observar
			// usando sensores.terreno
			int k = 0;

			for(unsigned int i = 0; i < 4; i++){
				for(unsigned int j = 0; j <= i*2; j++ ){
					switch (actual.orientacion) {
						//si estamos mirando al norte
						case 0:
								if (mapaResultado[fil - i][col - i + j] == '?')
									mapaResultado[fil - i][col - i + j] = sensores.terreno[k];
								k++;
							break;

						//si estamos mirando al este
						case 1:

							if (mapaResultado[fil - i + j][col + i] == '?')
								mapaResultado[fil - i + j][col + i] = sensores.terreno[k];
							k++;

							break;

						//si estamos mirando al sur
						case 2:

							if (mapaResultado[fil + i][col + i - j] == '?')
								mapaResultado[fil + i][col + i - j] = sensores.terreno[k];
							k++;

							break;

						//si estamos mirando al oeste
						case 3:

							if (mapaResultado[fil + i - j][col - i] == '?')
								mapaResultado[fil + i - j][col - i] = sensores.terreno[k];
							k++;

							break;
					}
				}
			}


			// comprobamos si ha cambiado el objetivo y en caso de que se mueva
			// recolocamos el destino
			if (sensores.destinoF != destino.fila or sensores.destinoC != destino.columna){
				destino.fila = sensores.destinoF;
				destino.columna = sensores.destinoC;
				hayPlan = false;
			}

			// si no hay plan, actualizamos el estado actual y buscamos el plan
			if (!hayPlan){
				actual.fila = fil;
				actual.columna = col;
				actual.orientacion = brujula;
				hayPlan =  pathFinding(sensores.nivel, actual, destino, plan);
			}

			// si encuentra el plan, lo ejecutamos
			if (hayPlan and plan.size()>0){

				sigAccion = actIDLE;

				// si el plan pasa por un obstaculo, tenemos que recalcular el plan
				// luego no hacemos nada, y establecemos hayPlan a falso
				if (EsObstaculo(sensores.terreno[2]) and plan.front() == actFORWARD ){
					hayPlan = false;
				} else if (sensores.superficie[2] != 'a' || plan.front() != actFORWARD ){
					sigAccion = plan.front();
					plan.erase(plan.begin());
				}

			}

			// realizamos una accion, queda una menos para recalcular el plan
			recalcular--;

			// cuando hemos ejecutado 4 acciones, recalculamos el plan
			if (recalcular == 0){
				hayPlan = false;

				// si tenemos mas de la mita de tiempo, recalculamos cada 4 acciones
				if (sensores.tiempo < 150){
					recalcular = 4;
				} else if (sensores.tiempo < 200){
					// si nos queda entre 150 y 200 segundos recalculamos
					// cada 15 acciones
					recalcular = 15;
				} else if (sensores.tiempo < 250) {
					// si nos queda entre 200 y 250 segundos recalculamos cada 40 acciones
					recalcular = 40;
				} else {
					// si hemos consumido 250 segundos recalculamos cada 80 acciones
					recalcular = 80;
				}
			}

		} else{
			// si no estamos bien situados

			// actualizamos unas variables "imaginarias" que contienen la
			// una posicion relativa
			switch (ultimaAccion){
				case actTURN_R: brujula = (brujula+1)%4; break;
				case actTURN_L: brujula = (brujula+3)%4; break;
				case actFORWARD:
					switch (brujula){
						case 0: fil_img--; break;
						case 1: col_img++; break;
						case 2: fil_img++; break;
						case 3: col_img--; break;
					}
					break;
			}

			actual.orientacion = brujula;

			// cada vez que pasamos un paso en una casilla, actualizamos su
			// valor en 1
			mapaImg[fil_img][col_img]++;

			//si no hayPlan, lo usaremos para saber si tenemos un PK en el campo
			// de vision
			if (!hayPlan){

				// buscamos si hay una casilla PK en el campo de vision
				int posK = -1;

				for (auto it = sensores.terreno.begin(); it != sensores.terreno.end() && posK == -1; ++it){
					if ((*it) == 'K')
						posK = distance(sensores.terreno.begin(), it);
				}

				//si no la encontramos, seguimos un movimiento reactivo basado en
				// un pulgarcito, es decir, almacenar sobre que casilla avanzamos
				// y evitar pasar de nuevo por ahi
				if (posK == -1){

					int c_izq, c_der, c_del;

					//si es un obstaculo, el valor de pasar por ahi lo ponemos
					// muy alto para que evite seleccionar ese camino
					if (EsObstaculo(sensores.terreno[2])){
						switch (brujula) {
							case 0: mapaImg[fil_img-1][col_img] = 999999; break;
							case 1: mapaImg[fil_img][col_img+1] = 999999; break;
							case 2: mapaImg[fil_img+1][col_img] = 999999; break;
							case 3: mapaImg[fil_img][col_img-1] = 999999; break;
						}
					}

					// seleccionamos los tres siguientes candidatos, ir a la
					// izquierda, a la derecha, o avanzar
					switch (brujula) {
						case 0:
							c_izq = mapaImg[fil_img][col_img-1];
							c_der = mapaImg[fil_img][col_img+1];
							c_del = mapaImg[fil_img-1][col_img];
							break;
						case 1:
							c_izq = mapaImg[fil_img-1][col_img];
							c_der = mapaImg[fil_img+1][col_img];
							c_del = mapaImg[fil_img][col_img+1];
							break;
						case 2:
							c_izq = mapaImg[fil_img][col_img+1];
							c_der = mapaImg[fil_img][col_img-1];
							c_del = mapaImg[fil_img+1][col_img];
							break;
						case 3:
							c_izq = mapaImg[fil_img+1][col_img];
							c_der = mapaImg[fil_img-1][col_img];
							c_del = mapaImg[fil_img][col_img-1];
							break;

					}

					//por defecto avanzamos
					sigAccion = actFORWARD;


					if (c_der < c_del){

						if (c_izq < c_der) {
							// si el de menor valor es ir a la izquierda
							sigAccion = actTURN_L;
						}
						else {
							// si el de menor valor es ir a la derecha
							sigAccion = actTURN_R;
						}

					} else if (c_izq < c_del) {
						if (c_izq < c_der) {
							// si el de menor valor es ir a la izquierda
							sigAccion = actTURN_L;
						}
						else {
							// si el de menor valor es ir a la derecha
							sigAccion = actTURN_R;
						}
					}
					// en otro caso, el mejor es avanzar, luego no tocamos nada


				}

				else{
					// si encontramos un PK en el campo de vision

					int av = 0;
					int lateral = 0;

					// comprobamos cuantas casillas hay que avanzar
					if ( posK > 0 and posK < 4 ){
						av = 1;
						// si tenemos que avanzar 1, miramos si tenemos qe ir a la
						// derecha o a la izquierda
						if (posK > 2 )
							lateral = 1;
						else if (posK < 2)
							lateral = -1;

					} else if (posK < 9){
						// si tenemos que avanzar 2
						av = 2;
						// miramos si tenemos que ir a la izquierda o a la derecha
						// y comprobamos si lateralmente tenemos que avanzar 1 o 2
						// casillas
						if (posK > 6 )
							if (posK % 2 == 0) lateral = 2; else lateral = 1;
						else if (posK < 6 )
							if (posK % 2 == 0) lateral = -2; else lateral = -1;

					} else {
						// si tenemos que avanzar 3 casillas
						av = 3;
						// miramos si tenemos que ir a la izquierda o a la derecha
						// y comprobamos si lateralmente tenemos que avanzar 1 o 2
						// casillas
						if (posK > 12 )
							if (posK % 2 == 0) lateral = 2; else lateral = 1;
						else if (posK < 12 )
							if (posK % 2 == 0) lateral = -2; else lateral = -1;

						// si son las ultimas casillas del campo de visión tenemos
						// que avanzar un poco mas
						if (posK == 9) lateral -= 2;
						if (posK == 15) lateral += 2;

					}

					// limpiamos el plan
					plan.clear();
					//introducimos los avances
					for (int i = 0; i < av; i++){
						plan.push_back(actFORWARD);
					}

					// dependiendo de si lateral es positivo o negativo
					// tenemos que girar a la derecha o a la izquierda
					if (lateral > 0){
						plan.push_back(actTURN_R);
					} else{
						plan.push_back(actTURN_L);
					}

					// introducimos los avances que realizamos una vez giramos
					for (int i = 0; i < abs(lateral); i++){
						plan.push_back(actFORWARD);
					}

					//pintamos el plan
					PintaPlan(plan);

					// ejecutamos el plan
					hayPlan = true;


				}

			}

			// si tenemos plan, lo ejecutamos
			if (hayPlan and plan.size()>0){

				sigAccion = actIDLE;

				// si el plan pasa por un obstaculo, giramos a la derecha y recalculamos
				if (EsObstaculo(sensores.terreno[2]) and plan.front() == actFORWARD ){
					hayPlan = false;
					sigAccion = actTURN_R;
				} else if (sensores.superficie[2] != 'a' || plan.front() != actFORWARD ){
					//si no pasa por obstaculo, y delante no tenemos un aldeano
					// ejecutamos el plan
					sigAccion = plan.front();
					plan.erase(plan.begin());
				}

			}

			// si encontramos el PK actualizamos fila y columna y avisamos
			// de que estamos bien situads
			if (sensores.terreno[0] == 'K'){
				fil = sensores.mensajeF;
				col = sensores.mensajeC;
				ultimaAccion = actIDLE;
				sigAccion = actIDLE;
				estoyBienSituado = true;

			}

		}


	}


	//Recordar ultima accion
	ultimaAccion = sigAccion;
  	return sigAccion;
}


// Llama al algoritmo de busqueda que se usará en cada comportamiento del agente
// Level representa el comportamiento en el que fue iniciado el agente.
bool ComportamientoJugador::pathFinding (int level, const estado &origen, const estado &destino, list<Action> &plan){
	switch (level){
		case 1: cout << "Busqueda en profundad\n";
			      return pathFinding_Profundidad(origen,destino,plan);
						break;
		case 2: cout << "Busqueda en Anchura\n";
			      return pathFinding_Anchura(origen, destino, plan);
						break;
		case 3: cout << "Busqueda Costo Uniforme\n";
					return pathFinding_CostoUniforme(origen, destino, plan);
						break;
		case 4: cout << "Busqueda para el reto\n";
					return pathFinding_A_Estrella(origen, destino, plan);
						// Incluir aqui la llamada al algoritmo de búsqueda usado en el nivel 2
						break;
	}
	cout << "Comportamiento sin implementar\n";
	return false;
}


//---------------------- Implementación de la busqueda en profundidad ---------------------------

// Dado el código en carácter de una casilla del mapa dice si se puede
// pasar por ella sin riegos de morir o chocar.


// Comprueba si la casilla que hay delante es un obstaculo. Si es un
// obstaculo devuelve true. Si no es un obstaculo, devuelve false y
// modifica st con la posición de la casilla del avance.
bool ComportamientoJugador::HayObstaculoDelante(estado &st){
	int fil=st.fila, col=st.columna;

  // calculo cual es la casilla de delante del agente
	switch (st.orientacion) {
		case 0: fil--; break;
		case 1: col++; break;
		case 2: fil++; break;
		case 3: col--; break;
	}

	// Compruebo que no me salgo fuera del rango del mapa
	if (fil<0 or fil>=mapaResultado.size()) return true;
	if (col<0 or col>=mapaResultado[0].size()) return true;

	// Miro si en esa casilla hay un obstaculo infranqueable
	if (!EsObstaculo(mapaResultado[fil][col])){
		// No hay obstaculo, actualizo el parámetro st poniendo la casilla de delante.
    st.fila = fil;
		st.columna = col;
		return false;
	}
	else{
	  return true;
	}
}



//estructura nodo, trabajaremos con ella para consular los estados, la
// secuencia de acciones para llegar a ese estado, el coste de llegar a ese
// estado, la heuristica de ese nodo, y el coste total (coste + heuristica)
struct nodo{
	estado st;
	list<Action> secuencia;
	int coste;
	int heuristica;
	int coste_total;
};

struct ComparaEstados{
	bool operator()(const estado &a, const estado &n) const{
		if ((a.fila > n.fila) or (a.fila == n.fila and a.columna > n.columna) or
	      (a.fila == n.fila and a.columna == n.columna and a.orientacion > n.orientacion))
			return true;
		else
			return false;
	}
};


// Implementación de la búsqueda en profundidad.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado,ComparaEstados> generados; // Lista de Cerrados
	stack<nodo> pila;											// Lista de Abiertos

  nodo current;
	current.st = origen;
	current.secuencia.clear();

	pila.push(current);

  while (!pila.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		pila.pop();
		generados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (generados.find(hijoTurnR.st) == generados.end()){
			hijoTurnR.secuencia.push_back(actTURN_R);
			pila.push(hijoTurnR);
		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (generados.find(hijoTurnL.st) == generados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			pila.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (generados.find(hijoForward.st) == generados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				pila.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la pila
		if (!pila.empty()){
			current = pila.top();
		}
	}

  cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}


	return false;
}

bool ComportamientoJugador::pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();

	// usamos una cola
	queue<nodo> cola_nodos;
	set<estado,ComparaEstados> generados;

	// establecemos el primer nodo al origen, y lo inssertamos en la cola
	nodo current;
  	current.st = origen;
  	current.secuencia.clear();


	cola_nodos.push(current);

	// mientras queden nodos por explirar y no lleguemos al destino
	while (!cola_nodos.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		// abrimos el primer nodo, y lo metemos en cerrados
		cola_nodos.pop();
		generados.insert(current.st);

		//Generamos nodo correspondiente a girar a la derecha
		nodo rightSon = current;
		rightSon.st.orientacion = (rightSon.st.orientacion+1) % 4;
		if(generados.find(rightSon.st) == generados.end()){
			rightSon.secuencia.push_back(actTURN_R);
			cola_nodos.push(rightSon);
		}


		//Generamos nodo correspondiente a girar a la izquierda
		nodo leftSon = current;
		leftSon.st.orientacion = (leftSon.st.orientacion+3) % 4;
		if(generados.find(leftSon.st) == generados.end()){
			leftSon.secuencia.push_back(actTURN_L);
			cola_nodos.push(leftSon);
		}

		//Generamos nodo correspondiente a avanzar
		nodo fowardSon = current;
		if(!HayObstaculoDelante(fowardSon.st)){
			if(generados.find(fowardSon.st) == generados.end()){
				fowardSon.secuencia.push_back(actFORWARD);
				cola_nodos.push(fowardSon);
			}
		}

		//Cogemos el primer nodo de la cola
		if (!cola_nodos.empty()){
			current = cola_nodos.front();
		}

	}

  	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}


	return false;
}


// Funcion para calcular el coste de pasar de un estado origen a un nuevo estado
//
int  ComportamientoJugador::calcularCoste(const estado &origen, const estado &n_casilla){
	int coste = 0;

	// si la diferencia entre el estado origen y la nueva casilla es la orientacion
	// quiere decir que se ha hecho un giro, que tiene coste 1
	if(origen.orientacion != n_casilla.orientacion)
		coste++;
	else switch ((mapaResultado[n_casilla.fila][n_casilla.columna])) {
		// en caso contrario
		case 'A': coste += 10; break;
		case 'B': coste += 5; break;
		case 'T': coste += 2; break;
		case 'S': coste += 1; break;
		case 'K': coste += 1; break;
		case '?': coste += 2; break;
	}


	return coste;
}

// funcion para comparar si dos estados son iguales
bool ComparaEstado(const estado &a, const estado &n) {
	return (a.fila == n.fila and a.columna == n.columna and a.orientacion == n.orientacion);
}

// Functor para ordenar los nodos por coste total
struct OrdenaNodo{
	bool operator()(const nodo &a, const nodo &n) const{
		return a.coste_total < n.coste_total;
	}
};


multiset<nodo, OrdenaNodo>::const_iterator nodoEncontrado(const multiset<nodo, OrdenaNodo> & abiertos, const nodo & nodo){
	for(auto it = abiertos.begin(); it!= abiertos.end(); ++it){
		if(ComparaEstado((*it).st, nodo.st )){
			return it;
		}
	}

	return abiertos.end();
}



bool ComportamientoJugador::pathFinding_CostoUniforme(const estado &origen, const estado &destino, list<Action> &plan) {
	cout << "Calculando plan\n";
	plan.clear();

	//usamos un multiset en el que ordenamos por coste_total
	multiset<nodo, OrdenaNodo> abiertos;
	set<estado,ComparaEstados> generados;

	// almacenamos el nodo origen, con coste 0
	nodo current;
	current.st = origen;
	current.secuencia.clear();

	current.coste_total = 0;

	abiertos.insert(current);

	// mientras que queden nodos por explorar y no estemos en el destino
	while (!abiertos.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		// abrimos el primer nodo y lo almacenamos en cerrados
		abiertos.erase(abiertos.begin());
		generados.insert(current.st);

		// calculamos el nodo de girar a la derecha, y si no lo hemos explorado,
		// trabajamos con el
		nodo rightSon = current;
		rightSon.st.orientacion = (rightSon.st.orientacion+1) % 4;
		if(generados.find(rightSon.st) == generados.end()){

			//calculamos el coste de ir a ese estado
			rightSon.coste_total += calcularCoste(current.st, rightSon.st );

			// buscamos el nodo en abiertos
			auto it = nodoEncontrado(abiertos, rightSon);

			// si lo encontramos
			if (it != abiertos.end()){
				// nos quedamos con el de mejor coste
				if (rightSon.coste_total < it->coste_total){
					abiertos.erase(it);
					rightSon.secuencia.push_back(actTURN_R);
					abiertos.insert(rightSon);
				}
			} else{
				// si no lo encontramos lo añadimos a abiertos
				rightSon.secuencia.push_back(actTURN_R);
				abiertos.insert(rightSon);
			}

		}

		// hijo resultante de girar a la izquierda
		nodo leftSon = current;
		leftSon.st.orientacion = (leftSon.st.orientacion+3) % 4;
		if(generados.find(leftSon.st) == generados.end()){

			// calculamos el coste de ir a ese estado
			leftSon.coste_total += calcularCoste(current.st, leftSon.st );

			// lo buscamos en  abiertos
			auto it = nodoEncontrado(abiertos, leftSon);

			// si lo encontramos nos quedamos con el mejor de los dos
			if (it != abiertos.end()){
				if (leftSon.coste_total < it->coste_total){
					abiertos.erase(it);
					leftSon.secuencia.push_back(actTURN_L);
					abiertos.insert(leftSon);
				}
			} else{
				// si no lo añadimos
				leftSon.secuencia.push_back(actTURN_L);
				abiertos.insert(leftSon);
			}

		}

		// nodo resultante de avanzar
		nodo fowardSon = current;
		//si no es un obstaculo ni lo hemos abierto, trabajamos con el
		if(!HayObstaculoDelante(fowardSon.st)){
			if(generados.find(fowardSon.st) == generados.end()){

				// calculamos el coste de cambiar de estado
				fowardSon.coste_total += calcularCoste(current.st, fowardSon.st );

				// lo buscamos en abiertos
				auto it = nodoEncontrado(abiertos, fowardSon);

				// si el estado ya esta en abiertos, nos quedamos con el de mejor coste
				if (it != abiertos.end()){
					if (fowardSon.coste_total < it->coste_total){
						abiertos.erase(it);
						fowardSon.secuencia.push_back(actFORWARD);
						abiertos.insert(fowardSon);
					}
				} else{
					//si no esta, lo añadimos abiertos
					fowardSon.secuencia.push_back(actFORWARD);
					abiertos.insert(fowardSon);
				}


			}
		}

		//mientras queden nodos por explorar, seguimos
		if (!abiertos.empty()){
			current = (*abiertos.begin());
		}

	}

	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		cout << "Coste del plan " << current.coste_total << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}

	return false;
}








bool ComportamientoJugador::pathFinding_A_Estrella(const estado &origen, const estado &destino, list<Action> &plan) {
	cout << "Calculando plan\n";
	plan.clear();

	// almacenamos los nodos en un multiset ordenado por coste_total
	multiset<nodo, OrdenaNodo> abiertos;
	set<estado,ComparaEstados> generados;

	// seleccionamos el primer nodo como origen
	nodo current;
	current.st = origen;
	// establecemos el coste del estado a 0
	current.st.coste = 0;

	current.secuencia.clear();

	//establecemos el coste del nodo a 0
	current.coste = 0;
	current.coste_total = 0;

	// calculamos su heuristica usando distancia manhattan
	int distancia_manhattan = abs(current.st.fila - destino.fila) + abs(current.st.columna - destino.columna);
	// actualizamos su coste total e insertamos en abiertos
	current.coste_total += distancia_manhattan;
	current.heuristica = distancia_manhattan;
	abiertos.insert(current);

	// mientras queden nodos por explorar y no estemos en el destino
	while (!abiertos.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		// borramos el nuevo nodo current y lo metemos en cerrados
		abiertos.erase(abiertos.begin());
		generados.insert(current.st);

		// hijo resultante de girar a la derecha
		nodo rightSon = current;
		rightSon.st.orientacion = (rightSon.st.orientacion+1) % 4;

		// calculamos su coste, heuristica, y coste total a partir de estas
		rightSon.coste += calcularCoste(current.st, rightSon.st );
		rightSon.heuristica = abs(rightSon.st.fila - destino.fila) + abs(rightSon.st.columna - destino.columna);
		rightSon.coste_total = rightSon.coste + rightSon.heuristica;

		rightSon.st.coste = rightSon.coste;

		// buscamos el nodo en generados, es decir en los cerrados
		auto it = generados.find(rightSon.st);

		if(it == generados.end()){

			// si no lo encontramos lo buscamos en abiertos
			auto it = nodoEncontrado(abiertos, rightSon);

			// si lo encontramos en abiertos, nos quedamos con el de mejor
			// coste_total
			if (it != abiertos.end()){
				if (rightSon.coste_total < it->coste_total){
					abiertos.erase(it);
					rightSon.secuencia.push_back(actTURN_R);
					abiertos.insert(rightSon);
				}
			} else{
				// si no lo encontramos lo añadimos a abiertos
				rightSon.secuencia.push_back(actTURN_R);
				abiertos.insert(rightSon);
			}

		} else {
			// si lo encontramos, nos quedamos con el estado que tenga mejor coste
			if (it->coste > rightSon.coste){
				generados.erase(it);
				rightSon.secuencia.push_back(actTURN_R);
				abiertos.insert(rightSon);
			}
		}

		// hijo resultante de girar a la izquierda
		nodo leftSon = current;
		leftSon.st.orientacion = (leftSon.st.orientacion+3) % 4;

		// calculamos su coste, heuristica, y coste total a partir de estas
		leftSon.coste += calcularCoste(current.st, leftSon.st );
		leftSon.heuristica = abs(leftSon.st.fila - destino.fila) + abs(leftSon.st.columna - destino.columna);
		leftSon.coste_total = leftSon.coste + leftSon.heuristica;

		leftSon.st.coste = leftSon.coste;

		// buscamos el estado en generados, es decir los ya explorados
		it = generados.find(leftSon.st);

		if(it == generados.end()){

			//si no lo encontramos, buscamos el nodo en abiertos
			auto it = nodoEncontrado(abiertos, leftSon);

			// si encontramos el nodo en abiertos, nos quedamos con el de mejor coste
			if (it != abiertos.end()){
				if (leftSon.coste_total < it->coste_total){
					abiertos.erase(it);
					leftSon.secuencia.push_back(actTURN_L);
					abiertos.insert(leftSon);
				}
			} else{
				// si no lo encontramos, lo añadimos
				leftSon.secuencia.push_back(actTURN_L);
				abiertos.insert(leftSon);
			}



		} else{
			// si lo encontramos, nos quedamos con el estado que tenga mejor coste
			if (it->coste > leftSon.coste){
				generados.erase(it);
				leftSon.secuencia.push_back(actTURN_L);
				abiertos.insert(leftSon);
			}
		}

		// hijo resultante de avanzar
		nodo fowardSon = current;
		// si no es un obstaculo
		if(!HayObstaculoDelante(fowardSon.st)){

			// calculamos su coste, heuristica y coste total
			fowardSon.coste += calcularCoste(current.st, fowardSon.st );
			fowardSon.heuristica = abs(fowardSon.st.fila - destino.fila) + abs(fowardSon.st.columna - destino.columna);
			fowardSon.coste_total = fowardSon.coste + fowardSon.heuristica;

			fowardSon.st.coste = fowardSon.coste;

			// lo buscamos en generados, es decir, los ya explorados
			it = generados.find(fowardSon.st);

			if(it == generados.end()){
				// si no lo encontramos lo buscamos en abiertos
				auto it = nodoEncontrado(abiertos, fowardSon);

				// si lo encontramos en abiertos, nos quedamos con el de mejor coste
				if (it != abiertos.end()){
					if (fowardSon.coste_total < it->coste_total){
						abiertos.erase(it);
						fowardSon.secuencia.push_back(actFORWARD);
						abiertos.insert(fowardSon);
					}
				} else{
					// si no lo encontramos en abiertos, lo añadimos
					fowardSon.secuencia.push_back(actFORWARD);
					abiertos.insert(fowardSon);
				}



			} else{
				// si lo encontramos en cerrados, nos quedamos con el estado con mejor coste
				if (it->coste > fowardSon.coste){
					generados.erase(it);
					fowardSon.secuencia.push_back(actFORWARD);
					abiertos.insert(fowardSon);
				}
			}
		}


		if (!abiertos.empty()){
			current = (*abiertos.begin());
		}

	}

	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		cout << "Coste del plan " << current.coste << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}

	return false;
}







// Sacar por la términal la secuencia del plan obtenido
void ComportamientoJugador::PintaPlan(list<Action> plan) {
	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			cout << "A ";
		}
		else if (*it == actTURN_R){
			cout << "D ";
		}
		else if (*it == actTURN_L){
			cout << "I ";
		}
		else {
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}



void AnularMatriz(vector<vector<unsigned char> > &m){
	for (int i=0; i<m[0].size(); i++){
		for (int j=0; j<m.size(); j++){
			m[i][j]=0;
		}
	}
}


// Pinta sobre el mapa del juego el plan obtenido
void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan){
  AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			switch (cst.orientacion) {
				case 0: cst.fila--; break;
				case 1: cst.columna++; break;
				case 2: cst.fila++; break;
				case 3: cst.columna--; break;
			}
			mapaConPlan[cst.fila][cst.columna]=1;
		}
		else if (*it == actTURN_R){
			cst.orientacion = (cst.orientacion+1)%4;
		}
		else {
			cst.orientacion = (cst.orientacion+3)%4;
		}
		it++;
	}
}



int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
