/**
 * @author Søren Nørgaard
 * @date 2012-07-19
 * Strips an export file from LTSpice4 into a regular CSV formatted file.
 * Does: Strip up to 50 signals, Mag and Phase.
 * Does not: Take standard cli arguments - only simple ones. Doesn't count the columns/number of signals.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define STRINGLEN 256
#define MAXSIGNALS 50

typedef struct {
  char mag[STRINGLEN];
  char phase[STRINGLEN];
} signal;
int countSignals(char * firstLine){
  int ret;
  ret = 2;

  return ret;
}

int main(int argc, const char *argv[]) {
  FILE *ltsFile;
  if(argc > 1){
    ltsFile = fopen(argv[1], "r");
  } else {
    ltsFile = fopen("lts2.txt", "r, ccs=UTF-8");
  }
  FILE *outFile;
  if(argc > 2) {
    outFile = fopen(argv[0], "w");
  } else {
    outFile = fopen("out.csv", "w");
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
