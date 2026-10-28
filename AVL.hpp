// Árvore AVL: em todo nó, |altura(esq) - altura(dir)| <= 1.
#pragma once
#include "Arvores.hpp"

class AVL : public Arvore {
    struct No {
        int chave, alt;
        No *esq, *dir;
        No(int c) : chave(c), alt(1), esq(0), dir(0) {}
    };
    No* raiz;
    long nRot;

    static int alt(No* n) { return n ? n->alt : 0; }
    static int fb(No* n) { return n ? alt(n->esq) - alt(n->dir) : 0; }
    static void atualizar(No* n);
    No* rotDir(No* a);
    No* rotEsq(No* a);
    No* balancear(No* n);
    No* inserir(No* n, int x, bool& ok);
    No* remover(No* n, int x, bool& ok);
    static void liberar(No* n);
    static void emOrdem(No* n, std::vector<int>& v);
    static void imprimir(No* n, int nivel);
    static int checar(No* n, long lo, long hi);   // devolve altura ou -1 se inválida

public:
    AVL() : raiz(0), nRot(0) {}
    ~AVL() { liberar(raiz); }
    bool inserir(int x);
    bool remover(int x);
    bool buscar(int x) const;
    void emOrdem(std::vector<int>& saida) const { emOrdem(raiz, saida); }
    void imprimir() const;
    int altura() const { return alt(raiz); }
    bool validar() const { return checar(raiz, -2147483648LL, 2147483647LL) >= 0; }
    long rotacoes() const { return nRot; }
    const char* nome() const { return "AVL"; }
};