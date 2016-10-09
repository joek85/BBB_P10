//
// Created by joek on 10/4/16.
//


#include "BBBio_lib/BBBiolib.h"

#include "font_5x7.h"
#include "BBB_P10.h"

static unsigned char PixelTable[8] = {
        0x80,
        0x40,
        0x20,
        0x10,
        0x08,
        0x04,
        0x02,
        0x01
};


unsigned char bb[64*nPANEL];
void output_low(char p ,char g) {
    pin_low(p, g);
}
void output_high(char p ,char g) {
    pin_high(p, g);
}
void send_8_pixels(unsigned char bytes){
    int color;
    for (int i = 7; i >= 0; --i) {
        color = (bytes & (1 << i));
        if (color == 0){
            output_high(8,Pixel);
        } else{
            output_low(8,Pixel);
        }
        output_low(8,CLK);
        output_high(8,CLK);
    }
}
void Setup_Pins(){
    iolib_init();
    iolib_setdir(9,A, BBBIO_DIR_OUT);
    iolib_setdir(9,B, BBBIO_DIR_OUT);
    iolib_setdir(9,OE, BBBIO_DIR_OUT);
    iolib_setdir(9,SCLK, BBBIO_DIR_OUT);
    iolib_setdir(8,Pixel, BBBIO_DIR_OUT);
    iolib_setdir(8,CLK, BBBIO_DIR_OUT);
    output_low(9,SCLK);
    output_low(8,CLK);
    output_low(8,Pixel);
    output_low(9,A);
    output_low(9,B);
    output_high(9,OE);
}
void Free_BBB_P10(){
    output_low(9,OE);
    output_low(9,A);
    output_low(9,B);
    output_low(9,SCLK);
    output_low(8,CLK);
    output_low(8,Pixel);
    iolib_free();
}
void Clear_Buffer(bool fill) {
    if (fill)
        memset(bb,0xFF,sizeof(bb));
    else
        memset(bb,0x00,sizeof(bb));
}

