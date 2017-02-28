//
//  I2dAtlas.cpp
//  Mouson
//
//  Created by easy proger on 10.08.11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "I2dAtlas.h"

#include "I2dImage.h"


I2dImg::I2dImg(int _id, const I2dSImage* _I2dImg){
    id = _id;
    img = _I2dImg;
}

void I2dImg::Rotate() const{
    int _width = width;
    width = height;
    height = _width;
//    width+=2;
//    height+=2;
    
    rotated = !rotated;
}

bool I2dImg::operator< (const I2dImg& i){
    if(img->bits == i.img->bits)
        return id < i.id;
    
    return img->bits < i.img->bits;
}

I2dAtlas::I2dAtlas():count_node(COUNT_NODES_IN_ATLAS){
    cur_node = 0;
    free_leafs.alloc(count_node*free_leafs.node_sz());
    //free_leafs.set_default(0);
    free_leafs.ignore_id();
    free_leafs.push_back(&nodes[0]);
}

void I2dAtlas::Size(int x, int y, int width, int height){
    nodes[0].rc.Init(x, y, width, height);
    k_div = float(width)/float(height);
    free_leafs.push_back(&nodes[0]);
}

I2dAtlas::I2dNode::I2dNode(){
    child[1] = child[0] = parent = 0;
    image_id = -1;
    free = true;
}

void I2dAtlas::I2dNode::Clear(){
    child[1] = child[0] = parent = 0;
    image_id = -1;
    free = true;
}

void I2dAtlas::I2dNode::Rectangle::Init(int _x, int _y, int w, int h){
    x = _x;
    y = _y;
    width = w;
    height = h;
}

I2dAtlas::I2dNode* I2dAtlas::Insert(const I2dImg& I2dImg){
    int max_rc, min_rc;
    int max_I2dImg, min_I2dImg;
    int dw, dh;
    
    if(cur_node > count_node - 4)
        return 0;
    
    for(free_leafs.begin(); !free_leafs.is_end(); free_leafs.next()){
        I2dAtlas::I2dNode& leaf = *(free_leafs.iterator->data);
        if(leaf.child[0] != 0 && leaf.child[1] != 0 && !leaf.free){
            continue;
        }else{
            if(leaf.image_id != -1)
                continue;
            
            max_rc = leaf.rc.width;
            min_rc = leaf.rc.height;
            max_I2dImg = I2dImg.width;
            min_I2dImg = I2dImg.height;
            
            if(max_rc < min_rc){
                max_rc = min_rc;
                min_rc = leaf.rc.width;
            }
            if(max_I2dImg < min_I2dImg){
                I2dImg.Rotate();
                max_I2dImg = I2dImg.width;
                min_I2dImg = I2dImg.height;           
            }
            if(max_I2dImg > max_rc || min_I2dImg > min_rc)
                continue;
            
            leaf.free = false;
            
            free_leafs.pop_iterator();
            if(max_I2dImg == max_rc && min_I2dImg == min_rc){             
                leaf.image_id = 0;             
                return &leaf;
            }
            
            leaf.child[0] = &nodes[++cur_node];
            leaf.child[1] = &nodes[++cur_node];
            
            leaf.child[0]->parent = &leaf;
            leaf.child[1]->parent = &leaf;
            
            dw = leaf.rc.width - I2dImg.width;
            dh = leaf.rc.height - I2dImg.height;
            
            if(dw < 0 || dh < 0){
                I2dImg.Rotate();
                dw = leaf.rc.width - I2dImg.width;
                dh = leaf.rc.height - I2dImg.height;
            }
            
            //leaf.rc.x+=1;
            
            
            if(dw>float(dh)*k_div){
                leaf.child[0]->rc.Init(leaf.rc.x, leaf.rc.y,
                                       I2dImg.width, leaf.rc.height);
                leaf.child[1]->rc.Init(leaf.rc.x + I2dImg.width, leaf.rc.y,
                                       leaf.rc.width - I2dImg.width, leaf.rc.height);
            }else{
                leaf.child[0]->rc.Init(leaf.rc.x, leaf.rc.y,
                                       leaf.rc.width, I2dImg.height);
                leaf.child[1]->rc.Init(leaf.rc.x, leaf.rc.y + I2dImg.height,
                                       leaf.rc.width, leaf.rc.height - I2dImg.height);
            }
//            leaf.child[1]->rc.x+=2;
//            leaf.child[1]->rc.y+=2;
//            leaf.child[1]->rc.width-=2;
//            leaf.child[1]->rc.height-=2;
            free_leafs.push_before_iterator(leaf.child[1]);
            free_leafs.prev();
            
            return InsertLeaf(I2dImg, *leaf.child[0]);
        }
    }
    
    return 0;
}

