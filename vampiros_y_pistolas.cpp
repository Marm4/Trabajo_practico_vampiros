#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <sstream>
#include <iomanip>

using namespace std;

#define ERROR -1
#define EXITO 0

typedef struct elemento{
	string nombre;
	int posicionX;
	int posicionY;
	int cantidad;
	elemento *siguiente;
}elemento_t;

typedef struct tipo_elemento{
		elemento *humano;
		elemento *zombie;
		elemento *vampiro;
		elemento *varios;
}tipo_elemento_t;

elemento_t* copiar_datos(string nombre, string coordenadaX_string, string coordenadaY_string, string cantidad){
	int coordenadaX, coordenadaY;

	stringstream geek(coordenadaX_string);	//
	geek >> coordenadaX;				//  Casteo los strings a INT.
							//
	stringstream geek2(coordenadaY_string);	//
	geek2 >> coordenadaY;


	elemento_t *nuevo_elemento = new elemento_t [sizeof(elemento_t)];
	if(!nuevo_elemento) return NULL;

	if(nombre == "humano" && cantidad == "CV ")
		nombre = "humano CV";


	nuevo_elemento->nombre = nombre;
	nuevo_elemento->posicionX = coordenadaX;
	nuevo_elemento->posicionY = coordenadaY;

	if(nombre == "agua" || nombre == "bala"){
		int cantidad_aux;

		stringstream geek3(cantidad);
		geek3 >> cantidad_aux;

		nuevo_elemento->cantidad = cantidad_aux;
	}
	else
		nuevo_elemento->cantidad = 1; //->cantidad solamente sirve para los elementos bala & agua

	nuevo_elemento->siguiente = NULL;
	return nuevo_elemento;
}

elemento_t* procesar_datos(char *leer){
	string nombre;
	string coordenadaX;
	string coordenadaY;
	string cantidad;
	char corte[] = " ";
	int contador = 0;

	while(leer[contador] != corte[0]){
			nombre =  nombre + leer[contador];
			contador++;
	}

	contador++;
	corte[0] = '(';
	while(leer[contador] != corte[0]){
		cantidad =  cantidad + leer[contador];
		contador++;
	}

	contador++;
	corte[0] = ',';
	while(leer[contador] != corte[0]){
			coordenadaX =  coordenadaX + leer[contador];
			contador++;
	}

	contador = contador+2;
	corte[0] = ')';
	while(leer[contador] != corte[0]){
				coordenadaY =  coordenadaY + leer[contador];
				contador++;
	}

	return copiar_datos(nombre, coordenadaX, coordenadaY, cantidad);
}

elemento_t* leer_archivo(int &elementos_almacenados){
	string leer;
	char leerChar[100];

	ifstream archivo;
	archivo.open("estado.txt", ios::in); //Abrimos el archivo en modo Lectura.

	if(archivo.fail()){
		cout<<"No se puedo abrir el archivo\n";
		return NULL;
	}
	
	for(int i = 0; i<=elementos_almacenados; i++)
		getline(archivo, leer);


	if(leer == "\0")
		return NULL;

	strcpy(leerChar, leer.c_str()); // Casteo de String a Char

	return procesar_datos(leerChar);
}

bool comprobar_superposicion(elemento_t *primer_nodo, elemento_t* nuevo_elemento){
	if((primer_nodo->posicionX == nuevo_elemento->posicionX) && (primer_nodo->posicionY == nuevo_elemento->posicionY))
		return true;
	
	return false;
}

void agregar_elemento(elemento_t *primer_nodo, elemento_t* nuevo_elemento){
	bool superposicion = false;
	string nombre = nuevo_elemento->nombre;

	while(primer_nodo->siguiente != NULL){
		primer_nodo = primer_nodo->siguiente;

		if(nombre == "humano" || nombre == "humano CV" || nombre == "Vanesa" || nombre == "zombi" || nombre == "vampiro" || nombre == "Nosferatu" || nombre == "Vampirella")
			superposicion = comprobar_superposicion(primer_nodo, nuevo_elemento);
		}
		
	if(!superposicion)
		primer_nodo->siguiente = nuevo_elemento;
}

int crear_elementos(tipo_elemento_t* elementos, int &elementos_almacenados){	
	elemento_t* nuevo_elemento = leer_archivo(elementos_almacenados);
	if(!nuevo_elemento)
		return EXITO;
		
	string nombre = nuevo_elemento->nombre;

	if(nombre == "humano" || nombre == "humano CV" || nombre == "Vanesa"){
		if(!elementos->humano)
			elementos->humano = nuevo_elemento;
		else
			agregar_elemento(elementos->humano, nuevo_elemento);
	}

	else if(nombre == "zombi"){
		if(!elementos->zombie)
			elementos->zombie = nuevo_elemento;
		else
			agregar_elemento(elementos->zombie, nuevo_elemento);
	}

	else if(nombre == "vampiro" || nombre == "Nosferatu" || nombre == "Vampirella"){
		if(!elementos->vampiro)
			elementos->vampiro = nuevo_elemento;
		else
			agregar_elemento(elementos->vampiro, nuevo_elemento);
	}
	
	else{
		if(!elementos->varios)
			elementos->varios = nuevo_elemento;
		else
			agregar_elemento(elementos->varios, nuevo_elemento);
	}
	
	elementos_almacenados++;
	return crear_elementos(elementos, elementos_almacenados);
}

