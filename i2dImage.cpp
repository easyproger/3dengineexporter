//
//  Image.cpp
//  Mouson
//
//  Created by easy proger on 10.08.11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "I2dImage.h"
#include <math.h>
#include <fstream>

#define SIZE_CH_BUF (2048*2048*4)
#define SIZE_FLT_BUF (256*256)

unsigned char* I2dSImage::temp_ch_buf  = 0;
float* I2dSImage::temp_flt_buf     = 0;
int I2dSImage::class_ref           = 0;

using namespace std;

I2dSImage::I2dSImage(unsigned char* _img, int _w, int _h, int _bits)
:base(_img), width(_w), height(_h), bits(_bits){
    if(temp_ch_buf == 0)
        temp_ch_buf = new unsigned char[SIZE_CH_BUF];
    
    if(temp_flt_buf == 0)
        temp_flt_buf = new float[SIZE_FLT_BUF];
    
    class_ref++;
}

I2dSImage::I2dSImage(const I2dSImage& _img){
    base = _img.base;
    width = _img.width;
    height = _img.height;
    bits = _img.bits;
    
    class_ref++;
}

I2dSImage::~I2dSImage(){
    class_ref--;
    if(class_ref < 1){

        
        if(temp_ch_buf != 0){
            delete[] temp_ch_buf;
            temp_ch_buf = 0;
        }
        if(temp_flt_buf != 0){
            delete[] temp_flt_buf;
            temp_flt_buf = 0;
        }
    }
}


void I2dSImage::Release(){
    if(base != 0){
        if(base != temp_ch_buf)
            delete[] base;
        
        base = 0;
        bits = 0;
        height = 0;
        width = 0;
    }
}

void I2dSImage::Init(int w, int h, int b){
    Release();
    base = new unsigned char[w*h*(b/8)];
    for(int n=0; n<w*h*(b/8); n++)
        base[n] = 200;
    
    width = w;
    height = h;
    bits = b;
}

void I2dSImage::Scale(int w, int h){
    if(base != 0){
        if(w != width || h != height){
            int num_cl = bits/8;
            unsigned char* temp = new unsigned char[w*h*num_cl];
            
            float _h = float(height)/float(h);
            float _w = float(width)/float(w);
            
            int dw = int(_w + .5f);
            int dh = int(_h + .5f);
            
            if(dw<2)
                dw = 2;
            if(dh<2)
                dh = 2;
            
            float* matrix = 0;
            if(dw*dh <= SIZE_FLT_BUF)
                matrix = temp_flt_buf;
            else
                matrix = new float[dw*dh];
            
            for(int y=0; y<h; y++){
                float orig_y = ( _h*(float(y) + .5f) ) - .5f;
                int sy = int(orig_y - float(dh-1)/2.f);
                int fy = sy+dh;
                if(fy>height)
                    fy = height;
                
                for(int x=0; x<w; x++){
                    float orig_x = ( _w*(float(x) + .5f) ) - .5f;
                    int sx = int(orig_x - float(dw-1)/2.f);
                    
                    int fx = sx+dw;
                    if(fx>width)
                        fx = width;
                    
                    for(int n=0; n<dw*dh; n++)
                        matrix[n] = .0f;
                    
                    for(int my=sy; my<fy; my++){
                        float ty = float(my) - orig_y;
                        ty *= ty;
                        int offset = (my-sy)*dw;
                        for(int mx=sx; mx<fx; mx++){
                            float tx = float(mx) - orig_x;
                            tx *= tx;
                            matrix[offset + (mx - sx)] = sqrtf(tx + ty);
                        }
                    }
                    
                    float sum = .0f;
                    for(int n=0; n<dw*dh; n++)
                        sum += matrix[n];
                    
                    for(int n=0; n<dw*dh; n++)
                        matrix[n] = sum - matrix[n];
                    
                    sum = 0.f;
                    for(int n=0; n<dw*dh; n++)
                        sum += matrix[n];
                    
                    sum = 1.f/sum;
                    for(int n=0; n<dw*dh; n++)
                        matrix[n] *= sum;
                    
                    for(int c=0; c<num_cl; c++){
                        sum = .0f;
                        for(int my=sy; my<fy; my++){
                            int offset = (my-sy)*dw;
                            for(int mx=sx; mx<fx; mx++){
                                int to = (int(my)*width + int(mx))*num_cl + c;
                                sum += float(base[to]) * matrix[offset + mx - sx];
                            }
                        }
                        
                        temp[(y*w + x)*num_cl + c] = int(sum);
                    }
                }
            }
            if(matrix != temp_flt_buf)
                delete[] matrix;
            
            delete[] base;
            base = temp;
            
            width = w;
            height = h;             
        }
    }
}

