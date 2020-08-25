#include <iostream>
#include <fstream>
#include <string>
#include "time.hpp"
using std::string;


int main (int argc, char **argv){

    if(argc > 0){
        Time time;
        int arestas, vertices, comandos;    
        std::ifstream entrada;
        string nomeArquivo = argv[1];
        entrada.open(nomeArquivo);

        if (!entrada) {
            std::cout << "Falha para abrir o arquivo";
        }
        
        entrada >> vertices;
        entrada >> arestas;
        entrada >> comandos;
        
        time.setVertices(entrada, vertices);
        time.setArestas(entrada, arestas);
        time.executarComandos(entrada, comandos);

        entrada.close();
    }
    else{
        std::cout << "Por favor explicite como parâmetro um arquivo a ser lido";
    }
    
}