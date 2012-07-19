/**
 * @author Søren Nørgaard
 * @date 2012-07-19
 * Strips an export file from LTSpice4 into a regular CSV formatted file.
 * Does: Strip up to 50 signals, Mag and Phase. Counts number of signals.
 * Does not: Take standard cli arguments - only simple ones.
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#define STRINGLEN 256
#define MAXSIGNALS 50

typedef struct {
  char mag[STRINGLEN];
  char phase[STRINGLEN];
} signal;
int countSignals(char * firstLine){
  int ret = 0;
  int i;
  for (i = 0; i < strlen(firstLine); i++){
    if (firstLine[i] == '\t') {
      ret++;
    }
  }

  return ret;
}

int main(int argc, char *argv[]) {
  FILE *ltsFile;
  FILE *outFile;
  char strOfile[STRINGLEN] = {'\0'};
  char strIfile[STRINGLEN] = {'\0'};

  /* Command line arguments */
  /* if(argc > 1){ */
  /*   ltsFile = fopen(argv[1], "r"); */
  /* } else { */
  /*   ltsFile = fopen("lts2.txt", "r, ccs=UTF-8"); */
  /* } */
  /* if(argc > 2) { */
  /*   outFile = fopen(argv[0], "w"); */
  /* } else { */
  /*   outFile = fopen("out.csv", "w"); */
  /* } */
  int c;
  int oflag = 0;
  while ((c = getopt (argc, argv, "i:o:")) != -1){
    switch (c){
    case 'i':
      strcpy(strIfile, optarg);
      break;
    case 'o':
      strcpy(strOfile, optarg);
      break;
    case '?':
      if (optopt = 'c')
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      else if (isprint (optopt))
        fprintf(stderr, "Unknown option `-%c'.\n", optopt);
      else
        fprintf(stderr, "Unknown option character `\\c%x'.\n", optopt);
      return 1;
      break;
    default:
      abort();
      break;
    }
  }
  if (strlen(strIfile) > 0) {
    ltsFile = fopen(strIfile, "r");
  } else {
    printf("No real input file given\n");
    return 1;
  }

  if (strlen(strOfile) > 0) {
    outFile = fopen(strOfile, "w");
  } else {
    printf("No real output file given\n");
    return 1;
  }


  char tmpStr[STRINGLEN];
  char freq[STRINGLEN];
  char *token;
  char volt[STRINGLEN][MAXSIGNALS];
  signal signals[MAXSIGNALS];
  int numOfSignals;
  int i = 0;

  /* Count number of signals */
  numOfSignals = countSignals(fgets(tmpStr, STRINGLEN, ltsFile));
  /* Write header to file */
  fprintf(outFile, "Freq,");
  for (i = 0; i < numOfSignals; i++){
    fprintf(outFile, "M(%d),Ph(%d)", i+1, i+1);
    if (i < numOfSignals-1)
      fprintf(outFile, ",");
    else
      fprintf(outFile, "\n");
  }

  /* Parse file */
  while (fgets(tmpStr, STRINGLEN, ltsFile)){
    /* Part freqs and volts */
    token = strtok(tmpStr, "\t");
    strcpy(freq, token);
    for (i = 0; i < numOfSignals; i++){
      token = strtok(NULL, "\t");
      strcpy(volt[i], token);
    }

    /* Part individual signals into mag and phase */
    for (i = 0; i < numOfSignals; i++){
      token = strtok(volt[i], "\n");
      sscanf(token, "(%[^d]dB,%[^°)]°)", signals[i].mag, signals[i].phase); 
      /* printf("%s\n",volt[1]); */
    }

    /* Write to new file */
    fprintf(outFile, "%s,", freq);
    for (i = 0; i < numOfSignals; i++){
      fprintf(outFile, "%s,%s", signals[i].mag, signals[i].phase);
      if (i < numOfSignals-1)
        fprintf(outFile, ",");
      else
        fprintf(outFile, "\n");
    } 

  }
  

  fclose(ltsFile);
  fclose(outFile);
  return 0;
}