void I2dSImage::ScaleToTemp(int w, int h){
    if(base != 0){
        if((w != width || h != height) && w*h*(bits/8) <= SIZE_CH_BUF){
            int num_cl = bits/8;
            unsigned char* temp = temp_ch_buf;
            if(base == temp){
                temp = &base[width*height*num_cl];
                if((width*height + w*h)*num_cl > SIZE_CH_BUF){
                    // TODO: enlarge temp_ch_buf

                    
                    return;
                }
            }
            
            float _h = float(height)/float(h);
            float _w = float(width)/float(w);
            
            int dw = int(_w + .5f);
            int dh = int(_h + .5f);
            
            if(dw<2)
                dw = 2;
            if(dh<2)
                dh = 2;
            
            if(dw*dh <= SIZE_FLT_BUF){
                float* matrix = temp_flt_buf;
                
                for(int y=0; y<h; y++){
                    float orig_y = ( _h*(float(y) + .5f) ) - .5f;
                    int sy = int(orig_y - float(dh-1)/2.f);
                    int fy = sy+dh;
                    if(fy>height)
                        fy = height;
                    
                    for(int x=0; x<w; x++){
                        float orig_x = ( _w*(float(x) + .5f) ) - .5f;
                        int sx = int(orig_x - float(dw-1)/2.f);
                        
                        int fx = sx+dw;
                        if(fx>width)
                            fx = width;
                        
                        for(int i=0; i<dw*dh; i++)
                            matrix[i] = .0f;
                        
                        for(int i=sy; i<fy; i++){
                            float ty = float(i) - orig_y;
                            ty *= ty;
                            int offset = (i-sy)*dw;
                            for(int mx=sx; mx<fx; mx++){
                                float tx = float(mx) - orig_x;
                                tx *= tx;
                                matrix[offset + (mx - sx)] = sqrtf(tx + ty);
                            }
                        }
                        
                        float sum = .0f;
                        for(int i=0; i<dw*dh; i++)
                            sum += matrix[i];
                        
                        for(int i=0; i<dw*dh; i++)
                            matrix[i] = sum - matrix[i];
                        
                        sum = 0.f;
                        for(int i=0; i<dw*dh; i++)
                            sum += matrix[i];
                        
                        sum = 1.f/sum;
                        for(int i =0; i<dw*dh; i++)
                            matrix[i] *= sum;
                        
                        for(int c=0; c<num_cl; c++){
                            sum = .0f;
                            for(int i=sy; i<fy; i++){
                                int offset = (i-sy)*dw;
                                for(int mx=sx; mx<fx; mx++){
                                    int to = (int(i)*width + int(mx))*num_cl + c;
                                    sum += float(base[to]) * matrix[offset + mx - sx];
                                }
                            }
                            
                            temp[(y*w + x)*num_cl + c] = int(sum);
                        }
                    }
                }
                base = temp;
                
                width = w;
                height = h;
            }
        }
    }
}

void I2dSImage::Scale(int w, int h, unsigned char** out){
    if(base != 0){
        if(w != width || h != height){
            //if(out == 0)
            *out = new unsigned char[w*h*(bits/8)];
            unsigned char* modify_base = *out;
            
            int num_cl = bits/8;
            
            float _h = float(height)/float(h);
            float _w = float(width)/float(w);
            
            int dw = int(_w + .5f);
            int dh = int(_h + .5f);
            
            if(dw<2)
                dw = 2;
            if(dh<2)
                dh = 2;
            
            float* matrix = 0;
            if(dw*dh <= SIZE_FLT_BUF)
                matrix = temp_flt_buf;
            else
                matrix = new float[dw*dh];
            
            for(int y=0; y<h; y++){
                float orig_y = ( _h*(float(y) + .5f) ) - .5f;
                int sy = int(orig_y - float(dh-1)/2.f);
                int fy = sy+dh;
                if(fy>height)
                    fy = height;
                
                for(int x=0; x<w; x++){
                    float orig_x = ( _w*(float(x) + .5f) ) - .5f;
                    int sx = int(orig_x - float(dw-1)/2.f);
                    
                    int fx = sx+dw;
                    if(fx>width)
                        fx = width;
                    
                    for(int i=0; i<dw*dh; i++)
                        matrix[i] = .0f;
                    
                    for(int i=sy; i<fy; i++){
                        float ty = float(i) - orig_y;
                        ty *= ty;
                        int offset = (i-sy)*dw;
                        for(int mx=sx; mx<fx; mx++){
                            float tx = float(mx) - orig_x;
                            tx *= tx;
                            matrix[offset + (mx - sx)] = sqrtf(tx + ty);
                        }
                    }
                    
                    float sum = .0f;
                    for(int i=0; i<dw*dh; i++)
                        sum += matrix[i];
                    
                    for(int i=0; i<dw*dh; i++)
                        matrix[i] = sum - matrix[i];
                    
                    sum = 0.f;
                    for(int i=0; i<dw*dh; i++)
                        sum += matrix[i];
                    
                    sum = 1.f/sum;
                    for(int i=0; i<dw*dh; i++)
                        matrix[i] *= sum;
                    
                    for(int c=0; c<num_cl; c++){
                        sum = .0f;
                        for(int i=sy; i<fy; i++){
                            int offset = (i-sy)*dw;
                            for(int mx=sx; mx<fx; mx++){
                                int to = (int(i)*width + int(mx))*num_cl + c;
                                sum += float(base[to]) * matrix[offset + mx - sx];
                            }
                        }
                        
                        modify_base[(y*w + x)*num_cl + c] = int(sum);
                    }
                }
            }
            if(matrix != temp_flt_buf)
                delete[] matrix;
        }
    }
}

