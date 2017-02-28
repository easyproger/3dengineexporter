//
//  I2dMod_Array.h
//  Mouson
//
//  Created by easy proger on 10.08.11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef Mouson_I2dMod_Array_h
#define Mouson_I2dMod_Array_h



//->-------------=Массив=------------------------------------------------------
template<class C>
class I2dModArray{
private:
    struct Node{
        C data;
        unsigned int id;
        Node* next;
        Node* prev;
    }; 
    Node guardian;
    C def;
    
    Node* free_nodes;
    Node* pool;
    Node* tail;
    mutable Node* e;
    
    // count of uses elements
    unsigned int sz;
    // count of all elements
    unsigned int alloc_sz;
    
    bool use_id;
    
protected:
    Node*   _new_node();
    void    _free_node(Node*);
public:
    Node* prev_iterator;
    Node* iterator;
    I2dModArray();
    ~I2dModArray();
    
    // Alloc size in bytes
    void alloc(size_t _sz);
    size_t node_sz() { return sizeof(Node); };
    
    void ignore_id() { if (sz == 0) use_id = false; };
    void set_default(C* t) { def = *t; };
    bool push_back(const C& new_e, unsigned int id = 0);
    bool push_before_iterator(const C& new_e, unsigned int id = 0);
    //bool __push_after_iterator(const C& new_e, unsigned int id = 0); // disabled
    bool pop_iterator();
    bool pop_id(unsigned int id);
    bool pop_pos(unsigned int i);
    void next() { prev_iterator = iterator; iterator = iterator->next; };
    void prev() { iterator = prev_iterator; prev_iterator = prev_iterator->prev; };
    void begin(unsigned int i = 0);
    void end()  { prev_iterator = 0; iterator = tail; };
    const bool is_head()    { if (sz > 0) return iterator == guardian.next;else return true; };
    const bool is_end()     { if (sz > 0) return iterator == tail->next;else return true; };
    
    inline const C& front() { if (sz > 0) return guardian.next->data;else return def; }
    inline const C& back()  { if (sz > 0) return tail->data;else return def; }
    inline const unsigned int size() { return sz; }
    inline const C& get_default() { return def; };
    inline const C& read       (unsigned int id) const;
    inline       C& write      (unsigned int id);
    inline const C& operator[] (unsigned int i ) const;
    inline       C& operator() (unsigned int i );
    
    int clear();
    int Release();
};
//-*-------------------------------------------------------------------------*-

//->-------------=Конструктор=-------------------------------------------------
template<class C> I2dModArray<C>::I2dModArray()
{
    guardian.next = guardian.prev = tail = 0;
    guardian.id = 0;
    free_nodes = 0;
    pool = 0;
    sz = 0;
    alloc_sz = 0;
    
    use_id = true;
}
//-*-------------------------------------------------------------------------*-

template<class C> I2dModArray<C>::~I2dModArray()
{
    
}

template<class C> void I2dModArray<C>::alloc(size_t _sz)
{
    //size_t element_sz = sizeof(Node) + sizeof(Node) % _align;
    _sz -= _sz % sizeof(Node);
    alloc_sz = _sz / sizeof(Node); //element_sz;
    pool = new Node[alloc_sz];
    if(pool != 0){     
        // init free_nodes
        free_nodes = pool;
        free_nodes->prev = 0;
        
        Node* next_el = pool;
        for(unsigned int i(0); i<alloc_sz - 1; i++){
            next_el++;
            free_nodes->next = next_el;
            free_nodes->id = 0xfffffbad;
            next_el->prev = free_nodes;         
            free_nodes = free_nodes->next;
        }
        free_nodes->next = 0;
        free_nodes = pool;
    }else
        alloc_sz = 0;
}


template<class C>  void I2dModArray<C>::_free_node(typename I2dModArray<C>::Node* node)
{
    if(node != 0){
        if(node->id == 0xfffffbad){

        }else{
            node->data.~C();
            
            node->next = free_nodes;
            node->prev = 0;
            node->id = 0xfffffbad;
            free_nodes = node;
        }
    }
}

