#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//~ Este programa hace uso de la función to_string(int) y pop_back() del estandar C++11

//~ Tipo: void, Método
//~ Nombre : inicializar_mapa_clon
//~ Parametros: char** mapa, char** clon, int FILAS, COLUMNAS
//~ Retorna: No retorna nada
//~ Descripción: Copia el mapa original (mapa)(matriz dinamica 2-dimensional)
//~		en el mapa clon (clon) sobreescribiendo este último, FILAS y COLUMNAS
//~		son las dimensiones de ambos mapas

void inicializar_mapa_clon(char** mapa, char** clon, int FILAS, int COLUMNAS){
	for (int i = 0; i < FILAS ; i++)
	{
		for (int j = 0; j < COLUMNAS ; j++)
		{
			clon[i][j] = mapa[i][j];
		}
	}
}

//~ Tipo: void, Método
//~ Nombre: print_map
//~ Parametros: char** mapa, int F, int C
//~ Retorna: No retorna
//~ Descripción: Imprime por salida estardár el mapa (matriz dinamica 2-dimensional) que se le
//~		pase por parametros.

//~ void print_map(char **mapa, int F, int C){
	//~ for (int i = F-1; i >= 0; i--)
		//~ {
			//~ cout << i << " | ";
			//~ for (int j = 0; j < C; j++)
			//~ {
				//~ cout << mapa[i][j] << " ";
			//~ }
			//~ cout << endl;
		//~ }
		//~ cout << "   ";
		//~ for (int i = 0; i < C; i++)
		//~ {
			//~ cout << " -";
		//~ }
		//~ cout << endl << "    " ;
		//~ for (int i = 0; i < C; i++)
		//~ {
			//~ cout << i << " ";
		//~ }
		//~ cout << endl << endl;
//~ }

//~ Tipo: bool, Función (Recursiva)
//~ Nombre: BT
//~ Parametros: char **mapa, char **clon, int FILAS, int COLUMAS, int c, string &camino, bool &alive, bool &fixed
//~ Retorna: * True si llega a la salida.
//~			 * False en caso contrario.
//~ Descripción: Dado un mapa(mapa), y un punto de inicio mapa[f][c] distinto de '#',
//~ 	recorre el mapa simulando un tanque averiado, hasta encontrar la salida (mapa[FILAS-1][COLUMNAS-1]),
//~ 	usando una copia del mapa original (clon) para marcar los puntos en los que ya se ha estado
//~ 	y registrando el camino seguido en la variable camino, la variable fixed se usa para saber
//~		si el tanque fue reparado (el tanque se repara si encuentra un punto de reparación 'X',
//~ 	mientres este averiado solo podrá moverse hacia arriba y hacia la derecha).

