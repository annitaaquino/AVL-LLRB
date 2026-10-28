// Árvore Rubro-Negra na variante LLRB (Sedgewick): ligações vermelhas
// sempre inclinadas para a esquerda. Equivalente a uma árvore 2-3.
#pragma once
#include "Arvores.hpp"

class RubroNegra : public Arvore {
    enum Cor { PRETO = 0, VERMELHO = 1 };
    struct No {
        int chave;
        Cor cor;
        No *esq, *dir;
        No(int c) : chave(c), cor(VERMELHO), esq(0), dir(0) {}  // todo nó novo é vermelho
    };
    No* raiz;
    long nRot, nCor;

    static bool vermelho(No* n) { return n && n->cor == VERMELHO; }
    No* rotEsq(No* h);
    No* rotDir(No* h);
    void trocarCores(No* h);
    No* corrigir(No* h);
    No* moverVermEsq(No* h);
    No* moverVermDir(No* h);
    No* removerMin(No* h);
    No* inserir(No* h, int x, bool& ok);
    No* remover(No* h, int x);
    static void liberar(No* n);
    static void emOrdem(No* n, std::vector<int>& v);
    static void imprimir(No* n, int nivel);
    static int alturaDe(No* n);
    static int checar(No* n, long lo, long hi);   // altura negra ou -1 se inválida

public:
    RubroNegra() : raiz(0), nRot(0), nCor(0) {}
    ~RubroNegra() { liberar(raiz); }
    bool inserir(int x);
    bool remover(int x);
    bool buscar(int x) const;
    void emOrdem(std::vector<int>& saida) const { emOrdem(raiz, saida); }
    void imprimir() const;
    int altura() const { return alturaDe(raiz); }
    bool validar() const;
    long rotacoes() const { return nRot; }
    long recolorizacoes() const { return nCor; }
    const char* nome() const { return "Rubro-Negra (LLRB)"; }
};