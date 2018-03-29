#include <iostream>
using namespace std;


//1.1�ݹ麯����ʽչ��������
//�ݹ���ֹ����
template<typename T> 
T sum(T t) 
{
    return t;
}

//չ������
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
    //1.1�ݹ麯����ʽչ��������
    cout << sum(1, 2, 3, 4) << endl;

    //1.2���ű��ʽչ��������
    expand(1,2,3,4);

    return 0;
}