// Programa interativo: o usuário escolhe AVL ou Rubro-Negra e opera sobre ela.
#include <iostream>
#include <limits>
#include "AVL.hpp"
#include "RubroNegra.hpp"
using namespace std;

static int lerInt(const char* msg) {
    int x;
    cout << msg;
    while (!(cin >> x)) {
        if (cin.eof()) exit(0);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Valor invalido. " << msg;
    }
    return x;
}

int main() {
    cout << "=== Logistica Black Friday - Arvores Balanceadas ===\n";
    int tipo;
    do tipo = lerInt("1) AVL  2) Rubro-Negra)\n> ");
    while (tipo != 1 && tipo != 2);

    Arvore* t = (tipo == 1) ? (Arvore*)new AVL() : (Arvore*)new RubroNegra();
    cout << "Usando: " << t->nome() << "\n";

    int op;
    do {
        cout << "\n1) Inserir  2) Remover  3) Buscar  4) Em ordem\n5) Imprimir  6) Altura/Rotacao/Validacao  0) Sair\n";
        op = lerInt("> ");
        if (op == 1) {
            int x = lerInt("Id da encomenda: ");
            cout << (t->inserir(x) ? "Inserido.\n" : "Valor repetido: insercao ignorada.\n");
        } else if (op == 2) {
            int x = lerInt("Id da encomenda: ");
            cout << (t->remover(x) ? "Removido.\n" : "Id inexistente: nada removido.\n");
        } else if (op == 3) {
            int x = lerInt("Id da encomenda: ");
            cout << (t->buscar(x) ? "Encontrado.\n" : "Nao encontrado.\n");
        } else if (op == 4) {
            vector<int> v;
            t->emOrdem(v);
            if (v.empty()) cout << "(arvore vazia)";
            for (size_t i = 0; i < v.size(); i++) cout << v[i] << " ";
            cout << "\n";
        } else if (op == 5) {
            t->imprimir();
        } else if (op == 6) {
            cout << "Altura: " << t->altura() << " | Rotacoes: " << t->rotacoes()
                 << " | Recoloracoes: " << t->recolorizacoes()
                 << " | Propriedades validas: " << (t->validar() ? "sim" : "NAO") << "\n";
        }
    } while (op != 0);

    delete t;
    return 0;
}