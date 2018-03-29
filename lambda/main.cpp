//Lambda function example is as follows,
//
//
//[](int x) {
//    std::cout << x << " ";
//}
//Here,
//
//[] is used to pass the outer scope elements
//(int x) shows argument x is passed
/*
 *How to pass outer scope elements inside lambda functions
 *
 *Case 1: Using [=]
 *[=](int &x) {
 *       // All outer scope elements has been passed by value
 *  }
 *
 *Case 2: Using [&]
 *[&](int &x) {
 *        // All outer scope elements has been passed by reference
 *  }
 *
 *
 *
 *
 *
 *
 *
 */

#include <iostream>
#include <algorithm>
#include <string>

void captureLocalVariables() {
/************************************************************************/
/* A simple Lambda syntax is,


[Captured variables](paameters) { function code }
1
[Captured variables](paameters) { function code }
Local variables from outer scope can be captured inside Lambda in 2 modes i.e.

By Value
By Reference                                                                     */
/************************************************************************/    
    std::string msg = "Hello";
    int counter = 10;

    // Defining Lambda function and
    // Capturing Local variables by Value
    auto func = [msg, counter]() mutable {
        std::cout << "Inside Lambda :: msg = " << msg << std::endl;
        std::cout << "Inside Lambda :: counter = " << counter << std::endl;

        // Change the counter & msg
        // Change will not affect the outer variable because counter variable is
        // captured by value in Lambda function
        msg = "Temp";
        counter = 20;

        std::cout << "Inside Lambda :: After changing :: msg = " << msg << std::endl;
        std::cout << "Inside Lambda :: After changing :: counter = " << counter << std::endl;

    };

    //Call the Lambda function
    func();

    //Values of local variables are not changed.
    std::cout << "msg = " << msg << std::endl;
    std::cout << "counter = " << counter << std::endl;
}

void captureMemberVariables() {
    /************************************************************************/
    /* Capturing Member variables inside Lambda Function
       To capture the member variables inside lambda function, capture the ¡°this¡± pointer by value i.e.
       std::for_each(vec.begin(), vec.end(), [this](int element){
       //....
       }
    */
    /************************************************************************/
}

int main() {
    int arr[] = { 1, 2, 3, 4, 5 };

    int mul = 5;

    std::for_each(arr, arr + sizeof(arr) / sizeof(int), [&](int x) {
        std::cout << x << " ";
        // Can modify the mul inside this lambda function because
        // all outer scope elements has write access here.
        mul = 3;
    });
    std::cout << std::endl;

    std::for_each(arr, arr + sizeof(arr) / sizeof(int), [=](int &x) {
        x = x*mul;
        // Can not modify the mul inside this lambda function because
        // all outer scope elements has read only access here.
        // mul = 9;

    });
    std::cout << std::endl;

    std::for_each(arr, arr + sizeof(arr) / sizeof(int), [](int x) {

        // No access to mul inside this lambda function because
        // all outer scope elements are not visible here.
        //std::cout<<mul<<" ";
        std::cout<<x<<" ";
    });


    captureLocalVariables();

    std::cout << std::endl;

}