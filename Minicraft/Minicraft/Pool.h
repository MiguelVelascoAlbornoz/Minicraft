#include <iostream>
#include <vector>

template<typename T>
class Pool {
private:
    std::vector<T*> memoryPool;
    std::vector<bool> allocated;
    size_t capacity;

public:
    Pool(size_t poolSize) : capacity(poolSize), memoryPool(poolSize), allocated(poolSize, false) {
        for (size_t i = 0; i < poolSize; ++i) {
            memoryPool[i] = new T(); // Crear objetos en el pool
        }
    }

    ~Pool(){
       

    }

    T* allocate() {
        for (size_t i = 0; i < capacity; ++i) {
            if (!allocated[i]) {
                allocated[i] = true;
                return memoryPool[i]; // Devuelve un objeto no asignado del pool
            }
        }
        return nullptr; // Si no hay objetos disponibles en el pool
    }

    void deallocate(T* obj) {
        for (size_t i = 0; i < capacity; ++i) {
            if (memoryPool[i] == obj) {
                allocated[i] = false; // Marca el objeto como no asignado
                break;
            }
        }
    }
};
