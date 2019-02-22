#include <iostream>

// class Base{
// public:
//     int mem_bpub = 1;
// private:
//     int mem_bpri = 2;
// protected:
//     int mem_bpro = 3;
// };

// class Derived1: private Base{
//     friend int add(Base& base, Derived1& derived);
// public:
//     int mem_dpub = 4;

//     int sum(){ return (mem_bpro + mem_bpub);}
// private:
//     int mem_dpri = 5;
// protected:
//     int mem_dpro = 6;
// };

// int add(Base& base, Derived1& derived){
//     return (base.mem_bpro + derived.mem_dpri);
// }

// int main(){
//     Derived1 ObjDerived;
//     //Base  ObjBase = ObjDerived;
//     //std::cout << ObjBase.mem_bpub << std::endl;
//     return 0;
// }
// 
class Base {
public:
    Base() = default;
    Base(const Base&) = default;
    Base(Base&&) = default;
    Base& operator=(const Base&) = default;
    Base& operator=(Base&&) = default;
    virtual ~Base() = default; //基类使用了虚析构函数，需要显式定义移动操作
private:
    int mem = 45;
};

int main(){
    Base base;
    Base base1(std::move(base));
    return 0;
}