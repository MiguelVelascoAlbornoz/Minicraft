#include "World.h"
#include <fstream> // Para std::ofstream
#include <filesystem> // Para std::filesystem
#include <array>
#include "main.h"

World::World() {
    std::remove("world.wld");

}

bool World::setBlockInChunk(uint8_t posX, uint8_t posY, uint8_t posZ, Block* block) {
    if (counterToFreefaces >= counterToFreefacesLimit) {
        //clearTemporaryFaces();
        counterToFreefaces = 0;
    }
    //Verifica si los valores de la posicion son validos

    if (posZ > 15 || posX > 15 || posY > 254 ) {
       return false;
    }
    //anhade el bloque a ser renderizado con sus 6 caras // Verifica si el archivo del mundo existe
    bool sameXZ = (posX == lastXpos && posZ == lastZpos);
   

   

    // Lee todos los datos del archivo
    

    //La posicion en la cual se encuentra el primer byte que guarda la informacion del X y Z en cuestion
    int headerBytePos;
    uint8_t headerByteValue;
    

    int miniChunkLength;
    int nextHeaderPos;
    if (sameXZ) {

        headerBytePos = lastHeaderBytePos;
        headerByteValue = (*data)[headerBytePos];
        miniChunkLength = headerByteValue * blockInformationSize + 2;
        nextHeaderPos = headerBytePos + miniChunkLength;

    }
    else {

        headerBytePos = 0;
        headerByteValue = (*data)[headerBytePos];
        int headerNr = posX + 16 * posZ;
        miniChunkLength = headerByteValue * blockInformationSize + 2;
        nextHeaderPos = headerBytePos + miniChunkLength;
        for (int i = 0; i < headerNr; ++i) {

            headerBytePos += headerByteValue * blockInformationSize + 2;//Para adelantar al siguiente header lo que se hace es tomar el header actual, sumar el numero de bloques que tiene y multiplicarlo por 4 ya que cada bloque pesa 4 bytes y sumar uno
            headerByteValue = (*data)[headerBytePos]; //Obtenemos el valor del header


        }
        //Valor de ese byte (Representa cuantos bloques hay en el X y Z en cuestion a lo largo de la posicion Y)
        miniChunkLength = headerByteValue * blockInformationSize + 2;
        nextHeaderPos = headerBytePos + miniChunkLength;
       
    }

  
    std::vector<Face*> facesToRender = getBlockRender(posX,posY,posZ);

    if (headerByteValue == 0) {//Esto es cuando no hay ningun bloque a lo largo del eje Y en el X y Z en cuestion
     
      
        insertBlock(headerBytePos + 1, posY, 1, &facesToRender);//Simplemente se añade al array de bytes un bloque en la posicion del header + 1
        (*data)[headerBytePos] = uint8_t(1);//Actualizamos el valor del header
        headerByteValue =1;
        miniChunkLength = headerByteValue * blockInformationSize + 2;
        nextHeaderPos = headerBytePos + miniChunkLength;
        (*data)[nextHeaderPos-1] = uint8_t(1);//Actualizamos el valor del header

    }
    else {
        
        int startIndex = 0;
        int endIndex = headerByteValue-1;
        int mediumIndex;
        uint8_t mediumPosY;

            while (startIndex <= endIndex) {
                if (startIndex == endIndex) {
                    mediumPosY = (*data)[headerBytePos + startIndex * blockInformationSize + 2];
                    if (mediumPosY > posY) {
                        mediumIndex = startIndex;
                        break;
                    }
                    else {
                        mediumIndex = startIndex+1;
                        break;
                    }
                }
                mediumIndex =(startIndex + endIndex) / 2;
                mediumPosY = (*data)[headerBytePos + mediumIndex * blockInformationSize + 2];
                if (mediumPosY < posY) {
                    startIndex = mediumIndex + 1;
                }
                else if (mediumPosY == posY) {
                    mediumIndex = -2;
                    break;
                }
                else {
                    endIndex = mediumIndex - 1;
                }
            
        }

        int insertionPos = headerBytePos+mediumIndex*blockInformationSize+1;
       if (mediumIndex == -2) {//Cuando hay que sobreescribir un bloque
            return false;
        }
 
        insertBlock(insertionPos, posY, 1,&facesToRender);//insertamos un bloque la posicion al final del todo
        
        headerByteValue++;
        miniChunkLength = headerByteValue * blockInformationSize + 2;
        nextHeaderPos = headerBytePos + miniChunkLength;
        (*data)[headerBytePos] = headerByteValue;
        (*data)[nextHeaderPos-1] = headerByteValue;


        int downBlockBytePos = insertionPos  - blockInformationSize;
        if (downBlockBytePos >= 0 && posY == (*data)[ 1 + downBlockBytePos] + 1) {
           
            facesToRender[1]->shouldDestroy = true;
            uintptr_t  pointer = 0;


            // Leer 8 bytes de *data e insertarlos en pointer
            for (int j = 0; j < 8; ++j) {
                // Desplazar los bytes hacia la izquierda y combinarlos en un número de 64 bits
                pointer |= static_cast<uintptr_t>((*data)[downBlockBytePos + 2 + j]) << (j * 8);
            }
            Face* face = reinterpret_cast<Face*>(pointer);
            if (face) {
                face->shouldDestroy = true;
            } else {
                std::cout << "Face not able to find testing down in " +std::to_string(posX) +"-"+ std::to_string(posY) +"-"+ std::to_string(posZ)  << std::endl;
                return false;
            }
 
        
            
        }
    
        if (posY == mediumPosY - 1) {
       
            facesToRender[0]->shouldDestroy = true;
            uintptr_t  pointer = 0;


            // Leer 8 bytes de *data e insertarlos en pointer
            for (int j = 0; j < 8; ++j) {
                // Desplazar los bytes hacia la izquierda y combinarlos en un número de 64 bits
                pointer |= static_cast<uintptr_t>((*data)[insertionPos+blockInformationSize +10 + j]) << (j * 8);//+10 por que 10 = 8 + 2
            }
            Face* face = reinterpret_cast<Face*>(pointer);
            if (face) {
                face->shouldDestroy = true;
            }
            else {
                std::cout << "Face not able to find testing up in " + std::to_string(posX) + "-" + std::to_string(posY) + "-" + std::to_string(posZ) << std::endl;
                return false;
                
            }

        }
       
    }

    // este y oeste
    if (posX < 15) { //Comprobamos la posicion en el oeste

         uint8_t eastBlockHeaderPosValue = (*data)[nextHeaderPos];
        if (eastBlockHeaderPosValue != 0) {
           
            int index = findBetweenBlocks(eastBlockHeaderPosValue,posY,nextHeaderPos);
            if (index != -1) {
                facesToRender[5]->shouldDestroy = true;
                uintptr_t  pointer = 0;

                int sumatorium = nextHeaderPos + blockInformationSize * index + 8 * 4 + 3;
                // Leer 8 bytes de *data e insertarlos en pointer
                for (int j = 0; j < 8; ++j) {
                    // Desplazar los bytes hacia la izquierda y combinarlos en un número de 64 bits
                    pointer |= static_cast<uintptr_t>((*data)[sumatorium + j]) << (j * 8);// 1+8*4 
                }
                Face* face = reinterpret_cast<Face*>(pointer);
                if (face) {
                    face->shouldDestroy = true;
                }
                else {
                    std::cout << "Face not able to find testing east in " + std::to_string(posX) + "-" + std::to_string(posY) + "-" + std::to_string(posZ) << std::endl;
                    return false;
                    
                }
           }          
        }
    }

    if (posX > 0) { //Comprobamos la posicion en el oeste
        int westBlockFooterPos = headerBytePos - 1;
        uint8_t westBlockFooterPosValue = (*data)[westBlockFooterPos];
        if (westBlockFooterPosValue != 0) {
            int westBlockHeaderPos = westBlockFooterPos- blockInformationSize * westBlockFooterPosValue -1;
           
            uint8_t index = findBetweenBlocks(westBlockFooterPosValue, posY, westBlockHeaderPos);
            if (index != -1) {
                facesToRender[4]->shouldDestroy = true;
                uintptr_t  pointer = 0;

                int sumatorium = westBlockHeaderPos + blockInformationSize * index + 8 * 5 + 3;
                // Leer 8 bytes de *data e insertarlos en pointer
                for (int j = 0; j < 8; ++j) {
                    // Desplazar los bytes hacia la izquierda y combinarlos en un número de 64 bits
                    pointer |= static_cast<uintptr_t>((*data)[sumatorium + j]) << (j * 8);
                }
                Face* face = reinterpret_cast<Face*>(pointer);
                if (face) {
                    face->shouldDestroy = true;
                }
                else {
                    std::cout << "Face not able to find testing west in " + std::to_string(posX) + "-" + std::to_string(posY) + "-" + std::to_string(posZ) << std::endl;
                    return false;

                }

             }
        
        }
    }

    if (posZ > 0) {//Comprobamos al sur
        int southHeaderPos;
        uint8_t southHeaderPosValue;
        if (sameXZ) {
            southHeaderPos = lastSouthHeaderPos;
            southHeaderPosValue = (*data)[southHeaderPos];
        }
        else {
            int southFooterPos = headerBytePos - 1;
            int headerCounter = 0;
      
            while (headerCounter != 15) {
                southHeaderPosValue = (*data)[southFooterPos];
                southFooterPos -= southHeaderPosValue * blockInformationSize + 2;
                headerCounter++;
            }
            southHeaderPosValue = (*data)[southFooterPos];
            southHeaderPos = southFooterPos - southHeaderPosValue * blockInformationSize - 1;
        }
       
        
        if (southHeaderPosValue != 0) {
            int index = findBetweenBlocks(southHeaderPosValue,posY,southHeaderPos);
            if (index != -1) {
                int southBlockEndPos = southHeaderPosValue * blockInformationSize + southHeaderPos + 2;

                facesToRender[2]->shouldDestroy = true;
                uintptr_t  pointer = 0;

                int sumatorium = southHeaderPos + blockInformationSize * index + 8 * 3 + 3;
                // Leer 8 bytes de *data e insertarlos en pointer
                for (int j = 0; j < 8; ++j) {
                    // Desplazar los bytes hacia la izquierda y combinarlos en un número de 64 bits
                    pointer |= static_cast<uintptr_t>((*data)[sumatorium + j]) << (j * 8);//1 + 8*3
                }
                Face* face = reinterpret_cast<Face*>(pointer);
                if (face) {
                    face->shouldDestroy = true;
                }
                else {
                    std::cout << "Face not able to find south east in " + std::to_string(posX) + "-" + std::to_string(posY) + "-" + std::to_string(posZ) << std::endl;
                    return false;

                }
            }

        }
        lastSouthHeaderPos = southHeaderPos;
    }

     if (posZ < 15) {//Comprobamos al norte
         uint8_t northHeaderPosValue;
         int northHeaderPos = headerBytePos;
         int headerCounter = 0;
         if (sameXZ) {
             northHeaderPos = lastNorthHeaderPos + blockInformationSize;
             northHeaderPosValue = (*data)[northHeaderPos];
         }
         else {
             while (headerCounter != 16) {
                 northHeaderPosValue = (*data)[northHeaderPos];
                 northHeaderPos += northHeaderPosValue * blockInformationSize + 2;
                 headerCounter++;
             }
             northHeaderPosValue = (*data)[northHeaderPos];

         }
       if (northHeaderPosValue != 0) {
           int index = findBetweenBlocks(northHeaderPosValue,posY,northHeaderPos);
           if (index != -1) {
               facesToRender[3]->shouldDestroy = true;
       
               uintptr_t  pointer = 0;

               int sumatorium = northHeaderPos + blockInformationSize * index + 8 * 2 + 3;
               // Leer 8 bytes de *data e insertarlos en pointer
               for (int j = 0; j < 8; ++j) {
                   // Desplazar los bytes hacia la izquierda y combinarlos en un número de 64 bits
                   pointer |= static_cast<uintptr_t>((*data)[sumatorium + j]) << (j * 8);
               }
               Face* face = reinterpret_cast<Face*>(pointer);
               if (face) {
                   face->shouldDestroy = true;
               }
               else {
                   std::cout << "Face not able to find testing north in " + std::to_string(posX) + "-" + std::to_string(posY) + "-" + std::to_string(posZ) << std::endl;
                   return false;

               }
           }             
        }
        lastNorthHeaderPos = northHeaderPos;
    }

for (int i = 0; i < 6; ++i) {
    
    temporaryFaces.push_back(facesToRender[i]);
     
    }   
lastHeaderBytePos = headerBytePos;
lastXpos = posX;
lastZpos = posZ;
counterToFreefaces++;
facesToRender.clear();
    return true;
}
int World::findBetweenBlocks(uint8_t blockHeaderPosValue, uint8_t posY, int headerPos) {
    uint8_t mediumIndex;
    uint8_t startIndex = 0;
    uint8_t endIndex = blockHeaderPosValue - 1;
    while (startIndex <= endIndex) {
        mediumIndex = (startIndex + endIndex) / 2;
        uint8_t mediumIndexPosY = (*data)[mediumIndex * blockInformationSize + 2 + headerPos];
        if (mediumIndexPosY < posY) {
            startIndex = mediumIndex + 1;
        }
        else if (mediumIndexPosY > posY) {
            endIndex = mediumIndex - 1;
        }
        else {
            return mediumIndex;
          
        }
    }
        return -1;
}
void World::moveTemporaryFaces() {
    clearTemporaryFaces();

    faces.reserve(faces.size() + temporaryFaces.size()); // Reservar espacio en faces si es posible

    // Mover todos los elementos de temporaryFaces a faces
    for (auto& face : temporaryFaces) {
        faces.push_back(std::move(face));
    }

    // Limpiar temporaryFaces
    temporaryFaces.clear();
}
void World::clearTemporaryFaces() {


    // Eliminar elementos que deben ser destruidos
    temporaryFaces.erase(std::remove_if(temporaryFaces.begin(), temporaryFaces.end(),
        [](Face* face) {
            if (face->shouldDestroy) {
                delete face; // Liberar la memoria del objeto
                return true; // Indicar que el elemento debe ser eliminado
            }
            return false; // El elemento no necesita ser eliminado
        }), temporaryFaces.end());


}
void World::insertBlock(int insertionPos, uint8_t posY, uint8_t blockID, std::vector<Face*>* facesPointer) {
    std::vector<uint8_t> blockData; // Bloque de datos a insertar
  
   
    blockData.push_back(uint8_t(blockID));

    blockData.push_back(uint8_t(posY));


    uintptr_t direccion;
 
    for (int i = 0; i < 6; ++i) {

        Face* face = (*facesPointer)[i];
         direccion = reinterpret_cast<uintptr_t>(face);
       
         for (int j = 0; j < 8; ++j) {
            
             blockData.push_back(static_cast<uint8_t>((direccion >> (j * 8)) & 0xFF));
            
        }
       
    }
 
    // Insertar el bloque de datos en la posición deseada en el vector data
    data->insert(data->begin() + insertionPos, blockData.begin(), blockData.end());

}

 
 void World::renderBlocks() {
   
     for (auto face : temporaryFaces) {
         face->draw();
     }

     
 }
 World::~World() {


     for (auto face : temporaryFaces) {
         delete face;
     }



    


 }
 std::vector<Face*> World::getBlockRender(int posX, int posY, int posZ) {
   
     std::vector<Face*> toReturn;
     
     GLfloat upVertices[12] = {
     posX, posY, posZ ,
     posX + 1, posY, posZ,
     posX + 1, posY, posZ + 1 ,
     posX , posY, posZ + 1 };
    


     toReturn.emplace_back(new Face(upVertices));
   
 


     GLfloat downVertices[12] = {
     posX, posY - 1, posZ ,
     posX + 1, posY - 1, posZ,
     posX + 1, posY - 1, posZ + 1 ,
     posX , posY - 1, posZ +  1};
 

     toReturn.emplace_back(new Face(downVertices));
    


     GLfloat southVertices[12] = {
     posX, posY, posZ ,
     posX + 1, posY, posZ ,
      posX + 1, posY - 1, posZ ,
     posX , posY - 1, posZ };


     toReturn.emplace_back(new Face(southVertices));


     GLfloat northVertices[12] = {
     posX, posY, posZ + 1,
     posX + 1, posY, posZ + 1,
     posX + 1, posY - 1, posZ + 1,
     posX, posY - 1, posZ + 1
     };
   

     toReturn.push_back(new Face(northVertices));



     GLfloat westVertices[12] = {
  posX, posY, posZ,
  posX, posY, posZ + 1,
  posX , posY - 1, posZ + 1,
  posX , posY - 1, posZ
     };
     toReturn.emplace_back(new Face(westVertices));



     GLfloat eastVertices[12] = {
     
      posX+1, posY, posZ,
      posX+1, posY, posZ + 1,
      posX+1, posY-1, posZ + 1,
      posX+1, posY - 1, posZ
     };
     toReturn.emplace_back(new Face(eastVertices));
  


  
    //0 : arriba
   //1 : abajo
   //2 : sur   -z
   //3 : norte +z
   //4 : oeste -x
   //5 : este  +x

     return std::move(toReturn);
 }

 void World::printData() {
     int headerValue = 0;
     int headerPos = 0;
     int headersCounter = 0;
     int byte = 0;
     while (headersCounter < 16 * 16) {
         headerValue = (*data)[headerPos];
         std::cout << RED << "-------- Header --------" << RESET << std::endl;
         std::cout << YELLOW << "[" << byte << "] Header: " << BLUE << headerValue << RESET << std::endl;
         byte++;
         int posZ = floor(headersCounter / 16);
         std::cout << YELLOW << "PosX: " << GREEN << headersCounter - posZ * 16 << RESET << std::endl;
         std::cout << YELLOW << "PosZ: " << GREEN << posZ << RESET << std::endl;
         if (headerValue != 0) {
             std::cout << MAGENTA << "-------- Block start --------" << RESET << std::endl;
         }
         for (int i = 0; i < headerValue; ++i) {

             uint8_t blockType = (*data)[headerPos + i * blockInformationSize + 1];
             std::cout << YELLOW << "[" << byte << "] Block type: " << GREEN << static_cast<int>(blockType) << RESET << std::endl;
             byte++;
             uint8_t yPos = (*data)[headerPos + i * blockInformationSize + 2];
             std::cout << YELLOW << "[" << byte << "] PosY: " << GREEN << static_cast<int>(yPos) << RESET << std::endl;
             byte++;

             byte += 8;

             byte += 8;

             byte += 8;

             byte += 8;

             byte += 8;

             byte += 8;
             if (i != headerValue - 1) {
                 std::cout << MAGENTA << "-------- Block start --------" << RESET << std::endl;
             }

         }
         std::cout << RED << "-------- Footer --------" << RESET << std::endl;
         std::cout << YELLOW << "[" << byte << "] Footer: " << BLUE << static_cast<int>((*data)[headerPos + blockInformationSize * headerValue + 1]) << RESET << std::endl;
         byte++;
         headerPos = headerPos + blockInformationSize * headerValue + 2;
         headersCounter += 1;

     }
 }
 bool checkWorld(std::string name)
 {
     std::ifstream file(name.c_str());
     bool value = file.good();
     file.close();
     return value;
 }
 bool World::loadChunkData() {
     // Lee el archivo del mundo
     std::ifstream inputFile("world.wld", std::ios::binary);
     if (!inputFile.is_open()) {
         std::cerr << "Error al abrir el archivo de entrada." << std::endl;
         return false;
     }
     data = new std::vector<uint8_t>((std::istreambuf_iterator<char>(inputFile)), (std::istreambuf_iterator<char>()));//Leemos todo el archivo
     inputFile.close();
     facesPool = new Pool<Face>(187500);
     return true;
 }

 bool World::saveChunkData() {
     std::ofstream outputFile("world.wld", std::ios::binary);
     if (!outputFile.is_open()) {
         return false;
     }
     outputFile.write(reinterpret_cast<char*>(data->data()), (*data).size());
     outputFile.close();
     data->clear();
     delete data;
 }
 bool World::createChunk() {
     std::ofstream outputFile("world.wld", std::ios::binary);
     if (!outputFile.is_open()) {
         std::cerr << "Error al abrir el archivo de salida." << std::endl;
         return false;
     }
     // Escribe valores predeterminados en el archivo
     std::vector<uint8_t> defaultData(16 * 16 * 2, 0);
     outputFile.write(reinterpret_cast<char*>(defaultData.data()), defaultData.size());
     outputFile.close();
 }