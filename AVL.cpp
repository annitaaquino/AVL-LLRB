#include "AVL.hpp"
#include <iostream>
#include <algorithm>
using namespace std;

void AVL::atualizar(No* n) { n->alt = max(alt(n->esq), alt(n->dir)) + 1; }

// Rotação simples à direita (caso LL): o filho esquerdo sobe.
AVL::No* AVL::rotDir(No* a) {
    No* b = a->esq;
    a->esq = b->dir;      // subárvore do meio troca de pai
    b->dir = a;
    atualizar(a); atualizar(b);
    nRot++;
    return b;
}

// Rotação simples à esquerda (caso RR): o filho direito sobe.
AVL::No* AVL::rotEsq(No* a) {
    No* b = a->dir;
    a->dir = b->esq;
    b->esq = a;
    atualizar(a); atualizar(b);
    nRot++;
    return b;
}

// Recalcula altura e aplica a rotação indicada pelo fator de balanceamento.
// Sinais diferentes (LR / RL) = rotação dupla = duas simples.
AVL::No* AVL::balancear(No* n) {
    atualizar(n);
    int f = fb(n);
    if (f > 1) {                                   // pesado à esquerda
        if (fb(n->esq) < 0) n->esq = rotEsq(n->esq);   // LR: primeiro esquerda no filho
        return rotDir(n);                               // LL
    }
    if (f < -1) {                                  // pesado à direita
        if (fb(n->dir) > 0) n->dir = rotDir(n->dir);   // RL: primeiro direita no filho
        return rotEsq(n);                               // RR
    }
    return n;
}

AVL::No* AVL::inserir(No* n, int x, bool& ok) {
    if (!n) { ok = true; return new No(x); }
    if (x < n->chave) n->esq = inserir(n->esq, x, ok);
    else if (x > n->chave) n->dir = inserir(n->dir, x, ok);
    else return n;                                 // duplicado: ignora
    return balancear(n);
}

AVL::No* AVL::remover(No* n, int x, bool& ok) {
    if (!n) return 0;                              // não existe
    if (x < n->chave) n->esq = remover(n->esq, x, ok);
    else if (x > n->chave) n->dir = remover(n->dir, x, ok);
    else {
        if (!n->esq || !n->dir) {                  // 0 ou 1 filho
            No* filho = n->esq ? n->esq : n->dir;
            delete n;
            ok = true;
            return filho;
        }
        No* s = n->dir;                            // 2 filhos: sucessor em ordem
        while (s->esq) s = s->esq;
        n->chave = s->chave;
        n->dir = remover(n->dir, s->chave, ok);
    }
    return balancear(n);
}

bool AVL::inserir(int x) { bool ok = false; raiz = inserir(raiz, x, ok); return ok; }
bool AVL::remover(int x) { bool ok = false; raiz = remover(raiz, x, ok); return ok; }

bool AVL::buscar(int x) const {
    No* n = raiz;
    while (n) {
        if (x == n->chave) return true;
        n = (x < n->chave) ? n->esq : n->dir;
    }
    return false;
}

void AVL::liberar(No* n) { if (n) { liberar(n->esq); liberar(n->dir); delete n; } }

void AVL::emOrdem(No* n, vector<int>& v) {
    if (!n) return;
    emOrdem(n->esq, v); v.push_back(n->chave); emOrdem(n->dir, v);
}

// Árvore "deitada": direita em cima, esquerda embaixo. Mostra chave e fb.
void AVL::imprimir(No* n, int nivel) {
    if (!n) return;
    imprimir(n->dir, nivel + 1);
    cout << string(nivel * 6, ' ') << n->chave << " (fb=" << fb(n) << ")\n";
    imprimir(n->esq, nivel + 1);
}

void AVL::imprimir() const {
    if (!raiz) cout << "(arvore vazia)\n"; else imprimir(raiz, 0);
}

// Confere ordem da ABB, altura guardada e |fb| <= 1 em todos os nós.
int AVL::checar(No* n, long lo, long hi) {
    if (!n) return 0;
    if (n->chave <= lo || n->chave >= hi) return -1;
    int e = checar(n->esq, lo, n->chave);
    int d = checar(n->dir, n->chave, hi);
    if (e < 0 || d < 0) return -1;
    if (abs(e - d) > 1) return -1;
    if (n->alt != max(e, d) + 1) return -1;
    return n->alt;
}