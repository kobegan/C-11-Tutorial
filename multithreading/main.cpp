#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <functional>
#include <string>
#include <mutex>
#include <condition_variable>
#include <future>

namespace threeDifferentWaysToCreateTheads {
    //In C++ 11 we can create additional threads by creating objects of std::thread class.
    //We can attach a callback with the std::thread object, that will be executed when this new thread starts.These callbacks can be,

    //    1.) Function Pointer
    //    2.) Function Objects
    //    3.) Lambda functions
    void thread_function()
    {
        for (int i = 0; i < 10; ++i)
            std::cout << "thread function executing" << std::endl;
    }

    void createThreadUsingFunctionPointer()
    {
        std::cout << "+++++++" << __FUNCTION__ << "+++++++\n";

        std::thread threadObj(thread_function);
        for (int i = 0; i < 10; ++i)
            std::cout << "Display from main thread\n";
        threadObj.join();
        std::cout << "Exit of Main function\n";

        std::cout << "\n";
        return;
    }

    void createThreadUsingLambdaFunctions()
    {
        std::cout << "+++++++" << __FUNCTION__ << "+++++++\n";

        int x = 9;
        std::thread threadObj([] {
            for (int i = 0; i < 10; i++)
            {
                std::cout << "Display Thread Executing\n";
            }
        });
        for (int i = 0; i < 10; i++)
        {
            std::cout << "Display From Main Thread\n";
        }
        threadObj.join();
        std::cout << "Exit of Main Thread\n";

        std::cout << "\n";
        return;
    }

    void thread_function2()
    {
        std::cout << "Inside Thread :: ID = " << std::this_thread::get_id() << std::endl;
    }

    void differentiatingBetweenThreads()
    {
        std::cout << "+++++++" << __FUNCTION__ << "+++++++\n";
        //  gives id of associated thread object, std::thread::get_id()
        //  Gives the identifier for the current thread, std::this_thread::get_id()
        std::thread threadObj1(thread_function2);
        std::thread threadObj2(thread_function2);

        if (threadObj1.get_id() != threadObj2.get_id())
        {
            std::cout << "Both Threads have different IDs\n";
        }
        std::cout << "From Main Thread :: ID of thread 1 = " << threadObj1.get_id() << std::endl;
        std::cout << "From Main Thread :: ID of thread 2 = " << threadObj2.get_id() << std::endl;

        threadObj1.join();
        threadObj2.join();
    }
}

namespace joiningAndDetachingThreads {
    //  std::thread th(funcPtr);
    //  th.join();
    class WorkerThread {
    public:
        void operator()()
        {
            std::cout << "Worker Thread " << std::this_thread::get_id() << " is Executing\n";
        }
    };

    void joiningThreads()
    {
        std::cout << "+++++++" << __FUNCTION__ << "+++++++\n";
        std::vector<std::thread> threadList;

        for (int i = 0; i < 10; ++i)
        {
            threadList.push_back(std::thread(WorkerThread()));
        }

        // Now wait for all the worker thread to finish i.e.
        // Call join() function on each of the std::thread object
        std::cout << "Wait for all the worker threads to finish\n";
        std::for_each(threadList.begin(), threadList.end(), std::mem_fn(&std::thread::join));
        std::cout << "Exiting from Main Thread\n";
    }

    void detachingThreads()
    {
        std::cout << "+++++++" << __FUNCTION__ << "+++++++\n";
        // Case 1: Never call join() or detach() on std::thread object with no associated executing thread
        // Detached threads are also called daemon / Background threads.  
        // To detach a thread we need to call std::detach() function on std::thread object i.e.
        // std::thread th(funcPtr);
        // th.detach();
        // After calling detach(), std::thread object is no longer associated with the actual thread of execution.
        // before calling join() or detach() we should check if thread is join - able every time i.e. threadObj.joinable()

        // Case 2 : Never forget to call either join or detach on a std::thread object with associated executing thread
        std::thread threadObj(WorkerThread());
        return;
    }
}

namespace passArgumentsToThreads {
    // By default all arguments are copied into the internal storage of new thread.

