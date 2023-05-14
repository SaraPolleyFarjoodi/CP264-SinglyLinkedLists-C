/*
--------------------------------------------------
Project: CP264-a5q1
File:    myrecord_llist_main.c   public test driver
--------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <math.h> 
#include "myrecord_sllist.h"

typedef struct {
  int count;
  float mean;
  float stddev; 
  float median; 
} STATS;

char letter_grade(float score);
int import_data(char *infilename, SLLIST *sllistp);
STATS process_data(SLLIST *sllistp);
int report_data(char *outfilename, SLLIST *sllistp,  STATS *stats);
void display(SLLIST *sllistp);
void display_searh_message(NODE *p, char *name);

int main(int argc, char* args[]) {
  char infilename[40] = "marks.txt";
  char outfilename[40] = "record_report.txt";
  if (argc > 1) {
    if (argc >= 2) strcpy(infilename, args[1]);
    if (argc >= 3) strcpy(outfilename, args[2]);
  }  

  printf("\nImport and display\n");
  SLLIST sllist = {0};   
  import_data(infilename, &sllist);
  display(&sllist);

  printf("\nInsert and search\n");
  char name[20] = "Moore";
  insert(&sllist, name, 92.0);
  NODE *p = search(&sllist, name);
  display_searh_message(p, name);
  
  printf("\nDelete and search\n");
  strcpy(name, "Wang");
  delete(&sllist, name);
  p = search(&sllist, name);
  display_searh_message(p, name);
  
  strcpy(name, "Allison");
  delete(&sllist, name);
  p = search(&sllist, name);
  display_searh_message(p, name);
 
  STATS stats = process_data(&sllist);
  if (stats.count >=1) {
    printf("\nstats:value\n");  	
	printf("count:%d\n", stats.count);
	printf("mean:%.1f\n", stats.mean);
	printf("stddev:%.1f\n", stats.stddev);
	printf("median:%.1f\n", stats.median);  
    printf("\nname:score,grade\n"); 
    NODE *np = sllist.start; 
    while (np != NULL) {
      printf("%s:%.1f,%c\n", np->data.name, np->data.score, letter_grade(np->data.score));
      np = np->next;
    }
  }
  
  report_data(outfilename, &sllist, &stats);
  clean(&sllist);
  return 0;
}

char letter_grade(float s){
  char r = 'F';
  if (s >= 90) r = 'S';
  else if (s >= 80) r = 'A';
  else if (s >= 70) r = 'B';
  else if (s >= 60) r = 'C';
  else if (s >= 50) r = 'D';  
  else r = 'F';
  return r;
}

void swap(float *first, float *second )
{
  float temp = *first;
  *first = *second;
  *second = temp;
}

void select_sort(float a[], int left, int right) 
{
  int i, j, k;
  float temp;
  for (i = left; i <= right; ++i) {
	k = i;
    for(j = i+1; j <= right; ++j) {	
	  if (a[j] < a[k]) {
		k = j;
	  }
	}
    if (i != k) {
	  swap(a+i, a+k);   
    }
  }	
}
  
float median(SLLIST *sllistp)
{
  if (sllistp->start == NULL) return 0;
  NODE *np = sllistp->start;
  int i=0, n = sllistp->length;
  
  float a[n]; 
  while (np) {
    a[i] = np->data.score;
    np = np->next;
    i++;
  }
  select_sort(a, 0, n-1);
  return (n % 2 == 1)? a[n/2] : (a[n/2-1] + a[n/2])/2 ;   
} 
 
int import_data(char *filename, SLLIST *sllistp) {
  char line[40], name[40];
  char *valuestr = NULL;
  char delimiters[] = ",";
  float score = 0;

  FILE *fp = fopen(filename, "r");  
  if (fp == NULL) {
    perror("Error while opening the file.\n");
    return 0;
  }
  while (fgets(line, sizeof(line), fp) != NULL) {
    valuestr = strtok(line, delimiters);
    strcpy(name, valuestr);
    valuestr = strtok(NULL, delimiters);
    score = atof(valuestr);
	if (strlen(name)>0 && score >0 ) // validate data
      insert(sllistp, name, score);
  }
  fclose(fp);
  return sllistp->length;
}

STATS process_data(SLLIST *sllistp) { 
  int count = 0;
  float mean = 0;
  float stddev = 0;
  NODE *np = sllistp->start;   
  while (np != NULL) {
    count++;
    mean += np->data.score;
    stddev += np->data.score * np->data.score ;
    np = np->next;
  }
  mean /= count;
  stddev = sqrt(stddev/count - mean*mean);
  
  STATS stats = {};
  stats.count = count;
  stats.mean = mean;
  stats.stddev = stddev;
  stats.median = median(sllistp);
  return stats;	
}


int report_data(char *filename, SLLIST *sllistp, STATS *stats) {
  if (stats == NULL || sllistp == NULL) return 1;
	
  FILE *fp = fopen(filename, "w");
    if (fp == NULL) { 
     perror("error, cannot open file");
     return 1;
  } 
  
  fprintf(fp, "%s:%s\n", "stats", "value");
  fprintf(fp, "%s:%d\n", "count", stats->count);
  fprintf(fp, "%s:%.1f\n", "mean", stats->mean);
  fprintf(fp, "%s:%.1f\n", "stddev", stats->stddev);
  fprintf(fp, "%s:%.1f\n", "median", stats->median);
  
  fprintf(fp, "\n%s,%s,%s\n", "name", "score", "grade");
  NODE *np = sllistp->start;
  
  while (np != NULL) {
    fprintf(fp, "%s,%.1f,%c\n", np->data.name, np->data.score, letter_grade(np->data.score));
    np = np->next;
  }
  fclose(fp);  

  return 0;
}

void display(SLLIST *sllistp) {
	NODE *np = sllistp->start;
	printf("length:%d\n", sllistp->length);
	while (np != NULL) {
		printf("%s,%.1f\n", np->data.name, np->data.score);
		np = np->next;
	}
}

void display_searh_message(NODE *p, char *name) {
  if (p == NULL)
    printf("%s:not fount\n", name);
  else
    printf("%s:%.1f\n", p->data.name, p->data.score);
}

 