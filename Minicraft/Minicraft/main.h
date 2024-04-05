#include <queue>
#include <mutex>
#pragma once
// Definir códigos de color ANSI
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
extern std::queue<std::string> commandQueue; // Cola para almacenar los comandos entrantes desde la consola
extern std::mutex queueMutex; // Mutex para proteger la cola de accesos concurrentes
extern bool showFps;
extern vec2 mousePos;
extern vec2 lastMousePos;
extern vec2 mouseDelta;
extern double currentTime;
extern bool leftClick;
extern double ellapsedTime;
extern bool mouseScalling;
extern bool mouseFoving;
