#include "operaciones.h"
#include <thread>
#include "fileManager_imp.h"

void atiendeCliente(int clientId){
    //crear interfaz de cliente
    FileManager_imp imp=FileManager_imp(clientId);
    //repetir
    do{
        //atiende operacion
        imp.recibeOp();
    }while(!imp.conectionClosed());//mientras no cierre conexion
        
    
}

int main(int argc, char** argv){
    //iniciar un servidor
    auto serverSocket=initServer(60000);
    
    while(1){
        //esperar conexiones en puerto
        while(!checkClient()){
            usleep(1000);
        }
        
        int clientId=getLastClientID();
        std::thread *th= new std::thread(atiendeCliente, clientId); //la funcion que va a atender para ejecutarlo en un thread aparte
    }

    //cerrar conexion
    close(serverSocket);
    return 0;
}