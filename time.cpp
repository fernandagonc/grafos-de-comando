#include "time.hpp"
#include <fstream>
#include <vector>
#include <queue> 
#include <stack>

using std::vector;
using std::queue;
using std::stack;

void Time::setVertices(std::ifstream &entrada, int vertices){
    Aluno aluno;
    int idade, i;
    this->vertices = vertices;
    for(i = 0; i < this->vertices; i++){
        aluno.id = i;
        entrada >> idade;
        aluno.idade = idade;
        
        alunos.push_back(aluno);
    }
}

void Time::setArestas(std::ifstream &entrada, int arestas){
    int pai, filho, i;
    this->arestas = arestas;
    for(i = 0; i <  this->arestas; i++){
        entrada >> pai;
        entrada >> filho;
        pai--; //ajuste para indices do vetor que começa em 0
        filho--;
        alunos[pai].adjList.push_back(filho); // lista de adjacencias dos filhos direto do nó
        
    }
}

void Time::executarComandos(std::ifstream &entrada, int comandos){
    char comando;
    int i, aluno1, aluno2;

    for(i = 0; i <  comandos; i++){
        entrada >> comando;
        if (comando == 'M'){
            meeting();
        }
        else if (comando == 'C'){
            entrada >> aluno1;
            aluno1--;
            commander(aluno1);
        }
        else if (comando == 'S'){
            entrada >> aluno1 >> aluno2;
            aluno1--;
            aluno2--;
            if(existeAresta(aluno1, aluno2)){
                swap(aluno1, aluno2);

                if(temCiclo()){
                    swap(aluno2, aluno1);
                    std::cout << "S N\n";
                }else{
                    std::cout << "S T\n";
                }          
            
            }
            else{
                std::cout << "S N\n";
            }
   
        }
    }
}

void Time::meeting(){
    queue<int> ordem;
    vector<bool> temPai(this->vertices);
    vector<bool> visited(this->vertices);
    list<int>::iterator iterator;
    

    for(int i=0; i < this->vertices; i++){//diferencia os alunos que não tem comandantes e devem falar primeiro
        for(iterator = alunos[i].adjList.begin(); iterator != alunos[i].adjList.end(); ++iterator)
            temPai[*iterator] = true;
    }
        
    for(int i=0; i < this->vertices; i++){
        if(temPai[i] == false){ //coloca na fila os alunos que não tem comandante
            ordem.push(alunos[i].id);
            visited[i] = true;
        }
    }


    std::cout << "M ";
    
    while(!ordem.empty()){//a partir dos nós "raiz" percorre as listas de adjacências
        int id = ordem.front();
        std::cout << id + 1 << " ";
        ordem.pop();

        for(iterator = alunos[id].adjList.begin(); iterator != alunos[id].adjList.end(); ++iterator){
            int idFilho = *iterator;
            if(visited[idFilho] == false){
                ordem.push(idFilho);
                visited[idFilho] = true;
            }
        }
    }

    std::cout << "\n";

}

void Time::commander(int aluno1){

    list<int>::iterator iterator;
    stack<int> comandantes;
    int idade, nroComandantes = 0;
    
    for(int i=0; i < this->vertices; i++){//verifica se o aluno é um nó raiz - não tem comandantes
        for(iterator = alunos[i].adjList.begin(); iterator != alunos[i].adjList.end(); ++iterator){
                if(*iterator == aluno1){
                    nroComandantes++;
                }
            }
    }
    
    if(nroComandantes == 0){//se não há comandantes retornamos
        std::cout << "C *" << '\n';
        return;
    }


    idade = idadeMin(aluno1);
    
    std::cout << "C " << idade << '\n';
    return;
 
}

