#pragma once
#include <iostream>
#include <vector>
#include <cstring>

#define MSG_OK 1
#define MSG_NOK 2

typedef enum fileManagerOp{
    constructorOp=1,  listFilesOp=2, readFileOp=3, writeFileOp=4, freeListedFilesOp=5, destructorOp=6
}fileManagerOp;


template<typename T>
                                //byte
inline void pack(std::vector<unsigned char> &packet, T data){   //copia los bytes de un dato a un paquete de salida
    
    int size=packet.size();
    unsigned char *ptr=(unsigned char*)&data;
    packet.resize(size+sizeof(T));
                                                        //avanza n posiciones en el paquete
    std::copy(ptr, ptr+sizeof(T), packet.begin()+size);


}

template<typename T>

inline T unpack(std::vector<unsigned char> &packet){
    
    T data;
    int dataSize=sizeof(T); 
    int packetSize=packet.size();
    T*ptr=(T*)packet.data();
    data=ptr[0];

    for(int i=dataSize; i<packetSize; i++){
        packet[i-dataSize]=packet[i];
    }
    //redimensionar
    packet.resize(packetSize-dataSize);

    return data;
}

template<typename T>
inline void packv(std::vector<unsigned char> &packet, T* data, int dataSize){
    for(int i=0; i<dataSize; i++){
        pack(packet, data[i]);
    }
}

template<typename T>
inline void unpackv(std::vector<unsigned char> &packet, T* data, int dataSize){
    for(int i=0; i<dataSize; i++){
        data[i]=unpack<T>(packet);
    }
}
