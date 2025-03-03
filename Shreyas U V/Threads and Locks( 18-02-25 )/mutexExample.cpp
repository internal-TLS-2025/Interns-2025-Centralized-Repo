#include<iostream>
#include<thread>
#include<mutex>

using namespace std;

mutex mtx;
int counter = 0;

void increaseCounter(string threadName)
{
	for(int i = 0; i < 5; i++){
		mtx.lock();
		counter++;
		cout << threadName << " increased counter to " << counter << endl;
		mtx.unlock();
		this_thread::sleep_for(chrono::milliseconds(100));
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
