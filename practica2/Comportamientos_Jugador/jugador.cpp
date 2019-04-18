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


	if (sensores.nivel == 4){
		//nivel 2

		if (sensores.mensajeF != -1){
			fil = sensores.mensajeF;
			col = sensores.mensajeC;
			ultimaAccion = actIDLE;
		}



		if (estoyBienSituado){

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


			int k = 0;

			switch (actual.orientacion) {
				case 0:
					for(unsigned int i = 0; i < 4; i++){
						for(unsigned int j = 0; j <= i*2; j++ ){
							if (mapaResultado[fil - i][col - i + j] == '?')
								mapaResultado[fil - i][col - i + j] = sensores.terreno[k];
							k++;
						}
					}
					break;

				case 1:
					for(unsigned int i = 0; i < 4; i++){
						for(unsigned int j = 0; j <= i*2; j++ ){
							if (mapaResultado[fil - i + j][col + i] == '?')
								mapaResultado[fil - i + j][col + i] = sensores.terreno[k];
							k++;
						}
					}
					break;

				case 2:
					for(unsigned int i = 0; i < 4; i++){
						for(unsigned int j = 0; j <= i*2; j++ ){
							if (mapaResultado[fil + i][col + i - j] == '?')
								mapaResultado[fil + i][col + i - j] = sensores.terreno[k];
							k++;
						}
					}
					break;

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



			if (sensores.destinoF != destino.fila or sensores.destinoC != destino.columna){
				destino.fila = sensores.destinoF;
				destino.columna = sensores.destinoC;
				hayPlan = false;
			}

			//cout << "Fila: " << fil << " Col: " << col << " Or: " << brujula << endl;
			if (!hayPlan){
				actual.fila = fil;
				actual.columna = col;
				actual.orientacion = brujula;
				hayPlan =  pathFinding(sensores.nivel, actual, destino, plan);
			}

			if (hayPlan and plan.size()>0){

				sigAccion = actIDLE;

				if (EsObstaculo(sensores.terreno[2]) and plan.front() == actFORWARD ){
					hayPlan = false;
				} else if (sensores.superficie[2] != 'a' ){
					sigAccion = plan.front();
					plan.erase(plan.begin());
				}

			}

			recalcular--;

			if (recalcular == 0){
				hayPlan = false;
				recalcular = 4;
			}

		} else{
			//por hacer


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

			if (!hayPlan){

				int posK = -1;

				for (auto it = sensores.terreno.begin(); it != sensores.terreno.end() && posK == -1; ++it){
					if ((*it) == 'K')
						posK = distance(sensores.terreno.begin(), it);
				}

				if (posK == -1){


					mapaImg[fil_img][col_img]++;


					int c_izq, c_der, c_del;

					if (EsObstaculo(sensores.terreno[2])){
						switch (brujula) {
							case 0: mapaImg[fil_img-1][col_img] = 99999; break;
							case 1: mapaImg[fil_img][col_img+1] = 99999; break;
							case 2: mapaImg[fil_img+1][col_img] = 99999; break;
							case 3: mapaImg[fil_img][col_img-1] = 99999; break;
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
				} else if (sensores.superficie[2] != 'a' ){
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

	} else{
		//Nivel 1

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


		if (sensores.destinoF != destino.fila or sensores.destinoC != destino.columna){
			destino.fila = sensores.destinoF;
			destino.columna = sensores.destinoC;
			hayPlan = false;
		}

		//cout << "Fila: " << fil << " Col: " << col << " Or: " << brujula << endl;
		if (!hayPlan){
			actual.fila = fil;
			actual.columna = col;
			actual.orientacion = brujula;
			hayPlan =  pathFinding(sensores.nivel, actual, destino, plan);
		}


		if (hayPlan and plan.size()>0){

			sigAccion = actIDLE;

			if (EsObstaculo(sensores.terreno[2]) and plan.front() == actFORWARD ){
				hayPlan = false;
			} else if (sensores.superficie[2] != 'a' ){
				sigAccion = plan.front();
				plan.erase(plan.begin());
			}

		}
		else{
			if (sensores.terreno[2] == 'P' || sensores.terreno[2] == 'M' ||
				 sensores.terreno[2] == 'D' || sensores.superficie[2] == 'a'){

				sigAccion = actTURN_R;
			}
			else{
				sigAccion = actFORWARD;
			}
		}

	}



	//movimiento




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




struct nodo{
	estado st;
	list<Action> secuencia;
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

		nodo rightSon = current;
		rightSon.st.orientacion = (rightSon.st.orientacion+1) % 4;
		if(generados.find(rightSon.st) == generados.end()){
			rightSon.secuencia.push_back(actTURN_R);
			cola_nodos.push(rightSon);
		}

		nodo leftSon = current;
		leftSon.st.orientacion = (leftSon.st.orientacion+3) % 4;
		if(generados.find(leftSon.st) == generados.end()){
			leftSon.secuencia.push_back(actTURN_L);
			cola_nodos.push(leftSon);
		}

		nodo fowardSon = current;
		if(!HayObstaculoDelante(fowardSon.st)){
			if(generados.find(fowardSon.st) == generados.end()){
				fowardSon.secuencia.push_back(actFORWARD);
				cola_nodos.push(fowardSon);
			}
		}

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

	switch ((mapaResultado[n_casilla.fila][n_casilla.columna])) {
		case 'A': coste += 10; break;
		case 'B': coste += 5; break;
		case 'T': coste += 2; break;
		case 'S': coste += 1; break;
		case 'K': coste += 1; break;
		case 'M': coste += 999999; break;
		case 'P': coste += 999999; break;
		case '?': coste += 3; break;
	}


	return coste;
}

bool ComparaNodo(const estado &a, const estado &n) {
	if (a.fila == n.fila and a.columna == n.columna and a.orientacion == n.orientacion)
		return true;
	else
		return false;
}



bool ComportamientoJugador::pathFinding_CostoUniforme(const estado &origen, const estado &destino, list<Action> &plan) {
	cout << "Calculando plan\n";
	plan.clear();

	multimap<int, nodo> abiertos;
	set<estado,ComparaEstados> generados;

	nodo current;
	current.st = origen;
	current.secuencia.clear();


	abiertos.insert(make_pair(0,current));
	int coste_antiguo = 0;

	while (!abiertos.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		abiertos.erase(abiertos.begin());
		generados.insert(current.st);

		nodo rightSon = current;
		rightSon.st.orientacion = (rightSon.st.orientacion+1) % 4;
		if(generados.find(rightSon.st) == generados.end()){


			bool encontrado = false;
			for(auto it = abiertos.begin(); it!= abiertos.end() and !encontrado; ++it){
				if(ComparaNodo((*it).second.st, rightSon.st )){
					encontrado = true;

					if (coste_antiguo+1 < it->first){
						abiertos.erase(it);
						rightSon.secuencia.push_back(actTURN_R);
						abiertos.insert(make_pair(coste_antiguo+1,rightSon));
					}

				}
			}

			if (!encontrado){
				rightSon.secuencia.push_back(actTURN_R);
				abiertos.insert(make_pair(coste_antiguo + 1,rightSon));
			}

		}

		nodo leftSon = current;
		leftSon.st.orientacion = (leftSon.st.orientacion+3) % 4;
		if(generados.find(leftSon.st) == generados.end()){

			bool encontrado = false;
			for(auto it = abiertos.begin(); it!= abiertos.end() and !encontrado; ++it){
				if(ComparaNodo((*it).second.st, leftSon.st )){
					encontrado = true;

					if (coste_antiguo+1 < it->first){
						abiertos.erase(it);
						leftSon.secuencia.push_back(actTURN_L);
						abiertos.insert(make_pair(coste_antiguo+1,leftSon));
					}

				}
			}

			if (!encontrado){
				leftSon.secuencia.push_back(actTURN_L);
				abiertos.insert(make_pair(coste_antiguo + 1,leftSon));
			}

		}

		nodo fowardSon = current;
		if(!HayObstaculoDelante(fowardSon.st)){
			if(generados.find(fowardSon.st) == generados.end()){
				bool encontrado = false;
				for(auto it = abiertos.begin(); it!= abiertos.end() and !encontrado; ++it){
					if(ComparaNodo((*it).second.st, fowardSon.st )){
						encontrado = true;

						if (calcularCoste(current.st, fowardSon.st )+coste_antiguo < it->first){
							abiertos.erase(it);
							fowardSon.secuencia.push_back(actFORWARD);
							abiertos.insert(make_pair(calcularCoste(current.st, fowardSon.st )+coste_antiguo,fowardSon));
						}

					}
				}

				if (!encontrado){
					fowardSon.secuencia.push_back(actFORWARD);
					abiertos.insert(make_pair(calcularCoste(current.st, fowardSon.st )+coste_antiguo,fowardSon));
				}
			}
		}


		if (!abiertos.empty()){
			current = abiertos.begin()->second;
			coste_antiguo = abiertos.begin()->first;
		}

	}

	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		cout << "Coste del plan " << coste_antiguo << endl;
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

	cout << origen.fila << " " << origen.columna << " " << destino.fila << " " << destino.columna << endl;

	multimap<int, nodo> abiertos;
	set<estado,ComparaEstados> generados;

	nodo current;
	current.st = origen;
	current.secuencia.clear();


	abiertos.insert(make_pair(0,current));
	int coste_antiguo = 0;

	int distancia_manhattan;

	while (!abiertos.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		abiertos.erase(abiertos.begin());
		generados.insert(current.st);

		nodo rightSon = current;
		rightSon.st.orientacion = (rightSon.st.orientacion+1) % 4;
		if(generados.find(rightSon.st) == generados.end()){


			bool encontrado = false;
			for(auto it = abiertos.begin(); it!= abiertos.end() and !encontrado; ++it){
				if(ComparaNodo((*it).second.st, rightSon.st )){
					encontrado = true;

					if (coste_antiguo+1 < it->first){
						abiertos.erase(it);
						rightSon.secuencia.push_back(actTURN_R);
						abiertos.insert(make_pair(coste_antiguo+1,rightSon));
					}

				}
			}

			if (!encontrado){
				rightSon.secuencia.push_back(actTURN_R);
				abiertos.insert(make_pair(coste_antiguo + 1,rightSon));
			}

		}

		nodo leftSon = current;
		leftSon.st.orientacion = (leftSon.st.orientacion+3) % 4;
		if(generados.find(leftSon.st) == generados.end()){

			bool encontrado = false;
			for(auto it = abiertos.begin(); it!= abiertos.end() and !encontrado; ++it){
				if(ComparaNodo((*it).second.st, leftSon.st )){
					encontrado = true;

					if (coste_antiguo+1 < it->first){
						abiertos.erase(it);
						leftSon.secuencia.push_back(actTURN_L);
						abiertos.insert(make_pair(coste_antiguo+1,leftSon));
					}

				}
			}

			if (!encontrado){
				leftSon.secuencia.push_back(actTURN_L);
				abiertos.insert(make_pair(coste_antiguo + 1,leftSon));
			}

		}


		nodo fowardSon = current;
		if(!HayObstaculoDelante(fowardSon.st)){
			if(generados.find(fowardSon.st) == generados.end()){
				distancia_manhattan = abs(fowardSon.st.fila - destino.fila) + abs(fowardSon.st.columna - destino.columna);
				bool encontrado = false;
				for(auto it = abiertos.begin(); it!= abiertos.end() and !encontrado; ++it){
					if(ComparaNodo((*it).second.st, fowardSon.st )){
						encontrado = true;

						if (calcularCoste(current.st, fowardSon.st )+coste_antiguo+distancia_manhattan < it->first){
							abiertos.erase(it);
							fowardSon.secuencia.push_back(actFORWARD);
							abiertos.insert(make_pair(calcularCoste(current.st, fowardSon.st )+coste_antiguo+distancia_manhattan,fowardSon));
						}

					}
				}

				if (!encontrado){
					fowardSon.secuencia.push_back(actFORWARD);
					abiertos.insert(make_pair(calcularCoste(current.st, fowardSon.st )+coste_antiguo+distancia_manhattan,fowardSon));
				}
			}
		}


		if (!abiertos.empty()){
			current = abiertos.begin()->second;
			coste_antiguo = abiertos.begin()->first;
		}

	}

	cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		cout << "Coste del plan " << coste_antiguo << endl;
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
