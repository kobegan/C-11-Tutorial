#include <iostream>
#include <thread>
#include <future>

void threadFunction(std::future<void> futureObj)
{
    std::cout << "Thread Start" << std::endl;
    while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
    {
        std::cout << "Doing Some Work" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    }
    std::cout << "Thread End" << std::endl;

}

// Stopping a Thread using std::future<>
// We can pass a std::future<void> object to thread and thread should exit when value in future is available. 
void stoppingThreadUsingFuture()
{
    //Create a std::promise object
    std::promise<void> exitSignal;

    //Fetch std::future object associated with promise
    std::future<void> futureObj = exitSignal.get_future();

    //Starting the thread & move the future object in lambda function by reference
    std::thread threadObj(&threadFunction, std::move(futureObj));

    //Wait for 10 sec
    std::this_thread::sleep_for(std::chrono::seconds(10));

    std::cout << "Asking Thread to Stop" << std::endl;

    //Set the value in promise
    exitSignal.set_value();

    //Wait for thread to join
    threadObj.join();

    std::cout << "Exiting Main Function" << std::endl;
}

//Creating a Stoppable Task
//Stoppable class that encapsulate the promise and future object

/*
 * Class that encapsulates promise and future object and
 * provides APT to set exit signal for the thread
 */
class Stoppable
{
    std::promise<void> exitSignal;
    std::future<void> futureObj;
public:
    Stoppable() : futureObj(exitSignal.get_future())
    {
    }
    Stoppable(Stoppable && obj) : exitSignal(std::move(obj.exitSignal)), futureObj(std::move(obj.futureObj))
    {
        std::cout << "Move Constructor is called\n";
    }
    Stoppable & operator=(Stoppable && obj)
    {
        std::cout << "Move Assignment is called\n";
        exitSignal = std::move(obj.exitSignal);
        futureObj = std::move(obj.futureObj);
        return *this;
    }
    ~Stoppable()
    {}

    //Task need to provide definition for this function
    //It will be called by thread function
    virtual void run() = 0;

    //Thread function to be executed by thread
    void operator()()
    {
        run();
    }

    //Checks if thread is requested to stop
    bool stopRequested()
    {
        //checks if value in future object is avaiable
        if (futureObj.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
        {
            return false;
        }
        return true;
    }

    //Request the thread to stop by setting value in promise object
    void stop()
    {
        exitSignal.set_value();
    }
};

//We just need to extend this class in our task class and provide definition for run() function, which will like our thread function i.e.
/*
* A Task class that extends the Stoppable Task
*/
class MyTask : public Stoppable {
public:
    // Function to be executed by thread function
    void run()
    {
        std::cout << "Task Start" << std::endl;

        // Check if thread is requested to stop ?
        while (stopRequested() == false)
        {
            std::cout << "Doing Some Work" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        }
        std::cout << "Task End" << std::endl;
    }
};

void creatingAStoppableTask()
{
    //Creating our task
    MyTask task;

    //creating a thread to execute our task
    std::thread th([&]() {
        task.run();
    });

    std::this_thread::sleep_for(std::chrono::seconds(10));

    std::cout << "Asking Task to Stop" << std::endl;
    // Stop the Task
    task.stop();

    //Waiting for thread to join
    th.join();
    std::cout << "Thread Joined" << std::endl;
    std::cout << "Exiting Main Function" << std::endl;
}


int main(int   argc,
    char *argv[])
{
    //stoppingThreadUsingFuture();
    
    creatingAStoppableTask();
    return 0;
}
