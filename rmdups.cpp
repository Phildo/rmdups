#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <math.h>

#define MAX_PASS_LEN 32
#define MAX_N_PASS 5000000
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

int main(int argc, char **argv)
{
  int most_hits = 0;
  char *pass = (char *)malloc(sizeof(char)*MAX_PASS_LEN*MAX_N_PASS);
  char test_pass[MAX_PASS_LEN];
  int pass_n = 0;
  int pass_i = 0;
  char *buff = (char *)malloc(sizeof(char)*BUFF_LEN);
  int buff_i = 0;
  size_t buff_l = 0;
  FILE *fp;
  int pass_exists = 0;
  int pass_conflicts = 0;
  int **hash = (int **)calloc(HASH_SIZE,sizeof(int *));

  int hits = 0;
  int hitnews = 0;
  int hitolds = 0;
  int misses = 0;

  fp = fopen("password.txt", "r");

  buff_l = fread(buff,sizeof(char),BUFF_LEN,fp);
  while(buff_l > 0)
  {
    while(buff_i < buff_l)
    {
      if(buff[buff_i] != '\n')
      {
        test_pass[pass_i] = buff[buff_i];
        pass_i++;
      }
      else
      {
        test_pass[pass_i] = '\0';

        //check if should append
        pass_exists = 0;
        unsigned long long hash_val = hash_pass(test_pass);
        if(hash[hash_val] != 0)
        {
          hits++;
          int n_hashhits = hash[hash_val][0];
          int hashhit_i = 0;
          for(int i = 0; !pass_exists && i < n_hashhits; i++)
          {
            pass_i = 0;
            pass_conflicts = 0;
            hashhit_i = hash[hash_val][i+1];
            while(!pass_conflicts && pass[hashhit_i*MAX_PASS_LEN+pass_i] != '\0')
            {
              if(pass[hashhit_i*MAX_PASS_LEN+pass_i] != test_pass[pass_i]) pass_conflicts = true;
              pass_i++;
            }
            if(!pass_conflicts && test_pass[pass_i] == '\0')
              pass_exists = true;
          }
          if(!pass_exists)
          {
            hitnews++;
            if((n_hashhits+1)%5 == 0)
            {
              hash[hash_val] = (int *)realloc(hash[hash_val],(n_hashhits+1+5)*sizeof(int));
              for(int j = n_hashhits+1; j < n_hashhits+5; j++)
                hash[hash_val][j] = 0;
            }
            hash[hash_val][0]++;
            if(n_hashhits+1 > most_hits) most_hits = n_hashhits+1;
            hash[hash_val][n_hashhits+1] = pass_n;
          }
          else
            hitolds++;
        }
        else
        {
          misses++;
          hash[hash_val] = (int *)realloc(hash[hash_val],5*sizeof(int));
          for(int j = 0; j < 5; j++)
            hash[hash_val][j] = 0;
          hash[hash_val][0] = 1;
          hash[hash_val][1] = pass_n;
        }
        //append
        if(!pass_exists)
        {
          pass_i = 0;
          while(test_pass[pass_i] != '\0')
          {
            pass[pass_n*MAX_PASS_LEN+pass_i] = test_pass[pass_i];
            pass_i++;
          }
          pass_n++;
        }

        pass_i = 0;
      }
      buff_i++;
    }
    buff_l = fread(buff,sizeof(char),BUFF_LEN,fp);
    buff_i = 0;
  }

  fclose(fp);

  //write
  fp = fopen("rmdupspassword.txt", "w+");

  buff_i = 0;
  pass_i = 0;
  int i = 0;
  while(i < pass_n)
  {
    if(pass[i*MAX_PASS_LEN+pass_i] != '\0')
    {
      buff[buff_i] = pass[i*MAX_PASS_LEN+pass_i];
      pass_i++;
    }
    else
    {
      buff[buff_i] = '\n';
      pass_i = 0;
      i++;
    }
    buff_i++;
    if(buff_i >= BUFF_LEN)
    {
      fwrite(buff,sizeof(char),buff_i,fp);
      buff_i = 0;
    }
  }
  fwrite(buff,sizeof(char),buff_i,fp);

  fclose(fp);

  printf("%d\n",most_hits);
}

