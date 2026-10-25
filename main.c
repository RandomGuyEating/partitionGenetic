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
int blocks[numBlocks];
bool generation[popSize][chromLength];

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

void generateChromosomes(bool generation[popSize][chromLength])
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

unsigned long long int heightOfTower(bool *chromosome, bool b)
{
    unsigned long long int height = 0;

    for (int i = 0; i < chromLength; i++)
    {
        if (b == true && chromosome[i] == 1)
        {
            height += blocks[i];
        }

        else if (b == false && chromosome[i] == 0)
        {
            height += blocks[i];
        }
    }

    return height;
}

void crossOver(bool generation[popSize][chromLength], int chr1, int chr2)
{
    int cutLocation = rand() % chromLength;
    for (int i = cutLocation; i < chromLength; i++)
    {
        bool temp = generation[chr1][i];           // Copy chr1 gene.
        generation[chr1][i] = generation[chr2][i]; // Put chr2 part gene into chr1.
        generation[chr2][i] = temp;                // Put saved chr1 part gene into chr2.
    }

    return;
}

void printChromosome(bool generation[popSize][chromLength], int nChromosome)
{
    for (int i = 0; i < chromLength; i++)
    {
        printf("%d", generation[nChromosome][i]);
    }
    printf("\n");

    return;
}

int main(void)
{

    generateBlockHeights(blocks);
    generateChromosomes(generation);

    unsigned long long int sum = 0;

    for (int i = 0; i < numBlocks; i++)
    {
        sum += blocks[i];
    }

    printf("%llu\n", sum);
    printf("%llu\n", heightOfTower(generation[0], false));
    printf("%llu\n", heightOfTower(generation[0], true));

    return 0;
}