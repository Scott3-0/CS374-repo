#include <stdio.h>
#include <stdlib.h>

struct Movie {
    char* title;
    int year;
    char** langs;
    float rating;
    struct Movie* next;
};

struct Movie* create_movie(char* title, int year, char** langs, float rating) {
    struct Movie* movie = malloc(sizeof(struct Movie));
    movie->title = title;
    movie->year = year;
    movie->langs = langs;
    movie->rating = rating;
    movie->next = NULL;
    return movie;
}

void readMovieFile(char* filePath){
	char *currLine = NULL;
	size_t len = 0;
	FILE *movieFile = fopen(filePath, "r");

	while(getline(&currLine, &len, movieFile) != -1){
		printf("\n%s", currLine);
	}

	free(currLine);
	fclose(movieFile);
	printf("Succesfull Process");
}

int main(int argc, char **argv){
	if(argc < 2){
		printf("PROVIDE VALID FILEPATH");
		return EXIT_FAILURE;
	}
	readMovieFile(argv[1]);
	return EXIT_SUCCESS;
}
