// Testes automáticos das árvores AVL e Rubro-Negra + comparação com os mesmos dados.
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <sstream>
#include "AVL.hpp"
#include "RubroNegra.hpp"
using namespace std;
 
static int total = 0, falhas = 0;
#define CHECK(cond, msg) do { total++; if (!(cond)) { falhas++; \
    cout << "  [FALHOU] " << msg << "  (linha " << __LINE__ << ")\n"; } } while (0)
 
static Arvore* nova(int tipo) { return tipo == 0 ? (Arvore*)new AVL() : (Arvore*)new RubroNegra(); }
 
static vector<int> conteudo(const Arvore& t) { vector<int> v; t.emOrdem(v); return v; }
 
// Limite teórico de altura (em nós): AVL <= 1,44 log2(n+2); LLRB <= 2 log2(n+1). Usa-se o maior (2 log2(n+1)).
static bool alturaOk(const Arvore& t, size_t n) { return t.altura() <= 2 * log2((double)n + 1) + 1e-9; }
 
static vector<int> seq(const string& tipo, int n, unsigned seed = 42) {
    vector<int> v;
    for (int i = 1; i <= n; i++) v.push_back(i);
    if (tipo == "decrescente") reverse(v.begin(), v.end());
    if (tipo == "aleatoria") { mt19937 g(seed); shuffle(v.begin(), v.end(), g); }
    return v;
}
 
// 1. vazia e um elemento
static void testeVaziaUmElemento(Arvore& t) {
    cout << " - arvore vazia e com 1 elemento\n";
    CHECK(t.altura() == 0, "vazia: altura 0");
    CHECK(!t.buscar(5), "vazia: buscar falha");
    CHECK(!t.remover(5), "vazia: remover retorna false");
    CHECK(conteudo(t).empty(), "vazia: em ordem vazio");
    CHECK(t.validar(), "vazia: valida");
    CHECK(t.inserir(7), "1 elem: inserir");
    CHECK(!t.inserir(7), "1 elem: duplicado ignorado");
    CHECK(t.altura() == 1 && t.validar(), "1 elem: altura 1 e valida");
    CHECK(t.buscar(7) && !t.buscar(8), "1 elem: busca");
    CHECK(!t.remover(8), "1 elem: remover inexistente");
    CHECK(t.remover(7), "1 elem: remover unico");
    CHECK(t.altura() == 0 && t.validar() && conteudo(t).empty(), "1 elem: volta a vazia");
    CHECK(!t.remover(7), "vazia de novo: remover false");
}
 
// 2. inserção crescente/decrescente/aleatória
static void testeInsercao(Arvore& t, const string& tipo) {
    cout << " - insercao " << tipo << " (n=1000), validando a cada passo\n";
    vector<int> v = seq(tipo, 1000);
    bool todasValidas = true;
    for (size_t i = 0; i < v.size(); i++) {
        t.inserir(v[i]);
        if (!t.validar()) { todasValidas = false; break; }
    }
    CHECK(todasValidas, tipo + ": propriedades validas apos cada insercao");
    vector<int> esperado = seq("crescente", 1000);
    CHECK(conteudo(t) == esperado, tipo + ": em ordem == 1..1000");
    CHECK(alturaOk(t, 1000), tipo + ": altura dentro do limite teorico");
}
 
// 3. busca 
static void testeBusca(Arvore& t) {
    cout << " - busca de existentes e inexistentes\n";
    for (int i = 1; i <= 100; i++) t.inserir(i * 2);          // pares 2..200
    bool ok = true;
    for (int i = 1; i <= 100; i++) ok = ok && t.buscar(i * 2);
    CHECK(ok, "todos os existentes encontrados");
    ok = true;
    for (int i = 0; i <= 100; i++) ok = ok && !t.buscar(i * 2 + 1);   // impares
    CHECK(ok, "nenhum inexistente encontrado");
    CHECK(!t.buscar(-5) && !t.buscar(0) && !t.buscar(1000000), "limites inexistentes");
}
 
