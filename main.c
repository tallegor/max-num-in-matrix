#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>    /* close() */
#include <fcntl.h>     /* open() and O_XXX flags */
#include <sys/stat.h>  /* S_IXXX flags */
#include <sys/types.h> /* mode_t */

#define N              4

int main (int argc, char ** argv)
{
	int fd[2];
	
	if (argc < 2)
	{
		fprintf (stderr, "Too few arguments\n");
		exit (1);
	}

	FILE *file = fopen (argv[1], "r");

	if (!file)
	{
		fprintf (stderr, "Cannot open file\n");
		exit (1);
	}	
	
	int arr[N][N];
	int max = 0;
	for (int i = 0; i < N; i++)
	{
    	for (int j = 0; j < N; j++)
		{
			fscanf (file, "%d", &arr[i][j]);
			
			if (abs (arr[i][j]) > max)
				max = arr[i][j];
		}
	}
	
	if (pipe (fd) < 0) {
        printf ("Pipe error!\n");
		return 1;
	}
	
	int p;
	if ( (p=fork ()) < 0)
	{
		printf ("Fork error!\n");
		return 1;
	}

	int lo = sizeof(int);
    if (p == 0)
    {
        close (fd[0]);
        if (write (fd[1], &max, lo) != lo)
        {
            printf ("Write error!\n");
            return 1;
        }
    }
    else
    {
        close (fd[1]);
        if ( (lo = read(fd[0], &max, lo) ) < 0)
        {
            printf ("Read error!\n");
            return 1;
        }
        if (lo)
            printf ("Max is %d\n", max);
    }
	
	fclose (file);
    return 0;
}
