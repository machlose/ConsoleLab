#include <stdbool.h>
#include <math.h>
typedef struct{
    int red;
    int green;
    int blue;
} rgb;
typedef struct{
    int hue;
    int saturation;
    int value;
} hsv;
rgb HsvToRgb(hsv color){
    rgb rgbColor;
    double huePrim = (double)color.hue/(double)60;
    double C = (double)(color.value*color.saturation)/(double)10000;
    double temp;
    double m = ((double)color.value/(double)100)-C;
    double intermedieate = modf(huePrim,&temp)+(double)((int)huePrim%2)-(double)1;
    intermedieate = intermedieate < 0 ? -intermedieate : intermedieate;
    double X = (double)C*((double)1-intermedieate);
    if(huePrim < 1){
        rgbColor.red = round((C+m)*(double)(double)255);
        rgbColor.green = round((X+m)*(double)255);
        rgbColor.blue = round(m*(double)255);
    }
    else if(huePrim < 2){
        rgbColor.red = round((X+m)*(double)255);
        rgbColor.green = round((C+m)*(double)255);
        rgbColor.blue = round(m*(double)255);
    }
    else if(huePrim < 3){
        rgbColor.red = round(m*(double)255);
        rgbColor.green = round((C+m)*(double)255);
        rgbColor.blue = round((X+m)*(double)255);
    }
    else if(huePrim < 4){
        rgbColor.red = round(m*(double)255);
        rgbColor.green = round((X+m)*(double)255);
        rgbColor.blue = round((C+m)*(double)255);
    }
    else if(huePrim < 5){
        rgbColor.red = round((X+m)*(double)255);
        rgbColor.green = round(m*(double)255);
        rgbColor.blue = round((C+m)*(double)255);
    }
    else if(huePrim <= 6){
        rgbColor.red = round((C+m)*(double)255);
        rgbColor.green = round(m*(double)255);
        rgbColor.blue = round((X+m)*(double)255);
    }

    return rgbColor;
}
hsv RgbToHsv(rgb color){
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

void AddHue(rgb* color,int move){
    hsv hsvColor = RgbToHsv(*color);
    hsvColor.hue += move;
    hsvColor.hue = hsvColor.hue % 360;
    if(hsvColor.hue == 0){
        hsvColor.hue = 1;
    }
    rgb rgbColor = HsvToRgb(hsvColor);
    color->red = rgbColor.red;
    color->green = rgbColor.green;
    color->blue = rgbColor.blue;
}
rgb initRgb(int red, int green, int blue){
    rgb newRgb;
    newRgb.red = red;
    newRgb.green = green;
    newRgb.blue = blue;
    return newRgb;
}