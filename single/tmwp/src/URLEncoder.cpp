#include<iostream>
using namespace std;
using namespace tmwp;
static void URLEncoder::encode(char *data)
{
int len=strlen(data);
char *encodedData=(char*)malloc(sizeof(char)*len+1);
int i=0;
j=0;
while(i<len)
{
if(data[i]==' ')
{
encodedData[j]='+';
j++;
}
else if(data[i]=='+'
i++;
}
}
static void URLEncoder::decode(char *data)
{

}