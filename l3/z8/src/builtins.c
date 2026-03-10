#include "builtins.h"

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stddef.h>
#include <limits.h>
#include <unistd.h>

int builtin_cd(char** args) {
	if(args[1] == NULL) {
		fprintf(stderr, "expected argument to \"cd\"\n");
	} else {
		if(chdir(args[1]) != 0)
			perror("cd");
	}
	return 1;
}

int builtin_pwd(char** _) {
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("pwd");
    } else {
        printf("%s\n", cwd);
    }

    return 1;
}

int builtin_help(char** _) {
	printf("MF who deadass need help with shell usage (skull emoji)\n");
	printf("Type program names and arguments, and hit enter.\n");
	printf("The following are built in:\n");

	for (size_t i = 0; i < builtin_func_count(); i++) {
		printf("\t%s\n", builtin_str[i]);
	}

	printf("Use the man command for information on other programs.\n");
	return 1;
}

int builtin_exit(char** _) {
	return 0;
}

const char* const builtin_str[] = {
	"cd",
	"pwd",
	"help",
	"exit"
};

const builtin_func_t builtin_func[] = {
	&builtin_cd,
	&builtin_pwd,
	&builtin_help,
	&builtin_exit
};

size_t builtin_func_count() {
	return sizeof(builtin_func) / sizeof(builtin_func_t);
}
