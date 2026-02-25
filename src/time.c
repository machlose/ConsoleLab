#include <stdio.h>
#include <time.h>

typedef double cltime;

typedef struct {
    cltime start;
    cltime end;
    cltime time;
    bool running;
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

typedef struct {
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
    cltime (*setTargetFPS)(Time* time, cltime fps);
    void (*resetTimer)(Timer* timer);
} Time;

static cltime sys_time() {
    return (cltime)clock() / CLOCKS_PER_SEC;
}

static cltime TimeFromStart(Time* time) {
    return time->now() - time->start;
}

static void setTargetFPS(Time* time, cltime fps) {
    if (fps > 0.0)
        time->targetFrameTime = 1.0 / fps;
    else
        time->targetFrameTime = 0.0;
}

void TimeInit(Time* time) {
    time->now = sys_time;
    time->setTargetFPS = setTargetFPS;
    time->resetTimer = TimerReset;

    time->start = sys_time();

    time->delta = 0.0;
    time->fps = 0;
    time->fpsTimer = 0.0;
    time->frameCount = 0;
    time->targetFrameTime = 0.0; // no limit
    TimerReset(&time->timer);
}

void FrameLimiter(Time* time) {
    if (time->targetFrameTime <= 0.0) return;

    cltime frameEnd = sys_time();
    double frameTime = frameEnd - time->last;

    while (frameTime < time->targetFrameTime) {
        frameEnd = sys_time();
        frameTime = frameEnd - time->last;
    }
}

void TimeUpdate(Time* time) {
    time->delta = time->now() - time->last;
    time->time = time->now() - time->start;
    time->fpsTimer += time->delta;
    time->frameCount++;
    
    if (time->fpsTimer >= 1.0) {
        time->fps = time->frameCount / time->fpsTimer;
        time->fpsTimer = 0.0;
        time->frameCount = 0;
    }
}
