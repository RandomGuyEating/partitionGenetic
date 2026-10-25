#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define numBlocks 20
// number of blocks in the input set
#define chromLength numBlocks
// length of chromosome
#define popSize 10
// number of chromosomes in population
#define mutateChance 5
// % chance that a mutation will happen when mutating a chromosome

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

void generateBlockHeights(int blocks[numBlocks])
{

    for (int i = 0; i < numBlocks; i++)
    {
        blocks[i] = rand();
    }

    return;
}

void generateChromosome(bool generation[popSize][chromLength])
{
    for (int i = 0; i < popSize; i++)
    {
        for (int j = 0; j < chromLength; j++)
        {
            generation[i][j] = (rand() > (RAND_MAX / 2));
        }
    }
    return;
}

void mutateChromosome(bool generation[popSize][chromLength], int nChromosome)
{

    for (int nGene = 0; nGene < chromLength; nGene++)
    {
        if (rand() > (RAND_MAX * (mutateChance / 100)))
        {
            generation[nChromosome][nGene] = !generation[nChromosome][nGene];
        }
    }

    return;
}

void crossOver()
{

    return;
}

void printChromosome(bool generation[popSize][chromLength], int nChromosome)
{
    for (int j = 0; j < chromLength; j++)
        printf("%d", generation[nChromosome][j]);
    printf("\n");
}

int main(void)
{

    int blocks[numBlocks];
    bool generation[popSize][chromLength];

    return 0;
}