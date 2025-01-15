#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define K 1.1314
#define COST_PER_METER 1370
#define UNIT_DISTANCE 10

typedef enum {false, true} bool;

typedef struct {
  int height;
  int pos;
  int len;
  int start;
  int end;
} Peak;

int* calculateHeights(char field[20][120], int n, int m) {
  int *heights = calloc(m, sizeof(int));
  for(int i = 0; i < m; i++) {
    int j = n;
    while(field[--j][i] == 'x');
    // nulti x je na visini 0, j se oduzima jednom previse puta pa se oduzima jos 2
    heights[i] = (n-j-2)*UNIT_DISTANCE;
  }
  return heights;
}

Peak* calculatePeaks(int* heights, int m, int* count) {
  *count = 0;
  Peak *peaks = (Peak*)calloc(1, sizeof(Peak));
  for(int i = 0; i < m; i++) {
    if (heights[i] != 0) {
      int start = i;
      peaks = (Peak*)realloc(peaks, ((*count)+1)*sizeof(Peak));
      int peak = heights[i];
      int len = 1;
      while (heights[++i] >= peak) {
        if (peak == heights[i]) len++;
        else len = 1; 
        peak = heights[i];
      }
      peaks[*count].height = peak;
      peaks[*count].pos = --i;
      peaks[*count].len = len;
      int prev = heights[i];
      while(prev >= heights[++i] && prev > 0) {
        prev = heights[i];
      }
      i--;
      int end = i;
      peaks[*count].start = start;
      peaks[*count].end = end;
      (*count)++;
    }
  }
  return peaks;
}

Peak* sortPeaks(Peak* peaks, int count) {
  Peak* sortedPeaks = (Peak*)malloc(sizeof(Peak)*count);
  if(sortedPeaks == NULL) exit(1);
  for(int i = 0; i < count; i++) sortedPeaks[i] = peaks[i]; 
  for(int i = 0; i < count-1; i++) {
    for(int j = i+1; j < count; j++) {
      if (sortedPeaks[i].height < sortedPeaks[j].height) {
        Peak prevPeak = sortedPeaks[i];
        sortedPeaks[i] = sortedPeaks[j];
        sortedPeaks[j] = prevPeak;
      }
    }
  }

  return sortedPeaks;
}

int main() {
  char field[20][120];
  int n,m;
  scanf("%d %d", &n, &m);
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < m; j++) {
      scanf(" %c", &field[i][j]);
    }
  }

  int* heights = calculateHeights(field, n, m);
  // for(int i = 0; i < m; i++) {
  //   printf("%d\n", heights[i]);
  // }
  
  int countPeaks;
  Peak* peaks = calculatePeaks(heights,m,&countPeaks);

  /*
  for(int i = 0; i < countPeaks; i++) {
    printf("Peak %d:\n  Height: %d\n  Pos: %d\n  Len: %d\n  Start: %d\n  End: %d\n\n", i, peaks[i].height, peaks[i].pos, peaks[i].len, peaks[i].start, peaks[i].end);
  }
  */

  Peak* sortedPeaks = sortPeaks(peaks, countPeaks);
  int peak1 = 0, peak2 = 0;
  for(int i = 0; i < countPeaks; i++) {
    if (peaks[i].height == sortedPeaks[0].height) peak1 = i;
    if (peaks[i].height == sortedPeaks[1].height) peak2 = i;
  }
  
  int zracnaUdaljenost = abs(peaks[peak1].pos - peaks[peak2].pos) * UNIT_DISTANCE;
  int visinskaRazlika = abs(peaks[peak1].height - peaks[peak2].height);

  double duljinaMosta = K*(double)sqrt(zracnaUdaljenost*zracnaUdaljenost + visinskaRazlika*visinskaRazlika);

  printf("Most je duljine %.2fm.\n", duljinaMosta);

  double srednjaVisina = (double)(peaks[peak1].height + peaks[peak2].height) / 2;
  int srednjaPozicija = (int)round((double)(peaks[peak1].pos + peaks[peak2].pos) / 2);
  
  bool opasno = heights[srednjaPozicija] > srednjaVisina/2 ? true : false;

/*
  for(int i = peak1+1; i < peak2; i++) {
    for(int j = peaks[i].start; j < peaks[i].end; j++) {
      if (heights[j] > srednjaVisina / 2) {
        opasno = true;
        break;
      }
    }
  }
*/

  // if (heights[(int)srednjaPozicija] > srednjaVisina / 2) opasno = true;
  /*
  for(int i = (int)srednjaPozicija-1; i < (int)srednjaPozicija+1; i++) {
    int peak = heights[i];
    
    if (peak > srednjaVisina / 2) {
      opasno = true;
      break;
    }
  }
  */
  // for(int i = peak1+1; i < peak2; i++) {
  //   if (peaks[i].height > (double)srednjaVisina/2) {
  //     if (peaks[i].pos > srednjaPozicija-peaks[i].len/2 && peaks[i].pos < srednjaPozicija+peaks[i].len/2) opasno = true;
  //   }
  // }

  if (opasno) printf("Most ipak nije sigurno graditi.\n");
  else {
    printf("Ne postoje opasnosti za gradnju.\n");
    double cijena = COST_PER_METER*duljinaMosta;
    printf("Cijena izgradnje je: %.2f EUR.", cijena);
  }

  return 0;
}