#include <iostream>
#include <vector>

namespace differenceBetweenLvalueAndRvalue {
    //  What is lvalue ?
    //  lvalue is anything whose address is accessible. It means we can take address of lvalue using & operator.  

    //  What is rvalue ?
    //  Rvalue is anything that is not lvalue. It means we cannot take address of rvalue and it also don¡¯t persist beyond the single expression.  
    //  Example 3:
    //    int x = 1;
    //  Here you can not take the address of 1 i.e.
    //    int * ptr = &(1); // Compile Error
    //  Therefore, 1 is not a lvalue and hence it 1 is rvalue.
    
    // Now, lets create a function that returns a integer i.e
    int getData()
    {
        int data = 0;
        return data;
    }
    // Here, the value retutned by getData() is temporary i.e. 
    // it will not persist beyond the expression where it is called. 
    // Therefore it will get copied. Therefore, we can not take the 
    // address of temporary value returned by getData() i.e.
    //  int * ptr = &getData(); // Compile error - Cannot take address of rvalue
    // Therefore, getData() is a rvalue and we cannot take the address of rvalue.
}

namespace isRvalueImmutable {

    //  rvalues of builtin data type is Immutable

    //  We can not modify the rvalue of builtin data type i.e.
    //  (x+7) = 7; // Compile error - Can not Modify rvalue
    // int getData();
    // Here getData() is a rvalue of buildin data type int and if we try to modify it then it will result in compile error i.e
    // getData() = 9; // Compile Error - Can not modify rvalue

    //  rvalue of User Defined data type is not Immutable

    // Create a User Defined Data Type i.e. a class of Person i.e.
    class Person {
        int mAge;
    public:
        Person() {
            mAge = 10;
        }
        void incrementAge()
        {
            mAge = mAge + 1;
        }
    };
    // Now create a function that will return Person¡¯s object as a rvalue i.e.
    Person getPerson()
    {
        return Person();
    }
    // Now getPerson() is a rvalue and we can not take its address i.e.
    // Person * personPtr = &getPerson(); // COMPILE ERROR
    // But we can modify this rvalue because it is of User Defined Data type(of Person class) i.e.
    void test()
    {
        getPerson().incrementAge();
    }
}

namespace rvalueReference {
    // lvalue references
    int x = 7;
    int & refX = x; // refX is a reference
    // With C++11 this reference has become lvalue reference and it can refer to lvalues only i.e.
    int & lvalueRef = x; // lvalueRef is a lvalue reference
    // Here, lvalueRef is a lvalue reference pointing to a lvalue x. A lvalue reference can not point to a rvalue i.e.
    // int & lvalueRef2 = (x + 1); // COMPILE Error - lvalue Reference Can't point to rvalue

    // rvalue Reference
    // Declaring rvalue reference
    int && rvalueRef = (x + 1); // rvalueRef is rvalue reference
    // Here, rvalueRef is rvalue reference which is pointing to a rvalue i.e. (x+1).

    int getData()
    {
        return 9;
    }
    // int & lvalueRef3 = getData(); // Compile error - lvalue Reference Can't point to rvalue
    // Although const lvalue reference can refer to temporary object returned by getData() but as its a const reference, so we can not modify this temporary.
    const int & lvalueRef3 = getData(); // OK but its const
    // But a rvalue reference can refer to rvalue without const i.e.
    int && rvalueRef2 = getData();
}

namespace moveConstructorAndRvalueReference {
    
    //  Problem of Temporary Objects
    //  The Idea behind this move semantics is to reduce the load of these temporary objects on memory. 
    //  Every time we return a object from a function then a temporary object is created, 
    //  which eventually get copied. In then end we create 2 copies of an object whereas, 
    //  we need only one. Let¡¯s understand by an example,
    class Container {
        int * m_Data;
    public:
        Container() {
            //Allocate an array of 20 int on heap
            m_Data = new int[20];

            std::cout << "Constructor: Allocation 20 int" << std::endl;
        }
        ~Container() {
            if (m_Data) {
                delete[] m_Data;
                m_Data = NULL;
            }
        }
        Container(const Container & obj) {
            //Allocate an array of 20 int on heap
            m_Data = new int[20];

            //Copy the data from passed object
            for (int i = 0; i < 20; i++)
                m_Data[i] = obj.m_Data[i];

            std::cout << "Copy Constructor: Allocation 20 int" << std::endl;
        }

        // Move Constructor
        Container(Container && obj)
        {
            // Just copy the pointer
            m_Data = obj.m_Data;

            // Set the passed object's member to NULL
            obj.m_Data = NULL;

            std::cout << "Move Constructor" << std::endl;
        }

        // Move Assignment Operator
        Container& operator=(Container && obj)
        {
            if (this != &obj)
            {
                // Just copy the pointer
                m_Data = obj.m_Data;

                // Set the passed object's member to NULL
                obj.m_Data = NULL;

                std::cout << "Move Assignment Operator" << std::endl;
            }

            return *this;
        }
    };

    // Create am object of Container and return
    Container* getContainer()
    {
        Container* obj = new Container();
        return obj;
    }
    // Now in main function we created a vector of Container type and inserted an object returned by getContainer() function i.e.
    void test()
    {
        // Create a vector of Container Type
        std::vector<Container*> vecOfContainers;

        //Add object returned by function into the vector
        vecOfContainers.push_back(getContainer());

        Container* obj;
        obj = getContainer();

        return;
    }
    // Now, at last there is one object in the vector  vecOfContainers.But we actually created 2 objects for it 
    // because getContainer() function returned a temporary object which got copied into a new object 
    // and then destructed.This 2nd object got inserted in vector.So, 2 objects of class Container are created at following step in above code,

    //    One inside the getContainer() function using Container class¡¯s default constructor
    //    Second while adding object in vector using Container class¡¯ copy constructor.

    // Solving Problem of Temporary Objects using rvalue references & Move Constructor

    // The getContainer() function here is a rvalue, so it can be referred by a rvalue reference. 
    // Also, using rvalue reference we can also overload functions. This time, 
    // we will overload the Constructor of class Container and this new Constructor will be called move constructor.

    // Move Constructor
    // Move constructor takes a rvalue reference as an argument

    // Let¡¯s see the move constructor for class Container above i.e.

    // Similar to Move Constructor we can have Move Assignment operator that will just shift the content. Checkout the complete example as follows,


}

int main()
{
    // isRvalueImmutable::test();

    moveConstructorAndRvalueReference::test();
    return 0;
}