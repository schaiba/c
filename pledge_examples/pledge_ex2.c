#include <stdio.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>

int main()
{
	char *filename = "/tmp/pledge.txt";//you need to have the file in place or use a member of argv[1]
	int fd = open(filename, O_RDONLY, 0);
	if(fd == -1)
	{
		perror("open");
		exit(1);
	}
	struct stat fbuf;
	if(stat(filename, &fbuf) == -1)
	{
		perror("stat");
		exit(1);
	}
	int offset = 2;//make sure you have something in your file, with zero-length files the below condition will fail & exit everytime. Exercise 1 : Why?
	if (offset < 0 || offset > fbuf.st_size-1) 
	{
		fprintf(stderr, "mmapdemo: offset must be in the range 0-%d\n", (int)fbuf.st_size-1);
		exit(1);
	}
	int pagesize = getpagesize();
	if(pledge("prot_exec", NULL) == -1) //The program will work without the call to pledge. Exercise 2 : Why? Exercise 3 : What happens if we move this block _below_ the call to mmap() ?
		err(1, "pledge");

	char *data = mmap(NULL, pagesize, PROT_READ & PROT_EXEC, MAP_PRIVATE, fd, 0); //Exercise 4 : Would you use different PROT_ flags? Why?
	if (data == (caddr_t)(-1))
       	{
		perror("mmap");
		exit(1);
	}

	printf("offset is %d\n", offset);
	printf("data at %d is %c\n", offset, data[offset]);

	return 0;
}
