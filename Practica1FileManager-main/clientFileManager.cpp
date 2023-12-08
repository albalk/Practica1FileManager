#include <iostream>
//#include "filemanager.h"
#include "fileManager_stub.h"

int main(int argc,char** argv)
{
    std::cout<<"llamada constructor\n";
    fileManager *fm=new fileManager("./dirprueba/");
    
    std::cout<<"\tllamada listfiles\n";
    std::vector<std::string*>* vfiles=fm->listFiles();
    std::cout<<"Lista de ficheros en el directorio de prueba:\n";
    for(unsigned int i=0;i<vfiles->size();++i)
    {
        std::cout<<"Fichero: "<<vfiles->at(i)->c_str()<<std::endl;
    }
    std::cout<<"Leyendo el primer fichero del directorio de prueba:\n";

    char* data=nullptr;
    unsigned long int fileLen=0;

    std::cout<<"\tllamada readfiles\n";
    fm->readFile(&(*(vfiles->at(0)))[0],data,fileLen);
    std::cout<<"Escribiendo el primer fichero del directorio de prueba:\n";
    fm->writeFile(&(*(vfiles->at(0)))[0],data,fileLen);
    std::cout<<"Liberando lista de ficheros:\n";
    fm->freeListedFiles(vfiles);
    std::cout<<"Liberando datos de fichero leído:\n";

    delete[] data;
    std::cout<<"delete data";
    delete fm;

    return 0;
}