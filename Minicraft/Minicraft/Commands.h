#include "Camera.h"
#include <vector>

using namespace std;
class Commands {

public:
    static const int commandsCount = 16;

    const static string commands[Commands::commandsCount][2]; 
    static void scaleCommand(std::vector<std::string> input, Camera* camera) {

		if (input.size() == 1) {
			cout << YELLOW << "[Console] Command syntax ----> /tp (value > 0)" << RESET << endl;
		}
		else {
			float value = stof(input[1]);
			if (value > 0) {
				camera->setZoom(value);
				cout << BLUE <<"[Console] Succesfully scaled" << RESET << endl;
			}
			else {
				throw runtime_error("Invalid scale value");
			}

		}
    };
	
    static void tpCommand(std::vector<std::string> input, Camera* camera) {
		if (input.size() == 1) {
			cout << YELLOW << "[Console] Command syntax ----> /tp posX posY" << RESET << endl;
		}
		else {
			
			vec3 pos = getPosArgument(input, camera->getPos());
			camera->setPos(pos);
			cout << BLUE << "[Console] Succesfully teleported" << RESET << endl;

		}
   
    }
	static vec3 getPosArgument(std::vector<std::string> input, vec3 playerPos) {
		if (input.size() != 4) {
			throw std::runtime_error("Pos argument exception");
		}
		float pos[3];
		for (int i = 0; i < 3; ++i) {
			if (input[i + 1] == "~") {
				pos[i] = (playerPos[i]);
			}
			else {
				pos[i] = stof(input[i + 1]);
			}
		}
		return (vec3(pos[0],pos[1],pos[2]));
	}

