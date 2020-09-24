#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 100

struct Stack {
    int stack[MAX_STACK_SIZE];
    int tam;
    void (*insert)(int);
    int (*pop)();
} this;

void insert(int elem)
{
    if (this.tam < MAX_STACK_SIZE)
        this.stack[this.tam++] = elem;
}

int pop()
{
    return this.tam == 0 ? -1 : this.stack[--this.tam];
}

int main()
{
    int vec_a[MAX_STACK_SIZE], vec_b[MAX_STACK_SIZE], vec_c[MAX_STACK_SIZE];
    this.tam = 0;
    this.insert = insert;
    this.pop = pop;

    for (int i = 0; i < MAX_STACK_SIZE; i++) this.insert(i);

    // 1º Não é contável pois this.tam é alterado
    // 2º Acesso à memória não contíguo
    for (int i = 0; i < this.tam; i += 2)
    {
        // 1º Altera o valor de this.tam
        // 2º Há uma chamada de função
        printf("poping from stack: %d\n", this.pop());

        if (i > 0) vec_b[i] = vec_b[i - 1] + 3; // Dependência de leitura após escrita

        // Diferentes iterações fazendo operações diferentes
        if (this.tam % 2 == 0) vec_a[i] = vec_b[i] + vec_c[vec_b[i]];
        else vec_a[i] = vec_b[i] * vec_c[vec_b[i]]; // Indexando vec_c por vec_b

        // Não tem apenas um ponto de saída
        if (this.tam % 2 == 0 && this.tam < (int) 0.7 * MAX_STACK_SIZE)
            break;
    }

    return 0;
}

/*
devec.c:15:8: note: not vectorized: not enough data-refs in basic block.
devec.c:16:32: note: not vectorized: no grouped stores in basic block.
devec.c:39:47: note: not vectorized: control flow in loop.
devec.c:31:5: note: not vectorized: latch block not empty.
devec.c:29:14: note: not vectorized: vectorization is not profitable.
*/
