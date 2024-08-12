#include <stdio.h>
#include <stdlib.h>

// Nome: Pedro Henrique Ferreira da Silva
// Matrícula: 535770

typedef enum cor {
    N = 'N',
    R = 'R'
} Cor;

typedef enum boleano {
    V = 1,
    F = 0
} Boleano;

typedef struct no {
    int chave;
    Cor cor;

    struct no *pai;

    struct no *esq;
    struct no *dir;
} No;

No *externo = NULL;

No* IniciarExterno() {
    No *no = malloc(sizeof(No));

    no->cor = N;

    return no;
}

No* IniciarNo(int x) {
    No *no = malloc(sizeof(No));

    no->chave = x;

    return no;
}

No* BuscarNo(int x, No *ptraiz) {
    No *y = ptraiz;

    while (y != externo) {
        if (y->chave == x) {
            break;
        } else {
            if (x < y->chave) {
                y = y->esq;
            } else {
                y = y->dir;
            }
        }
    }

    return y;
}

void MoverPai(No *u, No *v, No **ptraiz) {
    if (u->pai == externo) {
        *ptraiz = v;
    } else {
        if (u == u->pai->esq) {
            u->pai->esq = v;
        } else {
            u->pai->dir = v;
        }
    }

    v->pai = u->pai;
}

void RotacaoD(No *x, No **ptraiz) {
    No *y = x->esq;

    x->esq = y->dir;
    if (y->dir != externo) y->dir->pai = x;
    
    MoverPai(x, y, ptraiz);

    y->dir = x;
    x->pai = y;
}

void RotacaoE(No *x, No **ptraiz) {
    No *y = x->dir;

    x->dir = y->esq;
    if (y->esq != externo) y->esq->pai = x;

    MoverPai(x, y, ptraiz);

    y->esq = x;
    x->pai = y;
}

void RotaRN(No *z, No **ptraiz) {
    while (z->pai->cor == R) {
        if (z->pai == z->pai->pai->esq) {
            No *y = z->pai->pai->dir;

            if (y->cor == R) {
                z->pai->cor = y->cor = N;
                z->pai->pai->cor = R;
                z = z->pai->pai;
            } else {
                if (z == z->pai->dir) {
                    z = z->pai;
                    RotacaoE(z, ptraiz);
                }

                z->pai->cor = N;
                z->pai->pai->cor = R;
                RotacaoD(z->pai->pai, ptraiz);
            }
        } else {
            No *y = z->pai->pai->esq;

            if (y->cor == R) {
                z->pai->cor = y->cor = N;
                z->pai->pai->cor = R;
                z = z->pai->pai;

            } else {
                if (z == z->pai->esq) {
                    z = z->pai;
                    RotacaoD(z, ptraiz);
                }

                z->pai->cor = N;
                z->pai->pai->cor = R;
                RotacaoE(z->pai->pai, ptraiz);
            }
        }
    }

    (*ptraiz)->cor = N;
}

void InserirRN(No *z, No **ptraiz) {
    No *y = externo; No *pt = *ptraiz;

    while (pt != externo) {
        y = pt;

        if (z->chave == pt->chave) {
            /* Chave existente */ y = NULL; pt = externo;
        } else {
            if (z->chave < pt->chave) pt = pt->esq;
            else pt = pt->dir;
        }
    }

    if (y != NULL) {
        z->pai = y;

        if (y == externo) {
            *ptraiz = z;
        } else {
            if (z->chave < y->chave) {
                y->esq = z;
            } else {
                y->dir = z;
            }
        }

        z->esq = z->dir = externo;
        z->cor = R;

        RotaRN(z, ptraiz);
    }
}

No* Sucessor(No *z) {
    No *y = z->dir;

    while (y->esq != externo) {
        y = y->esq;
    }

    return y;
}

void RotaRemoverRN(No *x, No **ptraiz) {
    No *w;

    while (x != (*ptraiz) && x->cor != R) {
        if (x == x->pai->esq) {
            w = x->pai->dir;

            if (w->cor == R) {
                x->pai->cor = R; w->cor = N;
                RotacaoE(x->pai, ptraiz);
                w = x->pai->dir;
            }

            if (w->dir->cor == N && w->esq->cor == N) {
                w->cor = R;
                x = x->pai;
            } else {
                if (w->esq->cor == R) {
                    w->esq->cor = N; w->cor = R;
                    RotacaoD(w, ptraiz);
                    w = x->pai->dir;
                }

                w->cor = x->pai->cor;
                w->dir->cor = N;
                w->pai->cor = N;
                RotacaoE(x->pai, ptraiz);
                x = *ptraiz;
            }
        } else {
            w = x->pai->esq;

            if (w->cor == R) {
                x->pai->cor = R; w->cor = N;
                RotacaoD(x->pai, ptraiz);
                w = x->pai->esq;
            }

            if (w->esq->cor == N && w->dir->cor == N) {
                w->cor = R;
                x = x->pai;
            } else {
                if (w->dir->cor == R) {
                    w->dir->cor = N; w->cor = R;
                    RotacaoE(w, ptraiz);
                    w = x->pai->esq;
                }

                w->cor = x->pai->cor;
                w->esq->cor = N;
                w->pai->cor = N;
                RotacaoD(x->pai, ptraiz);
                x = *ptraiz;
            }
        }
    }

    x->cor = N;
}

