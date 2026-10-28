#include "RubroNegra.hpp"
#include <iostream>
#include <algorithm>
using namespace std;

// O nó que sobe herda a cor do antigo topo; o que desce fica vermelho.
RubroNegra::No* RubroNegra::rotEsq(No* h) {
    No* x = h->dir;
    h->dir = x->esq;
    x->esq = h;
    x->cor = h->cor;
    h->cor = VERMELHO;
    nRot++;
    return x;
}

RubroNegra::No* RubroNegra::rotDir(No* h) {
    No* x = h->esq;
    h->esq = x->dir;
    x->dir = h;
    x->cor = h->cor;
    h->cor = VERMELHO;
    nRot++;
    return x;
}

// Inverte a cor do nó e dos dois filhos (flipColors).
void RubroNegra::trocarCores(No* h) {
    h->cor = (h->cor == VERMELHO) ? PRETO : VERMELHO;
    h->esq->cor = (h->esq->cor == VERMELHO) ? PRETO : VERMELHO;
    h->dir->cor = (h->dir->cor == VERMELHO) ? PRETO : VERMELHO;
    nCor++;
}

// As 3 regras da LLRB, na ordem 1 -> 2 -> 3, aplicadas na volta da recursão.
RubroNegra::No* RubroNegra::corrigir(No* h) {
    if (vermelho(h->dir) && !vermelho(h->esq)) h = rotEsq(h);               // regra 1
    if (vermelho(h->esq) && vermelho(h->esq->esq)) h = rotDir(h);           // regra 2
    if (vermelho(h->esq) && vermelho(h->dir)) trocarCores(h);               // regra 3
    return h;
}

RubroNegra::No* RubroNegra::inserir(No* h, int x, bool& ok) {
    if (!h) { ok = true; return new No(x); }
    if (x < h->chave) h->esq = inserir(h->esq, x, ok);
    else if (x > h->chave) h->dir = inserir(h->dir, x, ok);
    else return h;                                  // duplicado: ignora
    return corrigir(h);
}

// Empresta um vermelho para o lado esquerdo antes de descer.
RubroNegra::No* RubroNegra::moverVermEsq(No* h) {
    trocarCores(h);
    if (vermelho(h->dir->esq)) {
        h->dir = rotDir(h->dir);
        h = rotEsq(h);
        trocarCores(h);
    }
    return h;
}

// Empresta um vermelho para o lado direito antes de descer.
RubroNegra::No* RubroNegra::moverVermDir(No* h) {
    trocarCores(h);
    if (vermelho(h->esq->esq)) {
        h = rotDir(h);
        trocarCores(h);
    }
    return h;
}

RubroNegra::No* RubroNegra::removerMin(No* h) {
    if (!h->esq) { delete h; return 0; }
    if (!vermelho(h->esq) && !vermelho(h->esq->esq)) h = moverVermEsq(h);
    h->esq = removerMin(h->esq);
    return corrigir(h);
}

// Só é chamada se x existe (verificado em remover(int)).
RubroNegra::No* RubroNegra::remover(No* h, int x) {
    if (x < h->chave) {
        if (!vermelho(h->esq) && !vermelho(h->esq->esq)) h = moverVermEsq(h);
        h->esq = remover(h->esq, x);
    } else {
        if (vermelho(h->esq)) h = rotDir(h);
        if (x == h->chave && !h->dir) { delete h; return 0; }
        if (!vermelho(h->dir) && !vermelho(h->dir->esq)) h = moverVermDir(h);
        if (x == h->chave) {                        // 2 filhos: troca pelo sucessor
            No* s = h->dir;
            while (s->esq) s = s->esq;
            h->chave = s->chave;
            h->dir = removerMin(h->dir);
        } else {
            h->dir = remover(h->dir, x);
        }
    }
    return corrigir(h);
}

bool RubroNegra::inserir(int x) {
    bool ok = false;
    raiz = inserir(raiz, x, ok);
    raiz->cor = PRETO;                              // raiz sempre preta
    return ok;
}

bool RubroNegra::remover(int x) {
    if (!buscar(x)) return false;                   // inexistente: não faz nada
    if (!vermelho(raiz->esq) && !vermelho(raiz->dir)) raiz->cor = VERMELHO;
    raiz = remover(raiz, x);
    if (raiz) raiz->cor = PRETO;
    return true;
}

bool RubroNegra::buscar(int x) const {
    No* n = raiz;
    while (n) {
        if (x == n->chave) return true;
        n = (x < n->chave) ? n->esq : n->dir;
    }
    return false;
}

void RubroNegra::liberar(No* n) { if (n) { liberar(n->esq); liberar(n->dir); delete n; } }

void RubroNegra::emOrdem(No* n, vector<int>& v) {
    if (!n) return;
    emOrdem(n->esq, v); v.push_back(n->chave); emOrdem(n->dir, v);
}

int RubroNegra::alturaDe(No* n) {
    return n ? max(alturaDe(n->esq), alturaDe(n->dir)) + 1 : 0;
}

// Árvore "deitada": direita em cima. (P) = preto, (V) = vermelho.
void RubroNegra::imprimir(No* n, int nivel) {
    if (!n) return;
    imprimir(n->dir, nivel + 1);
    cout << string(nivel * 6, ' ') << n->chave << (n->cor == VERMELHO ? " (V)" : " (P)") << "\n";
    imprimir(n->esq, nivel + 1);
}

void RubroNegra::imprimir() const {
    if (!raiz) cout << "(arvore vazia)\n"; else imprimir(raiz, 0);
}

// Verifica: ordem da ABB, sem dois vermelhos seguidos, sem vermelho à direita
// (LLRB) e mesma altura negra em todos os caminhos.
int RubroNegra::checar(No* n, long lo, long hi) {
    if (!n) return 1;                               // NIL conta como preto
    if (n->chave <= lo || n->chave >= hi) return -1;
    if (vermelho(n->dir)) return -1;
    if (vermelho(n) && (vermelho(n->esq) || vermelho(n->dir))) return -1;
    int e = checar(n->esq, lo, n->chave);
    int d = checar(n->dir, n->chave, hi);
    if (e < 0 || d < 0 || e != d) return -1;
    return e + (n->cor == PRETO ? 1 : 0);
}

bool RubroNegra::validar() const {
    if (vermelho(raiz)) return false;               // raiz tem que ser preta
    return checar(raiz, -2147483648LL, 2147483647LL) >= 0;
}