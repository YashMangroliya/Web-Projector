#include<stdio.h>
#include<string.h>
int main()
{
int rollNumber=101,chemistry=44,maths=89,hindi=44,english=87;
int physics=55;
FILE *f;
f=fopen("marksheet.sct","r");
char c,cc;
int i;
c=fgetc(f);
char line[1000];
char var[50];
int jj=0;

while(c!=EOF)
{
jj++;
  if(c=='$')
  {
  cc=fgetc(f);
  if(cc=='{')
  {
  c=fgetc(f);
  i=0;
  while(c!='}' && c!='\n' && c!=EOF)
  {
  var[i]=c;
  c=fgetc(f);
  i++;
  }
  var[i]='\0';
  if(c=='}')
  {
  if(strcmp(var,"physics")==0)
  {
  printf("%d",physics);
  }
  else if(strcmp(var,"chemistry")==0)
  {
  printf("%d",chemistry);
  }
  else if(strcmp(var,"maths")==0)
  {
  printf("%d",maths);
  }
  else if(strcmp(var,"hindi")==0)
  {
  printf("%d",hindi);
  }
  else if(strcmp(var,"english")==0)
  {
  printf("%d",english);
  }
  else if(strcmp(var,"rollNumber")==0)
  {
  printf("%d",rollNumber);
  }
  }
  }
  else
  {
  printf("%c",c);
  printf("%c",cc);
  }
  }
  else
  {
  printf("%c",c);
  }
c=fgetc(f);
}
fclose(f);
}