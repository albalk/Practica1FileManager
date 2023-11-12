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
        int port=60000;  //puerto del servidor
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
            packv(rpcOut, path.data(), tam);//empaquetar el contenido del string
    
            //enviar paquete
            sendMSG(serverConection.serverId, rpcOut);
            //recibir paquete
            recvMSG(serverConection.serverId, rpcIn);

            int ok = unpack<unsigned char>(rpcIn); //desempaqueta MSG_OK
            //si no es ok error
            if(ok!=MSG_OK){
                std::cout<<"Error"<<__FILE__<<":"<<__LINE__<<"\n";
            }

        }

        vector<string*>* listFiles(){
            //crear paquetes
            fileManagerOp op=listFilesOp;
            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;
            pack(rpcOut, op);   //empaqutea la operacion

            //enviar paquete
            sendMSG(serverConection.serverId, rpcOut);
            //recibir paquete
            recvMSG(serverConection.serverId,  rpcIn);

            //desempaqueta el tamaño del vector
            int tam =unpack<int>(rpcIn);
            //std::cout<<"\ttam: "<<tam<<"\n";
            vector<string*>* vfiles = new std::vector<std::string*>(); //inicializar vector

            //recorrer vfiles
            for(int i=0; i<tam; i++){
                //desempaquetar tamaño del elemento
                int tam2=unpack<int>(rpcIn); 
                char temp[tam2]; //temporal para guardar los string recibidos
                //desempaquetar el elemento en la variable temporal
                unpackv(rpcIn, temp, tam2); 
                //guardar el elemento en el vector
                vfiles->push_back(new std::string(temp)); 
            }

            int ok = unpack<unsigned char>(rpcIn); //desempaqueta MSG_OK
            //si no es ok error
            if(ok!=MSG_OK){
                std::cout<<"Error al recibir OK en listFiles  "<<__FILE__<<":"<<__LINE__<<"\n";
            }

            return vfiles;

        }

        void readFile(char* fileName, char* &data, unsigned long int & dataLength){
            //crear paquetes
            fileManagerOp op=readFileOp;
            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;

            pack(rpcOut, op);   //empaqutea la operacion

            int tamf= strlen(fileName)+1; //inicializa tamaño del file
            int tamd = strlen(data)+1; //inicializa tamaño de data
            //empaqueta los tamaños
            pack(rpcOut, tamf);
            pack(rpcOut, tamd);
            
            //empaquetar el contenido de filename
            packv(rpcOut, fileName, tamf);
            //empaquetar el contenido de data
            packv(rpcOut, data, tamd);
            //empaqueta datalength
            pack(rpcOut, dataLength);

            //enviar paquete
            sendMSG(serverConection.serverId, rpcOut);
            //recibir paquete
            recvMSG(serverConection.serverId,  rpcIn);
            
            int ok = unpack<unsigned char>(rpcIn); //desempaqueta MSG_OK

            //si no es ok error
            if(ok!=MSG_OK){
                std::cout<<"Error"<<__FILE__<<":"<<__LINE__<<"\n";
            }
        }

        void writeFile(char* fileName, char* data, unsigned long int dataLength){
            //crear paquetes
            fileManagerOp op=writeFileOp;
            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;

            pack(rpcOut, op);   //empaqutea la operacion
            
            int tamf= strlen(fileName)+1; //inicializa tamaño del file
            int tamd = strlen(data)+1; //inicializa tamaño de data
            //empaqueta los tamaños
            pack(rpcOut, tamf);
            pack(rpcOut, tamd);

            //empaquetar el contenido de filename
            packv(rpcOut, fileName, tamf);
            //empaquetar el contenido de data
            packv(rpcOut, data, tamd);
            //empaqueta datalength
            pack(rpcOut, dataLength);

            //enviar paquete
            sendMSG(serverConection.serverId, rpcOut);
            //recibir paquete
            recvMSG(serverConection.serverId,  rpcIn);

            int ok = unpack<unsigned char>(rpcIn); //desempaqueta MSG_OK
            //si no es ok error
            if(ok!=MSG_OK){
                std::cout<<"Error"<<__FILE__<<":"<<__LINE__<<"\n";
            }
        }

        void freeListedFiles(vector<string*>* fileList){
            //crear paquetes
            fileManagerOp op=freeListedFilesOp;
            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;

            pack(rpcOut, op);   //empaqutea la operacion

            //inicializar el tamaño del vector
            int tam = fileList->size();
            //empaquetar tamaño
            pack(rpcOut, tam);

            //recorrer fileList
            for(int i=0; i<tam; i++){
                //inicializar el tamaño de cada elemento del vector
                int tam2=fileList[i].size(); 
                //std::cout<<"\ttam :"<<tam2<<"\n";
                //empqaquetar el tamaño de cada elemento
                pack(rpcOut, tam2); 
                //empaquetar el contenido del elemento actual
                packv(rpcOut, fileList[i].data(), tam2);
            }

            //enviar paquete
            sendMSG(serverConection.serverId, rpcOut);
            //recibir paquete
            recvMSG(serverConection.serverId,  rpcIn);

            int ok = unpack<unsigned char>(rpcIn); //desempaqueta MSG_OK
            if(ok!=MSG_OK){
                std::cout<<"Error"<<__FILE__<<":"<<__LINE__<<"\n";
            }

        }

    //añadir free de los files
};