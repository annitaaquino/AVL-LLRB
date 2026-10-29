# Amazon Engineering Challenge — Árvores AVL e Rubro-Negra (C++)

**Autor:** _Annita Aquino Freitas_
**Matrícula:** _20250029891_


Protótipo para registrar, localizar e remover identificadores de encomendas (inteiros)
durante a Black Friday, com duas árvores balanceadas implementadas.

## Estrutura

```
Arvores.hpp            interface comum (inserir, remover, buscar, emOrdem, imprimir, altura, validar...)
AVL.hpp/.cpp           árvore AVL
RubroNegra.hpp/.cpp    árvore Rubro-Negra (variante LLRB)
main.cpp               programa interativo (usuário escolhe AVL ou Rubro-Negra)
testes.cpp             testes automáticos + comparação entre as árvores
resultado_testes.txt  saída dos testes que aparece na apresentação
```

## Como compilar e executar

Requer apenas `g++` (C++11)

```
./arvores     # programa interativo
./testes      # testes e comparação AVL x Rubro-Negra
```

`g++ -std=c++11 -Wall -Wextra -O2 -o arvores main.cpp avl.cpp rubronegra.cpp `

## Funcionalidades

Menu: **1** Inserir · **2** Remover · **3** Buscar · **4** Em ordem · **5** Imprimir · **6** Estatísticas (altura, rotações, recolorações, validação) · **0** Sair.

A impressão mostra a árvore "deitada" (direita em cima). Na AVL aparece o fator de
balanceamento `(fb=..)`; na Rubro-Negra aparece a cor: `(P)` preto, `(V)` vermelho.

Exemplo:
```
$ ./arvores
Escolha a arvore (1 = AVL, 2 = Rubro-Negra): 2
> 1 -> 20, 1 -> 10, 1 -> 30
> 5
      30 (P)
20 (P)
      10 (P)
```

## Decisões de implementação

- **Valores repetidos:** inserção ignorada, retorna `false` e o programa avisa o usuário.
- **Busca/remoção de inexistente:** não altera a árvore; retorna `false` e o programa avisa.
- **Remoção com 2 filhos:** troca pelo sucessor em ordem (menor da subárvore direita).
- **AVL:** cada nó guarda sua altura; após inserir/remover, a volta da recursão recalcula
  altura e fb e aplica LL/RR (rotação simples) ou LR/RL (duas simples).
- **Rubro-Negra:** foi usada a variante **LLRB** por ser mais simples de implementar. 
  Ligações vermelhas ficam sempre à esquerda. Nó novo entra vermelho; a correção usa 
  3 regras (rotação esquerda, rotação direita, troca de cores).
- **Contadores:** `rotacoes()` conta rotações **simples** (uma rotação dupla conta 2);
  `recolorizacoes()` conta trocas de cores (só na Rubro-Negra).
- **Altura:** contada em nós (vazia = 0, folha = 1).
- **Validação (`validar()`):** AVL confere ordem da ABB, altura guardada e |fb| ≤ 1;
  Rubro-Negra confere raiz preta, sem dois vermelhos seguidos, sem vermelho à direita e
  mesma altura negra em todos os caminhos.

## Testes

Para compilar a parte dos testes use o seguinte comando:

`g++ -std=c++11 -Wall -Wextra -O2 -o testes testes.cpp avl.cpp rubronegra.cpp `

`./testes` executa, nas duas árvores (AVL e Rubro-Negra LLRB), os seguintes grupos de teste:

- **Árvore vazia e com 1 elemento:** busca e remoção em árvore vazia, inserção de um elemento,
  inserção repetida (ignorada), remoção do único elemento e remoção em árvore que voltou a ficar vazia.
- **Inserção crescente, decrescente e aleatória** (n = 1000): as propriedades de balanceamento são
  validadas a cada inserção, o percurso em ordem deve resultar em 1..1000 e a altura deve respeitar
  o limite teórico (≤ 2·log₂(n+1)).
- **Busca** de elementos existentes (pares de 2 a 200) e inexistentes (ímpares e valores fora do intervalo).
- **Remoção** de folha, de nó com 1 filho, de nó com 2 filhos, da raiz e de elemento inexistente
  (que não altera a árvore). Em seguida, todos os elementos são removidos até a árvore esvaziar,
  validando as propriedades a cada remoção.
- **Teste aleatório:** 20 000 operações (inserir, remover, buscar) comparadas com um `std::set`
  (chaves de 0 a 999), conferindo os valores retornados, as propriedades de balanceamento, o limite
  de altura a cada operação e o conteúdo final.

No final, o programa **compara AVL e Rubro-Negra com os mesmos dados**, em dois critérios:

- **Altura e ajustes** (n = 1000): inserção de 1000 chaves em ordem crescente, decrescente e
  aleatória, seguida da remoção das mesmas 500 chaves nas duas árvores. Mostra altura, rotações
  (simples) e recolorações, e também a média de 30 sequências aleatórias diferentes.
- **Tempo de execução** (n = 100 000): mede, em milissegundos, a inserção de n chaves, a busca de
  n chaves em ordem aleatória e a remoção de n/2 chaves, nas três ordens de entrada. Cada fase é
  repetida 5 vezes e se guarda o melhor tempo, para reduzir ruído do sistema. A validação das
  propriedades fica fora da medição. O programa também imprime a razão entre os tempos
  (Rubro-Negra / AVL), pois os valores absolutos dependem da máquina.

Resultado atual: **234/234 verificações ok**. A saída completa está em `resultado_testes.txt`.