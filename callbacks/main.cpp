#include <iostream>
#include <string>

namespace functionPointers {
    // Now what is a callback?

    // Callback is a function that we pass to another APIs as argument while calling them.
    // Callbacks in C++ can be of 3 types,
    // 1.) Function Pointer
    // 2.) Function Objects / Functors
    // 3.) Lambda functions

    // Using Function Pointer as callback:

    std::string buildCompleteMessage(std::string rawData, std::string(*encrypterFunPtr)(std::string))
    {
        // Add some header and footer to data to make it complete message
        rawData = "[HEADER]" + rawData + "[FooTER]";

        // Call the callBack provided i.e. function pointer to encrypt the
        rawData = encrypterFunPtr(rawData);

        return rawData;
    }
    // In this case this a function Pointer is a Callback,
    //This encrypt function increment all letters in string by 1.
    std::string encryptDataByLetterInc(std::string data)
    {
        for (int i = 0; i < data.size(); i++)
        {
            if ((data[i] >= 'a' && data[i] <= 'z') || (data[i] >= 'A' && data[i] <= 'Z'))
                data[i]++;
        }
        return data;
    }

    // Now lets call build message API will this function as callback,
    void test()
    {
        std::string msg = buildCompleteMessage("SampleString", &encryptDataByLetterInc);
        std::cout << msg << std::endl;
    }
}

namespace functionObjectsAndFunctors {
    
    // What is a Function Objects:

    // A Function Object / Functor is a kind of Callback with State.
    // In a Programmer's terminology,
    // Object of a class which has overloaded operator() is called Function Object or Functor 
    // i.e. a class with overloaded operator() function is as follows,
    class MyFunctor
    {
    public:
        int operator()(int a, int b)
        {
            return a + b;
        }
    };
    // Now lets create a function object and call them,
    void test1() 
    {
        // Function objects / Functors can be called just like normal functions i.e.
        MyFunctor funObj;
        std::cout << funObj(2, 3) << std::endl;
    }

    std::string buildCompleteMessage(std::string rawData,
        std::string(*encrypterFunPtr)(std::string)) {
        // Add some header and footer to data to make it complete message
        rawData = "[HEADER]" + rawData + "[FooTER]";

        // Call the callBack provided i.e. function pointer to encrypt the
        rawData = encrypterFunPtr(rawData);

        return rawData;
    }

    // No suppose in our application we want to call this framework API three times with 3 different types of encryption logics i.e.

    // Encrypt by incrementing each letter by 1.
    // Encrypt by incrementing each letter by 2.
    // Encrypt by decrementing each letter by 1.
    // Is there any way to bind state with function pointers ?

    // Answers is yes ¨C  By making the them function objects or functors.

    // Let¡¯s create a Function Object / Functor for Encryption:
    class Encryptor {
        bool m_isIncremental;
        int m_count;
    public:
        Encryptor() {
            m_isIncremental = 0;
            m_count = 1;
        }
        Encryptor(bool isInc, int count) {
            m_isIncremental = isInc;
            m_count = count;
        }
        std::string operator()(std::string data) {
            for (int i = 0; i < data.size(); i++)
                if ((data[i] >= 'a' && data[i] <= 'z')
                    || (data[i] >= 'A' && data[i] <= 'Z'))
                    if (m_isIncremental)
                        data[i] = data[i] + m_count;
                    else
                        data[i] = data[i] - m_count;
            return data;
        }

    };

    //These function objects can be called just like functions in message builder API.
    std::string buildCompleteMessage(std::string rawData,
        Encryptor encyptorFuncObj) {
        // Add some header and footer to data to make it complete message
        rawData = "[HEADER]" + rawData + "[FooTER]";

        // Call the callBack provided i.e. function pointer to encrypt the
        rawData = encyptorFuncObj(rawData);

        return rawData;
    }

    // Now use this function object two solve our problem. Create a function object that can encrypt 
    // by decementing / increment each letter by specified number of times.
    void test2()
    {
        std::string msg = buildCompleteMessage("SampleString", Encryptor(true, 1));
        std::cout << msg << std::endl;

        msg = buildCompleteMessage("SampleString", Encryptor(true, 2));
        std::cout << msg << std::endl;

        msg = buildCompleteMessage("SampleString", Encryptor(false, 1));
        std::cout << msg << std::endl;

        return;
    }
}

int main()
{
    functionPointers::test();

    functionObjectsAndFunctors::test2();
    return 0;
}