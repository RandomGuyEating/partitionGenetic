#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

/*
    Elitist evolution strategy. The single best and worst are found, the worst is replaced with the best and the original
    best is kept unchanged.

    As this technically is a O(n^2) time complexity problem and we don't know if towerHeights can ever reach a minimum difference
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
int mutateChance = 80;

// % chance that two chromosomes will be crossed over
int crossOverChance = 65;

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
    }
    fclose(fp);

    return;
}

void generateBlockHeights()
/*
    Mostly for testing as input will be a .txt file?
*/
{

    for (int i = 0; i < numBlocks; i++)
    {
        blocks[i] = 1 + rand() % 1000;
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

int heightOfTower(bool *chromosome, bool b)
/*
    Returns height of either right or left tower as specified in the arguments.
*/
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
        generation[chr2][i] = temp;                // Put copied chr1 part gene into chr2.
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

int measureFitness(int nChromosome)
/*
    Measures how equal are the tower heights for the input chromosome
*/
{
    int diff = abs(heightOfTower(generation[nChromosome], true) - heightOfTower(generation[nChromosome], false));

    return diff;
}

void eliminateWeak(int worst, int best)
{
    for (int length = 0; length < chromLength; length++) // Replaces worst chromosome with best chromosome.
    {
        generation[worst][length] = generation[best][length];
    }
}

int simulateGenerations(int nTimes, int *bestChromosome)
{
    int stagnantGenerations = 0; // If reaches stagnant, ends simulation as this is assumed to be the best possible result.
    int bestDiff = 2147483646;
    *bestChromosome = 0; // Doesn't redeclare as this program is using the elitist approach.
    int n;

    for (n = 0; n < nTimes; n++)
    {
        int worstChromosome = 0; // Redeclares every loop to keep track of the worst chromosome in this generation, not all.
        bool improved = false;   // Keeps track if a new best chromosome was found.
        int worstDiff = -1;

        for (int k = 0; k < popSize; k++) // Finds the strongest and the weakest chromosomes.
        {
            int current = measureFitness(k);

            if (current < bestDiff)
            {
                bestDiff = current;
                *bestChromosome = k;
                improved = true;
            }

            if (current >= worstDiff)
            {
                worstDiff = current;
                worstChromosome = k;
            }
        }

        if (bestDiff <= 1)
        {
            return n;
        }

        if (improved)
        {
            stagnantGenerations = 0;
        }

        else
        {
            stagnantGenerations++;
            if (stagnantGenerations >= stagnant)
            {
                return n;
            }
        }

        eliminateWeak(worstChromosome, *bestChromosome);

        for (int nChromsome = 0; nChromsome < popSize; nChromsome++)
        /*
            Goes through each chromosome with %chance to mutate or crossover.
            % are defined at the start of the program.
        */
        {

            if (nChromsome == *bestChromosome)
            {
                continue; // Preserves the best chromosome.
            }
            if (rand() < (RAND_MAX / 100) * mutateChance)
            {
                mutateChromosome(nChromsome);
            }

            if (rand() < (RAND_MAX / 100) * crossOverChance)
            {
                int chromosome2;

                do // Ensures that the 2nd crossover chromosome is different from the chromosome 1 and the best one in this generation.
                {
                    chromosome2 = rand() % popSize;
                } while (chromosome2 == nChromsome || chromosome2 == *bestChromosome);

                crossOver(nChromsome, chromosome2);
            }
        }
    }
    return n;
}

void printTowers(int chromosome)
{
    int leftCount = 0;
    int rightCount = 0;
    int leftBlocks[numBlocks];
    int rightBlocks[numBlocks];

    for (int i = 0; i < numBlocks; i++)
    {
        if (generation[chromosome][i] == true)
        {
            rightBlocks[rightCount] = blocks[i];
            rightCount++;
        }
        else
        {
            leftBlocks[leftCount] = blocks[i];
            leftCount++;
        }
    }

    if (leftCount > rightCount)
    {
        int diff = leftCount - rightCount;

        // Start printing left side first..
        for (int i = 0; i < diff; i++)
        {
            printf("+------+\n");
            printf("| %4d |\n", leftBlocks[i]);
            printf("+------+\n");
        }

        for (int k = 0; k < rightCount; k++)
        {
            printf("+------+    +------+\n");
            printf("| %4d |    | %4d |\n", leftBlocks[diff + k], rightBlocks[k]);
            printf("+------+    +------+\n");
        }
    }
    else
    {
        int diff = rightCount - leftCount;

        // Start printing right tower first.
        for (int i = 0; i < diff; i++)
        {
            printf("            +------+\n");
            printf("            | %4d |\n", rightBlocks[i]);
            printf("            +------+\n");
        }

        for (int k = 0; k < leftCount; k++)
        {
            printf("+------+    +------+\n");
            printf("| %4d |    | %4d |\n", leftBlocks[k], rightBlocks[diff + k]);
            printf("+------+    +------+\n");
        }
    }

    printf("\nLeft Tower:  %d\n", heightOfTower(generation[chromosome], false));
    printf("Right Tower: %d\n", heightOfTower(generation[chromosome], true));
    printf("Difference:  %d\n", measureFitness(chromosome));
}

void testSimulations(int nSettings, int nSeeds, int nTimes)
/*
    Purely used for testing, to optimize and get the best results. My test arguments will be left in the main function.
*/
{
    int step = 100 / nSettings; // n amount of settings, spaced evenly.
    double bestAvgDiff = 2147483646.0;
    int bestMutate = 0;
    int bestCross = 0;

    printf("Mut%% | Cross%% | Avg Diff | Avg Generations\n");

    for (int m = 0; m < nSettings; m++)
    {
        for (int c = 0; c < nSettings; c++)
        {
            mutateChance = m * step;
            crossOverChance = c * step;

            double totalDiff = 0;
            double totalGens = 0;

            for (int seed = 0; seed < nSeeds; seed++)
            {
                int bestChromosome;

                srand(seed);
                generateBlockHeights();
                generateChromosomes();

                totalGens += simulateGenerations(nTimes, &bestChromosome);
                totalDiff += measureFitness(bestChromosome);
            }

            double avgDiff = totalDiff / nSeeds;
            double avgGens = totalGens / nSeeds;

            printf("%3d%% |  %3d%%  |  %6.2f  |     %7.1f\n", mutateChance, crossOverChance, avgDiff, avgGens);

            if (avgDiff < bestAvgDiff)
            {
                bestAvgDiff = avgDiff;
                bestMutate = mutateChance;
                bestCross = crossOverChance;
            }
        }
    }

    printf("\nBest setting: mutateChance = %d%%, crossOverChance = %d%% (Avg Diff: %.2f)\n", bestMutate, bestCross, bestAvgDiff);
}

int main(void)
{
    // testSimulations(10, 100, 10000);

    srand(time(NULL));

    int finalChromosome;

    readFile(numBlocks);
    generateChromosomes();
    int result = simulateGenerations(10000, &finalChromosome);
    printf("Number of generations it took: %d\n", result);
    printTowers(finalChromosome);

    return 0;
}