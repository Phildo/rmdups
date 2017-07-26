#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <math.h>

#define LINE_LEN 1024
#define BUFF_LEN (1024*1024)
#define HASH_SIZE (1024*1024*1024)

int mulmap[] = { 25, 9810, 53, 4117, 139, 111111, 17, 88901, 9, 2198002, 123, 14126, 1, 1723, 2, 87920, 1246, 1776, 391, 6, 9821, 211, 1909, 15, 9245, 154, 54440, 26, 9054, 7, 123456, 5432, 1092, 834759 };

int hash_pass(char *pass)
{
  unsigned long long h = 0;
  int i = 0;
  unsigned long long mul = 1;
  while(pass[i] != '\0')
  {
    mul = mulmap[i];
    h += (unsigned long long)((pass[i]%256)+1)*mul;
    i++;
  }
  return h%HASH_SIZE;
}

int same(char *a, char *b)
{
  int i = 0;
  while(a[i] == b[i] && a[i] != '\0') i++;
  if(a[i] == b[i]) return i;
  return 0;
}

int diff(char *a, char *b)
{
  int i = 0;
  while(a[i] == b[i] && a[i] != '\0') i++;
  if(a[i] == b[i]) return 0;
  return i;
}

void cpy(char *src, char *dst)
{
  int i = 0;
  while(src[i] != '\0') { dst[i] = src[i]; i++; }
  dst[i] = '\0';
}

int len(char *s)
{
  int i = 0;
  while(s[i] != '\0') i++;
  return i;
}

struct hash_o
{
  int hits_len;
  char *hits;
};

int exists(hash_o *o, char *s)
{
  int i = 0;
  while(i < o->hits_len)
  {
    if(same(&o->hits[i],s)) return 1;
    else i += len(&o->hits[i])+1;
  }
  return 0;
}

void append(char *s, hash_o *o)
{
  int l = len(s);
  o->hits = (char *)realloc(o->hits,(o->hits_len+l+1)*sizeof(int));
  cpy(s, &o->hits[o->hits_len]);
  o->hits_len += l+1;
}

int main(int argc, char **argv)
{
  int most_hits = 0;
  char *test = (char *)malloc(sizeof(char)*LINE_LEN);
  int pass_i = 0;
  char *buff = (char *)malloc(sizeof(char)*BUFF_LEN);
  int buff_i = 0;
  size_t buff_l = 0;
  FILE *fp;
  hash_o *hash = (hash_o *)calloc(HASH_SIZE,sizeof(hash_o));

  int hits = 0;
  int hitnews = 0;
  int hitolds = 0;
  int misses = 0;

  if(argc > 1)
    fp = fopen(argv[1], "r");
  else
    fp = fopen("password.txt", "r");

  buff_l = fread(buff,sizeof(char),BUFF_LEN,fp);
  while(buff_l > 0)
  {
    while(buff_i < buff_l)
    {
      if(buff[buff_i] != '\n')
      {
        test[pass_i] = buff[buff_i];
        pass_i++;
      }
      else
      {
        test[pass_i] = '\0';

        //check if should append
        unsigned long long hash_val = hash_pass(test);
        if(!exists(&hash[hash_val],test))
        {
          append(test,&hash[hash_val]);
          printf("%s\n",test);
        }

        pass_i = 0;
      }
      buff_i++;
    }
    buff_l = fread(buff,sizeof(char),BUFF_LEN,fp);
    buff_i = 0;
  }

  fclose(fp);
}

