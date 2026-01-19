#include <stdio.h>
#include <math.h>


int getNumInRange(int min, int max){ //keep sking user for value in between min and max 
	int n;
	
	do{
		printf("Enter a num between %d and %d (inclusive):", min, max);
		scanf("%d", &n);
	}while(n < min || n > max);

	return n;
}


int main(void){
	int sphereSegments = getNumInRange(2, 10);
}