    void threadCallBack(int x, std::string str)
    {
        std::cout << "Passed Number = " << x << std::endl;
        std::cout << "Passed String = " << str << std::endl;
    }

    void passArgumentsbyValue()
    {
        std::cout << "+++++++" << __FUNCTION__ << "+++++++\n";
        int x = 10;
        std::string str = "Sample String";
        std::thread threadObj(threadCallBack, x, str);
        threadObj.join();
    }
    /*How not to pass arguments to threads :*/

    //Don¡¯t pass addresses of variables from local stack to thread¡¯s callback function.
    //Because it might be possible that local variable in Thread 1 goes out of scope
    //but Thread 2 is still trying to access it through it¡¯s address.

    void threadCallback(int const & x)
    {
        int &y = const_cast<int &>(x);
        y++;
        std::cout << "Inside Thread x = " << x << std::endl;
    }

    void passArgumentsByReferences()
    {
        std::cout << "+++++++ " << __FUNCTION__ << " +++++++\n";
        int x = 9;
        std::cout << "In Main Thread: Before Thread Start x = " << x << std::endl;
        std::thread threadObj(threadCallback, std::ref(x));
        threadObj.join();
        std::cout << "In Main Thread: After Thread Joins x = " << x << std::endl;
        return;
    }

    class DummyClass {
    public:
        DummyClass()
        {}
        DummyClass(const DummyClass &obj)
        {}
        void sampleMemberFunction(int x)
        {
            std::cout << "Inside sampleMemberFunction x = " << x << std::endl;
        }
    };
    void assigningPointerToMemberFunctionAsThreadFunction()
    {
        std::cout << "+++++++ " << __FUNCTION__ << " +++++++\n";
        DummyClass dummyobj;
        int x = 10;
        std::thread threadObj(&DummyClass::sampleMemberFunction, &dummyobj, x);
        threadObj.join();
        return;
    }
}

namespace dataSharingAndRaceConditions {
    // What is a Race Condition?
    // When two or more threads perform a set of operations in parallel, 
    // that access the same memory location.Also, one or more thread out of them modifies the data in that memory location, 
    // then this can lead to an unexpected results some times.
    // This is called race condition.

    class Wallet {
        int mMoney;
    public:
        Wallet() :mMoney(0) {}
        int getMoney() { return mMoney; }
        void addMoney(int money)
        {
            std::cout << __FUNCTION__ << "\n";
            for (int i = 0; i < money; ++i)
            {
                mMoney++;
            }
        }
    };

    int testMultithreadedWallet()
    {
        Wallet walletObj;
        std::vector<std::thread> threads;

        for (int i =0; i < 10; ++i)
        {
            threads.push_back(std::thread(&Wallet::addMoney, &walletObj, 1000));
        }

        for (int i = 0; i < threads.size(); ++i)
        {
            threads.at(i).join();
        }
        return walletObj.getMoney();
    }

    void practicalExampleOfRaceCondition()
    {
        std::cout << "+++++++ " << __FUNCTION__ << " +++++++\n";
        int val = testMultithreadedWallet();
        for (int k = 0; k < 100000; k++)
        {
            if (val != 10000)
            {
                std::cout << "Error at count = " << k << " Money in Wallet = " << val << std::endl;
            }
        }
        return;
    }
}

namespace usingMutexToFixRaceConditions {
    //  To fix race conditions in multi - threaded environment we need mutex 
    //  i.e.each thread needs to lock a mutex before modifying or reading the shared data and 
    //  after modifying the data each thread should unlock the mutex.
    class Wallet
    {
        int mMoney;
        std::mutex mutex;
    public:
        Wallet() :mMoney(0) {}
        int getMoney() { return mMoney; }
        void addMoney(int money)
        {
            mutex.lock();
            for (int i = 0; i < money; ++i)
            {
                mMoney++;
            }
            mutex.unlock();
        }
    };

    int testMultithreadedWallet()
    {
        Wallet walletObject;
        std::vector<std::thread> threads;
        for (int i = 0; i < 5; ++i) {
            threads.push_back(std::thread(&Wallet::addMoney, &walletObject, 1000));
        }

        for (int i = 0; i < threads.size(); i++)
        {
            threads.at(i).join();
        }
        return walletObject.getMoney();
    }