void Scale(int x , int y , int bPixel) {
    if(x<0 || x>=(WIDTH*nPANEL) || y<0 || y>=HEIGHT) return;
    int index;
    int panel= ((x / (WIDTH*nPANEL)) + ((y / HEIGHT)));
    x=(x % (WIDTH*nPANEL))+ (panel<<5);
    y=y % HEIGHT;

    index = x/8 + y*(nPANEL << 2);

    char data = PixelTable[x & 0x07];
    if (bPixel == 0){
        bb[index] &= ~data;
    }else{
        bb[index] |= data;
    }
}
void Fill_Pixels(int row , int numpanels) {
    int offset= (numpanels << 2) * row;
    for (int i = 0; i < (numpanels << 2); ++i) {
        send_8_pixels(bb[i+offset+(((numpanels<<2)*3)<<2)]);
        send_8_pixels(bb[i+offset+(numpanels<<5)]);
        send_8_pixels(bb[i+offset+(numpanels<<4)]);
        send_8_pixels(bb[offset + i]);
    }
}
void select_row(int row) {
    output_low(9,OE);
    switch (row) {
        case 0:
            output_low(9,A);
            output_low(9,B);
            Fill_Pixels(row, nPANEL);
            break;
        case 1:
            output_high(9,A);
            output_low(9,B);
            Fill_Pixels(row, nPANEL);
            break;
        case 2:
            output_low(9,A);
            output_high(9,B);
            Fill_Pixels(row, nPANEL);
            break;
        case 3:
            output_high(9,A);
            output_high(9,B);
            Fill_Pixels(row,nPANEL);
            break;
        default:
            break;
    }
    output_low(9,SCLK);
    output_high(9,SCLK);
    output_high(9,OE);
}
void Scan_Panels(){
    for (int row=0;row<=3;row++) {
        select_row(row);
        usleep(2000);
    }
}
int Find_Index(unsigned char array[], size_t size, int target) {
    int i=0;
    while((i<size) && (array[i] != target)) i++;

    return (i<size) ? (i) : (-1);
}
void drawchar(char c,int x , int y){
    int cindex = c;
    int index =  Find_Index(font5x7_Indexes, sizeof(font5x7_Indexes),cindex);
    for (int i = 0; i < font_width; ++i) {
        for (int j = 0; j < font_height; ++j) {
            Scale(i+x,j+y,(font5x7[i+(font_width*index)] & (1 << j)));
        }
    }
}
void drawstring(const char *c , int x , int y) {
    int cnt = 0;
    for (int i = 0; i < strlen(c); ++i) {
        drawchar(*(c + i), i * font_width + x, y);
    }
}
void movestring(const char *msg, int direction, int alignment, __useconds_t duration) {
        switch (alignment) {
            case AlignTop:
                switch (direction) {
                    case Right:
                        for (int i = 0; i <= WIDTH * nPANEL; ++i) {
                            Clear_Buffer(false);
                            drawstring(msg, i, 0);
                            usleep(duration);
                        }
                        break;
                    case Left:
                        for (int i = WIDTH * nPANEL ; i >= 0; --i) {
                            Clear_Buffer(false);
                            drawstring(msg, i, 0);
                            usleep(duration);
                        }
                        break;
                    default:
                        break;
                }
                break;
            case AlignCenter:
                switch (direction) {
                    case Right:
                        for (int i = -1*((int)strlen(msg)*font_width); i <= WIDTH * nPANEL; ++i) {
                            Clear_Buffer(false);
                            drawstring(msg, i, HEIGHT / 4);
                            usleep(duration);
                        }
                        break;
                    case Left:;
                        for (int i = WIDTH * nPANEL; i > -1*((int)strlen(msg)*font_width); --i) {
                            Clear_Buffer(false);
                            drawstring(msg, i, HEIGHT / 4);
                            usleep(duration);
                        }
                        break;
                    default:
                        break;
                }
                break;
            case AlignBottom:
                switch (direction) {
                    case Right:
                        for (int i = 0; i <= WIDTH * nPANEL; ++i) {
                            Clear_Buffer(false);
                            drawstring(msg, i, HEIGHT - font_height);
                            usleep(duration);
                        }
                        break;
                    case Left:
                        for (int i = WIDTH * nPANEL; i >= 0; --i) {
                            Clear_Buffer(false);
                            drawstring(msg, i, HEIGHT - font_height);
                            usleep(duration);
                        }
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }

    }
void drawmessage(const char *msg, int halignment, int valignment) {
        switch (halignment) {
            case AlignHLeft:
                switch (valignment) {
                    case AlignTop:
                        drawstring(msg, 0, 0);
                        break;
                    case AlignCenter:
                        drawstring(msg, 0, HEIGHT / 4);
                        break;
                    case AlignBottom:
                        drawstring(msg, 0, HEIGHT - font_height);
                        break;
                    default:
                        break;
                }
                break;
            case AlignHCenter:
                switch (valignment) {
                    case AlignTop:
                        drawstring(msg, (WIDTH * nPANEL / 2 - 1) - (strlen(msg) * 2) - font_width - 1, 0);
                        break;
                    case AlignCenter:
                        drawstring(msg, (WIDTH * nPANEL / 2 - 1) - (strlen(msg) * 2) - font_width - 1, HEIGHT / 4);
                        break;
                    case AlignBottom:
                        drawstring(msg, (WIDTH * nPANEL / 2 - 1) - (strlen(msg) * 2) - font_width - 1,
                                   HEIGHT - font_height);
                        break;
                    default:
                        break;
                }
                break;
            case AlignHRight:
                switch (valignment) {
                    case AlignTop:
                        drawstring(msg, (WIDTH * nPANEL) - strlen(msg) * font_width, 0);
                        break;
                    case AlignCenter:
                        drawstring(msg, (WIDTH * nPANEL) - strlen(msg) * font_width, HEIGHT / 4);
                        break;
                    case AlignBottom:
                        drawstring(msg, (WIDTH * nPANEL) - strlen(msg) * font_width, HEIGHT - font_height);
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
void flashmessage(const char *msg, int halignment, int valignment, __useconds_t duration, int times) {
        for (int i = 0; i < times; ++i) {
            drawmessage(msg, halignment, valignment);
            usleep(duration);
            Clear_Buffer(false);
            usleep(duration);
        }
    }
void drawLine(int x1, int y1, int x2, int y2) {
        int dy = y2 - y1;
        int dx = x2 - x1;
        int stepx, stepy;

        if (dy < 0) {
            dy = -dy;
            stepy = -1;
        } else {
            stepy = 1;
        }
        if (dx < 0) {
            dx = -dx;
            stepx = -1;
        } else {
            stepx = 1;
        }
        dy <<= 1;
        dx <<= 1;

        Scale(x1, y1, 1);
        if (dx > dy) {
            int fraction = dy - (dx >> 1);
            while (x1 != x2) {
                if (fraction >= 0) {
                    y1 += stepy;
                    fraction -= dx;
                }
                x1 += stepx;
                fraction += dy;
                Scale(x1, y1, 1);
            }
        } else {
            int fraction = dx - (dy >> 1);
            while (y1 != y2) {
                if (fraction >= 0) {
                    x1 += stepx;
                    fraction -= dy;
                }
                y1 += stepy;
                fraction += dx;
                Scale(x1, y1, 1);
            }
        }
    }
void drawBox(int x1, int y1, int x2, int y2) {
        drawLine(x1, y1, x2, y1);
        drawLine(x2, y1, x2, y2);
        drawLine(x2, y2, x1, y2);
        drawLine(x1, y2, x1, y1);
    }

