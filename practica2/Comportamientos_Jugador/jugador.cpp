
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
	// Estoy en el nivel 1
	/*
	if (sensores.nivel != 4){
		if (sensores.mensajeF != -1){
			fil = sensores.mensajeF;
			col = sensores.mensajeC;
			brujula = 0; // 0 corresponde con Norte

			actual.fila = fil;
			actual.columna = col;
			actual.orientacion = brujula;

			destino.fila = sensores.destinoF;
			destino.columna = sensores.destinoC;
		}

		bool hay_plan = pathFinding (sensores.nivel, actual, destino, plan);

	}
	else {
		// Estoy en el nivel 2
		cout << "Aún no implementado el nivel 2" << endl;
	}*/



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
			switch (actual.orientacion) {
				//si estamos mirando al norte
				case 0:
					for(unsigned int i = 0; i < 4; i++){
						for(unsigned int j = 0; j <= i*2; j++ ){
							if (mapaResultado[fil - i][col - i + j] == '?')
								mapaResultado[fil - i][col - i + j] = sensores.terreno[k];
							k++;
						}
					}
					break;

				//si estamos mirando al este
				case 1:
					for(unsigned int i = 0; i < 4; i++){
						for(unsigned int j = 0; j <= i*2; j++ ){
							if (mapaResultado[fil - i + j][col + i] == '?')
								mapaResultado[fil - i + j][col + i] = sensores.terreno[k];
							k++;
						}
					}
					break;

				//si estamos mirando al sur
				case 2:
					for(unsigned int i = 0; i < 4; i++){
						for(unsigned int j = 0; j <= i*2; j++ ){
							if (mapaResultado[fil + i][col + i - j] == '?')
								mapaResultado[fil + i][col + i - j] = sensores.terreno[k];
							k++;
						}
					}
					break;

				//si estamos mirando al oeste
				case 3:
					for(unsigned int i = 0; i < 4; i++){
						for(unsigned int j = 0; j <= i*2; j++ ){
							if (mapaResultado[fil + i - j][col - i] == '?')
								mapaResultado[fil + i - j][col - i] = sensores.terreno[k];
							k++;
						}
					}
					break;
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
				recalcular = 4;
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

			// en 
			mapaImg[fil_img][col_img]++;

			if (!hayPlan){

				int posK = -1;

				for (auto it = sensores.terreno.begin(); it != sensores.terreno.end() && posK == -1; ++it){
					if ((*it) == 'K')
						posK = distance(sensores.terreno.begin(), it);
				}

				if (posK == -1){

					int c_izq, c_der, c_del;

					if (EsObstaculo(sensores.terreno[2])){
						switch (brujula) {
							case 0: mapaImg[fil_img-1][col_img] = 999999; break;
							case 1: mapaImg[fil_img][col_img+1] = 999999; break;
							case 2: mapaImg[fil_img+1][col_img] = 999999; break;
							case 3: mapaImg[fil_img][col_img-1] = 999999; break;
						}
					}

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


					//cout << c_izq << " " << c_der << " " << c_del << " " << mapaImg[fil_img][col_img] << endl;




					sigAccion = actFORWARD;


					if (c_der < c_del){

						if (c_izq < c_der) {
							sigAccion = actTURN_L;
						}
						else {
							sigAccion = actTURN_R;
						}

					} else if (c_izq < c_del) {
						if (c_izq < c_der) {
							sigAccion = actTURN_L;
						}
						else {
							sigAccion = actTURN_R;
						}
					}



				}

				else{


						int av = 0;
						int lateral = 0;

						if ( posK > 0 and posK < 4 ){
							av = 1;
							if (posK > 2 )
								lateral = 1;
							else if (posK < 2)
								lateral = -1;

						} else if (posK < 9){
							av = 2;
							if (posK > 6 )
								if (posK % 2 == 0) lateral = 2; else lateral = 1;
							else if (posK < 6 )
								if (posK % 2 == 0) lateral = -2; else lateral = -1;

						} else {
							av = 3;
							if (posK > 12 )
								if (posK % 2 == 0) lateral = 2; else lateral = 1;
							else if (posK < 12 )
								if (posK % 2 == 0) lateral = -2; else lateral = -1;

							if (posK == 9) lateral -= 2;
							if (posK == 15) lateral += 2;

						}

						plan.clear();
						for (int i = 0; i < av; i++){
							plan.push_back(actFORWARD);
						}

						if (lateral > 0){
							plan.push_back(actTURN_R);
						} else{
							plan.push_back(actTURN_L);
						}

						for (int i = 0; i < abs(lateral); i++){
							plan.push_back(actFORWARD);
						}

						PintaPlan(plan);

						hayPlan = true;


				}

			}

			if (hayPlan and plan.size()>0){

				sigAccion = actIDLE;

				if (EsObstaculo(sensores.terreno[2]) and plan.front() == actFORWARD ){
					hayPlan = false;
					sigAccion = actTURN_R;
				} else if (sensores.superficie[2] != 'a' || plan.front() != actFORWARD ){
					sigAccion = plan.front();
					plan.erase(plan.begin());
				}

			}

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

	queue<nodo> cola_nodos;
	set<estado,ComparaEstados> generados;

	nodo current;
  	current.st = origen;
  	current.secuencia.clear();


	cola_nodos.push(current);

	while (!cola_nodos.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

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

int  ComportamientoJugador::calcularCoste(const estado &origen, const estado &n_casilla){
	int coste = 0;

	if(origen.orientacion != n_casilla.orientacion)
		coste++;
	else switch ((mapaResultado[n_casilla.fila][n_casilla.columna])) {
		case 'A': coste += 10; break;
		case 'B': coste += 5; break;
		case 'T': coste += 2; break;
		case 'S': coste += 1; break;
		case 'K': coste += 1; break;
		case 'M': coste += 999999; break;
		case 'P': coste += 999999; break;
		case '?': coste += 2; break;
	}


	return coste;
}

bool ComparaNodo(const estado &a, const estado &n) {
	return (a.fila == n.fila and a.columna == n.columna and a.orientacion == n.orientacion);
}


struct OrdenaNodo{
	bool operator()(const nodo &a, const nodo &n) const{
		return a.coste_total < n.coste_total;
	}
};


bool ComportamientoJugador::pathFinding_CostoUniforme(const estado &origen, const estado &destino, list<Action> &plan) {
	cout << "Calculando plan\n";
	plan.clear();

	multiset<nodo, OrdenaNodo> abiertos;
	set<estado,ComparaEstados> generados;

	nodo current;
	current.st = origen;
	current.secuencia.clear();

	current.coste_total = 0;

	abiertos.insert(current);

	while (!abiertos.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		abiertos.erase(abiertos.begin());
		generados.insert(current.st);

		nodo rightSon = current;
		rightSon.st.orientacion = (rightSon.st.orientacion+1) % 4;
		if(generados.find(rightSon.st) == generados.end()){

			rightSon.coste_total += calcularCoste(current.st, rightSon.st );

			bool encontrado = false;
			for(auto it = abiertos.begin(); it!= abiertos.end() and !encontrado; ++it){
				if(ComparaNodo((*it).st, rightSon.st )){
					encontrado = true;

					if (rightSon.coste_total < it->coste_total){
						abiertos.erase(it);
						rightSon.secuencia.push_back(actTURN_R);
						abiertos.insert(rightSon);
					}

				}
			}

			if (!encontrado){
				rightSon.secuencia.push_back(actTURN_R);
				abiertos.insert(rightSon);
			}

		}

		nodo leftSon = current;
		leftSon.st.orientacion = (leftSon.st.orientacion+3) % 4;
		if(generados.find(leftSon.st) == generados.end()){

			leftSon.coste_total += calcularCoste(current.st, leftSon.st );

			bool encontrado = false;
			for(auto it = abiertos.begin(); it!= abiertos.end() and !encontrado; ++it){
				if(ComparaNodo((*it).st, leftSon.st )){
					encontrado = true;

					if (leftSon.coste_total < it->coste_total){
						abiertos.erase(it);
						leftSon.secuencia.push_back(actTURN_L);
						abiertos.insert(leftSon);
					}

				}
			}

			if (!encontrado){
				leftSon.secuencia.push_back(actTURN_L);
				abiertos.insert(leftSon);
			}
		}

		nodo fowardSon = current;
		if(!HayObstaculoDelante(fowardSon.st)){
			if(generados.find(fowardSon.st) == generados.end()){

				fowardSon.coste_total += calcularCoste(current.st, fowardSon.st );

				bool encontrado = false;
				for(auto it = abiertos.begin(); it!= abiertos.end() and !encontrado; ++it){
					if(ComparaNodo((*it).st, fowardSon.st )){
						encontrado = true;

						if (fowardSon.coste_total < it->coste_total){
							abiertos.erase(it);
							fowardSon.secuencia.push_back(actFORWARD);
							abiertos.insert(fowardSon);
						}

					}
				}

				if (!encontrado){
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

	multiset<nodo, OrdenaNodo> abiertos;
	set<estado,ComparaEstados> generados;

	nodo current;
	current.st = origen;
	current.st.coste = 0;

	current.secuencia.clear();

	current.coste = 0;
	current.coste_total = 0;

	int distancia_manhattan = abs(current.st.fila - destino.fila) + abs(current.st.columna - destino.columna);
	current.coste_total += distancia_manhattan;
	current.heuristica = distancia_manhattan;
	abiertos.insert(current);

	while (!abiertos.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		abiertos.erase(abiertos.begin());
		generados.insert(current.st);

		nodo rightSon = current;
		rightSon.st.orientacion = (rightSon.st.orientacion+1) % 4;

		rightSon.coste += calcularCoste(current.st, rightSon.st );
		rightSon.heuristica = abs(rightSon.st.fila - destino.fila) + abs(rightSon.st.columna - destino.columna);
		rightSon.coste_total = rightSon.coste + rightSon.heuristica;

		auto it = generados.find(rightSon.st);

		if(it == generados.end()){

			bool encontrado = false;
			for(auto it = abiertos.begin(); it!= abiertos.end() and !encontrado; ++it){
				if(ComparaNodo((*it).st, rightSon.st )){
					encontrado = true;

					if (rightSon.coste_total < it->coste_total){
						abiertos.erase(it);
						rightSon.secuencia.push_back(actTURN_R);
						abiertos.insert(rightSon);
					}

				}
			}

			if (!encontrado){
				rightSon.secuencia.push_back(actTURN_R);
				abiertos.insert(rightSon);
			}

		} else {
			if (it->coste > rightSon.coste){
				generados.erase(it);
				rightSon.secuencia.push_back(actTURN_R);
				abiertos.insert(rightSon);
			}
		}

		nodo leftSon = current;
		leftSon.st.orientacion = (leftSon.st.orientacion+3) % 4;

		leftSon.coste += calcularCoste(current.st, leftSon.st );
		leftSon.heuristica = abs(leftSon.st.fila - destino.fila) + abs(leftSon.st.columna - destino.columna);
		leftSon.coste_total = leftSon.coste + leftSon.heuristica;

		it = generados.find(leftSon.st);

		if(it == generados.end()){


			bool encontrado = false;
			for(auto it = abiertos.begin(); it!= abiertos.end() and !encontrado; ++it){
				if(ComparaNodo((*it).st, leftSon.st )){
					encontrado = true;

					if (leftSon.coste_total < it->coste_total){
						abiertos.erase(it);
						leftSon.secuencia.push_back(actTURN_L);
						abiertos.insert(leftSon);
					}

				}
			}

			if (!encontrado){
				leftSon.secuencia.push_back(actTURN_L);
				abiertos.insert(leftSon);
			}
		} else{
			if (it->coste > leftSon.coste){
				generados.erase(it);
				leftSon.secuencia.push_back(actTURN_L);
				abiertos.insert(leftSon);
			}
		}


		nodo fowardSon = current;
		if(!HayObstaculoDelante(fowardSon.st)){

			fowardSon.coste += calcularCoste(current.st, fowardSon.st );
			fowardSon.heuristica = abs(fowardSon.st.fila - destino.fila) + abs(fowardSon.st.columna - destino.columna);
			fowardSon.coste_total = fowardSon.coste + fowardSon.heuristica;

			it = generados.find(fowardSon.st);

			if(it == generados.end()){

				bool encontrado = false;
				for(auto it = abiertos.begin(); it!= abiertos.end() and !encontrado; ++it){
					if(ComparaNodo((*it).st, fowardSon.st )){
						encontrado = true;

						if (fowardSon.coste_total < it->coste_total){
							abiertos.erase(it);
							fowardSon.secuencia.push_back(actFORWARD);
							abiertos.insert(fowardSon);
						}

					}
				}

				if (!encontrado){
					fowardSon.secuencia.push_back(actFORWARD);
					abiertos.insert(fowardSon);
				}
			} else{
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
