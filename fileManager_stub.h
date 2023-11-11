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
            std::cout<<"\tConstructor paso1\n";
            serverConection=initClient(ip, port);
            fileManagerOp op=constructorOp;
            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;

            std::cout<<"\tConstructor paso2\n";
            pack(rpcOut, op);   //empaqutea la operacion

            std::cout<<"\tConstructor paso3\n";
            int tam = path.length();//declarar tamaño string
            pack(rpcOut, tam);//pack tamaño string
            packv(rpcOut, path.data(), tam);//packv string
    

            sendMSG(serverConection.serverId, rpcOut);
            recvMSG(serverConection.serverId, rpcIn);

            std::cout<<"\tConstructor paso7\n";
            if(rpcIn[0]!=MSG_OK){
                std::cout<<"Error"<<__FILE__<<":"<<__LINE__<<"\n";
            }

        }

        vector<string*>* listFiles(){
            //crear paquetes
            std::cout<<"\tListFiles paso1\n";
            fileManagerOp op=listFilesOp;
            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;
            std::cout<<"\tListFiles paso2\n";
            pack(rpcOut, op);   //empaqutea la operacion

            sendMSG(serverConection.serverId, rpcOut);
            recvMSG(serverConection.serverId,  rpcIn);

            std::cout<<"\tListFiles paso7\n";
            int tam =unpack<int>(rpcIn);
            //std::cout<<"\ttam: "<<tam<<"\n";
            vector<string*>* vfiles = new std::vector<std::string*>(); //inicializar

            //vfiles->resize(tam);
            
            std::cout<<"\tListFiles paso8\n";
            //unpackv(rpcIn, (char*)vfiles, tam);

            for(int i=0; i<tam; i++){
                int tam2=unpack<int>(rpcIn); //desempaquetar tamaño del elemento
                char temp[tam2]; //temporal para guardar los string recibidos
                unpackv(rpcIn, temp, tam2); //desempaquetar el elemento en la variable temporal
                vfiles->push_back(new std::string(temp)); //guardar el elemento en la lista
            }

            std::cout<<"\tListFiles paso9\n";
            int ok = unpack<unsigned char>(rpcIn);
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
            std::cout<<"\tReadFiles paso1\n";

            pack(rpcOut, op);   //empaqutea la operacion
            std::cout<<"\tReadFiles paso2\n";

            int tamf= strlen(fileName)+1; //tamaño del file
            int tamd = strlen(data)+1; //tamaño de data
            pack(rpcOut, tamf);
            pack(rpcOut, tamd);
            std::cout<<"\tReadFiles paso3\n";
            packv(rpcOut, fileName, tamf);
            std::cout<<"\tReadFiles paso4\n";
            packv(rpcOut, data, tamd);  //seguramnete tremendo error de datos
            std::cout<<"\tReadFiles paso5\n";
            pack(rpcOut, dataLength);
            std::cout<<"\tReadFiles paso6\n";

            sendMSG(serverConection.serverId, rpcOut);
            recvMSG(serverConection.serverId,  rpcIn);
            
            std::cout<<"\tReadFiles paso12\n";
            int ok = unpack<unsigned char>(rpcIn);
            std::cout<<"\tContenido OK: "<<ok<<"\n";
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
            
            int tamf= strlen(fileName)+1;
            int tamd = strlen(data)+1;
            pack(rpcOut, tamf);
            pack(rpcOut, tamd);
            packv(rpcOut, fileName, tamf);
            packv(rpcOut, data, tamd);
            pack(rpcOut, dataLength);

            sendMSG(serverConection.serverId, rpcOut);
            recvMSG(serverConection.serverId,  rpcIn);

            int ok = unpack<unsigned char>(rpcIn);
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
            std::cout<<"\tFreeListedFiles paso1\n";

            pack(rpcOut, op);   //empaqutea la operacion
            std::cout<<"\tFreeListedFiles paso2\n";

            int tam = fileList->size();
            pack(rpcOut, tam);
            std::cout<<"\tFreeListedFiles paso3\n";

            for(int i=0; i<tam; i++){ //recorrer fileList
                int tam2=fileList[i].size(); //inicializar el tamaño de cada elemento del vector
                //std::cout<<"\ttam :"<<tam2<<"\n";
                pack(rpcOut, tam2); //empqaquetar el tamaño de cada elemento
                packv(rpcOut, fileList[i].data(), tam2); //empaquetar
            }

            std::cout<<"\tFreeListedFiles paso4\n";

            sendMSG(serverConection.serverId, rpcOut);
            recvMSG(serverConection.serverId,  rpcIn);

            std::cout<<"\tFreeListedFiles paso9\n";
            int ok = unpack<unsigned char>(rpcIn);
            if(ok!=MSG_OK){
                std::cout<<"Error"<<__FILE__<<":"<<__LINE__<<"\n";
            }

        }

    //añadir free de los files
};