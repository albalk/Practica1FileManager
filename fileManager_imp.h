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
                    //f=new FileManager();    //clase declarada en stub
                    //declarar tamaño string
                    //unpack tamaño string
                    //unpackv string con el nombre del fichero
                    //pack MSG_OK
                }break;
                case listFilesOp:{

                }break;
                case readFileOp:{

                }break;
                case writeFileOp:{

                }break;
                case freeListedFilesOp:{

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