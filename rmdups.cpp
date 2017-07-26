#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define MAX_PASS_LEN 64
#define MAX_N_PASS 5000000
#define BUFF_LEN 1024*1024

int main(int argc, char **argv)
{
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
        for(int i = 0; !pass_exists && i < pass_n; i++)
        {
          pass_i = 0;
          pass_conflicts = 0;
          while(!pass_conflicts && pass[i*MAX_PASS_LEN+pass_i] != '\0')
          {
            if(pass[i*MAX_PASS_LEN+pass_i] != test_pass[pass_i]) pass_conflicts = true;
            pass_i++;
          }
          if(!pass_conflicts && test_pass[pass_i] == '\0')
            pass_exists = true;
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
  }

  fclose(fp);

  //write
  fp = fopen("rmdupspassword.txt", "w+");

  buff_i = 0;
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
}

