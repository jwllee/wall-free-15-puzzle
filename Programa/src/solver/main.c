#include <stdio.h>
#include <stdlib.h>
#include "state.h"


int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Modo de uso: ./solver [test.txt] [output.txt]\n");
        printf("Donde:\n");
        printf("\t[test.txt] es el archivo de input\n");
        printf("\t[output.txt] es el nombre del archivo a escribir con el output\n");
    }

    FILE *file = fopen(argv[0], "r");

    int size;
    fscanf(file, "%d", &size);

    int *start = malloc(size * size * sizeof(int));

    int ind;
    for (int i = 0; i < size * size; ++i)
    {
        ind = i * size;
        fscanf(file, " %d ", &start[ind]);

        for (int j = 1; j < size; ++j)
        {
            ind = i * size + j;
            fscanf(file, ",%d", &start[ind]);
        }
    }

    int *start_record = create_record(size, start);

    // free memories
    free(start);
    fclose(file);

    return 0;
}
