/**
 * @author Søren Nørgaard
 * @date 2012-07-19
 * Strips an export file from LTSpice4 into a regular CSV formatted file.
 * Does: Strip a single signal, Mag and Phase.
 * Does not: Strip multiple signals.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define STRINGLEN 256

int main(int argc, const char *argv[]) {
  FILE *ltsFile;
  if(argc > 1){
    ltsFile = fopen(argv[1], "r");
  } else {
    ltsFile = fopen("lts.txt", "r, ccs=UTF-8");
  }
  FILE *outFile;
  if(argc > 2) {
    outFile = fopen(argv[2], "w");
  } else {
    outFile = fopen("out.csv", "w");
  }
  char tmpStr[STRINGLEN];
  char freq[STRINGLEN];
  char volt[STRINGLEN];
  char mag[STRINGLEN];
  char phase[STRINGLEN];

  fprintf(outFile, "Freq,Mag,Phase\n");
  /* Strip first line */
  fgets(tmpStr, STRINGLEN, ltsFile);
  while (fgets(tmpStr, STRINGLEN, ltsFile)){
    /* Split into Freq and Voltage */
    sscanf(tmpStr, "%[^\t]\t %[^\n]", freq, volt);
    /* Split Voltage into Mag and Phase */
    sscanf(volt, "(%[^d]dB,%[^°)]°)", mag, phase); 
    /* Print to file */
    fprintf(outFile, "%s,%s,%s\n", freq, mag, phase);
  }
  

  fclose(ltsFile);
  fclose(outFile);
  return 0;
}
