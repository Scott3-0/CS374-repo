#include <stdio.h>
#include <math.h>

 
//This function will keep asking the user for a value in between the min and max
//and stores it as an int
int getWholeNumInRange(int min, int max){ 
	int n;
	
	do{
		printf("\nEnter a whole num between %d and %d (inclusive):", min, max);
		scanf("%d", &n);
	}while(n < min || n > max);

	return n;
}


//Same as getWholeNumInRange(); except it outputs a float allowing for decimal inputs 
//between min and max
float getNumInRange(int min, int max){
        float n;

        do{
                printf("\nEnter a num between %d and %d (inclusive):", min, max);
                scanf("%f", &n);
        }while(n < min || n > max);

        return n;
}

//Instead of checking for a range between min and max, we keep asking user
//for a positive real value (Only really used for radius input)
float getPosValue(void){
	float n;
	do{
		printf("\nEnter a real number >= 0: ");
		scanf("%f", &n);
	}while(n < 0);

	return n;
}

//Calculates surface area between two heights
//NOTE: has to be that ha > hb!!
float calcSurfArea(float radius, float ha, float hb){
	float a = sqrt((radius*radius) - (ha*ha)); 	//radius of top height
	float b = sqrt((radius*radius) - (hb*hb)); 	//radius of btm height
	float h = ha - hb; 				//height diff.

	float areaTop = M_PI*a*a;			//area of top 
	float areaBtm = M_PI*b*b;			//area of bottom
	float areaLat = 2*M_PI*radius*h;		//area on sides of sphere

	return (areaTop + areaBtm + areaLat);		//return sum
}

//Calculates volume between two heights
//NOTE: has to be that ha > hb!!
float calcVol(float radius, float ha, float hb){
	float a = sqrt((radius*radius) - (ha*ha));	//radius of top height
        float b = sqrt((radius*radius) - (hb*hb));	//radius of btm height
        float h = ha - hb; 				//height diff.
	
	float z = 3*a*a + 3*b*b + h*h; 			//internal function var to simplify return line
	return ((M_PI*h*z) /6);				//return volume from h, z, and PI constant
}

/* ===========================================
 *	Scott Bremmer
 *	ID: 934-079-543
 *
 *	Programming Assignment 1
 *	CS374: OS1
 *	eCampus, Winter Term 2026
 *	DUE Jan 18th, 2026
 *	
 *
 *	Desc:
 *	This program takes in a radius, number of segments, and segment heights
 *	and calculates the surface area and radius for those segments.
 *	It additionally returns the average surface area and volume for all segments.
 *	
 *	Limits:
 *	Radius >= 0
 *	2 <= Sphere Segment Count <= 10
 *	ha > hb
 * =========================================== */
int main(void){
	int sphereSegs = getWholeNumInRange(2, 10); 		//number of sphere segs we are calculating for (i.e. array size)
	float ha[sphereSegs], hb[sphereSegs];			//larger height and smaller height (ha > hb)
	float radius;						//radius 
	float surfaceArea[sphereSegs], volume[sphereSegs]; 	//SA and Vol for each seg
	float avgSA = 0;					// average surface area
       	float avgVol = 0;					// average volume

	radius = getPosValue();
	
	//CLI input control loop
	for(int i = 0; i < sphereSegs; i++){
		printf("\nenter smaller h");
		hb[i] = getNumInRange(0, radius);
		printf("\nenter larger h");
		ha[i] = getNumInRange(hb[i], radius);
		
		surfaceArea[i] = calcSurfArea(radius, ha[i], hb[i]);
		volume[i] = calcVol(radius, ha[i], hb[i]);
		
		//add calculated vals to averages
		avgSA += surfaceArea[i]; 
		avgVol += volume[i];

		printf("\n=============================");
		printf("\nRadius: %.2f \n %.2f < height < %.2f", radius, hb[i], ha[i]);
		printf("\n=============================");
	}
	//calculate averages by dividing sums by number of sphere segments
	avgSA /= sphereSegs;
	avgVol /= sphereSegs;

	printf("\n=============================");
	printf("\n\t\tVolume\tSurface Area");
	//CLI output control loop
	for(int i = 0; i < sphereSegs; i++){
		printf("\nSegment %d:\t| %.3f\t\t| %.3f", i+1, volume[i], surfaceArea[i]);	
	}
	printf("\n=============================");

	printf("\nAvg Volume: %.3f\nAvg Surface Area: %.3f", avgVol, avgSA);
}
