#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <err.h>

int open_ro(const char *file)
{
	int filedesc = open(file, O_RDONLY);
	if(filedesc < 0)
		return 1;
	if(pledge("stdio rpath", NULL) == -1) //will work
		err(1, "pledge");

	close(filedesc);
	return 0;
}


int open_rdwr(const char *file)
{
	int filedesc = open(file, O_RDWR);
	if(filedesc < 0)
		return 1;
	if(pledge("stdio rpath", NULL) == -1) //won't work, will fail with uncatchable SIGABRT
		err(1, "pledge");

	close(filedesc);
	return 0;
}



int main()
{
	open_ro("/tmp/pledge.txt");
	open_rdwr("/tmp/pledge.txt");

	return 0;
}
