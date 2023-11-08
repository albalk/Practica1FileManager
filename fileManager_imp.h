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
                    int tam = unpack<int>(rpcIn); //declarar y unpack tamaño string
                    std::string nombre; //declara path

                    nombre.resize(tam); //resize
                    unpackv(rpcIn, (char*)nombre.data(), tam); //unpackv string con el nombre del fichero
                    
                    f=new FileManager(nombre);    //clase declarada en stub
                    pack(rpcOut, (unsigned char)MSG_OK);//pack MSG_OK
                }break;
                case listFilesOp:{
                    vector<string*>* vfiles=f->listFiles(); //llamada a la funcion
                    //int tam = sizeof(vfiles);

                    //packv vfiles
                    pack(rpcOut, (unsigned char)MSG_OK); //pack mensaje
                    
                    //return vfiles;

                    //liberar memoria
                    //llamada a freeListedFilesOp

                }break;
                case readFileOp:{

                }break;
                case writeFileOp:{

                }break;
                case freeListedFilesOp:{
                    int tam = unpack<int>(rpcIn);//unpack tamaño
                    //unpackv


                    vector<string*>* vfiles;
                    unpackv(rpcIn, vfiles, tam);
                    f->freeListedFiles(vfiles);

                    pack(rpcOut, (unsigned char)MSG_OK);

                    //liberar memoria

                }break;
                default:{
                    std::cout<<"Error: funcion no definida\n";
                    pack(rpcOut, (unsigned char)MSG_NOK);
                }break;
            }

            //devuelve resultados
            sendMSG(clientId, rpcOut);
        }

};