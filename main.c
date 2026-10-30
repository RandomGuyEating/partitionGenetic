#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/*
    Elitist evolution strategy. The single best and worst are found, the worst is replaced with the best and the original
    best is kept unchanged.

    As this technically is a O(n * n^2) time complexity problem and we don't know if towerHeights can ever reach a minimum difference
    of 0 or any other number for that matter. To evaluate when to stop I tried measuring if there was any improvement in between the best
    chromosomes of each generation as I save them from each generation.
*/

// number of blocks in the input set
#define numBlocks 20
// length of chromosome
#define chromLength numBlocks
// number of chromosomes in population
#define popSize 10
// % chance that a chromosome will be mutated
#define mutateChance 2
// % chance that two chromosomes will be crossed over
#define crossOverChance 0.01
// how many times will the code simulate evolution when the best chromosome doesn't change anymore. Can be treated as a sort of 'accuracy'
#define stagnant 100
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

    int gene = rand() % chromLength;
    generation[nChromosome][gene] = !generation[nChromosome][gene];

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
    int cutLocation = 1 + (rand()) % (chromLength - 1);
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
    /* Maybe make the function calculate not absolute difference, but use smth similar to my gradient descent, where big diffs
    get penalized more and low diffs are rewarded more. Maybe helps converge to good answer faster? Since there is only one answer,
    escaping local minima isn't an issue, thus don't need to make random jumps, just need to make the guesses tighter the longer it goes*/
    int diff = abs(heightOfTower(generation[nChromosome], true) - heightOfTower(generation[nChromosome], false));

    return diff;
}

void eliminateWeak(int worst, int best)
{
    for (int length = 0; length < chromLength; length++) // Replaces worst with best
    {
        generation[worst][length] = generation[best][length];
    }
}

int simulateGenerations(int nTimes)
{

    for (int n = 0; n < nTimes; n++)
    {
        int bestChromosome; // Redeclares every loop to keep track of best chromosome in this generation, not all;
        int worstChromosome;

        int bestDiff = 21459789;
        int worstDiff = 0;

        int stagnantGenerations = 0;

        for (int k = 0; k < popSize; k++) // Finds the strongest and the weakest chromosomes
        {
            int current = measureFitness(k);

            if (current <= bestDiff)
            {
                bestDiff = current;
                bestChromosome = k;
                stagnantGenerations = 0;
            }

            else // If there is no improvement over the best chromosome for some times, ends simulation
            {
                stagnantGenerations++;
                if (stagnantGenerations >= stagnant)
                {
                    return n;
                    break;
                }
            }

            if (current >= worstDiff)
            {
                worstDiff = current;
                worstChromosome = k;
            }
        }

        eliminateWeak(worstChromosome, bestChromosome);

        for (int nChromsome = 0; nChromsome < popSize; nChromsome++)
        // Goes through each chromosome with %chance to mutate or crossover.
        // % are defined at the start of the program.
        {

            if (nChromsome == bestChromosome)
            {
                continue; // Saves the best chromosome.
            }
            if (rand() < (RAND_MAX / 100) * mutateChance)
            {
                mutateChromosome(nChromsome);
            }

            if (rand() < (RAND_MAX / 100) * crossOverChance)
            {
                int chromosome1 = rand() % popSize;
                int chromosome2;

                do
                {
                    chromosome2 = rand() % popSize;
                } while (chromosome2 == chromosome1 || chromosome2 == bestChromosome);

                crossOver(chromosome1, chromosome2);
            }
        }
    }
    return 0;
}

int main(void)
{

    generateBlockHeights();
    generateChromosomes();

    return 0;
}