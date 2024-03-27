#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <color_defs.h>
#define MAXBUFSIZE 2084 

void piece_execvp(char*);
void recall_flank();
void tokenize_pwd(char*, char**, size_t);
void loop();
void handle_tryquit(int sig){

  loop();
}

int main(char argc, char **argv){
   

  char* c_spaces = "   "; 
  printf("░▒▓███████▓▒░░▒▓███████▓▒░░▒▓█▓▒░░▒▓█▓▒░ \n");
  printf("░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ \n");
  printf("░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ \n");
  printf("░▒▓█▓▒░░▒▓█▓▒░▒▓███████▓▒░ ░▒▓██████▓▒░  \n");
  printf("░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░  ░▒▓█▓▒░     \n");
  printf("░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░  ░▒▓█▓▒░     \n");
  printf("░▒▓███████▓▒░░▒▓█▓▒░░▒▓█▓▒░  ░▒▓█▓▒░     \n");
  printf("%s   ____ __ __ ____ __    __ \n", c_spaces);
  printf("%s  / __// // // __// /   / / \n", c_spaces);
  printf("%s _\\ \\ / _  // _/ / /__ / /__\n", c_spaces);
  printf("%s/___//_//_//___//____//____/\n", c_spaces);
   
  int for_build = argc > 1 ? 1 : 0;
  // For when projects is built;
  if(for_build == 1){
    struct sigaction sig_act;
    sig_act.sa_handler = handle_tryquit;
    sigemptyset(&sig_act.sa_mask);
    sig_act.sa_flags = 0;
    sigaction(SIGINT, &sig_act, NULL);
  }

  loop();
  return 0;
}

void loop(){
  char buf[MAXBUFSIZE];
  pid_t child_pid;
  int status;
  
  recall_flank();
  while(fgets(buf, MAXBUFSIZE, stdin) != NULL){
    if(buf[strlen(buf) - 1] == '\n'){
      buf[strlen(buf) - 1] = 0;
    }

    if((child_pid = fork()) == 0){
      piece_execvp(buf);
      printf("%s: Could not execute command\n", strtok(buf, " "));
      exit(EXIT_FAILURE);
    }
    if(waitpid(child_pid, &status, 0) == -1){
      printf("Failed waiting for child\n");
      exit(EXIT_FAILURE);
    } 
    recall_flank();
  }

}

void recall_flank(){
  size_t size = 256;
  char **args= (char **)malloc(size * sizeof(char *));
  char *working_directory = malloc(size * sizeof(char));
  int dirs_amnt = 0;
  if(getcwd(working_directory, size) == NULL){
    printf("%s\n", strerror(errno));
    exit(EXIT_FAILURE);
  } else {
    char* wrd;
    while((wrd = strtok(working_directory, "/")) != NULL){
      args[dirs_amnt] = wrd;
      dirs_amnt++;
      working_directory = NULL;
    }
  }
  const char* naming = 
    "\n > ";
  printf(BBLU);
  int dirs_pool = dirs_amnt > 4 ? dirs_amnt / 2 : 0;
  if(dirs_pool != 0){
    int to_prev = dirs_amnt - 4;
    for(int i = 0; i < to_prev; i++){
      printf("../");
    }
  }
  for(int i = dirs_pool; i < dirs_amnt; i++){
    printf("%s/", args[i]);
  }
  printf("%s", naming);
  printf(WHT);
}


void piece_execvp(char *buffer){
  char **args= (char **)malloc(256 * sizeof(char *));
  int t_n = 0;
  char* wrd;
  while((wrd = strtok(buffer, " ")) != NULL){
    args[t_n] = wrd;
    t_n++;
    buffer = NULL;
  }
  args[t_n] = NULL;
  execvp(args[0], args);
}



