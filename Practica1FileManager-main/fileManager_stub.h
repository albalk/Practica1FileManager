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
        std::string ip= "127.0.0.1";//"172.31.50.30";  //ip del servidor
        int port=60000;  //puerto del servidor
        connection_t serverConection;
    
    public:
        fileManager(std::string path){ //constructor
            //conexion con el servidor
            serverConection=initClient(ip, port);
            fileManagerOp op=constructorOp;
            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;

            pack(rpcOut, op);   //empaqutea la operacion

            int tam = path.length();//declarar tamaño string
            pack(rpcOut, tam);//pack tamaño string
            packv(rpcOut, (char*)path.data(), tam);//empaquetar el contenido del string
    
            //enviar paquete
            sendMSG(serverConection.serverId, rpcOut);
            //recibir paquete
            recvMSG(serverConection.serverId, rpcIn);

            unsigned char ok = unpack<unsigned char>(rpcIn); //desempaqueta MSG_OK
            //si no es ok error
            if(ok!=MSG_OK){
                std::cout<<"Error"<<__FILE__<<":"<<__LINE__<<"\n";
            }

        };

        ~fileManager(){
			//cerrar conexión
			fileManagerOp op=destructorOp;
			std::vector<unsigned char> rpcOut;
			std::vector<unsigned char> rpcIn;

			pack(rpcOut,op);
			
			sendMSG(serverConection.serverId,rpcOut);
			recvMSG(serverConection.serverId,rpcIn);

            unsigned char ok = unpack<unsigned char>(rpcIn);

			if(ok!=MSG_OK)
			{
				std::cout<<"ERROR "<<__FILE__<<":"<<__LINE__<<"\n";				
			}
			closeConnection(serverConection.serverId);
		};

        std::vector<std::string*>* listFiles(){
            std::vector<std::string*>* vfiles = new std::vector<std::string*>(); //inicializar vector
            //crear paquetes
            fileManagerOp op=listFilesOp;
            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;
            pack(rpcOut, op);   //empaqutea la operacion

            //enviar paquete
            sendMSG(serverConection.serverId, rpcOut);
            //recibir paquete
            recvMSG(serverConection.serverId, rpcIn);

            unsigned char ok = unpack<unsigned char>(rpcIn); //desempaqueta MSG_OK
            //si no es ok error
            if(ok!=MSG_OK){
                std::cout<<"Error al recibir OK en listFiles  "<<__FILE__<<":"<<__LINE__<<"\n";
            }else{
                //desempaqueta el tamaño del vector
                int tam =unpack<int>(rpcIn);
                //std::cout<<"\ttam: "<<tam<<"\n";
                
                //recorrer vfiles
                for(int i=0; i<tam; i++){
                    std::string *temp = new std::string();
                    //desempaquetar tamaño del elemento
                    int tam2=unpack<int>(rpcIn); 
                    temp->resize(tam2); //temporal para guardar los string recibidos
                    //desempaquetar el elemento en la variable temporal
                    unpackv(rpcIn, (char*)temp->data(), tam2); 
                    //guardar el elemento en el vector
                    vfiles->push_back(temp); 
                }
            }

            return vfiles;

        }

        void readFile(char* fileName, char* &data, unsigned long int &dataLength){
            //crear paquetes
            fileManagerOp op=readFileOp;
            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;

            pack(rpcOut, op);   //empaqutea la operacion

            int tamf= strlen(fileName)+1; //inicializa tamaño del file
            //empaqueta los tamaños
            pack(rpcOut, tamf);
            
            //empaquetar el contenido de filename
            packv(rpcOut, fileName, tamf);

            //enviar paquete
            sendMSG(serverConection.serverId, rpcOut);
            //recibir paquete
            recvMSG(serverConection.serverId,  rpcIn);
            
            unsigned char ok = unpack<unsigned char>(rpcIn); //desempaqueta MSG_OK
            //si no es ok error
            if(ok!=MSG_OK){
                std::cout<<"Error"<<__FILE__<<":"<<__LINE__<<"\n";
            }else{
                dataLength = unpack<unsigned long int>(rpcIn);
                data = new char[dataLength];
                unpackv(rpcIn, (char*)data, dataLength);
            }
        }

        void writeFile(char* fileName, char* data, unsigned long int dataLength){
            //crear paquetes
            fileManagerOp op=writeFileOp;
            std::vector<unsigned char> rpcOut;
            std::vector<unsigned char> rpcIn;

            pack(rpcOut, op);   //empaqutea la operacion
            
            int tamf= strlen(fileName)+1; //inicializa tamaño del file
            //empaqueta los tamaños
            pack(rpcOut, tamf);

            //empaquetar el contenido de filename
            packv(rpcOut, fileName, tamf);
            //empaqueta datalength
            pack(rpcOut, dataLength);
            //empaquetar el contenido de data
            packv(rpcOut, data, dataLength);
            

            //enviar paquete
            sendMSG(serverConection.serverId, rpcOut);
            //recibir paquete
            recvMSG(serverConection.serverId,  rpcIn);

            unsigned char ok = unpack<unsigned char>(rpcIn); //desempaqueta MSG_OK
            //si no es ok error
            if(ok!=MSG_OK){
                std::cout<<"Error"<<__FILE__<<":"<<__LINE__<<"\n";
            }
        }

        void freeListedFiles(std::vector<std::string*>* fileList){
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
                std::string *file = (*fileList)[i];
                //inicializar el tamaño de cada elemento del vector
                int tam2 = file->size();
                //std::cout<<"\ttam :"<<tam2<<"\n";
                //empqaquetar el tamaño de cada elemento
                pack(rpcOut, tam2); 
                //empaquetar el contenido del elemento actual
                packv(rpcOut, (char*) file->data(), tam2);
            }

            //enviar paquete
            sendMSG(serverConection.serverId, rpcOut);
            //recibir paquete
            recvMSG(serverConection.serverId,  rpcIn);

            unsigned char ok = unpack<unsigned char>(rpcIn); //desempaqueta MSG_OK
            if(ok!=MSG_OK){
                std::cout<<"Error"<<__FILE__<<":"<<__LINE__<<"\n";
            }

        }
};