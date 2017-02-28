//
//  Atlas.h
//  Mouson
//
//  Created by easy proger on 10.08.11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef Mouson_Atlas_h
#define Mouson_Atlas_h



#include "I2dmod_array.h"

class I2dSImage;

struct I2dImg{
    const I2dSImage* img;
    int id;
    
    mutable int width;
    mutable int height;     
    mutable bool rotated;
    
    I2dImg(int _id, const I2dSImage* _img);
    void Rotate() const;
    bool operator< (const I2dImg& i);
};

#define COUNT_NODES_IN_ATLAS 2048



class I2dAtlas{
public:
    
	struct I2dNode{
        struct Rectangle{
            int x, y;
            int width, height;
            void Init(int, int, int, int);
        };
        Rectangle rc;
        I2dNode* child[2];
        I2dNode* parent;
        int image_id;
        bool free; 
        bool rotated;
        I2dNode();     
        void    Clear();       
    };
private:
    I2dNode nodes[COUNT_NODES_IN_ATLAS];
    I2dModArray<I2dNode*> free_leafs;
    const int count_node;
    
    int cur_node;
    float k_div;
    
    
    I2dNode* InsertLeaf(const I2dImg& img, I2dNode& branch);
    
    bool Remove(int);
    I2dNode* Get(int);
public:
    I2dAtlas();
    void Size(int x, int y, int width, int height);
    I2dNode* Insert(const I2dImg& img);
    
    void Clear();
    void Release();
};


#endif
