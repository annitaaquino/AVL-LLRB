// Interface comum das duas árvores (AVL e Rubro-Negra).
// Assim o main e os testes usam as duas do mesmo jeito.
#pragma once
#include <vector>
 
class Arvore {
public:
    virtual ~Arvore() {}
    virtual bool inserir(int x) = 0;               // false se x já existe
    virtual bool remover(int x) = 0;               // false se x não existe
    virtual bool buscar(int x) const = 0;
    virtual void emOrdem(std::vector<int>& saida) const = 0;
    virtual void imprimir() const = 0;
    virtual int altura() const = 0;                // em nós (vazia = 0, folha = 1)
    virtual bool validar() const = 0;              // confere as propriedades da árvore
    virtual long rotacoes() const = 0;             // rotações simples realizadas
    virtual long recolorizacoes() const { return 0; }
    virtual const char* nome() const = 0;
};