void I2dSImage::ScaleBest(int w, int h){
    if(base != 0){
        if(w != width || h != height){
            unsigned char* temp = new unsigned char[w*h*(bits/8)];
            
            int num_cl = bits/8;
            unsigned int tcl[4];
            
            float _h = float(height)/float(h);
            float _w = float(width)/float(w);
            
            float _sh = (height > h) ? 0.1f : 0.1f;
            float _sw = (width > w) ? 0.1f : 0.1f;
            
            float z;
            unsigned int offset;
            
            for(int nh=0; nh<h; nh++){
                for(int nw=0; nw<w; nw++){
                    for(int i = 0; i<num_cl; i++)
                        tcl[i] = 0;
                    
                    z = 0.0f;
                    
                    for(float i=0; (i<_h)&&(int(_h*float(nh)+i)<height); i+=_sh){
                        for(float j=0; (j<_w)&&(int(_w*float(nw)+j)<width); j+=_sw){
                            
                            offset = ( int(_h*float(nh)+i)*width + int(_w*float(nw)+j) )*num_cl;
                            
                            for(int ij = 0; ij<num_cl; ij++)
                                tcl[ij] += base[offset + ij];
                            
                            z++;
                        }
                    }
                    
                    for(int ij = 0; ij<num_cl; ij++)
                        temp[(nh*w + nw)*num_cl + ij] = int(float(tcl[ij])/z);
                }
            }
            
            delete[] base;
            width = w;
            height = h;
            base = temp;
        }
    }
}

void I2dSImage::_Pow2Scale(int& w, int& h){
    int b_side = (height>width) ? height : width;
    int s_side = (height<width) ? height : width;
    
    int tb_side = 2;
    double k = b_side;
    
    while(k>2){
        k /= 2.;
        tb_side *= 2;
    }
    
    int ts_side = 2;
    k = s_side;
    while(k>2){
        k /= 2.;
        ts_side *= 2;
    }
    
    if( ((tb_side*3)/4 <= b_side) && (tb_side>b_side) ){
        tb_side /= 2;
        if(ts_side>s_side)
            ts_side /= 2;
    }   
    
    if(height>width){
        h = tb_side;
        w = ts_side;
    }else{
        h = ts_side;
        w = tb_side;
    }
}

void I2dSImage::Pow2Scale(){
    int w, h;
    _Pow2Scale(w, h);
    Scale(w, h);
}

void I2dSImage::Pow2ScaleT(){
    int w, h;
    _Pow2Scale(w, h);
    ScaleToTemp(w, h);
}

void I2dSImage::Load(FILE* fin){
    if(fin != 0){
        bool r;
        fread(&r, sizeof(bool), 1, fin);
        if(r){
            fread(&width, sizeof(int), 1, fin);
            fread(&height, sizeof(int), 1, fin);
            fread(&bits, sizeof(int), 1, fin);
            if(base != 0){
                delete[] base;
                base = 0;
            }
            base = new unsigned char[width*height*(bits/8)];
            fread(&base[0], sizeof(unsigned char), width*height*(bits/8), fin);
        }
    }
}

void I2dSImage::Save(ofstream* fout){
    if(fout->is_open() != 0){
        // Desc
        bool r = (base != 0);
        fout->write((char*) &r, sizeof(bool));
        if(r){
            fout->write((char*) &width, sizeof(int));
            fout->write((char*) &height, sizeof(int));
            fout->write((char*) &bits, sizeof(int));
            fout->write((char*) &base[0], sizeof(unsigned char)*width*height*(bits/8));
        }
    }
}