void RemoverRN(No *z, No **ptraiz) {
    No *y = z; Cor corOriginal = y->cor;
    
    No *x;

    if (y->esq == externo) {
        x = z->dir;
        MoverPai(z, z->dir, ptraiz);
    } else {
        if (y->dir == externo) {
            x = z->esq;
            MoverPai(z, z->esq, ptraiz);
        } else {
            y = Sucessor(z); corOriginal = y->cor;
            x = y->dir;

            if (y->pai == z) {
                x->pai = y;
            } else {
                MoverPai(y, x, ptraiz);
                y->dir = z->dir;
                y->dir->pai = y;
            }

            MoverPai(z, y, ptraiz);
            y->esq = z->esq;
            y->esq->pai = y;
            y->cor = z->cor;
        }
    }

    free(z);

    if (corOriginal == N) {
        RotaRemoverRN(x, ptraiz);
    }
}

int ContarNos(No *pt) {
    if (pt == externo) return 0;

    return 1 + ContarNos(pt->esq) + ContarNos(pt->dir);
}

int AlturaNEsq(No *pt) {
    if (pt == externo) return 0;

    if (pt->cor == N) {
        return AlturaNEsq(pt->esq) + 1;
    } 

    return AlturaNEsq(pt->esq);
}

int AlturaNDir(No *pt) {
    if (pt == externo) return 0;
    
    if (pt->cor == N) {
        return AlturaNDir(pt->dir) + 1;
    }

    return AlturaNDir(pt->dir);
}

Boleano ChecarRN(No *pt) {
    if (pt == externo) return V;

    if (AlturaNEsq(pt->esq) != AlturaNDir(pt->dir)) return F;

    if (pt->cor == R && pt->pai->cor == R) return F;

    return ChecarRN(pt->esq) && ChecarRN(pt->dir);
}

int main() {
    int qtd_rns = 1000, qtd_nos = 10000, valor_maximo = 100000, qtd_remocoes = 1000;   
    
    // Iniciando o nó externo

    externo = IniciarExterno();

    // Passo 1: Criar qtd_rns ́árvores Rubro-Negras

    No *rns[qtd_rns];

    for (int i = 0; i < qtd_rns; i++) {
        rns[i] = externo;
    }
    
    // Passo 2: Inserir qtd_nos em cada árvore

    printf("\nInserindo %d nós nas %d árvores...\n\n", qtd_nos, qtd_rns);

    printf("---------------------\n");

    for (int i = 0; i < qtd_rns; i++) {
        printf("Inserindo %d nós na %d° árvore...\n", qtd_nos, i + 1);

        int qtd_nos_atual = ContarNos(rns[i]);

        while (qtd_nos_atual < qtd_nos) {
            int valor = rand() % valor_maximo;

            No *noEncontrado = BuscarNo(valor, rns[i]);

            if (noEncontrado == externo) {
                InserirRN(IniciarNo(valor), &rns[i]);

                qtd_nos_atual += 1;
            }
        }

        printf("A %d° árvore possui %d nós\n", i + 1, ContarNos(rns[i]));

        // Passo 3: Verificar se a árvore é Rubro-Negra

        if (ChecarRN(rns[i]) == V) {
            printf("A %d° árvore é Rubro-Negra\n", i + 1);
        } else {
            printf("A %d° árvore não é Rubro-Negra\n");
        } 

        printf("---------------------\n");
    }

    // Passo 4: Remover qtd_remocoes nós de cada árvore

    printf("\nRemovendo %d nós nas %d árvores...\n\n", qtd_remocoes, qtd_rns);

    printf("---------------------\n");

    for (int i = 0; i < qtd_rns; i++) {
        printf("Removendo %d nós na %d° árvore...\n", qtd_remocoes, i + 1);

        int qtd_nos_atual = ContarNos(rns[i]);

        while (qtd_nos_atual > (qtd_nos - qtd_remocoes)) {
            int valor = rand() % valor_maximo;

            No *noEncontrado = BuscarNo(valor, rns[i]);

            if (noEncontrado != externo) {
                RemoverRN(noEncontrado, &rns[i]);

                qtd_nos_atual -= 1;
            } 
        }

        printf("A %d° árvore possui %d nós\n", i + 1, ContarNos(rns[i]));

        // Passo 5: Verificar se a árvore é Rubro-Negra

        if (ChecarRN(rns[i])) {
            printf("A %d° árvore é Rubro-Negra\n", i + 1);
        } else {
            printf("A %d° árvore não é Rubro-Negra\n");
        } 

        printf("---------------------\n");
    }

    return 0;
}