#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


bool ispangram(char *s) {
  // declare array and initalize it to 0
  int arr[26];
  for (int i = 0; i < 26; i++) {
    arr[i] = 0;
  }

  // iterate through the characters of the line
  while (*s) {
    // store the character in a variable
    char letter = *s;

    //check if letter is uppercase, if not, convert it to uppercase
    if (letter >= 'A' && letter <= 'Z') {
      letter = letter + 32;
    }

    // if it is a letter, turn the value of the array at the index of ascii (so 0 for a and 25 for z) to 1
    if (letter >= 'a' && letter <= 'z') {
      int ascii_value = (int) letter;
      int alphabetic_index = ascii_value - 97;
      arr[alphabetic_index] = 1;
    }
    
    s = s + 1;
  }

  // if all the values of the array are 1, return true.
  for (int i = 0; i < 26; i++) {
    if (!arr[i]) return false;
  }

  return true;
}

int main() {
  size_t len;
  ssize_t read;
  char *line = NULL;
  while ((read = getline(&line, &len, stdin)) != -1) {
    if (ispangram(line))
      printf("%s", line);
  }

  if (ferror(stdin))
    fprintf(stderr, "Error reading from stdin");

  free(line);
  fprintf(stderr, "ok\n");
}
