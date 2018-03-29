#include <iostream>
#include <memory> // We need to include this for shared_ptr
#include <functional>

void shared_ptrTest()
{
    //Creating a shared_ptr through make_shared
    std::shared_ptr<int> p1 = std::make_shared<int>();
    *p1 = 78;
    std::cout << "p1 = " << *p1 << std::endl;

    //Shows the reference count
    std::cout << "p1 reference count = " << p1.use_count() << std::endl;

    //Second shared_ptr object will also point to same pointer
    //It will make the reference count to 2
    std::shared_ptr<int> p2(p1);

    //Shows the reference count
    std::cout << "p2 reference count = " << p2.use_count() << std::endl;
    std::cout << "p1 reference count = " << p1.use_count() << std::endl;

    //Comparing smart pointers
    if (p1 == p2) {
        std::cout << "p1 and p2 are pointing to same pointer\n" << std::endl;
    }

    std::cout << "Reset p1" << std::endl;

    p1.reset();

    //Reset the shared_ptr, in this case it will not point to any Pointer internally
    //hence its reference count will become 0
    
    std::cout << "p1 Reference Count = " << p1.use_count() << std::endl;
    std::cout << "p2 Reference Count = " << p2.use_count() << std::endl;

    //Reset the shared_ptr, in this case it will point to a new Pointer internally
    //hence its reference count will become 1.

    p1.reset(new int(11));

    std::cout << "p1 Reference Count = " << p1.use_count() << std::endl;

    //Assigning nullptr will de-attach the associated pointer and make it to point null
    p1 = nullptr;

    std::cout << "p1 Reference Count = " << p1.use_count() << std::endl;

    if (!p1)
    {
        std::cout << "p1 is NULL" << std::endl;
    }
    return;
}

//shared_ptr Custom Deleter Test
struct Sample
{
    Sample()
    {
        std::cout << "CONSTRUCTOR\n";
    }
    ~Sample()
    {
        std::cout << "DESTRUCTOR\n";
    }
};

//function that calls the delete [] on received pointer
void deleter(Sample* x)
{
    std::cout << "DELETE FUNCTION CALLED\n";
    delete[] x;
}

void shared_ptrCustomDeleterTest()
{
    std::shared_ptr<Sample> p3(new Sample[12], deleter);

    std::shared_ptr<Sample> p4(new Sample[12], [](Sample * x) {
        std::cout << "LAMBDA DELETER FUNCTION CALLED\n";
        delete[] x;
    });
    return;
}

//releasing memory or resources to a pool 
struct Sample2
{
};

//Memory Pool Dummy Kind of Implementation
template<typename T>
class MemoryPool
{
public:
    T *AquireMemory()
    {
        std::cout << "AQUIRE MEMORY\n";
        return (new T());
    }

    void ReleaseMemory(T *ptr)
    {
        std::cout << "RELEASE MEMORY\n";
        delete ptr;
    }
};

void releaseingMemoryTest()
{
    std::shared_ptr<MemoryPool<Sample2> > memoryPoolPtr = std::make_shared<MemoryPool<Sample2> >();

    std::shared_ptr<Sample2> p3(memoryPoolPtr->AquireMemory(),
        std::bind(&MemoryPool<Sample2>::ReleaseMemory, memoryPoolPtr,
            std::placeholders::_1));
    return;
}

namespace share_ptrVSPointer {
    struct Sample
    {
        void dummyFunction()
        {
            std::cout << "dummyFunction" << std::endl;
        }
    };
    void shared_ptrVSPointerTest()
    {
        std::shared_ptr<share_ptrVSPointer::Sample> ptr = std::make_shared<share_ptrVSPointer::Sample>();
        (*ptr).dummyFunction(); //Will Work

        ptr->dummyFunction(); //Will Work

        // ptr[0]->dummyFunction(); // This line will not compile.
        // ptr++;  // This line will not compile.
        //ptr--;  // This line will not compile.

        std::shared_ptr<share_ptrVSPointer::Sample> ptr2(ptr);

        if (ptr == ptr2) {   //Will Work
            std::cout << "ptr and ptr2 are equal\n";
        }
        
        //When we create shared_ptr object without assigning any value then its empty.
        //Where as, without declaration raw pointer will contain garbage value
        //With shared_ptr user can check for emptiness like this
        std::shared_ptr<share_ptrVSPointer::Sample> ptr3;
        if (!ptr3)
            std::cout << "Yes, ptr3 is empty" << std::endl;
        if (ptr3 == NULL)
            std::cout << "ptr3 is empty" << std::endl;
        if (ptr3 == nullptr)
            std::cout << "ptr3 is empty" << std::endl;

        //access the internal raw pointer from shared_ptr object like this
        std::shared_ptr<share_ptrVSPointer::Sample> ptr4 = std::make_shared<share_ptrVSPointer::Sample>();
        share_ptrVSPointer::Sample *rawptr = ptr4.get();
        if (rawptr != NULL) {
            std::cout << "ptr4 is not empty\n";
        }
        return;
    }
}

