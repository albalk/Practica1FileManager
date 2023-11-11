#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include "operaciones.h"
#include "filemanager.h"

//intermediario entre cliente (stub) la persona

class FileManager_imp{
    private:
        int clientId=-1;
        FileManager* f=nullptr;

    public:
        FileManager_imp(int clientId):clientId(clientId){}; //inicializa clientId con el que nos pasan por parametro
        
        bool conectionClosed(){
            return f==nullptr;
        }

        void recibeOp(){
            std::vector<unsigned char> rpcIn;
            std::vector<unsigned char> rpcOut;

            //recibe operaciones
            recvMSG(clientId, rpcIn);
            fileManagerOp operacion = unpack<fileManagerOp>(rpcIn); //fileManagerOp -> estructura en operaciones.h

            //ejecuta
            switch(operacion){
                case constructorOp:{
                    std::cout<<"\tConstructor paso4\n";
                    int tam = unpack<int>(rpcIn); //declarar y unpack tamaño string
                    std::string nombre; //declara path

                    std::cout<<"\tConstructor paso5\n";
                    nombre.resize(tam); //resize
                    unpackv(rpcIn, (char*)nombre.data(), tam); //unpackv string con el nombre del fichero
                    
                    std::cout<<"\tConstructor paso6\n";
                    f=new FileManager(nombre);
                    pack(rpcOut, (unsigned char)MSG_OK);//pack MSG_OK
                }break;
                case listFilesOp:{
                    std::cout<<"\tListFiles paso3\n";
                    vector<string*>* vfiles = new std::vector<std::string*>; //declarar e inicializar
                    vfiles=f->listFiles(); //llamada a la funcion
                    int tam = vfiles->size();
                    //std::cout<<"\ttam: "<<tam<<"\n";
                    std::cout<<"\tListFiles paso4\n";
                    pack(rpcOut, tam);
                    //packv vfiles
                    std::cout<<"\tListFiles paso5\n";
                    //packv(rpcOut, (char*)vfiles, tam);

                    for(int i=0; i<tam; i++){ //recorrer vfiles
                        int tam2=vfiles[i].size(); //inicializar el tamaño de cada elemento del vector
                        //std::cout<<"\ttam :"<<tam2<<"\n";
                        pack(rpcOut, tam2); //empqaquetar el tamaño de cada elemento
                        packv(rpcOut, vfiles[i].data(), tam2); //empaquetar
                    }

                    std::cout<<"\tListFiles paso6\n";
                    pack(rpcOut, (unsigned char)MSG_OK); //pack mensaje
                    

                    //liberar memoria

                }break;
                case readFileOp:{
                    std::cout<<"\tReadFiles paso7\n";
                    int tamf = unpack<int>(rpcIn);
                    int tamd = unpack<int>(rpcIn);
                    std::cout<<"\tReadFiles paso7,5\n";

                    char filename[tamf];
                    char data[tamd];
                    unpackv(rpcIn, (char*)filename, tamf);
                    std::cout<<"\tReadFiles paso8\n";
                    unpackv(rpcIn, (char*)&data, tamd);
                    std::cout<<"\tReadFiles paso9\n";
                    long int dataLength = unpack<long int>(rpcIn);
                    std::cout<<"\tReadFiles paso10\n";

                    //f->readFile((char*)filename, *data, dataLength);
                    std::cout<<"\tReadFiles paso11\n";

                    pack(rpcOut, (unsigned char)MSG_OK);


                }break;
                case writeFileOp:{
                    int tamf = unpack<int>(rpcIn);
                    int tamd = unpack<int>(rpcIn);
                    char filename[tamf];
                    char data[tamd];
                    unpackv(rpcIn, (char*)filename, tamf);
                    unpackv(rpcIn, (char*)data, tamd);
                    long int dataLength = unpack<long int>(rpcIn);

                    f->writeFile(filename, data, dataLength);

                    pack(rpcOut, (unsigned char)MSG_OK);

                }break;
                case freeListedFilesOp:{
                    int tam = unpack<int>(rpcIn);//unpack tamaño
                    std::cout<<"\tFreeListedFiles paso5\n";
                    //unpackv
                    vector<string*>* fileList = new std::vector<std::string*>(); //inicializar
                    
                    std::cout<<"\tFreeListedFiles paso6\n";
                    for(int i=0; i<tam; i++){
                        int tam2=unpack<int>(rpcIn); //desempaquetar tamaño del elemento
                        char temp[tam2]; //temporal para guardar los string recibidos
                        unpackv(rpcIn, temp, tam2); //desempaquetar el elemento en la variable temporal
                        fileList->push_back(new std::string(temp)); //guardar el elemento en la lista
                    }

                    std::cout<<"\tFreeListedFiles paso7\n";
                    f->freeListedFiles(fileList);

                    std::cout<<"\tFreeListedFiles paso8\n";
                    pack(rpcOut, (unsigned char)MSG_OK);

                    //liberar memoria

                }break;
                default:{
                    std::cout<<"Error: funcion no definida\n";
                    pack(rpcOut, (unsigned char)MSG_NOK);
                }break;
            }

            //devuelve resultados
            std::cout<<"\tAntes de devolver los resultados\n";
            sendMSG(clientId, rpcOut);
            std::cout<<"\tDespues de devolver los resultados\n";
        }

};