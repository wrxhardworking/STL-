//
// Created by 28108 on 2022/12/31.
//
#include <iostream>

#ifndef VECTOR_VECTOR_H
#define VECTOR_VECTOR_H


//迭代器
template<typename Vector>

class VectorIterator{

public:

    using ValueType = typename Vector::ValueType;

    using PointerType = ValueType*;

    using ReferenceType = ValueType&;

    explicit VectorIterator(PointerType ptr):
    m_Ptr(ptr){}

    //在原基础上做改变
    VectorIterator& operator++(){
        m_Ptr++;
        return *this;
    }

    //先拷贝在赋值
    VectorIterator operator++(int){
        VectorIterator iterator = *this;
        //调用上面的++重载函数
        ++(*this);
        return iterator;
    }

    VectorIterator& operator--(){
        m_Ptr--;
        return *this;
    }

    VectorIterator operator--(int){
        VectorIterator iterator = *this;
        //调用上面的--重载函数
        --(*this);
        return iterator;
    }

    ReferenceType operator[](int index){
        return *(m_Ptr + index);
    }

    PointerType operator->(){
        return m_Ptr;
    }

    ReferenceType operator*(){
        return *m_Ptr;
    }

    bool operator==(const VectorIterator& other){
        return m_Ptr == other.m_Ptr;
    }

    bool operator!=(const VectorIterator& other){
        //调用上面重载的==
        return !(*this == other);
    }

public:

private:

    PointerType m_Ptr;

};


template<typename T>
class Vector{

public:

    using ValueType = T;

    using Iterator = VectorIterator<Vector<T>>;

public:
    Vector():size(0),capacity(0),m_data(nullptr){
        Realloc(2);
    }

    ~Vector(){
        Clear();
        ::operator delete(m_data,sizeof(T)*capacity);
    }

    void PopBack(){
        if(size>0){
            m_data[--size].~T();
        }
    }

    size_t Size() const {
        return size;
    }

    size_t Capacity() const {
        return capacity;
    }

    void Clear(){
        for(int i =0 ;i<size;++i){
            m_data[i].~T();
        }
        size = 0;
    }

    void PushBack(const T& value){
        if(size>=capacity){
            Realloc(capacity+capacity/2);
        }
        // fixme
        new (&m_data[size++]) T (value);
//        m_data[size++] = value;
    }

    void PushBack(T&& value){
        if(size>=capacity){
            Realloc(capacity+capacity/2);
        }
        //focus
        //这里必须std::move 因为即使参数是右值 但是进入函数的时候就为了左值
        //fixme
        new (&m_data[size++]) T (std::move(value));
//        m_data[size++] =std::move(value) ;
    }

    template<typename ... Args>
    T& EmplaceBack(Args && ... args){
        if(size>=capacity){
            Realloc(capacity+capacity/2);
        }
        //focus
        //在原有的内存上直接构造 减少了一次构造 placement new 进行构造
        new (&m_data[size]) T (std::forward<Args>(args) ...);
        return m_data[size++];
    }

    T& operator[](size_t index) const{
        if(index>size){
            std::cout<<"越界了"<<std::endl;
        }
        return  m_data[index];
    }

    //预留空间
    void Reserve(size_t len){
        if(len>capacity) {
            Realloc(len);
            capacity = len;
        }
    }

    Iterator begin(){
        return Iterator (m_data);
    }
    Iterator end(){
        return Iterator (m_data+size);
    }

private:

    void Realloc(size_t new_capacity){
        //分配了内存 并未构造
        T * new_data = (T*)operator new(sizeof(T) * new_capacity);
        for(int i = 0;i<size;++i)
            // focus
            //存储的对象可能存在移动语义
            //只分配了内存 没有进行构造 所以 使用placement new 进行构造 实现对象从旧内存到新内存的转移
            new (&new_data[i]) T (std::move(m_data[i]));

        for(int i = 0;i<size;++i)
            //focus
            //若是没有移动语义 调用析构函数释放对上面的内存
            m_data[i].~T();

        ::operator delete(m_data,sizeof(T)*capacity);
        capacity = new_capacity;
        m_data = new_data;
    }


private:
    size_t size;
    size_t capacity;
    T* m_data;
};


#endif //VECTOR_VECTOR_H