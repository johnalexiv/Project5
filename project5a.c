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

struct Philosopher
{
    int id;
    pthread_t thread;
    sem_t *leftFork;
    sem_t *rightFork;
    sem_t *room;
};

bool isValidNumOfArguments(int);
bool isDeadlockSimulation(char);
bool isSolutionSimulation(char);
void philosopherSimulation();
struct Philosopher *createAndInitializePhilosophers(sem_t *, sem_t *, int);
void deadLockSimulation();
sem_t *createAndInitializeForks(int);
sem_t *createAndInitializeRoom(int);
void initializePhilosophers(struct Philosopher *, int);
void startPhilosopher(void *);
void diningPhilosopher(struct Philosopher *);
void pickUpForks(struct Philosopher *);
void putDownForks(struct Philosopher *);
void waitForPhilosophers(struct Philosopher *, int);

bool enable_room;

int main(int argc, char *argv[])
{
    if ( !isValidNumOfArguments(argc) )
        exit(0);

    char argument = argv[1][0];
    if ( isDeadlockSimulation(argument) )
        enable_room = false;
    else if ( isSolutionSimulation(argument) )
        enable_room = true;
    else
        exit(0);

    philosopherSimulation();

    return 0;
}

bool isValidNumOfArguments(int argc)
{
    if (argc != 2)
        return false;
    return true;
}

bool isDeadlockSimulation(char argument)
{   
    if ( argument == '1' )
        return true;
    return false;
}

bool isSolutionSimulation(char argument)
{
    if ( argument == '0' )
        return true;
    return false;
}

void philosopherSimulation()
{
    int numOfPhilosophers = 5;
    sem_t *forks = createAndInitializeForks(numOfPhilosophers);
    sem_t *room = createAndInitializeRoom(numOfPhilosophers);
    struct Philosopher *philosophers = createAndInitializePhilosophers(forks, room, numOfPhilosophers);

    initializePhilosophers(philosophers, numOfPhilosophers);

    waitForPhilosophers(philosophers, numOfPhilosophers);
}

sem_t *createAndInitializeForks(int numOfForks)
{
    int i;
    sem_t *forks = (sem_t *)calloc(numOfForks, sizeof(sem_t));

    for ( i = 0; i < numOfForks; i++ )
        sem_init(&forks[i], 0, 1);

    return forks;
}

sem_t *createAndInitializeRoom(int numOfPhilosophers)
{   
    sem_t *room = (sem_t *)malloc(sizeof(sem_t));
    sem_init(&room[0], 0, numOfPhilosophers - 1);
    return room;
}

struct Philosopher *createAndInitializePhilosophers(sem_t *forks, sem_t *room, int numOfPhilosophers)
{
    int i;
    struct Philosopher *philosophers = (struct Philosopher *)calloc(numOfPhilosophers, sizeof(struct Philosopher));

    for ( i = 0; i < numOfPhilosophers; i++ )
    {
        philosophers[i].id = i;
        pthread_t thread;
        philosophers[i].thread = thread;
        philosophers[i].room = room;

        if ( i == 0 )
            philosophers[i].leftFork = &forks[numOfPhilosophers - 1];
        else
            philosophers[i].leftFork = &forks[i - 1];

        if ( i == numOfPhilosophers - 1 )
            philosophers[i].rightFork = &forks[0];
        else
            philosophers[i].rightFork = &forks[i + 1];
    }

    return philosophers;
}

void initializePhilosophers(struct Philosopher *philosophers, int numOfPhilosophers)
{
    if ( philosophers == NULL )
        return;

    int i;
    for ( i = 0; i < numOfPhilosophers; i++ )
        pthread_create(&philosophers[i].thread, NULL, (void *)startPhilosopher, &philosophers[i]);
}

void startPhilosopher(void *argument)
{
    struct Philosopher *philosopher = (struct Philosopher *)argument;

    int i = 0;
    while(true)
        diningPhilosopher(philosopher);    

    pthread_exit(NULL);
}

void diningPhilosopher(struct Philosopher *philosopher)
{
    printf("Philosopher %d is done thinking and now ready to eat.\n", philosopher->id);

    pickUpForks(philosopher);

    printf("Philosopher %d: EATING\n", philosopher->id);

    putDownForks(philosopher);
}

void pickUpForks(struct Philosopher *philosopher)
{
    if ( enable_room )
        sem_wait(philosopher->room);

    sem_wait(philosopher->leftFork);
    printf("Philosopher %d: taking left fork %d\n", philosopher->id, philosopher->id);

    sem_wait(philosopher->rightFork);
    printf("Philosopher %d: taking right fork %d\n", philosopher->id, philosopher->id + 1);
}

void putDownForks(struct Philosopher *philosopher)
{
    sem_post(philosopher->leftFork);
    printf("Philosopher %d: putting down left fork %d\n", philosopher->id, philosopher->id);

    sem_post(philosopher->rightFork);
    printf("Philosopher %d: putting down right fork %d\n", philosopher->id, philosopher->id + 1);

    if ( enable_room )
        sem_post(philosopher->room);
}

void waitForPhilosophers(struct Philosopher *philosophers, int numOfPhilosophers)
{
    int i;
    for ( i = 0; i < numOfPhilosophers; i++ )
        pthread_join(philosophers[i].thread, NULL);
}
















