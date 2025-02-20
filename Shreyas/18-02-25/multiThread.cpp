#include<iostream>
#include<thread>
#include<chrono>

using namespace std;

void task1()
{
	for(int i = 1; i <= 5; i++){
		cout << "Task 1 is running (" << i << ")" << endl;
		this_thread::sleep_for(chrono::seconds(1));
	}
}

void task2()
{
        for(int i = 1; i <= 5; i++){
                cout << "Task 2 is running (" << i << ")" << endl;
		this_thread::sleep_for(chrono::seconds(1));
        }
}

int main()
{
	thread t1(task1);
	thread t2(task2);

	t1.join();
	t2.join();

	cout << "All tasks completed!" << endl;

	return 0;
}
