//
// Created by 28108 on 2023/1/3.
//
#include <iostream>
#include <cstring>
#include "../Vector.hpp"

class String{
public:
    String() = default;
    //有参构造
    explicit String(const char * string){
        printf("created\n");
        size = strlen(string);
        data = new char [size];
        //将字符串的内容进行拷贝
        memcpy(data,string,size);
    }
    //拷贝构造
    String(const String & other){
        printf("copied\n");
        this->size = other.size;
        this->data = new char [size];
        memcpy(this->data,other.data,size);
    }

    //移动构造
    String(String && other) noexcept{
        printf("moved\n");
        this->size = other.size;
        this->data = other.data;
        other.data = nullptr;
        other.size = 0;
    }
    //移动语义
    String & operator =(String&& other) noexcept {
        printf("moved\n");
        if(this!=&other) {
            //这里要释放之前申请的资源 因为构造的时候可能已经初始化
            if(this->data== nullptr)
                delete this->data;
            this->size = other.size;
            this->data = other.data;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }
    ~String(){
        printf("destroy\n");
        delete(data);
    }
    void print(){
        for(size_t i =0;i<size;++i){
            printf("%c",this->data[i]);
        }
        printf("\n");
    }

private:
    char * data;
    size_t size;
};


int main(){
#if 0
    //1.自定义类型测试
    Vector<String> a;
    String s1 ("i");
    String s2 ("love");
    String s3 ("you");
    a.PushBack(std::move(s1));
    a.PushBack(s2);
    a.EmplaceBack("you");
    for(auto x : a){
        x.print();
    }
    for(int i = 0;i<a.Size();++i){
        a[i].print();
    }
#elif 1
    //2.标准string类型
    Vector<std::string> b;
    std::string s1 = "i";
    std::string s2 = "love";
    b.PushBack(std::move(s1));
    b.PushBack(s2);
    b.EmplaceBack("you");
    for(auto x : b){
        std::cout<<x<<" ";
    }
    std::cout<<"\n";
    for(int i = 0;i<b.Size();++i){
        std::cout<<b[i]<<" ";
    }
#endif
}