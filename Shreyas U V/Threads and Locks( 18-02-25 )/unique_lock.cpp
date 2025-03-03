#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

mutex mtx;
int counter = 0;

void increaseCounter(string threadName)
{
    for (int i = 0; i < 5; ++i) {
 	{       
       		 unique_lock<mutex> lock(mtx); 
        	 ++counter;
       		 cout << threadName << " increased counter to " << counter << endl;
	}
        this_thread::sleep_for(chrono::seconds(1));
        
     }
}

int main()
{
    thread t1(increaseCounter, "Thread 1");
    thread t2(increaseCounter, "Thread 2");

    t1.join();
    t2.join();


    cout << "Final counter value: " << counter << endl;

    return 0;
}
