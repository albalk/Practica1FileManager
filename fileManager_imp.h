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
                    vector<string*>* vfiles=f->listFiles(); //llamada a la funcion
                    int tam = vfiles->size();
                    std::cout<<"\tListFiles paso4\n";
                    pack(rpcOut, tam);
                    //packv vfiles
                    std::cout<<"\tListFiles paso5\n";
                    //packv(rpcOut, (char*)vfiles, tam);

                    for(int i=0; i<tam; i++){
                        int tam2=vfiles[i].size();
                        pack(rpcOut, tam2);
                        packv(rpcOut, vfiles[i].data(), tam2);
                    }

                    std::cout<<"\tListFiles paso6\n";
                    pack(rpcOut, (unsigned char)MSG_OK); //pack mensaje
                    
                    //return vfiles;

                    //liberar memoria
                    //llamada a freeListedFilesOp

                }break;
                case readFileOp:{
                    int tamf = unpack<int>(rpcIn);
                    int tamd = unpack<int>(rpcIn);
                    char filename[tamf];
                    char data[tamd];
                    unpackv(rpcIn, (char*)filename, tamf);
                    unpackv(rpcIn, (char*)&data, tamd);
                    long int dataLength = unpack<long int>(rpcIn);

                    f->writeFile(filename, data, dataLength);

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