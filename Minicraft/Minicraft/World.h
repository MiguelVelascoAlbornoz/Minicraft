#pragma once
#include <vector>
#include "Face.h"
#include "Block.h"
#include <mutex>
#include <condition_variable>
#include <list>
#include "Pool.h"
class World
{
public:
	World();
	bool saveChunkData();

	
	bool loadChunkData();
	
	bool createChunk();
	bool setBlockInChunk(uint8_t posX, uint8_t posY, uint8_t posZ, Block* block);
	void printData();
	void clearTemporaryFaces();
	void insertBlock(int insertionPos, uint8_t posY, uint8_t blockID, std::vector<Face*>* facesPointer);
	
	~World();

	std::vector<Face*> faces;
	void renderBlocks();



	void moveTemporaryFaces();


	std::string worldName;

private:
	const int counterToFreefacesLimit = 256;
	int counterToFreefaces = 0;
	int findBetweenBlocks(uint8_t blockHeaderPosValue, uint8_t posY, int headerPos);

	Pool<Face>* facesPool = nullptr;

	std::vector<Face*> getBlockRender(int posX, int posY, int posZ);
	int lastXpos = -1;
	int lastZpos = -1;
	int lastSouthHeaderPos;
	int lastHeaderBytePos;
	int lastNorthHeaderPos;
	const int blockInformationSize = 50;
	std::vector<uint8_t>* data;
	std::list<Face*> temporaryFaces;


};

