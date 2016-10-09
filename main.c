//
// Created by joek on 10/4/16.
//

#include <stdbool.h>
#include "BBB_P10.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

bool Scanning = true;
const char *MSG = "BBB_P10 LED DISPLAY IS A LIBRARY FOR BEAGLEBONE BLACK.";
typedef struct {
    pthread_mutex_t mutex;
    pthread_t thread;
}threadstruct;

void *scan_thread(void *data){
    threadstruct *threadstruct1 = (threadstruct*)data;
    while(Scanning){
        pthread_mutex_lock(&threadstruct1->mutex);
        Scan_Panels();
        pthread_mutex_unlock(&threadstruct1->mutex);
    }
    pthread_exit(NULL);
}
void *display_thread(void *data){
    threadstruct *threadstruct1 = (threadstruct*)data;
    while(Scanning){

        pthread_mutex_lock(&threadstruct1->mutex);
        movestring(MSG,Left,AlignCenter,50000);
        pthread_mutex_unlock(&threadstruct1->mutex);
        sleep(1);

        pthread_mutex_lock(&threadstruct1->mutex);
        movestring("SCROLLING RIGHT",Right,AlignCenter,50000);
        pthread_mutex_unlock(&threadstruct1->mutex);
        sleep(1);

        pthread_mutex_lock(&threadstruct1->mutex);
        movestring("SCROLLING LEFT",Left,AlignTop,10000);
        pthread_mutex_unlock(&threadstruct1->mutex);
        sleep(1);

        pthread_mutex_lock(&threadstruct1->mutex);
        Clear_Buffer(false);
        drawBox(0,0,nPANEL*WIDTH-1,15);
        drawmessage("LEFT TOP",AlignHLeft,AlignTop);
        pthread_mutex_unlock(&threadstruct1->mutex);
        sleep(1);

        pthread_mutex_lock(&threadstruct1->mutex);
        Clear_Buffer(false);
        drawBox(0,0,nPANEL*WIDTH-1,15);
        drawmessage("CENTER TOP",AlignHCenter,AlignTop);
        pthread_mutex_unlock(&threadstruct1->mutex);
        sleep(1);

        pthread_mutex_lock(&threadstruct1->mutex);
        Clear_Buffer(false);
        drawBox(0,0,nPANEL*WIDTH-1,15);
        drawmessage("RIGHT TOP",AlignHRight,AlignTop);
        pthread_mutex_unlock(&threadstruct1->mutex);
        sleep(1);

        pthread_mutex_lock(&threadstruct1->mutex);
        Clear_Buffer(false);
        drawBox(0,0,nPANEL*WIDTH-1,15);
        drawmessage("LEFT TOP",AlignHLeft,AlignTop);
        pthread_mutex_unlock(&threadstruct1->mutex);
        sleep(1);

        pthread_mutex_lock(&threadstruct1->mutex);
        Clear_Buffer(false);
        drawBox(0,0,nPANEL*WIDTH-1,15);
        drawmessage("CENTER CENTER",AlignHCenter,AlignCenter);
        pthread_mutex_unlock(&threadstruct1->mutex);
        sleep(1);

        pthread_mutex_lock(&threadstruct1->mutex);
        Clear_Buffer(false);
        drawBox(0,0,nPANEL*WIDTH-1,15);
        drawmessage("RIGHT BOTTOM",AlignHRight,AlignBottom);
        pthread_mutex_unlock(&threadstruct1->mutex);
        sleep(1);

        pthread_mutex_lock(&threadstruct1->mutex);
        Clear_Buffer(false);
        flashmessage("FLASHING STRING",AlignHCenter,AlignCenter,350000,5);
        pthread_mutex_unlock(&threadstruct1->mutex);
        sleep(1);

    }
//
    pthread_exit(NULL);
}
int main(int argc, char *argv[]) {
    int res;
    Setup_Pins();
    Clear_Buffer(false);
    drawmessage(MSG,AlignHCenter,AlignCenter);

    threadstruct *threadstruct1 = malloc(sizeof(threadstruct));
    pthread_mutex_init(&(threadstruct1->mutex), NULL);
    res = pthread_create(&threadstruct1->thread, NULL, scan_thread, threadstruct1);
    if (res < 0) {
        printf("failed to create thread %d\n", res);
        return res;
    }

    threadstruct *threadstruct2 = malloc(sizeof(threadstruct));
    pthread_mutex_init(&(threadstruct2->mutex), NULL);
    res = pthread_create(&threadstruct2->thread, NULL, display_thread, threadstruct2);
    if (res < 0) {
        printf("failed to create thread %d\n", res);
        return res;
    }

    pthread_join(threadstruct1->thread, NULL);
    pthread_join(threadstruct2->thread, NULL);

    pthread_mutex_destroy(&threadstruct1->mutex);
    pthread_mutex_destroy(&threadstruct2->mutex);

    Free_BBB_P10();

    return 0 ;
}