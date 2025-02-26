#include<fcntl.h>
#include<unistd.h>
#include<iostream>
#include<cstring>

using namespace std;

int main()
{
	const char* fifoPath = "my_fifo";
	int fd = open(fifoPath, O_WRONLY);

	const char* message = "Hello from Writer!";
	write(fd, message, strlen(message) + 1);
	close(fd);

	return 0;
}