namespace createShared_ptrObjectCarefully {
    //1.) Try not to use same raw pointer for creating more than one shared_ptr object 
    //    because in that case different shared_ptr objects will not get to know that 
    //    they are sharing the pointer with any other shared_ptr objects.

    //2.) Don¡¯t create shared_ptr object from memory from stack instead of heap.
    void test()
    {
        //int x = 12;
        //std::shared_ptr<int> ptr(&x);
        return;
    }
}

namespace theProblemOfCyclicReferences {
    //using weak_ptr to fix the problem
    //Weak_ptr allows sharing but not owning an object.It¡¯s object is created by a shared_ptr.
    
    //With weak_ptr object we cannot directly use operators * and -> to access the associated memory. 
    //First we have to create a shared_ptr through weak_ptr object by calling its lock() function,  
    //then only we can use it.
    void test()
    {
        std::shared_ptr<int> ptr = std::make_shared<int>(4);
        std::weak_ptr<int> weakPtr(ptr);
        std::shared_ptr<int> ptr_2 = weakPtr.lock();
        if (ptr_2)
        {
            std::cout << (*ptr_2) << std::endl;
        }
        std::cout << "Reference Count :: " << ptr_2.use_count() << std::endl;
        if (weakPtr.expired() == false)
        {
            std::cout << "Not expired yet" << std::endl;
        }
        return;
    }
}

namespace unique_ptrTutorialExamples {
    //A unique_ptr object is always the unique owner of associated raw pointer. We can not copy a unique_ptr object, its only movable.
    struct Task
    {
        int mId;
        Task(int id) :mId(id)
        {
            std::cout << "Task::Constructor" << std::endl;
        }
        ~Task()
        {
            std::cout << "Task::Destructor" << std::endl;
        }
    };

    void test()
    {
        // Create a unique_ptr object through raw pointer
        std::unique_ptr<Task> taskPtr(new Task(23));

        //Access the element through unique_ptr
        int id = taskPtr->mId;

        std::cout << id << std::endl;

        //Creating a empty unique_ptr object
        // Empty unique_ptr object
        std::unique_ptr<int> ptr1;

        //Check if a unique_ptr<> object is empty
        // Check if unique pointer object is empty
        if (!ptr1)
            std::cout << "ptr1 is empty" << std::endl;

        // Check if unique pointer object is empty
        if (ptr1 == nullptr)
            std::cout << "ptr1 is empty" << std::endl;

        return;
    }

    void transferingOwnershipTest()
    {
        std::unique_ptr<Task> taskPtr2(new Task(55));

        //Transfer the ownership
        std::unique_ptr<Task> taskPtr4 = std::move(taskPtr2);

        if (taskPtr2 == nullptr) {
            std::cout << "taskPtr2 is empty" << std::endl;
        }

        //ownership of taskptr2 is transfered to taskptr4
        if (taskPtr4 != nullptr) {
            std::cout << "taksptr4 is not empty" << std::endl;
        }

        std::cout << taskPtr4->mId << std::endl;
    }

    void releasingAssociatedRawPointerTest()
    {
        // Create a unique_ptr object through raw pointer
        std::unique_ptr<Task> taskPtr5(new Task(55));

        if (taskPtr5 != nullptr)
            std::cout << "taskPtr5 is not empty" << std::endl;

        // Release the ownership of object from raw pointer
        Task * ptr = taskPtr5.release();

        if (taskPtr5 == nullptr)
            std::cout << "taskPtr5 is empty" << std::endl;
    }
}

int main(int argc, char *argv[])
{
    shared_ptrTest();
    shared_ptrCustomDeleterTest();
    releaseingMemoryTest();
    share_ptrVSPointer::shared_ptrVSPointerTest();

    createShared_ptrObjectCarefully::test();

    theProblemOfCyclicReferences::test();

    unique_ptrTutorialExamples::test();
    unique_ptrTutorialExamples::transferingOwnershipTest();
    unique_ptrTutorialExamples::releasingAssociatedRawPointerTest();
    return 0;
}