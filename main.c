#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define CHARS 1000

/*Enums and structs*/
enum mode {major, minor};
typedef enum mode mode;

enum tone {C, Csharp, D, Dsharp, E, F, Fsharp, G, Gsharp, A, Asharp, B};
typedef enum tone tone;

struct note{
  int tone;
  int octave;
  int lenght;
};
typedef struct note note;

struct song_data{
  unsigned int tempo;
  mode mode;
};
typedef struct song_data data;

/*Prototypes*/
void findNoteLength(double x, int *, int *);
void print_note(int);
int get_hex(FILE*, int[]);
void fill_song_data(data*, int[], int);
int count_notes(int[], int);
void fill_note(int, note*);

int main(int argc, const char *argv[]){
  /*Variables*/
  int high, low, j, numbers_in_text = 0, notes, i = 0;
  data data;
  FILE *f = fopen(argv[1],"r");
  int *hex = (int *) malloc(CHARS * sizeof(int));
  if(hex == NULL){
    printf("Memory allokation failed, bye!");
    exit(EXIT_FAILURE);
  }
  printf("Lee er et kæmpe fuckhoved\n");
  /*Reading the data from the file*/
  numbers_in_text = get_hex(f, hex);
  fill_song_data(&data, hex, numbers_in_text);
  notes = count_notes(hex, numbers_in_text);
  note *note_ar = (note*) malloc(notes * sizeof(note));
  
  /*Read and proces the hex array*/
  for(j = 0; j < numbers_in_text; j++){

    /* finder tonerne + oktav nr. */
    if(hex[j] == 0x90){
      fill_note(hex[1+j], &note_ar[i]);
      printf("Oktav nr: %d\n", note_ar[i].octave);
      printf("Tonen: ");
      print_note(note_ar[i].tone);
      putchar('\n');
      i++;
    }

    /* finder tonelængden */
    /*if(hex[j] == 0x90){
      findNoteLength(hex[j+3], &high, &low);
      printf("Nodelængde: %d/%d\n\n", high, low);
    }*/
  }

  /*Clean up and close*/
  fclose(f);
  free(hex);

  return 0;
}

/*A function, that retrieves the hexadecimals from the files and also returns the number of files*/
int get_hex(FILE *f, int hex_ar[]){
  int i = 0, c;
 
  while( (c = fgetc(f)) != EOF && i < CHARS){
    hex_ar[i] = c;
    i++;
  }
  
  return i;
}

int count_notes(int hex[], int ammount){
  int i = 0, res = 0;
  for(i = 0; i < ammount; i++){
    if(hex[i] == 0x90){
      res++;
    }
  }
  return res;
}

void fill_song_data(data *data, int hex[], int numbers_in_text){
  int j;
  /*Find the mode of the song, initialised as minor atm*/
  data->mode = minor;
  for(j = 0; j < numbers_in_text; j++){
    /* finder tempoet */
    if(hex[j] == 0x51){
      data->tempo =  60000000/((hex[j+2] << 16) | (hex[j+3] << 8) | (hex[j+4])); 
      printf("Tempo: %d bpm\n\n", data->tempo);
    }
  }
}


/* Vi har ikke skalleret brøkken, Fix! */
void findNoteLength (double x, int *high, int *low){
  double func = 16*((x*x)*(0.0000676318287050830)+(0.0128675448628599*x)-2.7216713227147);
  double temp = func;
  double temp2 = (int) temp;

  if (!(temp - (double) temp2 < 0.5)){
    func += 1;
  }

  printf("x: %f og func: %f\n", x, func);
  *high = (int) func;
  *low = 16;
}

void fill_note(int i_tone, note *note){
  note->tone = i_tone % 12;
  note->octave = i_tone / 12;

}

/*A function to print the note based on input*/
void print_note(int tone_number){
  tone t = (tone) tone_number;

  switch (t){
    case C     : printf("C") ; break;
    case Csharp: printf("C#"); break;
    case D     : printf("D") ; break;
    case Dsharp: printf("D#"); break;
    case E     : printf("E") ; break;
    case F     : printf("F") ; break;
    case Fsharp: printf("F#"); break;
    case G     : printf("G") ; break;
    case Gsharp: printf("G#"); break;
    case A     : printf("A") ; break;
    case Asharp: printf("A#"); break;
    case B     : printf("B") ; break;
    default    : printf("Undefined note"); break;
  }
}