double contador_elementos(elemento_t* elemento_contador){
	double contador=0;

	if(!elemento_contador)	return contador;
	else	contador++;

	while(elemento_contador->siguiente){
		contador++;
		elemento_contador = elemento_contador->siguiente;
	}
	return contador;
}

double calcular_porcentaje(tipo_elemento_t* elemento_calcular, string tipo){
	double porcentaje = 0;
	if(tipo == "humano")
		porcentaje = contador_elementos(elemento_calcular->humano)/(contador_elementos(elemento_calcular->humano) + contador_elementos(elemento_calcular->zombie) + contador_elementos(elemento_calcular->vampiro));

	else if(tipo == "zombie")
		porcentaje = contador_elementos(elemento_calcular->zombie)/(contador_elementos(elemento_calcular->humano) + contador_elementos(elemento_calcular->zombie) + contador_elementos(elemento_calcular->vampiro));

	else if(tipo == "vampiro")
		porcentaje = contador_elementos(elemento_calcular->vampiro)/(contador_elementos(elemento_calcular->humano) + contador_elementos(elemento_calcular->zombie) + contador_elementos(elemento_calcular->vampiro));

	return porcentaje*100;
}

void mostrar_varios(elemento_t* varios){
	double agua = 0;
	double cruz = 0;
	double bala = 0;
	double estaca = 0;
	double escopeta = 0;
	double porcentaje = 0;

	while(varios){
		if(varios->nombre == "agua")
			agua = agua + varios->cantidad;

		else if(varios->nombre == "cruz")
			cruz++;

		else if(varios->nombre == "bala")
			bala = bala + varios->cantidad;

		else if(varios->nombre == "estaca")
			estaca++;

		else if(varios->nombre == "escopeta")
			escopeta++;

		varios = varios->siguiente;
	}

	porcentaje = (agua/(agua+cruz+estaca))*100;
	cout<<"\nAgua Bendita/"<<agua<<"/"<<porcentaje<<endl;

	porcentaje = (cruz/(agua+cruz+estaca))*100;
	cout<<"\nCruces/"<<cruz<<"/"<<porcentaje<<endl;

	porcentaje = (estaca/(agua+cruz+estaca))*100;
	cout<<"\nEstacas/"<<estaca<<"/"<<porcentaje<<endl;

	porcentaje = (escopeta/bala)*100;
	cout<<"\nEscopetas/"<<escopeta<<"/"<<porcentaje<<endl;

	porcentaje = (bala/escopeta)*100;
	cout<<"\nBalas/"<<bala<<"/"<<porcentaje<<endl;


}

void mostrar_tabla(tipo_elemento_t* elementos){
	cout<<"\n\tELEMENTOS/CANTIDAD/PORCENTAJE"<<endl;
	if(elementos->humano)
		cout<<"\nHUMANOS/"<<std::fixed << std::setprecision(1)<<contador_elementos(elementos->humano)<<"/"<<calcular_porcentaje(elementos, "humano")<<endl;

	if(elementos->zombie)
		cout<<"\nZOMBIES/"<<contador_elementos(elementos->zombie)<<"/"<<calcular_porcentaje(elementos, "zombie")<<endl;

	if(elementos->vampiro)
		cout<<"\nVAMPIROS/"<<contador_elementos(elementos->vampiro)<<"/"<<calcular_porcentaje(elementos, "vampiro")<<endl;

	if(elementos->varios)
		mostrar_varios(elementos->varios);
}

string calcular_cuadrante(elemento_t* elemento){
	int x = elemento->posicionX; // Para acortar el codigo

	int y = elemento->posicionY; //

	if((x>0 && x<=500) && (y>0 && y<=300))
		return "NO";

	else if ((x>500 && x<=1000) && (y>0 && y<=300))
		return "NE";

	else if ((x>0 && x<=500) && (y>300 && y<=600))
		return "SO";

	else if((x>500 && x<=1000) && (y>300 && y<=600))
		return "SE";

	return "No se encuentra dentro del cudrante";
}

void imprimir_varios(elemento_t* elemento){
	if(!elemento) return;

	else if((elemento->nombre == "agua") || (elemento->nombre == "bala"))
		cout<<"\n\tNombre: "<<elemento->nombre<<" Cantidad: "<<elemento->cantidad<<" Ubicacion X: "<<elemento->posicionX<<" y: "<<elemento->posicionY<<" cuadrante: "<<calcular_cuadrante(elemento)<<endl;

	else
		cout<<"\n\tNombre: "<<elemento->nombre<<" Ubicacion X: "<<elemento->posicionX<<" y: "<<elemento->posicionY<<" cuadrante: "<<calcular_cuadrante(elemento)<<endl;

	imprimir_varios(elemento->siguiente);
}

