//******************************************************************
//
// CSCI 480 Assignment 4
//
// Author: David Tullis 
//
//******************************************************************

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <locale> 
#include <semaphore.h>
#include <pthread.h>
#include <bits/stdc++.h>
#include <sys/wait.h>
#include <unistd.h>



using namespace std;

string shared = "They're coming to get you, Barbara.";

sem_t crsem, rwsem;
int rc, wc = 0;
int counter = 0;
int r, w, i;

/**
* The writer locks down the critical section and removes the last
* character from the shared string.
********************************************************************/
void *writer(void *rno)
{
    for(int incr = 0; incr < i; incr++){
        //requests entry
        sem_wait(&rwsem);
        shared.pop_back();
        printf("writer %ld is writing ...\n", (long) rno);
        fflush(stdout);
        //exits
        sem_post(&rwsem);
        sleep(1);
    }
    pthread_exit(nullptr);  
}

/**
* Using semaphores, reader takes in the thread number and activates
* the reader portion of the RWP. Namely, it protects the critical
* section and reads from the shared string.
********************************************************************/
void *reader(void *rno)
{
    for(int incr = 0; incr < i; incr++){
        
        int temp = 0;
        
        //requests entry
        sem_wait(&crsem);
        //increments the count that holds the number of readers inside the Crit. Section
        counter++;
        printf("read_count increments to: %ld.\n", (long) counter);
        fflush(stdout);
        //if any readers are inside, writers are restricted from access
        if(counter == 1)
            sem_wait(&rwsem);
        //exits the section
        sem_post(&crsem);
        printf("reader %ld is reading ... content : ", (long) rno);
		printf("%s\n", shared.c_str());
        fflush(stdout);
        //requests entry
        sem_wait(&crsem);
        //decrements the number of inside readers
        counter--;
        printf("read_count decrements to: %ld.\n", (long) counter);
        fflush(stdout);
        //if there are no readers, prepares the handoff for the writers
        if(counter == 0){
            sem_getvalue(&rwsem, &temp);
            printf("Last reader. Semaphore value before signaling writer: %d.\n", temp);
            fflush(stdout);
            sem_post(&rwsem);
            sem_getvalue(&rwsem, &temp);
            printf("Last reader. Semaphore value after signaling writer: %d.\n", temp);
            fflush(stdout);
        }
        //exits
        sem_post(&crsem);
        sleep(1);
    }
    pthread_exit(nullptr);
}

/**
* Takes the input commands, creates the threads, joins the threads
* and then destroys/cleans them before finishing
********************************************************************/
int main(int argc, char *argv[])
{   
    string temp;

    //checks if the proper number of arguments are there
    if(argc == 4){
        temp = argv[1];
        stringstream placeholder(temp);
        placeholder >> r;
        temp = argv[2];
        stringstream placeholder2(temp);
        placeholder2 >> w;
        temp = argv[3];
        stringstream placeholder3(temp);
        placeholder3 >> i;
    } else {
        r = 10;
        w = 3;
        i = 43;
    }
 
    sem_init(&rwsem,0,1);
    sem_init(&crsem,0,1);

    pthread_t rthreads[99];
    pthread_t wthreads[99];

    cout << "Number of reader threads: " << r << endl;
    cout << "Number of writer threads: " << w << endl;
    cout << "Number of iterations per thread: " << i << endl;
		
    //creates the reader threads    
    for(int k = 0; k < r; k++){
        rc = pthread_create(&rthreads[k], NULL,reader, (void *)(long)k);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    //creates the writer threads
    for(int j = 0; j < w; j++){
        rc = pthread_create(&wthreads[j], NULL,writer, (void *)(long)j);
        if (wc){
            printf("ERROR; return code from pthread_create() is %d\n", wc);
            exit(-1);
        }
    }

    
    for(int k = 0; k < r; k++) {
        pthread_join(rthreads[k], NULL);   
    }

    for(int j = 0; j < w; j++) {
        pthread_join(wthreads[j], NULL);
    }
    
    //cleanup
    sem_destroy(&rwsem);
    sem_destroy(&crsem);
    
    cout << "All threads are done. " << endl;
    cout << "Resources cleaned up. " << endl;

	return 0;
}
