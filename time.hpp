#ifndef TIME
#define TIME

#include <iostream>
#include <vector>
#include <list>
#include <stack>

using std::stack;
using std::list;
using std::vector;

struct Aluno{
    list<int> adjList;
    int id, idade;
};

class Time{
    private:
        int arestas, vertices;
        vector<Aluno> alunos;

    public:
        void setVertices(std::ifstream &entrada, int vertices);
        void setArestas(std::ifstream &entrada, int arestas);
        void executarComandos(std::ifstream &entrada, int comandos);

        void meeting();

        void commander(int aluno1);
        vector<list<int> > transporGrafo();
        int idadeMin(int aluno1);

        bool existeAresta(int aluno1, int aluno2);
        void swap(int aluno1, int aluno2);
        bool temCiclo();
        bool buscaCiclo(int v, vector<bool> &visited, vector<bool> &stack);
        
};


#endif