#pragma once
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "operaciones.h"

//intermediario entre el cliente y persona
class fileManager {
    private:
        std::string ip="172.31.50.30";  //ip del servidor
        int port=6000;  //puerto del servidor
        connection_t serverConection;
    
    public:
        fileManager(string path){ //constructor
            //conexion con el servidor
            serverConection=initClient(ip, port);
            fileManagerOp op=constructorOp;
            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;

            pack(rpcOut, op);   //empaqutea la operacion

            int tam = path.length();//declarar tamaño string
            pack(rpcOut, tam);//pack tamaño string
            packv(rpcOut, path.data(), tam);//packv string
    

            sendMSG(serverConection.serverId, rpcOut);
            recvMSG(serverConection.serverId, rpcIn);

            if(rpcIn[0]!=MSG_OK){
                std::cout<<"Error"<<__FILE__<<":"<<__LINE__<<"\n";
            }

        }

        vector<string*>* listFiles(){
            //crear paquetes
            fileManagerOp op=listFilesOp;
            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;

            pack(rpcOut, op);   //empaqutea la operacion

            sendMSG(serverConection.serverId, rpcOut);
            recvMSG(serverConection.serverId,  rpcIn);

            if(rpcIn[0]!=MSG_OK){
                std::cout<<"Error"<<__FILE__<<":"<<__LINE__<<"\n";
            }

        }

        void readFile(char* fileName, char* &data, unsigned long int & dataLength){
            //crear paquetes
            fileManagerOp op=readFileOp;
            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;

            pack(rpcOut, op);   //empaqutea la operacion
        }

        void writeFile(char* fileName, char* data, unsigned long int dataLength){
            //crear paquetes
            fileManagerOp op=writeFileOp;
            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;

            pack(rpcOut, op);   //empaqutea la operacion
        }

        void freeListedFiles(vector<string*>* fileList){
            //crear paquetes
            fileManagerOp op=freeListedFilesOp;
            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;

            pack(rpcOut, op);   //empaqutea la operacion

            int tam = sizeof(fileList);
            pack(rpcOut, tam);
            packv(rpcOut, fileList, tam);

            sendMSG(serverConection.serverId, rpcOut);
            recvMSG(serverConection.serverId,  rpcIn);

            if(rpcIn[0]!=MSG_OK){
                std::cout<<"Error"<<__FILE__<<":"<<__LINE__<<"\n";
            }
        }

    //añadir free de los files
};