template<class C> typename I2dModArray<C>::Node* I2dModArray<C>::_new_node()
{
    Node* value = free_nodes;
    if(free_nodes != 0){
        free_nodes = free_nodes->next;
        value->id = 0;
    }else{
        
    }
    
    return value;
}



//->-------------=Установка итератора на позицию i-го элемента=----------------
template<class C> void I2dModArray<C>::begin(unsigned int i)
{
    if(i<sz)
    {
        prev_iterator = &guardian;
        iterator = guardian.next;
        while(i != 0)
        {
            prev_iterator = iterator;
            iterator = iterator->next;
            i--;
        }
    }else{
#ifdef _DEBUG
        //OutputDebugString("Warning (bool I2dModArray<C>::begin()): iterator = 0, check call this func\n");
#endif
        prev_iterator = tail;
        iterator = 0;
    }
}
//-*-------------------------------------------------------------------------*-

//->-------------=Оператор доступа только для чтения=--------------------------
template<class C> inline const C& I2dModArray<C>::operator[] (unsigned int i) const
{
    if(i<sz)
    {       
        e = guardian.next;
        for(unsigned int n=0; n<i; n++)
            e = e->next;
        return e->data;
    }else
        return def;
}

template<class C> inline const C& I2dModArray<C>::read(unsigned int id) const
{
    if(use_id){
        e = guardian.next;
        for(unsigned int n=0; n<sz; n++)
        {
            if(e->id == id)
                return e->data;
            e = e->next;
        }
    }
    
    return def;
}
//-*-------------------------------------------------------------------------*-

//->-------------=Оператор доступа для записи=---------------------------------
template<class C> inline C& I2dModArray<C>::operator() (unsigned int i)
{
    if(i<sz)
    {
        e = guardian.next;
        for(unsigned int n=0; n<i; n++)
            e = e->next;
        return e->data;
    }else
        return def;
}

template<class C> inline C& I2dModArray<C>::write(unsigned int id)
{
    if(use_id){
        e = guardian.next;
        for(unsigned int n=0; n<sz; n++)
        {
            if(e->id == id)
                return e->data;
            e = e->next;
        }
    }
#ifdef _DEBUG
    else
        //OutputDebugString("Warning (bool I2dModArray<C>::write()): use_id == false, check call this func\n");
#endif
    
    return def;
}
//-*-------------------------------------------------------------------------*-

//->-------------=Поместить элемент в конец массива=---------------------------
template<class C> bool I2dModArray<C>::push_back(const C& new_e, unsigned int id)
{
    bool b = true;
    if(use_id && sz > 0){
        e = guardian.next;
        
        for(unsigned int n=0; n<sz; n++){
            if(e->id == id){
                b = false;
                break;
            }
            
            e = e->next;
        }
    }
    
    if(b)
    {
        if(sz>0)
        {
            e = tail;
            e->next = _new_node();
            
            // out_of_memory plz    realloc more mem space
            if(e->next == 0)
                return false;
            
            e->next->prev = tail;
            
            tail = e = e->next;
            e->data = new_e;
            e->id = id;
            e->next = 0;
        }else{
            e = _new_node();
            // out_of_memory plz    realloc more mem space
            if(e == 0)
                return false;
            
            e->prev = &guardian;
            e->data = new_e;
            e->id = id;
            e->next = 0;
            guardian.next = tail = e;
        }
        sz++;
    }
    
    return b;
}
//-*-------------------------------------------------------------------------*-

//->-------------=Поместить элемент перед итератором=--------------------------
template<class C> bool I2dModArray<C>::push_before_iterator(const C& new_e, unsigned int id)
{
    bool b = true;
    if(use_id && sz > 0){
        e = guardian.next;
        
        for(unsigned int n=0; n<sz; n++){
            if(e->id == id){
                b = false;
                break;
            }
            
            e = e->next;
        }
    }
    
    if(b)
    {
        if(prev_iterator != 0)
        {
            prev_iterator->next = _new_node();
            // out_of_memory plz    realloc more mem space
            if(prev_iterator->next == 0)
                return false;
            
            prev_iterator->next->prev = prev_iterator;
            prev_iterator->next->data = new_e;
            prev_iterator->next->id = id;
            prev_iterator->next->next = iterator;
            if(iterator != 0)
                iterator->prev = prev_iterator->next;
            else
                tail = prev_iterator->next;
            
            prev_iterator = prev_iterator->next;
            sz++;
        }else
            return false;       
    }
    
    return b;
}
//-*-------------------------------------------------------------------------*-