I2dAtlas::I2dNode* I2dAtlas::InsertLeaf(const I2dImg& I2dImg, I2dNode& branch){
    int max_rc = branch.rc.width;
    int min_rc = branch.rc.height;
    int max_I2dImg = I2dImg.width;
    int min_I2dImg = I2dImg.height;
    //bool rotate = false;
    
    if(max_rc < min_rc){
        max_rc = min_rc;
        min_rc = branch.rc.width;
    }
    if(max_I2dImg < min_I2dImg){
        max_I2dImg = min_I2dImg;
        min_I2dImg = I2dImg.width;
    }
    
    if(max_I2dImg > max_rc || min_I2dImg > min_rc)
        return 0;
    
    branch.free = false;
    if(max_I2dImg == max_rc && min_I2dImg == min_rc){     
        branch.image_id = 0;
        return &branch;
    }
    
    branch.child[0] = &nodes[++cur_node];
    branch.child[1] = &nodes[++cur_node];
    
    branch.child[0]->parent = &branch;
    branch.child[1]->parent = &branch;
    
    int dw = branch.rc.width - I2dImg.width;
    int dh = branch.rc.height - I2dImg.height;
    
    if(dw>float(dh)*k_div){
        branch.child[0]->rc.Init(branch.rc.x, branch.rc.y,
                                 I2dImg.width, branch.rc.height);
        branch.child[1]->rc.Init(branch.rc.x + I2dImg.width, branch.rc.y,
                                 branch.rc.width - I2dImg.width, branch.rc.height);
    }else{
        branch.child[0]->rc.Init(branch.rc.x, branch.rc.y,
                                 branch.rc.width, I2dImg.height);
        branch.child[1]->rc.Init(branch.rc.x, branch.rc.y + I2dImg.height,
                                 branch.rc.width, branch.rc.height - I2dImg.height);
    }
    
    branch.child[0]->image_id = 0;
    branch.child[0]->free = false;
//    branch.child[1]->rc.x+=2;
//    branch.child[1]->rc.y+=2;
//    branch.child[1]->rc.width-=2;
//    branch.child[1]->rc.height-=2;
    free_leafs.push_before_iterator(branch.child[1]);
    
    return branch.child[0];
}

I2dAtlas::I2dNode* I2dAtlas::Get(int I2dImg_id){
    if(I2dImg_id < 0)
        return 0;
    
    int counter = cur_node;
    while(counter > 0){
        if(nodes[--counter].image_id == I2dImg_id)
            return &nodes[counter];
    }
    return 0;
}

// старый вариант не согласованный с free_leafs, не использовать в общем )
bool I2dAtlas::Remove(int I2dImg_id){
    I2dNode* node = Get(I2dImg_id);
    if(node != 0){
        node->image_id = -1;
        node->free = true;
        if(node->parent != 0){
            int brother_id = ((node->parent->child[0] == node)? 1 : 0);
            I2dNode* brother = node->parent->child[brother_id];
            while(brother->free){
                if(node->child[0] != 0){
                    node->child[0]->Clear();
                    node->child[0] = 0;
                }
                
                if(node->child[1] != 0){
                    node->child[1]->Clear();
                    node->child[1] = 0;
                }
                
                node = node->parent;
                
                node->image_id = -1;
                node->free = true;
                if(node->parent != 0){
                    brother_id = ((node->parent->child[0] == node)? 1 : 0);
                    brother = node->parent->child[brother_id];
                }else
                    break;
            }
        }
        
        return true;
    }else
        return false;
}

void I2dAtlas::Clear(){
    while(cur_node > 0)
        nodes[--cur_node].Clear();
    
    free_leafs.clear();
}

void I2dAtlas::Release(){
    free_leafs.Release();
}