// 4. remoção com diferentes configurações
static void remover1(Arvore& t, int x, const string& desc, vector<int>& ref) {
    bool r = t.remover(x);
    ref.erase(remove(ref.begin(), ref.end(), x), ref.end());
    CHECK(r, desc + ": retorna true");
    CHECK(!t.buscar(x), desc + ": chave some");
    CHECK(conteudo(t) == ref, desc + ": demais chaves preservadas");
    CHECK(t.validar(), desc + ": propriedades validas");
}
 
static void testeRemocao(Arvore& t) {
    cout << " - remocao: folha, 1 filho, 2 filhos, raiz, inexistente\n";
    int dados[] = {50, 30, 70, 20, 40, 60, 80, 10, 35, 45, 65, 90, 5, 15};
    vector<int> ref;
    for (size_t i = 0; i < sizeof(dados) / sizeof(int); i++) { t.inserir(dados[i]); ref.push_back(dados[i]); }
    sort(ref.begin(), ref.end());
    CHECK(t.validar(), "arvore inicial valida");
    remover1(t, 5,  "folha (5)", ref);
    remover1(t, 90, "folha/no com 0-1 filho (90)", ref);
    remover1(t, 20, "no com 1 filho ou 2 (20)", ref);
    remover1(t, 30, "no com 2 filhos (30)", ref);
    remover1(t, 50, "raiz original (50)", ref);
    size_t antes = conteudo(t).size();
    CHECK(!t.remover(999), "inexistente: retorna false");
    CHECK(!t.remover(50), "ja removido: retorna false");
    CHECK(conteudo(t).size() == antes && t.validar(), "inexistente nao altera a arvore");
    // esvazia tudo
    vector<int> resto = ref;
    bool ok = true;
    for (size_t i = 0; i < resto.size(); i++) ok = ok && t.remover(resto[i]) && t.validar();
    CHECK(ok && conteudo(t).empty() && t.altura() == 0, "remover todos ate esvaziar");
}
 
// 5. aleatório contra std::set
static void testeAleatorio(Arvore& t, unsigned seed) {
    cout << " - 20000 operacoes aleatorias comparadas com std::set\n";
    mt19937 g(seed);
    set<int> ref;
    bool iguais = true, validas = true, alturas = true;
    for (int i = 0; i < 20000; i++) {
        int x = g() % 1000, op = g() % 3;
        if (op == 0) { bool a = t.inserir(x), b = ref.insert(x).second; iguais = iguais && (a == b); }
        else if (op == 1) { bool a = t.remover(x), b = ref.erase(x) > 0; iguais = iguais && (a == b); }
        else { iguais = iguais && (t.buscar(x) == (ref.count(x) > 0)); }
        if (!t.validar()) validas = false;
        if (!alturaOk(t, ref.size())) alturas = false;
    }
    CHECK(iguais, "retornos de inserir/remover/buscar identicos ao std::set");
    CHECK(validas, "propriedades validas apos cada operacao");
    CHECK(alturas, "altura sempre dentro do limite teorico");
    CHECK(conteudo(t) == vector<int>(ref.begin(), ref.end()), "conteudo final identico ao std::set");
}
 
// 6. comparação
struct Medida { int altIns, altRem; long rotIns, rotRem, corIns, corRem; };
 
static Medida medir(int tipo, const vector<int>& ins, const vector<int>& rem) {
    Arvore* t = nova(tipo);
    Medida m = {0, 0, 0, 0, 0, 0};
    for (size_t i = 0; i < ins.size(); i++) t->inserir(ins[i]);
    m.altIns = t->altura(); m.rotIns = t->rotacoes(); m.corIns = t->recolorizacoes();
    for (size_t i = 0; i < rem.size(); i++) t->remover(rem[i]);
    m.altRem = t->altura(); m.rotRem = t->rotacoes() - m.rotIns; m.corRem = t->recolorizacoes() - m.corIns;
    CHECK(t->validar(), string("comparacao: ") + t->nome() + " valida ao final");
    delete t;
    return m;
}
 
