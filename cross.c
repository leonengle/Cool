/*Make a program that prints all the words that appear in each document and the line numbers that they appear in
omitting and, the, etc. */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct Node node;

struct Node {
	char* word;
	long int* line;
	node* child;
	long int* pointer;
} *root;

int inList(char *wordNow) {
	const char* filler[] = {"the", "be", "and", "a", "of", "to", "in", "I", "you", "it"};
	for (int i = 0; i < sizeof(filler)/sizeof(filler[0]); i++)
		if (!strcmp(filler[i], wordNow)) //if strings match
			return 1;
	return 0;
}

char *getWord(FILE *fp) { //returns char string of next word in file
	char c;
	int number = 0;
	char *wordNow;
	wordNow = malloc(sizeof(char)*50); //allocates memory to store the next word
	while((c = fgetc(fp)) != EOF) {
		if (isspace(c) && wordNow[0] == '\0') { //if word hasn't been recorded yet, keep recording
			continue;
		}
		else if (isspace(c)) { //if end of word stop recording
			break;
		}
		else if (isupper(c)) //if uppercase
			c += 32; //convert to lowercase
		else
			;
		if (isalpha(c)) { //if alphabetical, save to string
			wordNow[number] = c; 
			number++; //increment index
		}
	}
	return wordNow; //return word string
	
}

node *addNode(node *root, char *wordNow, long int location) {
	if(root == NULL) { //if node has not yet been created
		root = (node *)malloc(sizeof(node *)); //create space for node
		root->word = malloc(sizeof(char) * 50); //create space for word
		strcpy(root->word, wordNow); //assign word value
		root->line = (long int *)malloc(sizeof(long int) * 50); 
		root->pointer = root->line; 
		*root->line = location;
		root->child = NULL; //set child to null
	}
	else if (!strcmp(root->word,wordNow)) { //word matches current node
		root->line++;
		*root->line = location;
	}
	else { //word does not match current node
		root->child = addNode(root->child, wordNow, location);
	}
	return root;
}

int printNodes(node *root) {
	if(root != NULL) {
		printf("Word: \"%s\". Occurences:", root->word);
		printf(" %ld", *root->line);
		while((root->line)-- != root->pointer)
			printf(" %ld", *root->line);
		printf("\n");
		printNodes(root->child);
	}
	return 0;
}

int main(int argc, char *argv[]) { //takes system argument for filename
	if (argc != 2) {
        printf("Usage: %s arg1 arg2\n", argv[0]);
        return 1;  // Return a non-zero value to indicate an error
    }
	char *arg1 = argv[1];
	FILE * fp;
	fp = fopen(arg1, "r"); //open file with name of sys arg
	if (fp == NULL) {
        perror("Error opening file");
        exit(1); // Exit with an error code
    }
	root = NULL;
	int line = 0;
	char *wordNow = (char*)malloc(sizeof(char *));
	while(!feof(fp)) { //while not end of file
		strcpy(wordNow, getWord(fp));
		if (!inList(wordNow)) //if word is not in list
			root = addNode(root, wordNow, ftell(fp)); //set root equal to node
	}
	printf("%s", printNodes(root)?"Print Failure\n":"Printed Successfully\n");
	
}