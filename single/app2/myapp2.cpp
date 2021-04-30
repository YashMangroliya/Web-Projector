#include<tmwp>
#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<fstream>
using namespace std;
using namespace tmwp;
typedef struct _student
{
int rollNumber;
int physics;
int chemistry;
int maths;
int hindi;
int english;
}Student;

void one(Request &request,Response &response)
{
cout<<"One got called"<<endl;
request.forward("xyz.html");
}
void two(Request &request,Response &response)
{
cout<<"Two got called"<<endl;
request.setInt("aaa",111);
request.setInt("bbb",222);
request.setInt("ccc",333);
request.forward("three");
}
void three(Request &request,Response &response)
{
cout<<"Three got called"<<endl;
int aaa=request.getInt("aaa");
char data[100];
sprintf(data,"<h3>%d</h3>",aaa);
response.setMimeType("text/html");
response.write("<!DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>Whatever</title>");
response.write("</head>");
response.write("<body>");
response.write(data);
aaa=request.getInt("bbb");
sprintf(data,"<h3>%d</h3>",aaa);
response.write(data);
aaa=request.getInt("ccc");
sprintf(data,"<h3>%d</h3>",aaa);
response.write(data);

response.write("</body>");
response.write("</html>");
response.close();
}

void getStudMarks(Request &request,Response &response)
{

int rollNumber=atoi(request.get("rl"));
FILE *f=fopen("marks.xyz","rb");
if(f==NULL)
{
printf("Can not open file\n");
return;
}
Student s;
int fRollNumber;
bool found=false;
while(feof(f)==false)
{
fread(&s,1,sizeof(Student),f);
fRollNumber=s.rollNumber;
if(rollNumber==fRollNumber)
{
found=true;
break;
}
}
if(!found)
{
printf("Invalid roll number\n");
return;
}


request.setInt("rollNumber",s.rollNumber);
request.setInt("physics",s.physics);
request.setInt("chemistry",s.chemistry);
request.setInt("maths",s.maths);
request.setInt("hindi",s.hindi);
request.setInt("english",s.english);

request.forward("marksheet.sct");
response.close();
}
int main()
{
TMWebProjector server(8080);
server.onRequest("/one",one);
server.onRequest("/two",two);
server.onRequest("/three",three);
server.onRequest("/getStudentMarks",getStudMarks);
server.start();
return 0;
}