bool BT(char **mapa, char **clon, int FILAS,int COLUMNAS, int f, int c, string &camino,  bool &alive, bool &fixed){

	//~ print_map(mapa, FILAS, COLUMNAS);

	//~ Si (f,c) = (FILAS, COLUMNAS) estonces, ¡escapamos con éxito!.
	if ((f == FILAS-1) and (c == COLUMNAS-1) ){
		return true;
	}
	//~ Si mapa(f,c) es 'X'(fixPoint) y el tanque no esta ya reparado estonces
	//~ reparamos el tanque (fixed = true), reiniciamos el mapa clon para
	//~ poder volver por donde llegamos en caso de ser necesario, agregamos 'X'
	//~ al camino y marcamos mapa(f,c) como visitado.
	if (( mapa[f][c] == 'X') and (!fixed) ){
		camino += 'X';
		fixed = true;
		inicializar_mapa_clon(mapa, clon, FILAS, COLUMNAS);
		clon[f][c] = '*';
	}
	//~ Si aún no llegamos a la salida entonces probamos moviendonos en cada dirección posible
	//~ Para i: 0 = arriba ('W'), 1 = derecha ('D'), 2 = abajo ('S'), 3 = izquierda ('A').
	for (int i = 0; i < 4; i++)
	{
		if(i == 0){
		//~ Si la fila superior es válida(parte del mapa), no es un muro (!= '#') y no ha sido visitado:
			if (( f+1 < FILAS ) and (mapa[f+1][c] != '#') and (clon[f+1][c] != '*')){
				//~ Se marca el mapa clon en la siguiente posición como visitada y se actualiza el camino.
				clon[f+1][c] = '*';
				camino += 'W';
				//~ Se llama a BT con f+1
				if (BT( mapa, clon, FILAS, COLUMNAS, f+1, c, camino, alive, fixed)){
					//~ Si BT retorna true entonces escapamos, regresamos el mapa clon a su estado anterior
					//~ y rompemos el ciclo para que no busque más caminos.
					alive = true;
					clon[f+1][c] = mapa[f+1][c];
					break;
				}
				//~ Si BT retorno false retiramos retiramos el ultimo paso agregado al camino y regresamos el
				//~ mapa clon a su estado anterior, seguimos con el ciclo de ser posible,
				camino.pop_back();
				clon[f+1][c] = mapa[f+1][c];
			}
		}
		if(i == 1){
		//~ Si la columna a la derecha es válida(parte del mapa), no es un muro (!= '#') y no ha sido visitada:
			if (( c+1 < COLUMNAS ) and (mapa[f][c+1] != '#') and (clon[f][c+1] != '*')){
				clon[f][c+1] = '*';
				camino += 'D';
				//~ Se llama a BT con c+1
				if (BT( mapa, clon, FILAS, COLUMNAS,f,c+1, camino, alive, fixed)){
					alive = true;
					clon[f][c+1] = mapa[f][c+1];
					break;
				}
				camino.pop_back();
				clon[f][c+1] = mapa[f][c+1];
			}
		}
		if(i == 2){
			//Si el tanque esta reparado,  la fila inferior es válida(parte del mapa), no es un muro (!= '#') y no ha sido visitada:
			if ((fixed) and ( f-1 >= 0) and (mapa[f-1][c] != '#') and (clon[f-1][c] != '*') ){
				clon[f-1][c] = '*';
				camino += 'S';
				//~ Se llama a BT con f-1
				if (BT( mapa, clon, FILAS, COLUMNAS, f-1 , c , camino, alive, fixed)){
					alive = true;
					clon[f-1][c] = mapa[f-1][c];
					break;
				}
				camino.pop_back();
				clon[f-1][c] = mapa[f-1][c];
			}
		}
		if(i == 3){
		//Si el tanque esta reparado, la columna a la izquierda es válida(parte del mapa), no es un muro (!= '#') y no ha sido visitada:
			if ((fixed) and ( c-1 >= 0 ) and (mapa[f][c-1] != '#') and (clon[f][c-1] != '*') ){
				clon[f][c-1] = '*';
				camino += 'A';
				//~ Se llama a BT con c-1
				if (BT( mapa, clon, FILAS, COLUMNAS,f,c-1, camino, alive, fixed)){
					alive = true;
					clon[f][c-1] = mapa[f][c-1];
					break;
				}
				camino.pop_back();
				clon[f][c-1] = mapa[f][c-1];
			}
		}
		//~ Si no entramos por ninguno de los caso entonces no pudimos escapar y morimos.
		//~ alive pase a ser false;
		alive = false;
	}

	//~ Retornamos el estado de alive (True si encontramos la salida, false en caso contario).
	return alive;

	return false;
}

//~ Tipo: void, Método
//~ Nombre: procesar
//~ Parametros: char **mapa, char **clon, int FILAS, int COLUMNAS, ofstream &salida
//~ Retorna: No retorno, escribe en un fichero (ofstream salida).
//~ Descripción: Dado un mapa(matriz dinamica de tamaño FILAS por COLUMNAS),
//~ llama a la función BT para cada camino inicial válido ( distinto de '#')
//~ inicializando los valores que se pasan a BT como parametros en cada
//~ llamada (clon, f, c, alive, fixed, camino) y registrando todos los
//~ caminos que llevan a escapar con exito y escribiendo el resultado final en
//~ el archivo de salida.

