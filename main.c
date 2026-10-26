#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

// number of blocks in the input set
#define numBlocks 20
// length of chromosome
#define chromLength numBlocks
// number of chromosomes in population
#define popSize 10
// % chance that a mutation will happen when mutating a chromosome
#define mutateChance 5
// contains the heights of blocks
int blocks[numBlocks];
// contains popSize amount of chromosomes describing the sorting of blocks into two towers
bool generation[popSize][chromLength];

void readFile(int fileLength)
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

void generateBlockHeights()
{

    for (int i = 0; i < numBlocks; i++)
    {
        blocks[i] = rand() % 1000;
    }

    return;
}

void generateChromosomes()
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

void mutateChromosome(int nChromosome)
{

    for (int nGene = 0; nGene < chromLength; nGene++)
    {
        int gene = rand() % chromLength;
        generation[nChromosome][gene] = !generation[nChromosome][gene];
    }

    return;
}

int heightOfTower(bool *chromosome, bool b) // Assignment 4.
{
    int height = 0;

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

void crossOver(int chr1, int chr2)
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

void printChromosome(int nChromosome)
{
    for (int i = 0; i < chromLength; i++)
    {
        printf("%d", generation[nChromosome][i]);
    }
    printf("\n");

    return;
}

int measureFitness(int nChromosome) // Measures how equal are the tower heights for the input chromosome
{
}

int main(void)
{

    generateBlockHeights();
    generateChromosomes();

    return 0;
}