    void test()
    {
        int val = 0;
        for (int k = 0; k < 1000; k++)
        {
            if ((val = testMultithreadedWallet()) != 5000)
            {
                std::cout << "Error at count = " << k << "  Money in Wallet = " << val << std::endl;
                //break;
            }
        }
        return;
    }

    // std::lock_guard :
    // std::lock_guard is a class template, which implements the RAII for mutex.
    //    It wraps the mutex inside it¡¯s object and locks the attached mutex in its constructor.
    // When it¡¯s destructor is called it releases the mutex.

    class Wallet2
    {
        int mMoney;
        std::mutex mutex;
    public:
        Wallet2() :mMoney(0) {}
        int getMoney() { return mMoney; }
        void addMoney(int money)
        {
            std::lock_guard<std::mutex> lockGuard(mutex);
            // In constructor it locks the mutex

            for (int i = 0; i < money; ++i)
            {
                // If some exception occurs at this
                // poin then destructor of lockGuard
                // will be called due to stack unwinding.
                //
                mMoney++;
            }
            // Once function exits, then destructor
            // of lockGuard Object will be called.
            // In destructor it unlocks the mutex.
        }
    };
}


namespace eventHandling {
    // option 1 
    // Make a Boolean global variable with default value false.
    // Set its value to true in Thread 2 and Thread 1 will keep on checking its value 
    // in loop and as soon as it becomes true Thread 1 will continue with processing of data.
    class Application {
        std::mutex m_mutex;
        bool m_bDataloaded;
    public:
        Application() {
            m_bDataloaded = false;
        }
        void loadData()
        {
            //Make this thread sleep for 1 Second
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout << "Loading Data from XML\n";

            // Lock the data structure
            std::lock_guard<std::mutex> guard(m_mutex);

            //Set the flag to true, means data is loaded
            m_bDataloaded = true;
        }

        void mainTask()
        {
            std::cout << "Do something handshaking\n";

            //Acquire the lock
            m_mutex.lock();
            //Check if flag is set to true or not
            while (m_bDataloaded != true)
            {
                //Release the lock
                m_mutex.unlock();
                //sleep for 100 milli seconds
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                //Acquire the lock
                m_mutex.lock();
            }
            //Release the lock
            m_mutex.unlock();
            //Do processing on loaded data
            std::cout << "Do processing  on loaded data\n";

        }
    };

    void options1Test()
    {
        Application app;
        
        std::thread thread_1(&Application::mainTask, &app);
        std::thread thread_2(&Application::loadData, &app);

        thread_2.join();
        thread_1.join();

        // It has following disadvantages,
        // Thread will keep on acquiring the lock and release it just to check the value, 
        // therefore it will consume CPU cycles and will also make Thread 1 slow, because it needs to acquire same lock to update the bool flag.
    }

    // Option 2:

    // We can achieve this using Condition Variables.
    // Condition Variables

    // Condition Variable is a kind of Event used for signaling between two or more threads.
    // One or more thread can wait on it to get signaled, while an another thread can signal this.

    class Application2 {
        std::mutex m_mutex;
        std::condition_variable m_condVar;
        bool m_bDataloaded;
    public:
        Application2() {
            m_bDataloaded = false;
        }
        void loadData()
        {
            //Make this thread sleep for 1 Second
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout << "Loading Data from XML\n";

            // Lock the data structure
            std::lock_guard<std::mutex> guard(m_mutex);

            //Set the flag to true, means data is loaded
            m_bDataloaded = true;
            //Notify the condition variable
            m_condVar.notify_one();
        }
        
        bool isDataLoaded()
        {
            return m_bDataloaded;
        }

        void mainTask()
        {
            std::cout << "Do Some Handshaking" << std::endl;
            // Acquire the lock
            std::unique_lock<std::mutex> mlock(m_mutex);
            // Start waiting for the Condition Variable to get signaled
            // Wait() will internally release the lock and make the thread to block
            // As soon as condition variable get signaled, resume the thread and
            // again acquire the lock. Then check if condition is met or not
            // If condition is met then continue else again go in wait.
            m_condVar.wait(mlock, std::bind(&Application2::isDataLoaded, this));
            std::cout << "Do Processing On loaded Data" << std::endl;

        }
    };

