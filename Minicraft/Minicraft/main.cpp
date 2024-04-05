//#include "Simulation.hpp"
//Simulation* simulation; // Instancia de la clase Simulation
//VERSION 1.0
#include "Simulation.h"

#include "EventHandler.h"
#include <thread>
#include <windows.h>
#include "main.h"
#include "Commands.h"
#include <queue>
#include <mutex>
#include <bitset>


double endTime = 0;
double currentTime = 0;
double lastSecond = 0; //cuanta el tiempo en que se marco el ultimo segundo
double ellapsedTime = 0; ///tiempo entre un frame y otro
bool mouseScalling = false;
bool mouseFoving = false;
HWND consoleWindow; //Thread de la consola
static double fps = 0;
vec2 mousePos;
vec2 lastMousePos;
vec2 mouseDelta;
bool leftClick = false;
bool showFps = false;
Simulation* simulation;
using namespace std;

std::queue<std::string> commandQueue; // Cola para almacenar los comandos entrantes desde la consola
std::mutex queueMutex; // Mutex para proteger la cola de accesos concurrentes


void consoleThread() {
	string input;

	cout << "> ";
	while (true) {
		
			

		// Leer la entrada del usuario (incluyendo espacios en blanco)
		getline(cin, input);
		// Bloquear el acceso a la cola con un mutex mientras la modificamos
		{
			
			std::lock_guard<std::mutex> lock(queueMutex);
			commandQueue.push(input);
		}
	
	}
}

int main(int argc, char* argv[]) {
/*	std::vector<int>* array = new std::vector<int>[3];
	(*array)[0] = 1;
	(*array)[1] = 10;
	(*array)[2] = 10;

	uintptr_t direccion = reinterpret_cast<uintptr_t>(array);
	//direccion = 1;
	// Crear un array de 8 bytes
	uint8_t byte_array[8];

	// Copiar los bytes de la dirección de memoria en el array de bytes
	for (int i = 0; i < 8; ++i) {
		byte_array[i] = static_cast<uint8_t>((direccion >> (i * 8)) & 0xFF);
	}
	uintptr_t newPtr = 0;
	for (int i = 0; i < 8; ++i) {
		newPtr |= static_cast<uintptr_t>(byte_array[i]) << (i * 8);
	}
	std::vector<int>* recolect = reinterpret_cast<std::vector<int>*>(newPtr);
	std::cout << ((*recolect)[0]) << std::endl;
	delete[] array;
	
	//delete numero2;*/
   // consoleWindow = GetConsoleWindow();
	 simulation = new Simulation(false); // Crea una instancia de Simulation

	 glfwSetKeyCallback(simulation->getRenderer()->window, keyCallback);
	 glfwSetCursorPosCallback(simulation->getRenderer()->window, mouse_pos_callback);
	 glfwSetMouseButtonCallback(simulation->getRenderer()->window, mouse_button_callback);
	 glfwSetScrollCallback(simulation->getRenderer()->window, scroll_callback);

	std::thread console(consoleThread);
	console.detach();//para que la consola no crashee
	while (simulation->running && !glfwWindowShouldClose(simulation->getRenderer()->window)) {

		currentTime = (glfwGetTime());


		if (currentTime - lastSecond >= 1) {//On every second event
			 
			lastSecond = currentTime;
		    if (showFps)  std::cout << fps << std::endl;

		}
		mouseDelta.x = 0;
		mouseDelta.y = 0;
		glfwPollEvents();
	
		Commands::processCommands(simulation);

		simulation->update();



		//Update time
		endTime = glfwGetTime();
		ellapsedTime = endTime - currentTime;
		fps = 1 / ellapsedTime;

	}

	delete simulation; // Libera la memoria de la instancia de Simulation
	return 0;
}