void imprimir_ubicacion_distinto(elemento_t* elemento, string tipo){
	if(!elemento) return;

	if(elemento->nombre == tipo)
		cout<<"\n\tUbicacion X: "<<elemento->posicionX<<" y: "<<elemento->posicionY<<" cuadrante: "<<calcular_cuadrante(elemento)<<endl;

	imprimir_ubicacion_distinto(elemento->siguiente, tipo);
}

void imprimir_ubicacion(elemento_t* elemento, string tipo){
	if(tipo == "todos"){
		while(elemento){
			cout<<"\n\tUbicacion X: "<<elemento->posicionX<<" y: "<<elemento->posicionY<<" cuadrante: "<<calcular_cuadrante(elemento)<<endl;
			elemento = elemento->siguiente;
		}
	}

	else if(tipo == "vanesa")
		imprimir_ubicacion_distinto(elemento, "Vanesa");

	else if(tipo == "humano CV")
		imprimir_ubicacion_distinto(elemento, "humano CV");

	else if(tipo == "Nosferatu")
		imprimir_ubicacion_distinto(elemento, "Nosferatu");

	else if(tipo == "Vampirella")
		imprimir_ubicacion_distinto(elemento, "Vampirella");

	else if(tipo == "varios")
		imprimir_varios(elemento);

}

void mostrar_ubicacion(elemento_t* elemento, string tipo){
	int numero_ingresado;

	if(tipo == "humano"){
		cout<<"\n\n\t1.Todos los humanos \n\t2.Vanesa \n\t3.Caza Vampiros"<<endl;
		cin >> numero_ingresado;

		switch(numero_ingresado){
			case 1: imprimir_ubicacion(elemento, "todos");
			break;

			case 2: imprimir_ubicacion(elemento, "vanesa");
			break;

			case 3: imprimir_ubicacion(elemento, "humano CV");
			break;
			}
	}

	else if(tipo == "zombie")
		imprimir_ubicacion(elemento, "todos");

	else if(tipo == "vampiro"){
		cout<<"\n\n\t1.Todos los vampiros\n\t2.Nosferatu\n\t3.Vampirella"<<endl;
		cin>>numero_ingresado;

		switch(numero_ingresado){
				case 1: imprimir_ubicacion(elemento, "todos");
				break;

				case 2: imprimir_ubicacion(elemento, "Nosferatu");
				break;

				case 3: imprimir_ubicacion(elemento, "Vampirella");
				break;
		}
	}

	else if(tipo == "varios")
		imprimir_ubicacion(elemento, "varios");
}

void consultar_ubicacion(tipo_elemento_t* elementos){
	int numero_ingresado;

	cout<<"\n\nQue elemento desea buscar?\n\n\t1.Humanos\n\t2.Zombies\n\t3.Vamprios\n\t4.Elementos Varios\n\n\t0.Regresar al menu"<<endl;
	cin>>numero_ingresado;

	switch(numero_ingresado){
		case 1:mostrar_ubicacion(elementos->humano, "humano");
		break;

		case 2:mostrar_ubicacion(elementos->zombie, "zombie");
		break;

		case 3:mostrar_ubicacion(elementos->vampiro, "vampiro");
		break;

		case 4:mostrar_ubicacion(elementos->varios, "varios");
		break;

		case 0: return;
	}
	consultar_ubicacion(elementos);
}

void menu(tipo_elemento_t* elementos){
	int numero_ingresado;

	cout<<"\n\nBienvenido al menu de usuario\nIngrese un numero para continuar\n"<<endl;
	cout<<"\t1.Mostrar tabla (elementos, cantidades, porcentajes)\n\t2.Consultar sobre ubicacion de elementos\n\t"<<endl;
	cout<<"\t0.Terminar programa"<<endl;
	cin>>numero_ingresado;

	switch(numero_ingresado){
		case 1: mostrar_tabla(elementos);
		break;

		case 2: consultar_ubicacion(elementos);
		break;

		case 0: return;
		break;
	}

	menu(elementos);
}

tipo_elemento_t* nuevo_elementos(){
	tipo_elemento_t* elementos = new tipo_elemento_t[sizeof(tipo_elemento_t)];
	elementos->humano = NULL;
	elementos->zombie = NULL;
	elementos->vampiro = NULL;
	elementos->varios = NULL;
	
	return elementos;
}

void borrar(elemento_t* elemento){
	if(!elemento)
		return;

	borrar(elemento->siguiente);
	delete[](elemento);
}

void liberar_memoria(tipo_elemento_t* elementos){
	borrar(elementos->humano);
	borrar(elementos->zombie);
	borrar(elementos->vampiro);
	borrar(elementos->varios);
	delete(elementos);
}

int main(){
	int elementos_almacenados = 0;
	tipo_elemento_t* elementos = nuevo_elementos();


	crear_elementos(elementos, elementos_almacenados);
	menu(elementos);

	liberar_memoria(elementos);
	return 0;
}

