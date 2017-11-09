//
//  project5a.c
//  Project5
//
//  Created by John Alexander on 11/2/17.
//  Copyright © 2017 John Alexander. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

bool checkValidNumOfArguments(int);
bool isDeadlockSimulation(char);
bool isNormalSimulation(char);
void deadLockSimulation();
void normalSimulation();

int main(int argc, int *argv[])
{
    if ( checkValidNumOfArguments(argc) )
        exit(0);

    if ( isDeadlockSimulation(argv[1][0]) )
        deadLockSimulation();
    else if ( isNormalSimulation(argv[1][0]) )
        normalSimulation();

    printf("there were two arguments\n");

    return 0;
}

bool checkValidNumOfArguments(int argc)
{
    if (argc != 2)
        return true;
    return false;
}

bool isDeadlockSimulation(char argument)
{
    if ( argument == '0' )
        return true;
    return false;
}

bool isNormalSimulation(char argument)
{
    if ( argument == '1' )
        return true;
    return false;
}

void deadLockSimulation()
{
    printf("deadlock\n");
}

void normalSimulation()
{
    printf("normal\n");
}

