//->-------------=Удалить элемент из массива по текущему итератору=------------
template<class C> bool I2dModArray<C>::pop_iterator()
{
    if(iterator != 0 && prev_iterator != 0){
        prev_iterator->next = iterator->next;
        if(iterator->next == 0)
            tail = prev_iterator;
        else
            iterator->next->prev = prev_iterator;
        
        _free_node(iterator);
        iterator = prev_iterator->next;
        sz--;
        
        return true;
    }else
        return false;
    /*
     e = &guardian;
     for(unsigned int n = 0; n<sz; n++)
     {
     if(e->next == iterator)
     {
     Node* buf = e->next;
     e->next = e->next->next;
     if(n == sz - 1)
     tail = e;
     delete buf;
     sz--;
     iterator = e;
     return true;
     }else{
     e = e->next;
     }
     }
     return false;*/
}
//-*-------------------------------------------------------------------------*-

//->-------------=Удалить элемент из массива по id=----------------------------
template<class C> bool I2dModArray<C>::pop_id(unsigned int id)
{
    if(use_id){
        e = &guardian;
        for(unsigned int n = 0; n<sz; n++)
        {
            if(e->next->id == id)
            {
                Node* buf = e->next;
                e->next = e->next->next;
                if(n == sz - 1)
                    tail = e;
                else
                    e->next->prev = e;
                
                _free_node(buf);
                sz--;
                return true;
            }else{
                e = e->next;
            }
        }
    }
#ifdef _DEBUG
    else
        //OutputDebugString("Warning (bool I2dModArray<C>::pop_id()): use_id == false, check call this func\n");
#endif
    return false;
}
//-*-------------------------------------------------------------------------*-

//->-------------=Удалить элемент из массива по его положению в нем=-----------
template<class C> bool I2dModArray<C>::pop_pos(unsigned int i)
{                             
    if(i<sz)
    {
        e = &guardian;
        for(unsigned int n = 0; n<i; n++)
            e = e->next;
        
        Node* buf = e->next;
        e->next = e->next->next;
        if(i == sz - 1)
            tail = e;
        else
            e->next->prev = e;
        
        _free_node(buf);
        sz--;
    }
    return (i<sz);
}
//-*-------------------------------------------------------------------------*-

//->-------------=Удалить все элементы из массива=-----------------------------
template<class C> int I2dModArray<C>::clear()
{
    e = guardian.next;
    for(unsigned int n=0; n<sz; n++)
    {
        e = e->next;
        _free_node(guardian.next);
        guardian.next = e;
    }
    guardian.next = tail = iterator = e = 0;
    sz = 0;
    
    if(pool != 0){     
        // init free_nodes
        free_nodes = pool;
        free_nodes->prev = 0;
        
        Node* next_el = pool;
        for(unsigned int i(0); i<alloc_sz - 1; i++){
            next_el++;
            free_nodes->next = next_el;
            free_nodes->id = 0xfffffbad;
            next_el->prev = free_nodes;
            free_nodes = free_nodes->next;
        }
        free_nodes->next = 0;
        free_nodes = pool;
    }
    
    return sz;
}

template<class C> int I2dModArray<C>::Release()
{
    e = guardian.next;
    for(unsigned int n=0; n<sz; n++)
    {
        e = e->next;
        _free_node(guardian.next);
        guardian.next = e;
    }
    guardian.next = tail = iterator = e = 0;
    sz = 0;
    
    if(pool != 0){
        delete[] pool;//free(pool);
        pool = 0;
    }
    free_nodes = 0;
    alloc_sz = 0;
    
    return sz;
}
//-*-------------------------------------------------------------------------*-



#endif
