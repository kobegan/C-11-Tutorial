//#include <iostream>
//#include <array>
//void printArray(std::array<int, 10> & arr)
//{
//    // Printing array
//    for (auto & elem : arr)
//        std::cout << elem << " , ";
//    std::cout << std::endl;
//}
//int main()
//{
//    // Uninitialized array object contains elements with
//    // garbage values
//    std::array<int, 10> arr1;
//    printArray(arr1);
//    // Initialized array object
//    std::array<int, 10> arr2 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
//    printArray(arr2);
//    // First 2 values will be initialized and others will be 0.
//    std::array<int, 10> arr3 = { 34, 45 };
//    printArray(arr3);
//    std::array<int, 10> arr4;
//    // Fill all elements in array with same value
//    arr4.fill(4);
//    printArray(arr4);
//    return 0;
//}

#include <array>
#include <algorithm>
#include <iostream>

int main()
{
    // Initialized array object contains elements with
    std::array<int, 10> arr = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    // Iterate over std::array using range based for loop
    for (auto & elem : arr)
    {
        std::cout << elem << " , ";
    }
    std::cout << std::endl;
    // Iterate over std::array using for loop and operator []
    for (int i = 0; i < arr.size(); i++)
    {
        std::cout << arr[i] << " , ";
    }
    std::cout << std::endl;
    // Iterate over std::array using for Iterators
    auto it = arr.begin();
    while (it != arr.end())
    {
        std::cout << *it << " , ";
        it++;
    }
    std::cout << std::endl;
    // Iterate over std::array using for_each STL algorithm
    std::for_each(arr.begin(), arr.end(), [](const int & elem) {
        std::cout << elem << " , ";
    });
    std::cout << std::endl;
    return 0;
}