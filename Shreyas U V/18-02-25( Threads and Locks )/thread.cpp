#include<iostream>
#include<thread>
#include<chrono>
using namespace std;

void printMessage(string message, int count)
{
	for(int i = 1; i <= count; i++){
		cout << "Thread: " << message << "(" << i << ")\n";
		this_thread::sleep_for(chrono::seconds(1));
	}
}

int main()
{
	thread th(printMessage, "Hello from thread!", 5);
	
	for(int i = 1; i <= 5; i++){
		cout << "Main: Running main thread (" << i << ")\n";	
		this_thread::sleep_for(chrono::seconds(1));
	}

	th.join();

	cout << "Main: Thread execution finished!\n";

	return 0;
}