    void options2Test()
    {
        Application2 app;

        std::thread thread_1(&Application2::mainTask, &app);
        std::thread thread_2(&Application2::loadData, &app);

        thread_2.join();
        thread_1.join();
    }
}


namespace futurePromiseAndReturningValue {
    // Suppose in our application we created a thread that will 
    // compress a given folder and we want this thread to 
    // return the new zip file name and its size in result.

    // 1.) Old Way : Share data among threads using pointer
    // Pass a pointer to the new thread and this thread will set the data in it. 
    // Till then in main thread keep on waiting using a condition variable. 
    // When new thread sets the data and signals the condition variable, 
    // then main thread will wake up and fetch the data from that pointer.

    // 2.) C++11 Way : Using std::future and std::promise
    // std::future is a class template and its object stores the future value.
    // Actually a std::future object internally stores a value that will be 
    // assigned in future and it also provides a mechanism to access that value i.e. 
    // using get() member function. But if somebody tries to access this associated value of 
    // future through get() function before it is available, then get() function will block till value is not available.

    // std::promise is also a class template and its object promises to set the value in future. 
    // Each std::promise object has an associated std::future object that will 
    // give the value once set by the std::promise object. 

    //step by step
    // 1. Create a std::promise object in Thread1. std::promise<int> promiseObj;
    
    // 2. But now suppose Thread 1 created this promise object and passed it to Thread 2 object. Now how Thread 1 can know that when Thread 2 is going to set the value in this promise object?

    // 3. The answer is using std::future object. Every std::promise object has an associated std::future object, through which others can fetch the value set by promise.

    // 4. So, Thread 1 will create the std::promise object and then fetch the std::future object from it before passing the std¡±¡±promise object to thread 2 i.e.
    // std::future<int> futureObj = promiseObj.get_future();

    //Then Thread 1 will fetch the value set by Thread 2 in std::promise through std::future¡¯s get function,
    //    int val = futureObj.get();

    //But if value is not yet set by thread 2 then this call will get blocked until thread 2 sets the value in promise object i.e.
    //    promiseObj.set_value(45);
    void initiazer(std::promise<int> * promObj)
    {
        std::cout << "Inside Thread" << std::endl;
        promObj->set_value(35);
    }

    void test()
    {
        std::promise<int> promiseObj;
        std::future<int> futureObj = promiseObj.get_future();
        std::thread threadObj(initiazer, &promiseObj);
        std::cout << futureObj.get() << std::endl;
        threadObj.join();
        return;
    }
}

using namespace std::chrono;
namespace asyncTutorialAndExample {
    // what is std::async()
    // std::async() is a function template that accepts a callback(i.e.function or function object) 
    // as an argument and potentially executes them asynchronously.

    // template <class Fn, class... Args>
    // future<typename result_of<Fn(Args...)>::type> async(launch policy, Fn&& fn, Args&&... args);
    
    std::string fetchDataFromDB(std::string recvdData)
    {
        // Make sure that function takes 5 seconds to complete
        std::this_thread::sleep_for(seconds(5));

        //Do stuff like creating DB Connection and fetching Data
        return "DB_" + recvdData;
    }

    std::string fetchDataFromFile(std::string recvdData)
    {
        // Make sure that function takes 5 seconds to complete
        std::this_thread::sleep_for(seconds(5));

        //Do stuff like fetching Data File
        return "File_" + recvdData;
    }

    //Calling std::async with function pointer as callback
    void test1()
    {
        // Get Start Time
        system_clock::time_point start = system_clock::now();

        std::future<std::string> resultFromDB = std::async(std::launch::async, fetchDataFromDB, "Data");

        //Fetch Data from File
        std::string fileData = fetchDataFromFile("Data");

        //Fetch Data from DB
        // Will block till data is available in future<std::string> object.
        std::string dbData = resultFromDB.get();

        // Get End Time
        auto end = system_clock::now();

        auto diff = duration_cast <std::chrono::seconds> (end - start).count();
        std::cout << "Total Time Taken = " << diff << " Seconds" << std::endl;

        //Combine The Data
        std::string data = dbData + " :: " + fileData;

        //Printing the combined Data
        std::cout << "Data = " << data << std::endl;

        return;
    }

