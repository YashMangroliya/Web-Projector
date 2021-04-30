#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<string.h>
using namespace std;
void ff(const char *aa)
{
char *bb=(char *)malloc(sizeof(char)*strlen(aa)+1);
strcmp(bb,aa);
printf(bb);
}

int main()
{
ff("dd");
return 0;
}