	static void setFovCommand(std::vector<std::string> input, Camera* camera) {
		if (input.size() == 1) {
			cout << YELLOW << "[Console] Command syntax ----> /setFov value (0-180)"<< RESET << endl;
	
		}
		else {
			float value = stof(input[1]);
			if (value > 0 && value < 180) {
				camera->updateProjection(value,camera->getNear(),camera->getFar());
				cout << BLUE << "[Console] Projection succefully updated"<< RESET << endl;
			}
			else {
				throw runtime_error("Invalid fov value");
			}
	  // camera->updateProjection
		}
	}
	static void setDirectionCommand(std::vector<std::string> input, Camera* camera) {
		if (input.size() == 1) {
			cout << YELLOW <<"[Console] Command syntax ----> /setDirection xangle yangle zangle" << RESET << endl;
		}
		else {
			float x = stof(input[1]);
			float y = stof(input[2]);
			float z = stof(input[3]);
			vec3 direction(x,y,z);
			    camera->setDirection(direction);
				cout << BLUE <<"[Console] Direction succefully updated" << RESET <<endl;
		}
	}
	static void setSpeedCommand(std::vector<std::string> input, Camera* camera) {
		if (input.size() == 1) {
			cout << YELLOW<<"[Console] Command syntax ----> /setSpeed value > 0" << RESET << endl;
		}
		else {
			float value = stof(input[1]);
			if (value > 0) {
				camera->speed = value;
				cout << BLUE <<"[Console] Speed succefully updated" << RESET<< endl;
			}
			else {
				throw runtime_error("Invalid speed value");
			}
			// camera->updateProjection
		}
	}
	static void setSensibilityCommand(std::vector<std::string> input, Camera* camera) {
		if (input.size() == 1) {
			cout << YELLOW << "[Console] Command syntax ----> /setSensibility value > 0" << RESET << endl;
		}
		else {
			float value = stof(input[1]);
			if (value > 0) {
				camera->sensibility = value;
				cout << BLUE << "[Console] Sensibility succefully updated" << RESET << endl;
			}
			else {
				throw runtime_error("Invalid sensibility value");
			}
			// camera->updateProjection
		}
	}
	static void setNearCommand(std::vector<std::string> input, Camera* camera) {
		if (input.size() == 1) {
			cout << YELLOW <<"[Console] Command syntax ----> /setNear value" << RESET << endl;
		}
		else {
			float value = stof(input[1]);
			if (value > 0 && value < camera->getFar()) {
				camera->updateProjection(camera->getFov(), value, camera->getFar());
				cout << BLUE << "[Console] Projection succefully updated" << RESET << endl;
			}
			else {
				throw runtime_error("Invalid near value");
			}
				
			
		}
	}
	static void setFarCommand(std::vector<std::string> input, Camera* camera) {
		if (input.size() == 1) {
			cout << YELLOW <<"[Console] Command syntax ----> /setFar value "<< RESET << endl;
		}
		else {
			float value = stof(input[1]);
			if (value > 0 && value > camera->getNear()) {
				camera->updateProjection(camera->getFov(), camera->getNear(), value);
				cout << BLUE <<"[Console] Projection succefully updated"<<RESET << endl;
			}
			else {
				throw runtime_error("Invalid far value");
			}
				
			
			
		}
	}
	static void setBlockCommand(std::vector<std::string> input, World* world, Camera* camera) {
		if (input.size() == 1) {
			cout << YELLOW <<"[Console] Command syntax ----> /setBlock posX posY posZ "<<RESET << endl;
		}
		else {
			double starTime = glfwGetTime();
			vec3 pos = getPosArgument(input, camera->getPos());
			starTime = glfwGetTime();
			world->loadChunkData();
			std::cout << "loaddata:" << glfwGetTime() - starTime << std::endl;

			starTime = glfwGetTime();
			bool succesfully = world->setBlockInChunk(floor(pos.x),ceil(pos.y),floor(pos.z),NULL);
			std::cout << "total:" << glfwGetTime() - starTime << std::endl;

			starTime = glfwGetTime();
			world->saveChunkData();


			std::cout << "saveData:" << glfwGetTime() - starTime << std::endl;

			if (succesfully) {
				cout << BLUE <<"[Console] world succefully updated" << RESET<<endl;
			}
			else {
				throw runtime_error("World couldnt be updated");
			}
			


		}
	}
	static void processCommands(Simulation* simulation) {
		while (true) { // Mientras no se indique que se debe detener
			if (!commandQueue.empty()) {
				std::string command;
				{
					std::lock_guard<std::mutex> lock(queueMutex);
					command = commandQueue.front();
					commandQueue.pop();
				}
				if (!command.empty() && command[0] == '/') {//preguntamos si uso /
					try {
						command.erase(0, 1);
						if (command.empty()) {
							throw runtime_error("Empty command");
						}
						std::vector<std::string> inputSplited = Commands::split(command, " ");

						//if de preguntar que comandos es
						if ((inputSplited[0]) == Commands::commands[0][0]) {//TP COMMAND
							Commands::tpCommand(inputSplited,simulation->camera);
						}
						else if (inputSplited[0] == Commands::commands[1][0]) { //HELP COMMAND
							for (int i = 0; i < Commands::commandsCount; ++i) {
								cout << " ----> /" << YELLOW << Commands::commands[i][0] + " - " + Commands::commands[i][1] << RESET << endl;
							}
						}
						else if (inputSplited[0] == Commands::commands[2][0]) { // SCALE COMMAND
							Commands::scaleCommand(inputSplited,simulation->camera);
						}
						else if (inputSplited[0] == Commands::commands[3][0]) { //show fps
							showFps = !showFps;
							cout << BLUE <<"[Console] rule succesfully updated" << RESET <<endl;
						}
						else if (inputSplited[0] == Commands::commands[4][0]) {
							Commands::setFovCommand(inputSplited,simulation->camera);
						}
						else if (inputSplited[0] == Commands::commands[5][0]) {
							Commands::setNearCommand(inputSplited, simulation->camera);
						}
						else if (inputSplited[0] == Commands::commands[6][0]) {
							Commands::setFarCommand(inputSplited, simulation->camera);
						}
						else if (inputSplited[0] == Commands::commands[7][0]) {
							Commands::setSpeedCommand(inputSplited, simulation->camera);
						}
						else if (inputSplited[0] == Commands::commands[8][0]) {
							Commands::setDirectionCommand(inputSplited, simulation->camera);
						}
						else if (inputSplited[0] == Commands::commands[9][0]) {
							Commands::setSensibilityCommand(inputSplited, simulation->camera);
						}
						else if (inputSplited[0] == Commands::commands[10][0]) {
							simulation->camera->shader->loadShader();
							cout << BLUE <<"[Console] Shader succesfully reloaded"<< RESET << endl;
						}
						else if (inputSplited[0] == Commands::commands[11][0]) {
							mouseScalling = !mouseScalling;
							cout << BLUE << "[Console] rule succesfully updated" << RESET<< endl;
						}
						else if (inputSplited[0] == Commands::commands[12][0]) {
							mouseFoving = !mouseFoving;
							cout << BLUE << "[Console] rule succesfully updated" << RESET << endl;
						}
						else if (inputSplited[0] == Commands::commands[13][0]) {
							setBlockCommand(inputSplited, simulation->world,simulation->camera);

						}
						else if (inputSplited[0] == Commands::commands[14][0]) {
							Camera* camera = simulation->camera;
							vec3 cameraPos = camera->getPos();
							cout << "[Console] Your world pos is (" << GREEN << to_string(cameraPos.x) << RESET << "," << GREEN << to_string(cameraPos.y) << RESET << "," << GREEN << to_string(cameraPos.z) << RESET <<")" << endl;
						}
						else if (inputSplited[0] == Commands::commands[15][0]) {
							Camera* camera = simulation->camera;
							vec3 cameraPos = camera->getPos();
							vec3 cameraDirection = camera->getDirection();
							cout << "[Console] Camera settings: " << endl;
							cout << "---> Fov: " << GREEN << to_string(camera->getFov()) + "°" << RESET << endl;
							cout << "---> Pos: (" << GREEN << to_string(cameraPos.x) << RESET << ", " << GREEN << to_string(cameraPos.y) << RESET << ", " << GREEN << to_string(cameraPos.z) << RESET <<")" << endl;
							cout << "---> Direction: (" << GREEN << to_string(cameraDirection.x) + "°" << RESET << ", " << GREEN << to_string(cameraDirection.y) + "°" << RESET << ", " << GREEN << to_string(cameraDirection.z) + "°" << RESET << ")" << endl;
							cout << "---> Near: " << GREEN << to_string(camera->getNear())  << RESET << endl;
							cout << "---> Far: " << GREEN << to_string(camera->getFar())  << RESET << endl;
							cout << "---> Scale: " << GREEN << to_string(camera->getZoom())  << RESET << endl;
							cout << "---> Sensibility: " << GREEN << to_string(camera->sensibility)  << RESET << endl;
							cout << "---> Speed: " << GREEN << to_string(camera->speed) << RESET << endl;
						
							
						}
						else {  /*termina el if de preguntar que comando es*/
							cout << RED << "[Console] command cannot be ressolved" << RESET << endl;
						} 
					}
					catch (exception& e) {
						cout << RED << "[Console] " << e.what() << RESET << endl;
					}
				}
				else { //caso no usar     -> /
					cout << RED << "[Console] Invalid syntax, commands must start with (/)" << RESET  << endl;

				}
				cout << "> ";
			} /*caso no halla nada en cola */else {
				break;
			}
		}
	}
    static std::vector<std::string> split(std::string& text,const char* split) {
      
        size_t size = text.size();
        std::vector<std::string> strings;
        int copyStart = 0;
        for (int i = 0; i < size; ++i) {
            if (text[i] == *split) {
                std::vector<char> chars;
                for (int j = copyStart; j < i; ++j) {
                    chars.push_back(text[j]);
                }
                
                if (chars.size() != 0) {
                    chars.push_back('\0');
                    std::string s(chars.data());
                    strings.push_back(s);
                }
               
                copyStart = i + 1;

            }
           
               
        }
        std::vector<char> chars;
        for (int j = copyStart; j < size;++j) {
            chars.push_back(text[j]);
        }
        if (chars.size() != 0) {
            chars.push_back('\0');
            std::string s(chars.data());
            strings.push_back(s);

        }
        return strings;
    }
};
const std::string Commands::commands[Commands::commandsCount][2] {
    {"tp", "teleport the camera"},
    { "help","Say the list of commands" },
    { "scale", "changes de zoom of the camera" },
    { "fps", "toggles fps showing" },
	{ "setFov", "Adjust camera fov"},
	{ "setNear", "ADjusts the minimum distance to render"},
	{ "setFar", "Adjusts the maximum distance to render"},
	{ "setSpeed", "Adjusts movement speed"},
	{ "setDirection", "Adjusts camera facing"},
	{ "setSensibility", "Adjusts camera sensibility"},
	{ "reloadShader", "reloads shaders"},
	{ "toggleMouseScalling", "enables/dissables mouseScalling"},
	{ "toggleMouseFoving", "enables/dissables mouseFoving"},
    { "setBlock", "set a block in ther world in determinated position"},
    { "getPos", "return your pos in the world"},
	{ "getSettings", "return yout cameraSettings"}};
