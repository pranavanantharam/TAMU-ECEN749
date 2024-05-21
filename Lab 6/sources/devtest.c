#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define DEVICE_FILE		"/dev/multiplier"
#define INPUT_LEN		8
#define RESULT_LEN		12

int main()
{
	unsigned int read_i, read_j, result;
	int fd;					/* file descriptor */
	int i,j;				/* loop variables */
	int buffer[3] = {0};
	char input = 0;
	
	fd = open(DEVICE_FILE, O_RDWR);
	
	if( fd == -1 )
	{
		printf("Failed to open device file! \n");
		return -1;
	}
	
	for( i = 0; i <= 16; i++)
	{
		for( j = 0; j <= 16; j++ )
		{
			/* Write values to registers using char dev */
			if( input != 'q' ) 	/* Continue unless user entered 'q' */
			{
				/* Use write to write i and j to peripheral */
				buffer[0] = i;
				buffer[1] = j;

				write(fd, (char*)&buffer, INPUT_LEN);
				
				/* Read i, j and result using char dev */
				
				/* Use read to read from peripheral */
				read(fd, (char*)&buffer, RESULT_LEN);
				
				read_i = buffer[0];
				read_j = buffer[1];
				result = buffer[2];
				
				/* Print unsigned ints to screen */
				printf("%u * %u = %u \n", read_i, read_j, result);
				
				/* Validate result */
				if( result == (i*j) )
				{
					printf("Result Correct! \n");
				}
				else
				{
					printf("Result Incorrect! \n");
				}
				
				/* Read from terminal */
				input = getchar();
			}
		}
	}
	
	close(fd);
	return 0;
}
