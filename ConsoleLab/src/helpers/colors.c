#pragma once

#include <stdbool.h>
#include <math.h>
typedef struct{
    byte red;
    byte green;
    byte blue;
    byte alpha;
} rgba;
typedef struct{
    int hue;
    int saturation;
    int value;
} hsv;
rgba HsvTorgba(hsv color){
    rgba rgbaColor;
    double huePrim = (double)color.hue/(double)60;
    double C = (double)(color.value*color.saturation)/(double)10000;
    double temp;
    double m = ((double)color.value/(double)100)-C;
    double intermedieate = modf(huePrim,&temp)+(double)((int)huePrim%2)-(double)1;
    intermedieate = intermedieate < 0 ? -intermedieate : intermedieate;
    double X = (double)C*((double)1-intermedieate);
    if(huePrim < 1){
        rgbaColor.red = round((C+m)*(double)(double)255);
        rgbaColor.green = round((X+m)*(double)255);
        rgbaColor.blue = round(m*(double)255);
    }
    else if(huePrim < 2){
        rgbaColor.red = round((X+m)*(double)255);
        rgbaColor.green = round((C+m)*(double)255);
        rgbaColor.blue = round(m*(double)255);
    }
    else if(huePrim < 3){
        rgbaColor.red = round(m*(double)255);
        rgbaColor.green = round((C+m)*(double)255);
        rgbaColor.blue = round((X+m)*(double)255);
    }
    else if(huePrim < 4){
        rgbaColor.red = round(m*(double)255);
        rgbaColor.green = round((X+m)*(double)255);
        rgbaColor.blue = round((C+m)*(double)255);
    }
    else if(huePrim < 5){
        rgbaColor.red = round((X+m)*(double)255);
        rgbaColor.green = round(m*(double)255);
        rgbaColor.blue = round((C+m)*(double)255);
    }
    else if(huePrim <= 6){
        rgbaColor.red = round((C+m)*(double)255);
        rgbaColor.green = round(m*(double)255);
        rgbaColor.blue = round((X+m)*(double)255);
    }

    return rgbaColor;
}
hsv rgbaToHsv(rgba color){
    hsv hsvColor;
    int m = min(min(color.red,color.green),color.blue);
    int M = max(max(color.red,color.green),color.blue);
    int C = M-m;

    if(C == 0){
        hsvColor.hue = 0;
    }
    if(color.red == M){
        hsvColor.hue = (int)round((double)((double)((double)color.green-(double)color.blue)/(double)C)*(double)60)%360;
        if(hsvColor.hue < 0){
            hsvColor.hue += 360;
        }
    }
    else if(color.green == M){
        hsvColor.hue = (int)round(((double)((double)((double)color.blue-(double)color.red)/(double)C)+(double)2)*(double)60)%360;
    }
    else if(color.blue == M){
        hsvColor.hue = (int)round(((double)((double)((double)color.red-(double)color.green)/(double)C)+(double)+4)*(double)60)%360;
    }

    hsvColor.value = (int)round(((double)M/(double)255)*(double)100);
    if(hsvColor.value == 0){
        hsvColor.saturation = 0;
    }
    else{
        hsvColor.saturation = (int)round(((double)C/(double)M)*(double)100);
    }

    return hsvColor;
}

void AddHue(rgba* color,int move){
    hsv hsvColor = rgbaToHsv(*color);
    hsvColor.hue += move;
    hsvColor.hue = hsvColor.hue % 360;
    if(hsvColor.hue == 0){
        hsvColor.hue = 1;
    }
    rgba rgbaColor = HsvTorgba(hsvColor);
    color->red = rgbaColor.red;
    color->green = rgbaColor.green;
    color->blue = rgbaColor.blue;
}
rgba initrgba(int red, int green, int blue){
    rgba newrgba;
    newrgba.red = red;
    newrgba.green = green;
    newrgba.blue = blue;
    return newrgba;
}