void procesar(char **mapa, int FILAS, int COLUMNAS, ofstream &salida){

	bool alive = true;
	bool fixed = false;
	float casillas_validas = 0.0;
	float muertes = 0.0;
	string camino;
	string total_caminos = "";

	//~ Declaracion de mapa clon, como mapa auxiliar.
	//~ Será inicializado por la función inicializar_mapa_clon(...);
	char **mapa_clon;
	mapa_clon = new char *[FILAS];
	for (int i = 0; i < FILAS; i++)
	{
		mapa_clon[i] = new char[COLUMNAS];
	}


	//Ciclo Principal encargado de recorrer cada punto de inicio válido.
	for (int f = FILAS-1; f >= 0; f--)
	{
		for (int c = 0; c < COLUMNAS; c++)
		{
			//~ Inicializamos el mapa clon.
			inicializar_mapa_clon(mapa, mapa_clon, FILAS, COLUMNAS);
			//~ Descarto (f,c) si en el hay un muro.
			if (mapa[f][c] == '#'){
				continue;
			}

			camino = "Camino desde [" + to_string(f) + "][" + to_string(c) +"]: ";
			fixed = false;

			//~ Si (f,c) no es un muro, entonces, es una casilla valida.
			casillas_validas++;

			//~ Si BT(...) devuelve True para (f,c), entonces el tanque escapó desde (f,c)
			mapa_clon[f][c] = '*';
			if ( BT(mapa, mapa_clon, FILAS, COLUMNAS, f, c, camino, alive, fixed) ){
				//~ Como se encontro la salida, se añade el camino recorrido a la lista de caminos seguros.
				total_caminos += camino + "\n";
			}
			else{
			//~ Si murio, aumentamos el total de muertes.
				muertes++;
			}

		}//FForIn
	}//FForEx

	//~ cout << "Muertes:"  << muertes << endl
	//~ << "Casilla comprobadas: " << casillas_validas << endl;
	//~ cout << (muertes / casillas_validas * 100) << endl;;

	float PMS = (muertes / casillas_validas * 100.0);
	//~ cout << "Caso " << caso << ":"  <<endl;
	string camino_aux = "\nPorcentaje de muertes seguras: " + to_string(PMS) + "%";
	camino =  camino_aux + "\n" + total_caminos;
	//~ cout << camino;
	//~ cout << "\n";

	salida << camino << "\n" << endl;

	//Liberamos la memoria de mapa_clon
	for (int i = 0; i < FILAS; i++)
		{
			delete mapa_clon[i];
		}
	delete mapa_clon;

}