static void linha(const string& nome, const Medida& a, const Medida& r) {
    cout << left << setw(22) << nome << right
         << " | AVL h=" << setw(2) << a.altIns << "/" << setw(2) << a.altRem
         << " rot=" << setw(5) << a.rotIns << "/" << setw(4) << a.rotRem
         << " | RN h=" << setw(2) << r.altIns << "/" << setw(2) << r.altRem
         << " rot=" << setw(5) << r.rotIns << "/" << setw(4) << r.rotRem
         << " cor=" << setw(5) << r.corIns << "/" << setw(4) << r.corRem << "\n";
}
 
static void comparacao() {
    cout << "\n=== COMPARACAO AVL x RUBRO-NEGRA (mesmos dados, n = 1000, remove metade) ===\n";
    cout << "Formato: valor apos insercao / valor da fase de remocao (h = altura final)\n";
    cout << "log2(1001) = " << fixed << setprecision(2) << log2(1001.0) << "\n";
    string tipos[] = {"crescente", "decrescente", "aleatoria"};
    for (int k = 0; k < 3; k++) {
        vector<int> ins = seq(tipos[k], 1000);
        vector<int> rem = seq("aleatoria", 1000, 7);
        rem.resize(500);                                  // mesma metade removida nas duas
        linha(tipos[k], medir(0, ins, rem), medir(1, ins, rem));
    }
    // 30 sementes aleatorias: medias
    cout << "\nMedia de 30 sequencias aleatorias distintas (n = 1000, remove 500):\n";
    double aH[2] = {0, 0}, aR[2] = {0, 0}, rH[2] = {0, 0}, rR[2] = {0, 0}, rC[2] = {0, 0};
    int maxA = 0, maxR = 0;
    for (unsigned s = 1; s <= 30; s++) {
        vector<int> ins = seq("aleatoria", 1000, s), rem = seq("aleatoria", 1000, s + 100);
        rem.resize(500);
        Medida a = medir(0, ins, rem), r = medir(1, ins, rem);
        aH[0] += a.altIns; aR[0] += a.rotIns; aR[1] += a.rotRem;
        rH[0] += r.altIns; rR[0] += r.rotIns; rR[1] += r.rotRem; rC[0] += r.corIns; rC[1] += r.corRem;
        maxA = max(maxA, a.altIns); maxR = max(maxR, r.altIns);
    }
    cout << "  AVL: altura media " << aH[0] / 30 << " (max " << maxA << "), rotacoes ins " << aR[0] / 30
         << ", rem " << aR[1] / 30 << "\n";
    cout << "  RN : altura media " << rH[0] / 30 << " (max " << maxR << "), rotacoes ins " << rR[0] / 30
         << ", rem " << rR[1] / 30 << ", recolor. ins " << rC[0] / 30 << ", rem " << rC[1] / 30 << "\n";
}
 
int main() {
    for (int tipo = 0; tipo < 2; tipo++) {
        Arvore* ref = nova(tipo);
        cout << "\n=== Testes: " << ref->nome() << " ===\n";
        delete ref;
        { Arvore* t = nova(tipo); testeVaziaUmElemento(*t); delete t; }
        string ts[] = {"crescente", "decrescente", "aleatoria"};
        for (int k = 0; k < 3; k++) { Arvore* t = nova(tipo); testeInsercao(*t, ts[k]); delete t; }
        { Arvore* t = nova(tipo); testeBusca(*t); delete t; }
        { Arvore* t = nova(tipo); testeRemocao(*t); delete t; }
        { Arvore* t = nova(tipo); testeAleatorio(*t, 12345); delete t; }
    }
    comparacao();
    cout << "\nResultado: " << (total - falhas) << "/" << total << " verificacoes ok\n";
    return falhas ? 1 : 0;
}