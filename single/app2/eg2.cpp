#include<stdio.h>
int main()
{
typedef struct _student
{
int rollNumber;
int physics;
int chemistry;
int maths;
int hindi;
int english;
}Student;

FILE *f;
f=fopen("marks.xyz","wb");
if(f==NULL)
{
printf("Can not open file!!\n");
return 0;
}
int rollNumber=101;
for(int i=40;i<70;i=i+5,rollNumber++)
{
Student s;
s.rollNumber=rollNumber;
s.physics=i;
s.chemistry=i+1;
s.maths=i+2;
s.hindi=i+3;
s.english=i+4;
fwrite(&s,1,sizeof(Student),f);
}
fclose(f);
f=fopen("marks.xyz","rb");
Student s1;
for(int i=0;i<=8 && feof(f)==false ;i++)
{
printf("%d\n",feof(f));
printf("Printing data of %dth student\n",i);
Student s1;
fread(&s1,1,sizeof(Student),f);
printf("%d\n",s1.rollNumber);
printf("%d\n",s1.physics);
printf("%d\n",s1.chemistry);
printf("%d\n",s1.maths);
printf("%d\n",s1.hindi);
printf("%d\n",s1.english);
}

return 0;
}