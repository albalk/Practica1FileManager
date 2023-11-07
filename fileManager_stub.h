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
            //declarar tamaño string
            //pack tamaño string
            //packv string
    

            sendMSG(serverConection.serverId, rpcOut);
            recvMSG(serverConection.serverId, rpcIn);

            if(rpcIn[0]!=MSG_OK){
                std::cout<<"Error"<<__FILE__<<":"<<__LINE__<<"\n";
            }

        }

        vector<string*>* listFiles(){

        }

        void readFile(char* fileName, char* &data, unsigned long int & dataLength){

        }

        void writeFile(char* fileName, char* data, unsigned long int dataLength){

        }

        void freeListedFiles(vector<string*>* fileList){

        }

    //añadir free de los files
};