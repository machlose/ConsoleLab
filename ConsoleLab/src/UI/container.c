#pragma once
#include "../helpers/colors.c"
#include "../helpers/array.c"
#include <stdbool.h>
#define MAX_CHILDREN __INT8_MAX__

typedef struct {
    int top;
    int bottom;
    int right;
    int left;
} box;

typedef enum{
    line,
    doubleLine,
    bold,
} borderStyle;

typedef struct {
    box width;
    borderStyle style;
    rgba color;
    rgba background;
} border;


typedef struct element element;
typedef struct element{
    int x;
    int y;
    int width;
    int height;
    int boundingX;
    int boundingY;
    int boundingWidth;
    int boundingHeight;
    rgba background;
    rgba color;
    box margin;
    box padding;
    border border;
    element* parent;
    int childCount;
    element* children[MAX_CHILDREN];
} element;

void addChild(element* parent,element* child){
    parent->children[parent->childCount]=child;
    parent->childCount++;
}
void removeChild(element* parent,element* child){
    for(int i = 0; i < parent->childCount; i++){
        if(parent->children[i] == child){
            arrayDelete(parent->children,MAX_CHILDREN,sizeof(element*),i);
        }
    }
}

element createElement(int x, int y, int width, int height){
    element newElement;
    newElement.x = x;
    newElement.y = y;
    newElement.width = width;
    newElement.height = height;
    newElement.childCount = 0;
    return newElement;
}

void Fun(){
    border a;
}