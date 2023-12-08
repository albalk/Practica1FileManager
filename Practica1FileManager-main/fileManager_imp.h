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
                    //declarar y desempaquetar el tamaño del string
                    int tam = unpack<int>(rpcIn);
                    //declara path
                    std::string nombre;

                    nombre.resize(tam); //resize
                    //desempaqueta el nombre del fichero
                    unpackv(rpcIn, (char*)nombre.data(), tam);
                    
                    f=new FileManager(nombre); //llamada al constructor

                    pack(rpcOut, (unsigned char)MSG_OK); //empaqueta MSG_OK

                }break;
                case destructorOp:{
                    delete f;
				    f=nullptr;
				    pack(rpcOut,(unsigned char)MSG_OK);
                }break;
                case listFilesOp:{
                    //declarar e inicializar el vector de strings
                    vector<string*>* vfiles = new std::vector<std::string*>();
                    vfiles=f->listFiles(); //llamada a la funcion listFiles

                    pack(rpcOut, (unsigned char)MSG_OK); //empaqueta MSG_OK

                    //inicializar el tamaño del vector
                    int tam = vfiles->size();
                    //empaquetar el tamaño
                    pack(rpcOut, tam);

                    //recorrer vfiles
                    for(int i=0; i<tam; i++){
                        std::string *file = (*vfiles)[i];
                        //inicializar el tamaño de cada elemento del vector
                        int tam2 = file->size();
                        //std::cout<<"\ttam :"<<tam2<<"\n";
                        //empqaquetar el tamaño de cada elemento
                        pack(rpcOut, tam2); 
                        //empaquetar el contenido del elemento actual
                        packv(rpcOut, (char*) file->data(), tam2);
                    }

                    //liberar memoria

                }break;
                case readFileOp:{
                    //desempaqueta los tamaños del file
                    int tamf = unpack<int>(rpcIn);

                    //reserva de memoria
                    char *filename = new char[tamf];
                    char *data = nullptr;
                    unsigned long int dataLength = 0;

                    //desempaquetar contenido de filename
                    unpackv(rpcIn, (char*)filename, tamf);
                    
                    f->readFile(filename, data, dataLength); //llamada a la función readFile

                    pack(rpcOut, (unsigned char)MSG_OK); //empaqueta MSG_OK

                    pack(rpcOut, dataLength);
                    packv(rpcOut, data, dataLength);

                    delete[] filename;
                    delete[] data;
                }break;
                case writeFileOp:{
                    //desempaqueta el tamaño del file
                    int tamf = unpack<int>(rpcIn);

                    //reserva de memoria
                    char *filename = new char[tamf];

                    //desempaquetar contenido de filename
                    unpackv(rpcIn, (char*)filename, tamf);
                    long int dataLength = unpack<long int>(rpcIn);
                    char *data = new char[dataLength];
                    //desempaquetar contenido de data
                    unpackv(rpcIn, (char*)data, dataLength);
                    //desempaquetar dataLength
                    

                    f->writeFile(filename, data, dataLength); //llamada a la función writeFile

                    pack(rpcOut, (unsigned char)MSG_OK); //empaqueta MSG_OK

                }break;
                case freeListedFilesOp:{
                    //desempaquetar tamaño del vector
                    int tam = unpack<int>(rpcIn);
                    //inicializar vector
                    vector<string*>* fileList = new std::vector<std::string*>(); 
                    
                    //recorrer fileList
                    for(int i=0; i<tam; i++){
                        std::string *temp = new std::string();
                        //desempaquetar tamaño del elemento
                        int tam2=unpack<int>(rpcIn); 
                        temp->resize(tam2); //temporal para guardar los string recibidos
                        //desempaquetar el elemento en la variable temporal
                        unpackv(rpcIn, (char*)temp->data(), tam2); 
                        //guardar el elemento en el vector
                        fileList->push_back(temp); 
                    }

                    f->freeListedFiles(fileList); //llamada a la funcion freeListedFiles

                    pack(rpcOut, (unsigned char)MSG_OK); //empaqueta MSG_OK

                    //liberar memoria

                }break;
                default:{
                    std::cout<<"Error: funcion no definida\n";
                    pack(rpcOut, (unsigned char)MSG_NOK);
                }break;
            }

            //devuelve resultados
            //std::cout<<"\tAntes de devolver los resultados\n";
            sendMSG(clientId, rpcOut);
            //std::cout<<"\tDespues de devolver los resultados\n";
        }

};