#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

void mutation()
{

    return;
}

void crossOver()
{

    return;
}

void fillBlocks(int blocks[], int length)
{

    return;
}

void readFile(int blocks[], int fileLength)
{
    FILE *fp = fopen("input.txt", "r");

    if (fp == NULL)
    {
        printf("Unable to open file.");
        exit(1);
    }

    for (int i = 0; i < fileLength; i++)
    {
        fscanf(fp, "%d", &blocks[i]);

        if (fscanf(fp, "%d", &blocks[i]) != 1) // For debugging, remove later.
        {
            printf("Error reading block %d\n", i);
            fclose(fp);
            return;
        } // Last line to remove.
    }
    fclose(fp);

    return;
}

void generateBlockHeights(int blocks[], int n)
{

    for (int i = 0; i < n; i++)
    {
        blocks[i] = rand();
    }

    return;
}

int main(void)
{

#define numBlocks 20
    // number of blocks in the input set
#define chromLength numBlocks
    // length of chromosome
#define popSize 10
    // number of chromosomes in population

    int blocks[numBlocks];
    bool generation[popSize][chromLength];

    generateBlockHeights(blocks, numBlocks);

    for (int length = 0; length < numBlocks; length++)
    {
        printf("%d\n", blocks[length]);
    }

    return 0;
}