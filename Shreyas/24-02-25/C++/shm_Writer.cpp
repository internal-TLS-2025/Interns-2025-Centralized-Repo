#include<fcntl.h>
#include<sys/mman.h>
#include<unistd.h>
#include<sys/stat.h>
#include<iostream>
#include<cstring>

using namespace std;

int main()
{
	const char* shm_name = "/my_shm";
	const size_t SIZE = 128;
	
	int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, SIZE);
	char* ptr = (char*)mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

	const char* message = "Hello from Shared Memory!";
	strcpy(ptr, message);
	
	cout << "Message written to shared memory." << endl;

	munmap(ptr, SIZE);
	close(shm_fd);	

	return 0;
}
