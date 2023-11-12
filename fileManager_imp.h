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
                case listFilesOp:{
                    //declarar e inicializar el vector de strings
                    vector<string*>* vfiles = new std::vector<std::string*>;
                    vfiles=f->listFiles(); //llamada a la funcion listFiles

                    //inicializar el tamaño del vector
                    int tam = vfiles->size();
                    //empaquetar el tamaño
                    pack(rpcOut, tam);

                    //recorrer vfiles
                    for(int i=0; i<tam; i++){
                        //inicializar el tamaño de cada elemento del vector
                        int tam2=vfiles[i].size();
                        //std::cout<<"\ttam :"<<tam2<<"\n";
                        //empqaquetar el tamaño de cada elemento
                        pack(rpcOut, tam2); 
                        //empaquetar el contenido del elemento actual
                        packv(rpcOut, vfiles[i].data(), tam2);
                    }

                    pack(rpcOut, (unsigned char)MSG_OK); //empaqueta MSG_OK
                    

                    //liberar memoria

                }break;
                case readFileOp:{
                    //desempaqueta los tamaños del file y de data
                    int tamf = unpack<int>(rpcIn);
                    int tamd = unpack<int>(rpcIn);

                    //reserva de memoria
                    char filename[tamf];
                    char data[tamd];

                    //desempaquetar contenido de filename
                    unpackv(rpcIn, (char*)filename, tamf);
                    //desempaquetar contenido de data
                    unpackv(rpcIn, (char*)&data, tamd);
                    //desempaquetar dataLength
                    long int dataLength = unpack<long int>(rpcIn);

                    f->readFile((char*)filename, *data, dataLength); //llamada a la función readFile

                    pack(rpcOut, (unsigned char)MSG_OK); //empaqueta MSG_OK


                }break;
                case writeFileOp:{
                    //desempaqueta los tamaños del file y de data
                    int tamf = unpack<int>(rpcIn);
                    int tamd = unpack<int>(rpcIn);

                    //reserva de memoria
                    char filename[tamf];
                    char data[tamd];

                    //desempaquetar contenido de filename
                    unpackv(rpcIn, (char*)filename, tamf);
                    //desempaquetar contenido de data
                    unpackv(rpcIn, (char*)data, tamd);
                    //desempaquetar dataLength
                    long int dataLength = unpack<long int>(rpcIn);

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
                        //desempaquetar tamaño del elemento
                        int tam2=unpack<int>(rpcIn); 
                        char temp[tam2]; //temporal para guardar los string recibidos
                        //desempaquetar el elemento en la variable temporal
                        unpackv(rpcIn, temp, tam2); 
                        //guardar el elemento en el vector
                        fileList->push_back(new std::string(temp)); 
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