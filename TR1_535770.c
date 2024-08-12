#include <stdio.h>
#include <stdlib.h>

#define F 0
#define V 1

// Nome: Pedro Henrique Ferreira da Silva
// Matrícula: 535770

typedef struct no {
    int chave;
    int bal;
    struct no *esq;
    struct no *dir;
} No;

void Caso1(No **pt, int *h) {
    No *ptu = (*pt)->esq;

    if (ptu->bal == -1) {
        (*pt)->esq = ptu->dir;
        ptu->dir = *pt;
        *pt = ptu;
        (*pt)->dir->bal = 0;
    } else {
        No *ptv = ptu->dir;
        ptu->dir = ptv->esq;
        ptv->esq = ptu;
        (*pt)->esq = ptv->dir;
        ptv->dir = *pt;

        switch (ptv->bal) {
            case 1:
                (*pt)->bal = 0;
                ptu->bal = -1;
                break;
            case 0:
                (*pt)->bal = 0;
                ptu->bal = 0;
                break;
            case -1:
                (*pt)->bal = 1;
                ptu->bal = 0;
                break;
        }

        *pt = ptv;
    }

    (*pt)->bal = 0;
    *h = F;
}

void Caso2(No **pt, int *h) {
    No *ptu = (*pt)->dir;

    if (ptu->bal == 1) {
        (*pt)->dir = ptu->esq;
        ptu->esq = *pt;
        *pt = ptu;
        (*pt)->esq->bal = 0;
    } else {
        No *ptv = ptu->esq;
        ptu->esq = ptv->dir;
        ptv->dir = ptu;
        (*pt)->dir = ptv->esq;
        ptv->esq = *pt;

        switch (ptv->bal) {
            case 1:
                (*pt)->bal = -1;
                ptu->bal = 0;
                break;
            case 0:
                (*pt)->bal = 0;
                ptu->bal = 0;
                break;
            case -1:
                (*pt)->bal = 0;
                ptu->bal = 1;
                break;
        }

        *pt = ptv;
    }

    (*pt)->bal = 0;
    *h = F;
}

void IniciarNo(No **pt, int x) {
    *pt = malloc(sizeof(No));

    (*pt)->chave = x;
    (*pt)->bal = 0;
    (*pt)->esq = NULL;
    (*pt)->dir = NULL;
}

void InserirAVL(int x, No **pt, int *h) {
    if (*pt == NULL) {
        IniciarNo(pt, x);
        *h = V;
    } else {
        if (x == (*pt)->chave) {
            *h = F;
        } else if (x < (*pt)->chave) {
            InserirAVL(x, &(*pt)->esq, h);

            if (*h == V) {
                switch ((*pt)->bal) {
                    case 1:
                        (*pt)->bal = 0;
                        *h = F;
                        break;
                    
                    case 0:
                        (*pt)->bal = -1;
                        break;
                    
                    case -1:
                        Caso1(pt, h);
                        break;
                }
            }
        } else {
            InserirAVL(x, &(*pt)->dir, h);

            if (*h == V) {
                switch ((*pt)->bal) {
                    case -1:
                        (*pt)->bal = 0;
                        *h = F;
                        break;
                    case 0:
                        (*pt)->bal = 1;
                        break;
                    case 1:
                        Caso2(pt, h);
                        break;
                }
            }
        }
    }
}

void Caso1R(No **pt, int *h) {
    No *ptu = (*pt)->esq;

    if (ptu->bal <= 0) {
        (*pt)->esq = ptu->dir;
        ptu->dir = *pt;
        *pt = ptu;
        
        if (ptu->bal == -1) {
            ptu->bal = 0;
            (*pt)->dir->bal = 0;
            *h = V;
        } else {
            ptu->bal = 1;
            (*pt)->dir->bal = -1;
            *h = F;
        }
    } else {
        No *ptv = ptu->dir;
        ptu->dir = ptv->esq;
        ptv->esq = ptu;
        (*pt)->esq = ptv->dir;
        ptv->dir = *pt;
        *pt = ptv;

        switch (ptv->bal) {
            case 1:
                ptu->bal = -1;
                (*pt)->dir->bal = 0;
                break;
            case 0:
                ptu->bal = 0;
                (*pt)->dir->bal = 0;
                break;
            case -1:
                ptu->bal = 0;
                (*pt)->dir->bal = 1;
                break;
        }

        (*pt)->bal = 0;
        *h = V;
    }
}

void CasoR2(No **pt, int *h) {
    No *ptu = (*pt)->dir;

    if (ptu->bal >= 0) {
        (*pt)->dir = ptu->esq;
        ptu->esq = *pt;
        *pt = ptu;

        if (ptu->bal == 1) {
            ptu->bal = 0;
            (*pt)->esq->bal = 0;
            *h = V;
        } else {
            ptu->bal = -1;
            (*pt)->esq->bal = 1;
            *h = F;
        }

    } else {
        No *ptv = ptu->esq;
        ptu->esq = ptv->dir;
        ptv->dir = ptu;
        (*pt)->dir = ptv->esq;
        ptv->esq = *pt;
        *pt = ptv;

        switch (ptv->bal) {
            case 1:
                ptu->bal = 0;
                (*pt)->esq->bal = -1;
                break;
            case 0:
                ptu->bal = 0;
                (*pt)->esq->bal = 0;
                break;
            case -1:
                ptu->bal = 1;
                (*pt)->esq->bal = 0;
                break;
        }

        (*pt)->bal = 0;
        *h = V;
    }
}

