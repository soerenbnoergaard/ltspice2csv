/**
 * @author Søren Nørgaard
 * @date 2012-07-19
 * Strips an export file from LTSpice4 into a regular CSV formatted file.
 * Does: Strip up to 50 signals, Mag and Phase. Counts number of signals. Set a flag -a, to treat as AC, freq, mag, phase.
 * Does not: 
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

  /* Get command line argunemts */
  int c;
  int acFlag = 0;
  while ((c = getopt (argc, argv, "i:o:a")) != -1){
    switch (c){
    case 'i':
      strcpy(strIfile, optarg);
      break;
    case 'o':
      strcpy(strOfile, optarg);
      break;
    case 'a':
      acFlag = 1;
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
  char firstCol[STRINGLEN];
  char *token;
  char volt[STRINGLEN][MAXSIGNALS];
  signal signals[MAXSIGNALS];
  int numOfSignals;
  int i = 0;

  /* Count number of signals */
  numOfSignals = countSignals(fgets(tmpStr, STRINGLEN, ltsFile));
  /* Write header to file */
  
  if (acFlag)
    fprintf(outFile, "Freq,");
  else 
    fprintf(outFile, "Time,");
  for (i = 0; i < numOfSignals; i++){
    if (acFlag)
      fprintf(outFile, "M(%d),Ph(%d)", i+1, i+1);
    else
      fprintf(outFile, "V(%d)", i+1); 
    if (i < numOfSignals-1)
      fprintf(outFile, ",");
    else
      fprintf(outFile, "\n");
  }

  /* Parse file */
  while (fgets(tmpStr, STRINGLEN, ltsFile)){
    /* Part freqs and volts */
    token = strtok(tmpStr, "\t");
    strcpy(firstCol, token);
    for (i = 0; i < numOfSignals; i++){
      token = strtok(NULL, "\t");
      strcpy(volt[i], token);
    }

    /* Part individual signals into mag and phase  - IF AC ONLY. Remove newline for both AC and DC */
    for (i = 0; i < numOfSignals; i++){
      token = strtok(volt[i], "\n");
      if (acFlag) {
        sscanf(token, "(%[^d]dB,%[^°)]°)", signals[i].mag, signals[i].phase); 
      } else {
        sscanf(token, "%s", volt[i]);
      }
    }

    /* Write to new file */
    fprintf(outFile, "%s,", firstCol);
    for (i = 0; i < numOfSignals; i++){
      if (acFlag)
        fprintf(outFile, "%s,%s", signals[i].mag, signals[i].phase);
      else 
        fprintf(outFile, "%s", volt[i]);
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