vector<list<int> > Time::transporGrafo(){
    list<int>::iterator iterator;
    list<int> adjList;
    vector<list<int> > grafoTransposto;

    for(int i=0; i < this->vertices; i++){
        grafoTransposto.push_back(adjList);
    }

    for(int i=0; i < this->vertices; i++){
        for(iterator = alunos[i].adjList.begin(); iterator != alunos[i].adjList.end(); ++iterator){
               int indiceFilho = *iterator;
               grafoTransposto[indiceFilho].push_back(i);
        }
    }
    return grafoTransposto;
}


int Time::idadeMin(int aluno1){
    
    list<int>::iterator iterator;
    int id, idPai, idadeMin;
    queue<int>comandantes;

    vector<list<int> > grafoTransposto = transporGrafo();

    //busca os comandantes diretos do aluno1
    for(iterator = grafoTransposto[aluno1].begin(); iterator != grafoTransposto[aluno1].end(); ++iterator){
            id = *iterator;
            comandantes.push(id); 
    }

    id = comandantes.front();
    idadeMin = alunos[id].idade; //inicializando a idade minima com a idade de um dos comandantes

    //a partir dos comandantes diretos percorre as listas de adjacências
    //para encontrar os comandantes indiretos
    while(!comandantes.empty()){
        id = comandantes.front();
        
        if(alunos[id].idade < idadeMin)
                idadeMin = alunos[id].idade;

        for(iterator = grafoTransposto[id].begin(); iterator != grafoTransposto[id].end(); ++iterator){
            idPai = *iterator;
            comandantes.push(idPai);
            if(alunos[idPai].idade < idadeMin)
                idadeMin = alunos[idPai].idade;
        }

        comandantes.pop();
    }
             
    return idadeMin;   
}


bool Time::existeAresta(int aluno1, int aluno2){//procura uma correspondência de um aluno na lista de adjacencia do outro
    list<int>::iterator iterator;

    for(iterator = alunos[aluno1].adjList.begin(); iterator != alunos[aluno1].adjList.end(); ++iterator){
            if(*iterator == aluno2){
                return true;
            }
    }
    
    for(iterator = alunos[aluno2].adjList.begin(); iterator != alunos[aluno2].adjList.end(); ++iterator){
            if(*iterator == aluno1){
                return true;
            }
    }
    
    return false;

}

void Time::swap(int aluno1, int aluno2){
    
    list<int>::iterator iterator;
    //ao encontrar a correspondência de um aluno na lista do outro
    //deletamos e colocamos na lista do primeiro aluno, invertendo a relação de hierarquia
    for(iterator = alunos[aluno1].adjList.begin(); iterator != alunos[aluno1].adjList.end(); ++iterator){
        if((*iterator) == (aluno2)){
            alunos[aluno1].adjList.erase(iterator);
            alunos[aluno2].adjList.push_back(aluno1);
            return;   
        }
    }

    for(iterator = alunos[aluno2].adjList.begin(); iterator != alunos[aluno2].adjList.end(); ++iterator){
        if((*iterator) == (aluno1)){
            alunos[aluno2].adjList.erase(iterator);
            alunos[aluno1].adjList.push_back(aluno2);
            return;   
        }
    }   

}


bool Time::temCiclo(){
	vector<bool> visited(this->vertices);
    vector<bool> stack(this->vertices);


	for (int i = 0; i < this->vertices; i++){
		if (buscaCiclo(i, visited, stack)){
			return true;
        }
    }
    return false;
}

bool Time::buscaCiclo(int v, vector<bool> &visited,vector<bool> &stack ){
    list<int>::iterator iterator;
    visited[v] = true;
    stack[v] = true;

    for(iterator = alunos[v].adjList.begin(); iterator != alunos[v].adjList.end(); ++iterator){
            int idAdj = *iterator;
            if (!visited[idAdj] && buscaCiclo(idAdj, visited, stack)) {
                return true;
            } 
            else if (stack[idAdj])
                return true;
    }
        //nesse ponto, significa que não foi encontrado um ciclo a partir do vertice v
        stack[v] = false;
        return false;
}

