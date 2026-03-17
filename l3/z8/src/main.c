#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 64ull
#define TOKEN_DELIMITER " \t\r\n\a"

char* read_line(void) {
	size_t bufsize = BUFFER_SIZE;
	size_t position = 0;
	char* buffer = malloc(sizeof(char) * bufsize);
	int c;

	if(!buffer) {
		fprintf(stderr, "malloc error in read_line\n");
		exit(EXIT_FAILURE);
	}
	getline(NULL, NULL, NULL);

	while(1) {
		c = getchar();
		if(c == EOF || c == '\n') {
			buffer[position] = '\0';
			return buffer;
		}
		buffer[position] = (char)c;
		position++;

		if(position == bufsize) {
			bufsize += BUFFER_SIZE;
			buffer = realloc(buffer, sizeof(char) * bufsize);
			if(!buffer) {
				fprintf(stderr, "realloc error in read_line\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}

char** split_line(char* line) {
	size_t bufsize = BUFFER_SIZE;
	size_t position = 0;
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
	pid_t pid;
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
			waitpid(pid, &status, WUNTRACED);
		} while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}

int execute(char** args) {
	if(args[0] == NULL) return 1;

	for(size_t i = 0; i < builtin_func_count(); i++)
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

int main() {
	loop();
}
