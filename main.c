/* MIDI analysing mega device of epic proportions
 * Gruppe A412
 * Lee Paludan
 * Simon Madsen
 * Jonas Stolberg
 * Jacob Mortensen
 * Esben Kirkegaard
 * Arne Rasmussen
 * Tobias Morell
*/

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
void print_note(note);
int get_hex(FILE*, int[]);
void fill_song_data(data*, int[], int);
int count_notes(int[], int);
void fill_note(int, note*);
void printSongData(data);

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
  
  /*Reading the data from the file*/
  numbers_in_text = get_hex(f, hex);
  fill_song_data(&data, hex, numbers_in_text);
  notes = count_notes(hex, numbers_in_text);
  note *note_ar = (note*) malloc(notes * sizeof(note));
  if(note_ar == NULL){
    printf("Memory allocation failed, bye!");
    exit(EXIT_FAILURE);
  }
  // Variables
  int note = 0x01, eventType = 0x01, counter = 0;

  /*Read and proces the hex array*/
  for(j = 0; j < numbers_in_text; j++){
    // Hops over any noto-on, note-off or metaevent start
    // Also stores the tones read after a note-on
    if(hex[j] == 0x00 && (hex[j + 1] == 0x90 || hex[j + 1] == 0xff)){
      counter = 1;
      j += 3;
      if(hex[j - 2] == 0x90){
        note = hex[j - 1];
        fill_note(hex[j - 1], &note_ar[i]);
        i++;
      }
      else{
        eventType = hex[j - 1];
      }
    }
    else if(hex[j] == 0x80 && hex[j + 1] == note){
      j += 2;
      note = 0x01;
      counter = 0;
    }
    if(counter){
      // Here you can check for parameters inside a meta-event or MIDI-event
    }
    else{
      // Here you can check for parameters outside a meta-event or MIDI-event
      // e.g. between a note-off and the next MIDI-event or a meta-event
    }
  }
  
  for(i = 0; i < notes; i++)
    print_note(note_ar[i]);
  printSongData(data);

  /*Clean up and close*/
  fclose(f);
  free(hex);
  free(note_ar);

  return 0;
}

/**A function, that retrieves the hexadecimals from the files and also returns the number of files
 *


 */
int get_hex(FILE *f, int hex_ar[]){
  int i = 0, c;
 
  while( (c = fgetc(f)) != EOF && i < CHARS){
    hex_ar[i] = c;
    i++;
  }
  
  return i;
}

/*A function to count the number of notes in the entire song*/
int count_notes(int hex[], int ammount){
  int i = 0, res = 0;
  for(i = 0; i < ammount; i++){
    if(hex[i] == 0x90){
      res++;
    }
  }
  return res;
}

/*A function, that fills out the song data*/
void fill_song_data(data *data, int hex[], int numbers_in_text){
  int j;
  /*Find the mode of the song, initialised as minor atm*/
  data->mode = minor;
  for(j = 0; j < numbers_in_text; j++){
    /* finds the tempo */
    if(hex[j] == 0x51){
      data->tempo =  60000000/((hex[j+2] << 16) | (hex[j+3] << 8) | (hex[j+4]));
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

/*Fills the note with calculations from the hex-array*/
void fill_note(int i_tone, note *note){
  note->tone = i_tone % 12;
  note->octave = i_tone / 12;
}

/*A function to print the note based on input*/
void print_note(note note){
  printf("Tone: ");

  switch (note.tone){
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
  printf(", octave: %d\n", note.octave);
}

void printSongData(data data){
  printf("Tempo: %d\nMode: ", data.tempo);
  switch(data.mode){
    case minor: printf("minor"); break;
    case major: printf("major"); break;
    default: printf("unknown mode"); break;
  }
  putchar('\n');
}
