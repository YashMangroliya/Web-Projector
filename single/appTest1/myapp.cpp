#include<tmwp>
#include<iostream>
#include<stdlib.h>
#include<ctime>
using namespace std;
using namespace tmwp;

void dispatchTime(Request &request,Response &response)
{
time_t t=time(0);
char *now=ctime(&t);
printf("dispatchTime is called\n");
response.write("<!DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>The Clock</title>");
response.write("</head>");
response.write("<body>");
response.write("<h1>");
response.write(now);
response.write("</h1>");
response.write("<a href='/now'>Refresh</a><br>");
response.write("<a href='index.html'>Home</a><br>");
response.write("</body>");
response.write("</html>");
response.close();
}

void getCityView(Request &request,Response &response)
{
string cityCodeString=request.get("city");
int cityCode=atoi(cityCodeString.c_str());
if(cityCode==1) request.forward("Ujjain.html");
else if(cityCode==2) request.forward("Indore.html");
else if(cityCode==3) request.forward("Dewas.html");
else request.forward("index.html");
}
int main()
{
TMWebProjector server(8080);
server.onRequest("/now",dispatchTime);
server.onRequest("/getCity",getCityView);
server.start();
return 0;
}