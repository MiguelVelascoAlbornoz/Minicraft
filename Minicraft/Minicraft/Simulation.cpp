#include "Simulation.h"

#include "main.h"




Simulation::Simulation(bool fullscreen)
{
    if (!initRenderer(fullscreen)) {
		running = false;
        return;
    }
    glfwSetWindowUserPointer(renderer->window,this);
    setup();
}

Simulation::~Simulation()
{
    delete renderer;
    delete camera;
    delete world;
}

bool Simulation::initRenderer(bool fullscreen)
{
    renderer = new Renderer();
    
    if (!renderer->initRenderer(fullscreen)) {
        return false;
    }

    glfwSetWindowUserPointer(renderer->window, this);


    camera = new Camera(vec3(0, 0,0), 1.0f, renderer->getWindowSize(),120);
    return true;
}
std::vector<Face> faces;
void Simulation::setup() {
    world = new World();
    world->createChunk();
    world->loadChunkData();
    float c = 0;
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            int height =100;
            if (j == 15 && i == 15) {
                int a;
            }
            std::cout << std::to_string(((c * 100) / (16 * 16 * float(height)))) << "%" << std::endl;
            for (int k = 0; k < height; ++k) {
             
                
                world->setBlockInChunk(i, k, j, NULL);
                c++;
            }
           
        }
    }
    world->clearTemporaryFaces();
    //world->moveTemporaryFaces();
   // world->printData();
    world->saveChunkData();
 

   
   
}
void Simulation::generateWorld() {

}
void Simulation::update() {
    manageInputs();
    updateLogic();
    updateRender();

}
void Simulation::updateLogic()
{
}
void Simulation::manageInputs() {
    vec3 cameraPos = camera->getPos();
    vec3 cameraDirection = camera->getDirection();
    float realSpeed = camera->speed * ellapsedTime;
    if (glfwGetKey(renderer->window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        realSpeed *= 5;
    }
    if (glfwGetKey(renderer->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        vec3 newPos(cameraPos.x,cameraPos.y+realSpeed,cameraPos.z);
        camera->setPos(newPos);
        cameraPos = camera->getPos();
    }
     if (glfwGetKey(renderer->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        vec3 newPos(cameraPos.x, cameraPos.y - realSpeed, cameraPos.z);
        camera->setPos(newPos);
        cameraPos = camera->getPos();
    }
     if (glfwGetKey(renderer->window, GLFW_KEY_W) == GLFW_PRESS) {
        vec3 newPos(cameraPos.x-sin(radians(cameraDirection.y))*realSpeed, cameraPos.y, cameraPos.z - cos(radians(cameraDirection.y)) * realSpeed);
        camera->setPos(newPos);
        cameraPos = camera->getPos();
    }
     if (glfwGetKey(renderer->window, GLFW_KEY_S) == GLFW_PRESS) {
        vec3 newPos(cameraPos.x + sin(radians(cameraDirection.y)) * realSpeed, cameraPos.y, cameraPos.z + cos(radians(cameraDirection.y)) * realSpeed);
        camera->setPos(newPos);
        cameraPos = camera->getPos();
    }
     if (glfwGetKey(renderer->window, GLFW_KEY_A) == GLFW_PRESS) {
        vec3 newPos(cameraPos.x - cos(radians(cameraDirection.y)) * realSpeed, cameraPos.y, cameraPos.z + sin(radians(cameraDirection.y)) * realSpeed);
        camera->setPos(newPos);
        cameraPos = camera->getPos();
    }
     if (glfwGetKey(renderer->window, GLFW_KEY_D) == GLFW_PRESS) {
        vec3 newPos(cameraPos.x + cos(radians(cameraDirection.y)) * realSpeed, cameraPos.y, cameraPos.z - sin(radians(cameraDirection.y)) * realSpeed);
        camera->setPos(newPos);
    }
   

}

void Simulation::updateRender() {
   
    camera->shader->uniform1f("time", currentTime);
    renderer->background(0.0f,0.0f,0.0f);//ponemos y limpiamos background
    renderer->fill(camera->shader, 1.0f, 1.0f, 0.0f);
    camera->bindShader();//activamos el uso del shader program



    
    world->renderBlocks();
   
    
    
   

    camera->unBindShader();//desactivamos el uso del shader

    renderer->update();//actualizamos ventana
}
