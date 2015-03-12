#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// list of 100 most common english words, from:
// http://www.oxforddictionaries.com/words/the-oec-facts-about-the-language
// except for "a" and "i", so 98 words.
char* WORDS[] = { "the", "be", "to", "of", "and", "in", "that", "have", "it", "for", "not", "on", "with", "he", "as", "you", "do", "at", "this", "but", "his", "by", "from", "they", "we", "say", "her", "she", "or", "an", "will", "my", "one", "all", "would", "there", "their", "what", "so", "up", "out", "if", "about", "who", "get", "which", "go", "me", "when", "make", "can", "like", "time", "no", "just", "him", "know", "take", "people", "into", "year", "your", "good", "some", "could", "them", "see", "other", "than", "then", "now", "look", "only", "come", "its", "over", "think", "also", "back", "after", "use", "two", "how", "our", "work", "first", "well", "way", "even", "new", "want", "because", "any", "these", "give", "day", "most", "us" };

int main() {
  
  FILE *fp;
  long fl_size;
  char* buffer;
  size_t res;
  char* byte_arr;
  int size_of_text;

  // --- Read raw contents of file
  fp = fopen("cipher.txt", "r+");
  if (fp == NULL) {
    fprintf(stderr, "Couldn't open file.\n");
    exit(1);
  }

  // determine file size
  fseek(fp, 0, SEEK_END);
  fl_size = ftell(fp);
  rewind(fp);

  // +1 to null-terminate string, which strtok() expects
  buffer = (char*)malloc(sizeof(char)*fl_size+1);
  memset(buffer, 0, sizeof(char)*fl_size+1);
  if (buffer == NULL) {
    fprintf(stderr, "Malloc error.\n");
    exit(2);
  }

  // read file
  res = fread(buffer, 1, fl_size, fp);
  if (res != fl_size) {
    fprintf(stderr, "Reading error.\n");
    exit(3);
  }

  // --- Translate file contents to array of bytes
  // can be smaller but who cares
  byte_arr = (char*)malloc(sizeof(char)*fl_size);
  memset(byte_arr, 0, sizeof(char)*fl_size);

  // read the file contents as a comma-delimited list of strings
  char* strtok_res;
  strtok_res = strtok(buffer, ",");

  // translate each string to a number and store in byte_arr
  size_of_text = 0;
  while (strtok_res != NULL) {
    byte_arr[size_of_text] = atoi(strtok_res);
    strtok_res = strtok(NULL, ",");
    size_of_text++;
  }

  // --- XOR each element, search for common English words
  // test all combinations of 3-character passwords
  int pw[3];
  int counter;

  // +1 to null-terminate string, which strstr() expects
  char* xord_arr = (char*)malloc(size_of_text+1);
  memset(xord_arr, 0, size_of_text+1);
  for (pw[0] = 'a'; pw[0] <= 'z'; pw[0]++) {
    for (pw[1] = 'a'; pw[1] <= 'z'; pw[1]++) {
      for (pw[2] = 'a'; pw[2] <= 'z'; pw[2]++) {
	// XOR each byte, alternating between the password characters
	counter = 0;
	while (counter < size_of_text) {
	  xord_arr[counter] = byte_arr[counter] ^ pw[counter % 3];
	  counter++;
	}
	
	// test against common english words:
	int word_counter = 0;
	int counts = 0;
	for (; word_counter < 98; word_counter++) {
	  if (strstr(xord_arr, WORDS[word_counter]) != NULL) {
	    counts++;
	  }
	}

	// took some experimenting to arrive at a number that gives 1 result.
	if (counts > 25) {
	  // sum up the ASCII values
	  int sum = 0;
	  int s_counter = 0;
	  while (s_counter < size_of_text) {
	    sum += xord_arr[s_counter];
	    s_counter++;
	  }
	  printf("Answer: %i\n\n", sum);
	}
      }
    }
  }

  fclose(fp);
  free(buffer);
  free(byte_arr);
  free(xord_arr);
}

