#include <stdio.h>
#include <math.h>

 

int getWholeNumInRange(int min, int max){ //keep sking user for value in between min and max 
	int n;
	
	do{
		printf("\nEnter a whole num between %d and %d (inclusive):", min, max);
		scanf("%d", &n);
	}while(n < min || n > max);

	return n;
}

float getNumInRange(int min, int max){ //keep sking user for value in between min and max
        float n;

        do{
                printf("\nEnter a num between %d and %d (inclusive):", min, max);
                scanf("%f", &n);
        }while(n < min || n > max);

        return n;
}

float getPosValue(void){
	float n;
	do{
		printf("\nEnter a real number >= 0: ");
		scanf("%f", &n);
	}while(n < 0);

	return n;
}

float calcSurfArea(float radius, float ha, float hb){
	float a = sqrt((radius*radius) - (ha*ha));
	float b = sqrt((radius*radius) - (hb*hb));
	float h = ha - hb;

}

float calcVol(float radius, float ha, float hb){

}

int main(void){
	int sphereSegs = getWholeNumInRange(2, 10);
	float ha[sphereSegs], hb[sphereSegs];
	float radius;
	float surfaceArea[sphereSegs], vol[sphereSegs];


	radius = getPosValue();
	
	for(int i = 0; i < sphereSegs; i++){
		printf("\nenter smaller h");
		hb[i] = getNumInRange(0, radius);
		printf("\nenter larger h");
		ha[i] = getNumInRange(hb[i], radius);
		
		surfaceArea[i] = calcSurfArea(radius, ha[i], hb[i]);
		vol[i] = calcVol(radius, ha[i], hb[i]);

		printf("\n=============================");
		printf("\nRadius: %f \n %f < height < %f", radius, hb[i], ha[i]);
		printf("\n=============================");		
	
	
	}
}
