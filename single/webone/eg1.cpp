#include<tmwp>
#include<iostream>
using namespace std;
using namespace tmwp;
void doSomething(Request &rq,Response &rs)
{
cout<<"Great, this code got executed on server side"<<endl;
int dataCount=rq.getDataCount();
char **data=rq.getData();
if(dataCount>0) cout<<"Data that arrived"<<endl;
for(int r=0;r<dataCount;r++)
{
cout<<data[r]<<endl;
}
char *name=rq.get("nm");
char *city=rq.get("ct");
char * school=rq.get("school");
cout<<"Name is: "<<name<<endl;
cout<<"City is: "<<city<<endl;
cout<<"School is: "<<school<<endl;

rs.setMimeType("text/html");
rs.write("<!DOCTYPE HTML>");
rs.write("<html lang='en'>");
rs.write("<head>");
rs.write("<meta charset='utf-8'>");
rs.write("<title>Whatever</title>");
rs.write("</head>");
rs.write("<body>");
rs.write("<h2>Hello</h2>");
rs.write(name);
rs.write("<br>");
rs.write("</body>");
rs.write("</html>");
rs.close();
}
int main()
{
TMWebProjector server(8080);
server.onRequest("/register",doSomething);
server.start();
return 0;
}