#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

float genfloat(float a){
	return (float)rand()/(float)(RAND_MAX) * a;
}

int main(int argc, char **argv){
	int i;
	struct timeval tv;
	if (argc != 4){
		printf("usage: %s limit x_max y_max\n", argv[0]);
		return 0;
	}

	gettimeofday(&tv, NULL);
	srand(tv.tv_sec * 1000000 + tv.tv_usec);

	for (i = 0; i < atoi(argv[1]); i++){
		printf("%f %f\n", genfloat(atoi(argv[2])), genfloat(atoi(argv[3])));
	}
	return 0;
}