void Balancear(No **pt, char R, int *h) {
    if (*h == V) {
        if (R == 'D') {
            switch ((*pt)->bal) {
                case 1:
                    (*pt)->bal = 0;
                    break;
                case 0:
                    (*pt)->bal = -1;
                    *h = F;
                    break;
                case -1:
                    Caso1R(pt, h);
                    break;
            }
        } else {
            switch ((*pt)->bal) {
                case -1:
                    (*pt)->bal = 0;
                    break;
                case 0:
                    (*pt)->bal = 1;
                    *h = F;
                    break;
                case 1:
                    CasoR2(pt, h);
                    break;
            }
        }
    }
}

void Trocar(No **pt, No **pt2) {
    No *pt_aux = *pt2;
    *pt2 = *pt;
    *pt = pt_aux; 

    No *esq_aux = (*pt2)->esq;
    No *dir_aux = (*pt2)->dir;
    int bal_aux = (*pt2)->bal;

    (*pt2)->esq = (*pt)->esq;
    (*pt2)->dir = (*pt)->dir;
    (*pt2)->bal = (*pt)->bal;

    (*pt)->esq = esq_aux;
    (*pt)->dir = dir_aux;
    (*pt)->bal = bal_aux;
}

void RemoverAVL(int x, No **pt, int *h) {
    if (*pt == NULL) {
        *h = F;
    } else {
        if (x < (*pt)->chave) {
            RemoverAVL(x, &(*pt)->esq, h);
            Balancear(pt, 'E', h);
        } else {
            if (x > (*pt)->chave) {
                RemoverAVL(x, &(*pt)->dir, h);
                Balancear(pt, 'D', h);
            } else {
                No *aux = *pt;

                if ((*pt)->esq == NULL) {
                    *pt = (*pt)->dir;
                    *h = V;

                    free(aux);
                } else {
                    if ((*pt)->dir == NULL) {
                        *pt = (*pt)->esq;
                        *h = V;

                        free(aux);
                    } else {
                        No *s = (*pt)->dir;

                        if (s->esq == NULL) {
                            s->esq = (*pt)->esq;
                            s->bal = (*pt)->bal;
                            *pt = s;
                            *h = V;

                            free(aux);
                        } else {
                            No *PaiS;

                            while (s->esq != NULL) {
                                PaiS = s;
                                s = s->esq;
                            }

                            Trocar(pt, &PaiS->esq);
                            RemoverAVL(x, &(*pt)->dir, h);
                        }

                        Balancear(pt, 'D', h);
                    }
                }
            }
        }
    }
}

int Altura(No *pt) {
    if (pt == NULL) {
        return 0;
    } else {
        int esq = Altura(pt->esq);
        int dir = Altura(pt->dir);

        if (esq >= dir) {
            return esq + 1;
        } else {
            return dir + 1;
        }
    }
}

int AVL(No *pt) {
    if (pt == NULL) {
        return V;
    } else {
        int bal = Altura(pt->dir) - Altura(pt->esq);

        if (bal < -1 || bal > 1 || pt->bal != bal) {
            return F;
        } else {
            int esq = AVL(pt->esq);
            int dir = AVL(pt->dir);

            if (esq == F || dir == F) {
                return F;
            } else {
                return V;
            }
        }
    }
}

int Nos(No *pt) {
    if (pt == NULL) {
        return 0;
    } else {
        return 1 + Nos(pt->esq) + Nos(pt->dir);
    }
}

int main() {
    int qtd_avls = 1000, qtd_nos = 10000, valor_maximo = 100000, qtd_remocoes = 1000;

    // Passo 1: Criar qtd_avls árvores AVL

    printf("Criando %d árvores...\n", qtd_avls);

    No *avls[qtd_avls];
    int h = F;

    for (int i = 0; i < qtd_avls; i++) {
        avls[i] = NULL;
    }

    // Passo 2: Inserir qtd_nos nós em cada árvore

    printf("\nInserindo %d nós nas %d árvores...\n\n", qtd_nos, qtd_avls);

    printf("---------------------\n");

    for (int i = 0; i < qtd_avls; i++) {
        printf("Inserindo %d nós na %d° árvore...\n", qtd_nos, i + 1);

        for (int j = 0; j < qtd_nos; j++) {
            InserirAVL(rand() % valor_maximo, &avls[i], &h);
        }

        // Adiciona os nós restantes para que a árvore tenha qtd_nos nós
        while (Nos(avls[i]) < qtd_nos) {
            InserirAVL(rand() % valor_maximo, &avls[i], &h);
        }

        printf("A %d° árvore possui %d nós\n", i + 1, Nos(avls[i]));

        // Passo 3: Verificar se a árvore é AVL

        if (AVL(avls[i])) {
            printf("A %d° árvore é AVL\n", i + 1);
        } else {
            printf("A %d° árvore não é AVL\n");
        } 

        printf("---------------------\n");
    }

    // Passo 4: Remover qtd_remocoes nós de cada árvore

    printf("\nRemovendo %d nós nas %d árvores...\n\n", qtd_remocoes, qtd_avls);

    printf("---------------------\n");

    for (int i = 0; i < qtd_avls; i++) {
        printf("Removendo %d nós na %d° árvore...\n", qtd_remocoes, i + 1);

        for (int j = 0; j < qtd_remocoes; j++) {
            RemoverAVL(rand() % valor_maximo, &avls[i], &h);
        }

        // Remove os nós restantes para que a árvore tenha (qtd_nos - qtd_remocoes) nós
        while (Nos(avls[i]) > (qtd_nos - qtd_remocoes)) {
            RemoverAVL(rand() % valor_maximo, &avls[i], &h);
        }

        printf("A %d° árvore possui %d nós\n", i + 1, Nos(avls[i]));

        // Passo 5: Verificar se a árvore é AVL

        if (AVL(avls[i])) {
            printf("A %d° árvore é AVL\n", i + 1);
        } else {
            printf("A %d° árvore não é AVL\n");
        } 

        printf("---------------------\n");
    }

    return 0;
}