    /*
    * Function Object
    */
    struct DataFetcher
    {
        std::string operator()(std::string recvdData)
        {
            // Make sure that function takes 5 seconds to complete
            std::this_thread::sleep_for(seconds(5));
            //Do stuff like fetching Data File
            return "File_" + recvdData;

        }
    };
    //Calling std::async with Function Object as callback
    void test2()
    {
        //Calling std::async with function object
        std::future<std::string> fileResult = std::async(DataFetcher(), "Data");
        std::cout << "function object: " << fileResult.get() << std::endl;
    }

    //Calling std::async with Lambda function as callback
    void test3()
    {
        //Calling std::async with lambda function
        std::future<std::string> resultFromDB = std::async([](std::string recvdData) {
            std::this_thread::sleep_for(seconds(5));
            //Do stuff like creating DB Connection and fetching Data
            return "DB_" + recvdData;

        }, "Data");

        std::cout << "lambda result: " << resultFromDB.get() << std::endl;
    }
}

namespace packaged_taskExampleAndTutorial {
    /************************************************************************/
    /* 
      std::packaged_task<> is a class template and represents a asynchronous task. It encapsulates,

      1. A callable entity i.e either function, lambda function or function object.
      2 .A shared state that stores the value returned or thrown exception by associated callback.

      Need of std::packaged_task<>

      Using packaged_task<> with function to create Asynchronous tasks
      std::packaged_task<> can wrap around a normal function and make it applicable to run as asynchronous function.

      When std::packaged_task<> is called in a separate thread, 
      it calls the associated callback and stores the return  
      value/exception in its internal shared state. 
      This value can be accessed in other thread or 
      main function through std::future<> object.

    */
    /************************************************************************/

    std::string getDataFromDB(std::string recvdData)
    {
        // Make sure that function takes 5 seconds to complete
        std::this_thread::sleep_for(seconds(5));

        //Do stuff like creating DB Connection and fetching Data
        return "DB_" + recvdData;
    }

    void test()
    {

        // Creating std::packaged_task<> object
        // Create a packaged_task<> that encapsulated the callback i.e. a function
        std::packaged_task<std::string(std::string)> task(getDataFromDB);

        // Fetch the future object from it,
        // Fetch the associated future<> from packaged_task<>
        std::future<std::string> result = task.get_future();

        // Passing packaged_task<> to a thread,
        // std::packaged_task<> is movable but not copy-able, so we need to move it to thread i.e
        // Pass the packaged_task to thread to run asynchronously
        std::thread th(std::move(task), "Arg");

        // Join the thread. Its blocking and returns when thread is finished.
        th.join();

        // Fetch the result of packaged_task<> i.e. value returned by getDataFromDB()
        std::string data = result.get();

        std::cout << data << std::endl;
    }
}


int main(int   argc,
    char *argv[])
{
    //threeDifferentWaysToCreateTheads::createThreadUsingFunctionPointer();
    //threeDifferentWaysToCreateTheads::createThreadUsingLambdaFunctions();
    //threeDifferentWaysToCreateTheads::differentiatingBetweenThreads();

    //joiningAndDetachingThreads::joiningThreads();
    //joiningAndDetachingThreads::detachingThreads();

    //passArgumentsToThreads::passArgumentsbyValue();
    //passArgumentsToThreads::passArgumentsByReferences();
    //passArgumentsToThreads::assigningPointerToMemberFunctionAsThreadFunction();

    //dataSharingAndRaceConditions::practicalExampleOfRaceCondition();

    // usingMutexToFixRaceConditions::test();

    //eventHandling::options1Test();
    //eventHandling::options2Test();

    //futurePromiseAndReturningValue::test();

    //asyncTutorialAndExample::test1();
    //asyncTutorialAndExample::test2();
    //asyncTutorialAndExample::test3();

    packaged_taskExampleAndTutorial::test();

    return 0;
}
