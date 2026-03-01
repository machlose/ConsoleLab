#pragma once
#include <stdio.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif
// #ifndef TIME_MACROS_H
// #define TIME_MACROS_H

// #define TimerUpdateG() TimeUpdate(&ConsoleLab.time)
// #define FrameLimiterG() FrameLimiter(&ConsoleLab.time)
// #define SetFPSG(fps) setTargetFPS(&ConsoleLab.time, fps)

// #endif


typedef double cltime;

static cltime sys_time() {
    return (cltime)clock() / CLOCKS_PER_SEC;
}

typedef struct {
    cltime start;
    cltime end;
    cltime time;
    int running;
} Timer;

void TimerStart(Timer* timer) {
    timer->start = sys_time();
    timer->running = 1;
}

void TimerStop(Timer* timer) {
    if (timer->running) {
        timer->end = sys_time();
        timer->running = 0;
        timer->time = timer->end - timer->start;
    }
}

void TimerReset(Timer* timer) {
    timer->start = 0;
    timer->end = 0;
    timer->running = 0;
    timer->time = 0;
}

typedef struct clTime clTime;

typedef struct clTime {
    cltime start;
    cltime time; //from start
    cltime last; //last frame
    cltime delta;
    cltime fpsTimer;
    cltime targetFrameTime;

    int fps;
    int frameCount;

    Timer timer;
    cltime (*now)();
    void (*setTargetFPS)(clTime* time, cltime fps);
    void (*resetTimer)(Timer* timer);
    void (*update)(clTime* timer);
} clTime;

static void setTargetFPS(clTime* time, cltime fps) {
    if (fps > 0.0)
        time->targetFrameTime = 1.0 / fps;
    else
        time->targetFrameTime = 0.0;
}

void FrameLimiter(clTime* time){
    if (time->targetFrameTime <= 0.0) return;

    cltime remaining = time->targetFrameTime - sys_time()- time->last;

    if (remaining > 0.0){
    #ifdef _WIN32
        DWORD ms = (DWORD)(remaining * 1000.0);

        if (ms > 0){
            Sleep(ms - 1);
        } 
    #else
        struct timespec ts;
        ts.tv_sec = (time_t)remaining;
        ts.tv_nsec = (long)((remaining - ts.tv_sec) * 1e9);
        nanosleep(&ts, NULL);
    #endif
        while ((sys_time() - time->last) < time->targetFrameTime){}
    }
}

void TimeUpdate(clTime* time) {
    time->delta = time->now() - time->last;
    time->time = time->now() - time->start;
    time->fpsTimer += time->delta;
    time->frameCount++;
    
    if (time->fpsTimer >= 1.0) {
        time->fps = time->frameCount / time->fpsTimer;
        time->fpsTimer = 0.0;
        time->frameCount = 0;
    }

    time->last = time->now();
}

void TimeInit(clTime* time) {
    time->now = sys_time;
    time->setTargetFPS = setTargetFPS;
    time->resetTimer = TimerReset;
    time->update = TimeUpdate;

    time->start = sys_time();

    time->delta = 0.0;
    time->fps = 0;
    time->fpsTimer = 0.0;
    time->frameCount = 0;
    time->targetFrameTime = 0.0; // no limit
    TimerReset(&time->timer);
}

