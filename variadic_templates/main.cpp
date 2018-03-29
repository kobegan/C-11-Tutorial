#include <iostream>
using namespace std;


//1.1递归函数方式展开参数包
//递归终止函数
template<typename T> 
T sum(T t) 
{
    return t;
}

//展开函数
template <typename T, typename ...Types>
T sum(T first, Types... rest)
{
    return first + sum<T>(rest...);
}

template <class T> 
void printarg(T t)
{
    cout << t << endl;
}

template <class... Args>
void expand(Args... args)
{
    int arr[] = { (printarg(args), 0)... };
}


int main(void)
{
    //1.1递归函数方式展开参数包
    cout << sum(1, 2, 3, 4) << endl;

    //1.2逗号表达式展开参数包
    expand(1,2,3,4);

    return 0;
}