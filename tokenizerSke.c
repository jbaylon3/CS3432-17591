#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/* Return true (non-zero) if c is a whitespace characer
   ('\t' or ' ').
   Zero terminators are not printable (therefore false) */
bool delim_character(char c){
if(c == '\t' || c == ' ')
   return true;
return false;
}

/* Return true (non-zero) if c is a non-whitespace
   character (not tab or space).
   Zero terminators are not printable (therefore false) */
bool non_delim_character(char c){
if(c == '\t' || c == ' ' || c == '\0')
   return false;
return true;
}

/* Returns a pointer to the first character of the next
   space-separated word*/
char *word_start(char* str){
   while(delim_character(*str)){
      str ++;
   }
   return str;
}

/* Returns a pointer to the first space character of the zero
terminated string*/
char *end_word(char* str){
   while(non_delim_character(*str)){
      str ++;
   }
   return str;
}
// counts the number of words or tokens
int count_tokens(char* str){ 
   int count = 0;
   char* str1 = str;
   while(*str1 != '\0'){
      str1 = word_start(str1);
      //printf("%s\n", str1);
      while(*str1 != '\0'){
         str1 = end_word(str1);
         str1 = word_start(str1);
         count++;
      } 
      str1 = end_word(str1);
   }
   return count;
}
/* Returns a freshly allocated zero-terminated vector of freshly allocated
   space-separated tokens from zero-terminated str.
   For example, tokenize("hello world string") would result in:
     tokens[0] = "hello"
     tokens[1] = "world"
     tokens[2] = "string"
     tokens[3] = 0
*/
char *copy_str(char *inStr, short len){
   char* token = (char*) malloc(len*sizeof(char*));
   for(int i = 0;i < len; i++){
      token[i] = inStr[i];
   }
   return token;
}

char** tokenize(char* str){
   int size = count_tokens(str);
   char** tokens = (char**)malloc(size*sizeof(char*) + 1);
   char* start = word_start(str);
   char* end;
   short length;
   for(int i = 0; i < size; i++){
         end = end_word(start);
         length = end - start;
         tokens[i] = copy_str(start, length);
         start = word_start(end);
   }
   return tokens;
}


void print_all_tokens(char** tokens){
   char** temp = tokens;
   int word = 0;
   while(*temp != NULL){
      printf("Token[%d]: %s\n", word, *temp);
      word++;
      temp++;
   }
}

int main(){
   char string[] = "     Hello Works h uy iuyu";
   //printf("%s\n", end_word(string)); //jumps over letter
   //printf("%s\n", word_start(string));//jumps over space
   //printf("%d\n", count_tokens(string));
   printf("%s\n", string);
   char* start = word_start(string);
   char* end = end_word(start);
   short length = end - start;
   char* token = copy_str(word_start(string), length);
   //printf("%s\n", token);
   print_all_tokens(tokenize(string));
   //printf("hello wolrd");
   return 0;
}
