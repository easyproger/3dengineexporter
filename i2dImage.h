//
//  Image.h
//  Mouson
//
//  Created by easy proger on 10.08.11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef Mouson_Image_h
#define Mouson_Image_h


#include <fstream>
#include "stdio.h"

using namespace std;

class I2dSImage{
private:
    static unsigned char*   temp_ch_buf;
    static float*       temp_flt_buf;
    static int      class_ref;
public:
    unsigned char* base;
    int width;
    int height;
    int bits;
public:
    I2dSImage(unsigned char* _img=0, int _w=0, int _h=0, int _bits=0);
    I2dSImage(const I2dSImage&);
    ~I2dSImage();
    
    bool IsValid() const { return base != 0; };
    void Release();
    void Init(int w, int h, int b);
    void Scale(int w, int h); // отмасштабировать изображение, задайте новую ширину и высоту
    void ScaleToTemp(int w, int h);
    void Scale(int w, int h, unsigned char** out);
    void ScaleBest(int w, int h);
    
    void _Pow2Scale(int& w, int& h);
    void Pow2Scale();
    void Pow2ScaleT();
    inline unsigned char*  getPoint(int x, int y) {
        return &base[(y*width + x)*(bits/8)];
    };
    inline const unsigned char*  getPoint(int x, int y) const {
        return &base[(y*width + x)*(bits/8)];
    };
    
    void Load(FILE* fin);
    void Save(ofstream* fout);
};
#endif
