#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 64
#define TOKEN_DELIMITER " \t\r\n\a"

char* read_line(void) {
	char* line = NULL;
	size_t bufsize = 0;

	if(getline(&line, &bufsize, stdin) == -1) {
		if(feof(stdin)) exit(EXIT_SUCCESS);
		perror("readline");
		exit(EXIT_FAILURE);
	}

	return line;
}

char** split_line(char* line) {
	int bufsize = BUFFER_SIZE;
	int position = 0;
	char** tokens = malloc(sizeof(char*) * bufsize);
	char* token;

	if(!tokens) {
		fprintf(stderr, "malloc error in split_line\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, TOKEN_DELIMITER);
	while(token != NULL) {
		tokens[position++] = token;
		if(position >= bufsize) {
			bufsize += BUFFER_SIZE;
			tokens = realloc(tokens, sizeof(char*) * bufsize);
			if(!tokens) {
				fprintf(stderr, "realloc error in split_line\n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, TOKEN_DELIMITER);
	}

	tokens[position] = NULL;
	return tokens;
}

int launch(char** args) {
	pid_t pid, wpid;
	int status;

	pid = fork();
	if(pid == 0) {
		if (execvp(args[0], args) == -1)
			perror("my_shell");
		exit(EXIT_FAILURE);
	} else if(pid < 0) {
		perror("fork");
	} else {
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}

int execute(char** args) {
	if(args[0] == NULL) return 1;

	for(int i = 0; i < builtin_func_count(); i++)
		if(strcmp(args[0], builtin_str[i]) == 0)
			return builtin_func[i](args);

	return launch(args);
}

void loop(void) {
	char* line;
	char** args;
	int status;

	do {
		printf("> ");
		line = read_line();
		args = split_line(line);
		status = execute(args);

		free(line);
		free(args);
	} while(status);
}

int main(int argc, char** argv) {
	loop();
}
