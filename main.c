#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void findNoteLength(double x, int *, int *);

int main(int argc, const char *argv[]){
	unsigned int c;
	int high, low;

	char toner[12][3] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
	FILE *f = fopen(argv[1],"r");
	int i=0;
	int *hex = (int *) malloc(1000*sizeof(int));

	while( (c = fgetc(f)) != EOF){
		hex[i++] = c;
	}

	for(int j = 0; j<10000; j++){
			
			/* finder tempoet */
			if(hex[j] == 0x51){
				unsigned int tempo = (hex[j+2] << 16) | (hex[j+3] << 8) | (hex[j+4]); 
				printf("Tempo: %d bpm\n\n", 60000000/tempo);
			}

			/* finder tonerne + oktav nr. */
			if(hex[j] == 0x90){
				int tone = hex[1+j];
		 		printf("Oktav nr: %d\n", tone/12);
		 		printf("Tonen: %s\n", toner[tone%12]);	
			}

			/* finder tonelængden */
			if(hex[j] == 0x90){
				findNoteLength(hex[j+3], &high, &low);
				printf("Nodelængde: %d/%d\n\n", high, low);
			}
	}

	fclose(f);
	free(hex);

	return 0;
}


/* Vi har ikke skalleret brøkken, Fix! */
void findNoteLength (double x, int *high, int *low){
	double func = 16*((x*x)*(0.0000676318287050830)+(0.0128675448628599*x)-2.7216713227147);
	double temp = func;
	double temp2 = (int) temp;

	if (!(temp - (double) temp2 < 0.5)){
		func += 1;
	}	

	printf("x: %lf og func: %lf\n", x, func);
	*high = (int) func;
	*low = 16;
}
