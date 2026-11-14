#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

#define maxarguments 21

#define maxtokens (maxarguments+2)

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
  for (int i =0;i<N;i++){
    if (strcmp(cmd, allowed[i])==0) return 1;
  }
	// TODO
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise
	
	return 0;
}

int main() {
  // TODO
  // Add variables as needed

  char line[256];
  char *arguments[maxtokens];
  int tokens;
  int childpid;
  int status;
  posix_spawnattr_t spawnattributes;
   
  while (1) {

	  fprintf(stderr,"rsh>");

	  if (fgets(line,256,stdin)==NULL) continue;

	  if (strcmp(line,"\n")==0) continue;

	  line[strlen(line)-1]='\0';

    // TODO
    // Add code to spawn processes for the first 9 commands
    // And add code to execute cd, exit, help commands
    // Use the example provided in myspawn.c
    
    // tokenizer
    tokens =0;
    for (char *token = strtok(line, " \t"); token != NULL && tokens < maxtokens -1; token = strtok(NULL, " \t")){
      arguments[tokens++]=token;
    }
    arguments[tokens]=NULL;

    if(tokens==0) continue;

    if (!isAllowed(arguments[0])){
      printf("NOT ALLOWED!\n");
      fflush(stdout);
      continue;
    }

    if (strcmp(arguments[0], "exit") == 0) {
		  return 0;
	  }

    if (strcmp(arguments[0], "help")==0){
      printf("The allowed commands are:\n");
      for (int i = 0; i < N; i++) {
			  printf("%d: %s\n", i + 1, allowed[i]);
		  }
      fflush(stdout);
      continue;
    }

    if (strcmp(arguments[0], "cd")==0){
      if (tokens > 2){
        printf("-rsh: cd: too many arguments\n");
        fflush(stdout);
      } else if (tokens == 2){
        if (chdir(arguments[1])!=0){
          perror("-rsh: cd");
        }
      }
      continue;
    }

    fflush(stdout);
    posix_spawnattr_init(&spawnattributes);

    if (posix_spawnp(&childpid, arguments[0], NULL, &spawnattributes, arguments, environ) != 0){
      perror("spawn failed");
    } else {
      if (waitpid(childpid, &status, 0)==-1){
        perror("waitpid failed");
      }
    }

    posix_spawnattr_destroy(&spawnattributes);

  }
  return 0;
}
