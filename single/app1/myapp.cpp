#include<tmwp>
#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<fstream>
using namespace std;
using namespace tmwp;

class Student
{
private:
int rollNumber;
char name[36];

public:
Student()
{
this->rollNumber=0;
this->name[0]='\0';
}

Student(const Student &other)
{ 
this->rollNumber=other.rollNumber;
strcpy(this->name,other.name);
}

Student & operator=(Student other)
{ 
this->rollNumber=other.rollNumber;
strcpy(this->name,other.name);
return *this;
}

Student(int rollNumber,char *name)
{
this->rollNumber=rollNumber;
strcpy(this->name,name);
}

int getRollNumber()
{
return this->rollNumber;
}

void getName(char *name)
{
strcpy(name,this->name);
}

void setRollNumber(int rollNumber)
{
this->rollNumber=rollNumber;
}

void setName(const char *name)
{
strcpy(this->name,name);
}

};

void addStud(Request &request,Response &response)
{
int rollNumber=atoi(request.get("rl"));
char name[36];
strcpy(name,request.get("nm"));

Student s(rollNumber,name);

ofstream file;
file.open("Students.data", ios::app | ios::binary);
file.write((char*)&s,sizeof(Student));
file.close();

response.setMimeType("text/html");
response.write("<!DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>Whatever</title>");
response.write("</head>");
response.write("<body>");
response.write("<h3>Student Added</h3>");
response.write("<br>");
response.write("<a href='index.html'>OK</a>");
response.write("</body>");
response.write("</html>");
response.close();
}
void getAllStudents(Request &request,Response &response)
{
ifstream file("Students.data",ios::binary);
response.setMimeType("text/html");
response.write("<!DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>Whatever</title>");
response.write("</head>");
response.write("<body>");
response.write("<table>");
response.write("<thead>");
response.write("<th>S. No.</th><th>Roll Number</th><th>Name</th>");
response.write("</thead>");
response.write("<tbody>");
if(!file.fail())
{
Student s;
char name[36];
int i=1;
char dataToWrite[100];
while(1)
{
file.read((char*)&s,sizeof(Student));
if(file.fail()) break;
s.getName(name);
response.write("<tr>");
sprintf(dataToWrite,"<td>%d</td>",i);
response.write(dataToWrite);
sprintf(dataToWrite,"<td>%d</td>",s.getRollNumber());
response.write(dataToWrite)	;
sprintf(dataToWrite,"<td>%s</td>",name);
response.write(dataToWrite);
response.write("</tr>");
i++;
}
}
file.close();
response.write("</tbody>");
response.write("<a href='index.html'>OK</a>");
response.write("</body>");
response.write("</html>");
response.close();
}
void editStud(Request &request,Response &response)
{
int rollNumber=atoi(request.get("rl"));
char name[36];
ifstream file("Students.data",ios::binary);
if(!file.fail())
{
Student s;
while(1)
{
file.read((char*)&s,sizeof(Student));
if(file.fail()) break;
if(s.getRollNumber()==rollNumber)
{
s.getName(name);
break;
}
}

char dataToWrite[100];
response.setMimeType("text/html");
response.write("<!DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>Whatever</title>");
response.write("</head>");
response.write("<body>");
response.write("<form action='updateStudent'>");
sprintf(dataToWrite,"<b>Roll Number: %d<b><br>",rollNumber);
response.write(dataToWrite);
sprintf(dataToWrite,"<input type='hidden' id='rl' name='rl' value='%d'>",rollNumber);
response.write(dataToWrite);
response.write("Name:");
sprintf(dataToWrite,"<input type='text' id='nm' name='nm' value='%s'>",name);
response.write(dataToWrite);
response.write("<button type='submit'>Update</button>");
response.write("</form>");
response.write("<a href='index.html'>OK</a>");
response.write("</body>");
response.write("</html>");
response.close();
}
}

void updateStud(Request &request,Response &response)
{
int rollNumber=atoi(request.get("rl"));
char name[36];
strcpy(name,request.get("nm"));
fstream file("Students.data",ios::in | ios::out | ios::binary);
if(!file.fail())
{
Student s;
Student s1(rollNumber,name);
int pos=0;
while(1)
{
file.read((char*)&s,sizeof(Student));
if(file.fail()) break;
if(file.eof()) break;
if(s.getRollNumber()==rollNumber)
{
file.seekp(pos);
file.write((char*)&s1,sizeof(Student));
break;
}
else
{
pos=file.tellg();
}
}
file.close();
}

response.setMimeType("text/html");
response.write("<!DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>Whatever</title>");
response.write("</head>");
response.write("<body>");
response.write("<h3>Student Updated</h3>");
response.write("<br>");
response.write("<a href='index.html'>OK</a>");
response.write("</body>");
response.write("</html>");
response.close();
}
void confirmDeleteStud(Request &request,Response &response)
{
int rollNumber=atoi(request.get("rl"));
char name[36];
ifstream file("Students.data",ios::binary);
if(!file.fail())
{
Student s;
while(1)
{
file.read((char*)&s,sizeof(Student));
if(file.fail()) break;
if(s.getRollNumber()==rollNumber)
{
s.getName(name);
break;
}
}

char dataToWrite[100];
response.setMimeType("text/html");
response.write("<!DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>Whatever</title>");
response.write("</head>");
response.write("<body>");
response.write("<form action='deleteStudent'>");
sprintf(dataToWrite,"<b>Roll Number: %d<b><br>",rollNumber);
response.write(dataToWrite);
sprintf(dataToWrite,"<input type='hidden' id='rl' name='rl' value='%d'>",rollNumber);
response.write(dataToWrite);
sprintf(dataToWrite,"<b>Name: %s<b><br>",name);
response.write(dataToWrite);
response.write("Do you want to delete this Student?");
response.write("<button type='submit'>Delete</button>");
response.write("</form>");
response.write("<a href='index.html'>Home</a>");
response.write("</body>");
response.write("</html>");
response.close();
}
}

void deleteStud(Request &request,Response &response)
{
int rollNumber=atoi(request.get("rl"));
fstream file("Students.data",ios::in | ios::binary);
if(!file.fail())
{
Student s;
int pos=0;
fstream tmpFile("tmp.tmp",ios::app | ios::binary);
while(1)
{
file.read((char*)&s,sizeof(Student));
if(file.fail()) break;
if(file.eof()) break;
if(tmpFile.fail()) break;
if(s.getRollNumber()!=rollNumber)
{
tmpFile.write((char*)&s,sizeof(Student));
}
}
file.close();
file.open("Students.data",ios::out | ios::binary);
tmpFile.close();
tmpFile.open("tmp.tmp", ios::in | ios::binary);
while(1)
{
tmpFile.read((char*)&s,sizeof(Student));
if(tmpFile.fail()) break;
file.write((char*)&s,sizeof(Student));
}
tmpFile.close();
remove("tmp.tmp");
file.close();
}
response.setMimeType("text/html");
response.write("<!DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>Whatever</title>");
response.write("</head>");
response.write("<body>");
response.write("<h3>Student Deleted</h3>");
response.write("<br>");
response.write("<a href='index.html'>OK</a>");
response.write("</body>");
response.write("</html>");
response.close();
}


int main()
{
TMWebProjector server(8080);
server.onRequest("/addStudent",addStud);
server.onRequest("/getStudents",getAllStudents);
server.onRequest("/editStudent",editStud);
server.onRequest("/updateStudent",updateStud);
server.onRequest("/confirmDeleteStudent",confirmDeleteStud);
server.onRequest("/deleteStudent",deleteStud);
server.start();
return 0;
}