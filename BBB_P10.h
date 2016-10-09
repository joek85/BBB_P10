//
// Created by joek on 10/4/16.
//
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#ifndef BBB_P10_BBB_P10_H
#define BBB_P10_BBB_P10_H
#define WIDTH   32
#define HEIGHT  16
#define nPANEL  3
#define OE      15
#define A       23
#define B       12
#define SCLK    41
#define Pixel   15
#define CLK     17
#define font_width  5
#define font_height 7

enum MovingDirection{
    Right = 0,
    Left = 1
};
enum VAlignment{
    AlignTop = 0,
    AlignCenter = 1,
    AlignBottom = 2
};
enum HAlignment{
    AlignHLeft = 0,
    AlignHCenter = 1,
    AlignHRight = 2
};

void Setup_Pins();
void Free_BBB_P10();
void Clear_Buffer(bool fill);
void Scan_Panels();

void movestring(const char *msg, int direction, int alignment, __useconds_t duration);
void drawmessage(const char *msg, int halignment, int valignment);
void drawBox(int x1, int y1, int x2, int y2);
void drawLine(int x1, int y1, int x2, int y2);
void flashmessage(const char *msg, int halignment, int valignment, __useconds_t duration, int times);
#endif //BBB_P10_BBB_P10_H