int main(int argc, char **argv)
{
	ifstream entrada("TankEscape.in");
	ofstream salida("TankEscape.out", ios::out);

	if (! entrada){
		cerr << "No se ha podido leer el archivo de entrada." << endl ;
		return -1;
	}

	//n = número de casos a evaluar
	int n;
	string mensaje_caso;
	//Se lee n desde el archivo de entrada.
	entrada >> n;

	//For principal para la lectura de datos del los n casos y construción del mapa
	for (int i = 0; i < n ; i++) //hasta n pero para las pruebas solo estoy probando para 1 caso
	{
		mensaje_caso = "Caso " + to_string(i+1) + ":";
		salida << mensaje_caso;

		//~ Se declara y leen desde la entrada las variables f, c, w
		//~ que corresponden a las filas, columnas y cantidad de muros
		//~ del caso respectivamente.

		int f, c, w;
		entrada >> f;
		entrada >> c;
		entrada >> w;

		//~ Comprobamos que las dimensiones y cantidad de muros sean válidas.
		if (f < 1 or c < 1){
			salida << "\nEl mapa posee dimensiones inapropiadas.\n" << endl ;
			continue;
		}
		if (w < 0){
			salida << "\nNo puede haber un número negativo de muros.\n" << endl;
			continue;
		}
		//Equivalente a char mapa[f][c];
		char **mapa = new char*[f];
		for (int i = 0; i < f; i++)
		{
			mapa[i] = new char[c];
		}

		//Aqui se configura un mapa vacio
		for (int i = 0; i < f; i++)
		{
			for (int j = 0; j < c; j++)
			{
				mapa[i][j] = '.' ;
			}
		}
		//End

		//Aqui se añaden los muros horizontales al mapa vacio.
		bool good = true;
		for (int i = 0; i < w; i++)
		{
			//~ Para cada muro se leen desde la entrada tres variables
			//~ tipo entero fila, col1, col2, que indican la fila en la que
			//~ esta el muro y las columnas en la que empiezan y terminan
			//~ respectivamente.

			int fila, col1, col2, aux;
			entrada >> fila; //fila
			entrada >> col1; //comluna inicio
			entrada >> col2; //comluna final

			//~ Comprobamos que las coordenas del muro sea válida.
			if (fila < 0 or fila > f){
				salida << "\nLas coordenadas del muro N "<< i+1 <<" son inválidas\n" << endl;
				good = false;
				continue;
			}
			if (col1 < 0 or col1 > c ){
				salida << "\nLas coordenadas del muro N "<< i+1 <<" son inválidas\n" << endl;
				good = false;
				continue;
			}
			if (col2 < 0 or col2 > c ){
				salida << "\nLas coordenadas del muro N "<< i+1 <<" son inválidas\n" << endl;
				good = false;
				continue;
			}
			if (col2 < col1){
				salida << "\nLas coordenadas del muro N "<< i+1 <<" son inválidas\n" << endl;
				good = false;
				continue;
			}
			//Fin de comprobación de coordenas para los muros.

			//~ Aquí se implementan los muros promiamente siempre que no se estén
			//~ construyendo encima de otro o esten construidos directamente en la salida.
			aux = col1;
			while(aux < col2+1){
				if (mapa[fila][aux] == '#'){
					salida << "\nSe intentó construir un muro encima de otro.\n" << endl;
					good = false;
					continue;
				}
				else{
					mapa[fila][aux] = '#';
					aux++;
					if (mapa[f-1][c-1] == '#'){
						salida << "\nImposible escapar ya que hay un muro en la esquina superior derecha\n" << endl;
						good = false;
						continue;
					}
				}
			}//FinWhile
			//
		}

		//~ Comprobación : Si no se pudo agregar los muros (good = false)
		//~ Se salta el resto de caso y vuelve al inicio del ciclo para
		//~ leer los datos del siguiente caso, de haberlo.
		if (!good)
			continue;
		//~ End

		//~ Se leen la cantidad de puntos de reparación (fixPoints) desde la entrada
		//~ si son más de cuatro o negativos se salto el resto del caso y se vuelve
		//~ a iniciar el ciclo para leer los datos del siguiente caso de haberlo.
		int fixPoints;
		entrada >> fixPoints;

		if ( (fixPoints < 0) or (fixPoints > 4) ){
			salida << "\nEl número de fixPoints es menor que 0 o mayor que 4.\n" << endl;
			continue;
		}

		//~ Aqui se añaden los fix Points
		good = true;
		for (int i = 0; i < fixPoints; i++)
		{
			//~ Se leen desde la entrada fila y col que indican las coordenas en las que
			//~ se construira el fixpoint
			int fila, col;
			entrada >> fila;
			entrada >> col;

			//Comprobamos  que la coordena sea válida.
			if (fila < 0 or fila > f ){
				salida << "\nLas coordenadas del fixPoint N " << i+1 << " son inválidas.\n" << endl;
				good = false;
			}
			else if (col < 0 or col > c ){
				salida << "\nLas coordenadas del fixPoint N " << i+1 << " son inválidas.\n" << endl;
				good = false;
			}
			else if (mapa[fila][col] == '#'){
				salida << "\nLas coordenadas del fixPoint N " << i+1 << " son inválidas.\n" << endl;
				good = false;
			}
			else{
				//Si la coordana es válida.
				mapa[fila][col] = 'X';
			}

			//~ Comprobación: Si las coordenas de algún fixPoint no es válida
			//~ (good = false) se salta el resto de caso y vuelve al inicio
			//~ del ciclo para leer los datos del siguiente caso.
			if (!good)
				continue;

		}
		//~ End Fix Points

		//~ Aquí se manda aprocesar el mapa recien construido.
		procesar(mapa, f, c,salida);
		//~ print_map(mapa,f,c);

		//~ Liberamos la memoria de mapa.
		for (int i = 0; i < f; i++)
		{
			delete mapa[i];
		}
		delete mapa;
		//Fin - - -

		//~ Meter en una función:
		//~ Imprime mapa actual .
		//~ for (int i = f-1; i >= 0; i--)
		//~ {
			//~ cout << i << " | ";
			//~ for (int j = 0; j < c; j++)
			//~ {
				//~ cout << mapa[i][j] << " ";
			//~ }
			//~ cout << endl;
		//~ }
		//~ cout << "   ";
		//~ for (int i = 0; i < c; i++)
		//~ {
			//~ cout << " -";
		//~ }
		//~ cout << endl << "    " ;
		//~ for (int i = 0; i < c; i++)
		//~ {
			//~ cout << i << " ";
		//~ }
		//End Imprime

	}//Fin For principal

	//Cerramos los archivos de entrada y de salida.
	entrada.close();
	salida.close();

	return 0;
}
