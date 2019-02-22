// 类类型转换测试

#include <iostream>

using namespace std;

class SmallInt{
public:
    SmallInt(int i = 0):val(i){std::cout << "Constructor Called." << std::endl;}
    SmallInt& operator=(const SmallInt& rhs){
        val = rhs.val;
        std::cout << "Operator= Called." << std::endl;
        return *this;
    }
    explicit operator int() const{ 
        std::cout << "Operator int() Called." <<std::endl;
        return val;
    }
private:
    std::size_t val;
};

int main(){
    SmallInt s1 = 10;
    //类类型转换为整型，由于operator int()的存在
    cout << static_cast<int>(s1) + 10 <<endl;
    return 0;
}