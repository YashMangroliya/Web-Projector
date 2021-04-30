#include<tmwp>
#include<windows.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<map>
using namespace tmwp;
using namespace std;
int extensionEquals(const char *left,const char *right);
Request::Request(REQUEST *request)
{
this->request=request;
char *key;
char *value;
int valueLength;
int j=0;
char *data;
for(int i=0;i<this->request->dataCount;i++)
{
j=0;
data=this->request->data[i];
while(data[j]!='=') j++;
key=(char *)malloc(sizeof(char)*j+1);
strncpy(key,data,j);
key[j]='\0';
valueLength=strlen(data)-j;
value=(char*)malloc(sizeof(char)*(valueLength)+1);
j++;
strncpy(value,data+j,valueLength);
value[valueLength]='\0';
dataMap[key]=value;
}
}
char * Request::getMethod()
{
return this->request->method;
}
char * Request::getResource()
{
return this->request->resource;
}
char * Request::getMimeType()
{
return this->request->mimeType;
}
int Request::getDataCount()
{
return this->request->dataCount;
}
char ** Request::getData()
{
return this->request->data;
}
char * Request::get(const char *key)
{
return this->dataMap[key];
}
void Request::forward(const char *resource)
{
printf("Forwarding to resource ");
printf("%s\n",resource);
void (*ptrOnRequest)(Request &,Response &);
ptrOnRequest=this->urlMap[resource];
char responseBuffer[1024];
if(ptrOnRequest==NULL)
{




FILE *f;
f=fopen(resource,"rb");
if(f!=NULL) printf("sending %s\n",resource);
if(f==NULL)
{
printf("sending 404 page\n");
strcpy(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:164\nConnection: close\n\n<!DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM WEB PROJECTOR</title></head><body><h2 style='color:red'>Resource / not found</h2></body></html>");
send(this->request->clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
}
else
{
bool isExtensionSCT=false;
int len=strlen(resource);
int lastIndexOfDot=len-1;
while(lastIndexOfDot>0 && resource[lastIndexOfDot]!='.') lastIndexOfDot--;
if(lastIndexOfDot!=0) isExtensionSCT=extensionEquals(resource+lastIndexOfDot+1,"sct");
if(isExtensionSCT==false)
{
int length,i,rc;
fseek(f,0,2);
length=ftell(f);
fseek(f,0,0);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection: close\n\n",length);
send(this->request->clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
i=0;
while(i<length)
{
rc=length-1;
if(rc>1024) rc=1024;
fread(responseBuffer,rc,1,f);
send(this->request->clientSocketDescriptor,responseBuffer,rc,0);
i+=rc;
}
fseek(f,0,0);
fclose(f);
}
else
{
printf("Forwarding to a resource whose extension is sct\n");

fclose(f);
f=fopen(resource,"r");
int length,i,rc;
fseek(f,0,2);
length=ftell(f);
fseek(f,0,0);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection: close\n\n",length);
send(this->request->clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
printf("%s\n",responseBuffer);
printf("%d\n",this->intDataToForwardMap["rollNumber"]);
f=fopen("marksheet.sct","r");
char c,cc;
std::fill_n(responseBuffer,strlen(responseBuffer), 0);
strcpy(responseBuffer,"");
printf("Response Buffer length: %d\n",strlen(responseBuffer));
c=fgetc(f);
string line="";
char var[50];
char buffer[5];
while(c!=EOF)
{
  if(c=='\n')
  {
  line+=c;
  printf(responseBuffer);
  send(this->request->clientSocketDescriptor,line.c_str(),line.length(),0);
  line.clear();
  }
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
  if(c=='\n')
  {
  line+=c;
  send(this->request->clientSocketDescriptor,line.c_str(),line.length() ,0);
  line.clear();
  }
  if(c=='}')
  {
  if(this->intDataToForwardMap.find(var)!=this->intDataToForwardMap.end())
  {
  itoa(this->intDataToForwardMap[var],buffer,10);
  printf("%d",this->intDataToForwardMap[var]);
  line+=buffer;
  }
  }
  }
  else
  {
  printf("%c",c);
  printf("%c",cc);
  line+=c;
  }
  }
  else
  {
  printf("%c",c);
  line+=c;
  }
c=fgetc(f);
}
if(strlen(responseBuffer)>0) send(this->request->clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
fclose(f);
}

}
}
else
{
Request rq(this->request);
map<const char *,void (*)(Request &,Response &),cmp_str>::iterator itr; 
for (itr = this->urlMap.begin(); itr !=this->urlMap.end(); ++itr)
{
rq.urlMap[itr->first]=itr->second;
}
std::map<const char*,int,cmp_str>::iterator itr2;
for(itr2=this->intDataToForwardMap.begin(); itr2!=this->intDataToForwardMap.end(); ++itr2)
{
rq.intForwardedDataMap[itr2->first]=itr2->second;
}
Response rs(this->request->clientSocketDescriptor);
ptrOnRequest(rq,rs);
}
}
void Request::setInt(const char *key,int value)
{
char *k=new char[strlen(key)+1];
strcpy(k,key);
this->intDataToForwardMap[k]=value;
}
int Request::getInt(const char *key)
{
return this->intForwardedDataMap[key];	
}


Response::Response(int clientSocketDescriptor)
{
this->clientSocketDescriptor=clientSocketDescriptor;
this->flag=false;
}
void Response::setMimeType(const char* mimeType)
{
this->mimeType=(char*)malloc(sizeof(char)*strlen(mimeType)+1);
strcpy(this->mimeType,mimeType);
}
void Response::write(const char *dataToSend)
{
char responseBuffer[1024];
if(this->flag==false)
{
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:%s\nConnection: close\n\n",this->mimeType);
send(this->clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
this->flag=true;
}
strcpy(responseBuffer,dataToSend);
send(this->clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
}
void Response::close()
{
if(clientSocketDescriptor) closesocket(this->clientSocketDescriptor);
}
int extensionEquals(const char *left,const char *right)
{
char a,b;
while(*left && *right)
{
a=*left;
b=*right;
if(a>=65 && a<=90) a+=32;
if(b>=65 && b<=90) b+=32;
if(a!=b) return 0;
left++;
right++;
}
return *left==*right;
}

char * getMIMEType(char *resource)
{
char *mimeType;
mimeType=NULL;
int len=strlen(resource);
if(len<4) return mimeType;
int lastIndexOfDot=len-1;
while(lastIndexOfDot>0 && resource[lastIndexOfDot]!='.') lastIndexOfDot--;
if(lastIndexOfDot==0) return mimeType;
if(extensionEquals(resource+lastIndexOfDot+1,"html"))
{
mimeType=(char*)malloc(sizeof(char)*10);
strcpy(mimeType,"text/html");
}
else if(extensionEquals(resource+lastIndexOfDot+1,"css"))
{
mimeType=(char*)malloc(sizeof(char)*9);
strcpy(mimeType,"text/css");
}
else if(extensionEquals(resource+lastIndexOfDot+1,"js"))
{
mimeType=(char*)malloc(sizeof(char)*16);
strcpy(mimeType,"text/javascript");
}
else if(extensionEquals(resource+lastIndexOfDot+1,"bmp"))
{
mimeType=(char*)malloc(sizeof(char)*10);
strcpy(mimeType,"image/bmp");
}
else if(extensionEquals(resource+lastIndexOfDot+1,"jpeg"))
{
mimeType=(char*)malloc(sizeof(char)*11);
strcpy(mimeType,"image/jpeg");
}
else if(extensionEquals(resource+lastIndexOfDot+1,"jpg"))
{
mimeType=(char*)malloc(sizeof(char)*11);
strcpy(mimeType,"image/jpeg");
}
else if(extensionEquals(resource+lastIndexOfDot+1,"png"))
{
mimeType=(char*)malloc(sizeof(char)*10);
strcpy(mimeType,"image/png");
}
else if(extensionEquals(resource+lastIndexOfDot+1,"webp"))
{
mimeType=(char*)malloc(sizeof(char)*11);
strcpy(mimeType,"image/webp");
}
else if(extensionEquals(resource+lastIndexOfDot+1,"ico"))
{
mimeType=(char*)malloc(sizeof(char)*13);
strcpy(mimeType,"image/x-icon");
}
return mimeType;
}
char isClientSideResource(char * resource)
{
int i;
for(i=0;resource[i]!='\0' && resource[i]!='.';i++);
if(resource[i]=='\0') return 'N';
return 'Y';
}
REQUEST * parseRequest(char *bytes)
{
char method[11];
char resource[1001];
int i,j;
for(i=0;bytes[i]!=' ';i++) method[i]=bytes[i];
method[i]='\0';
i+=2;

char **data=NULL;
int dataCount=0;
if(strcmp(method,"GET")==0)
{
for(j=0;bytes[i]!=' ';j++,i++)
{
if(bytes[i]=='?') break;
resource[j]=bytes[i];
}
resource[j]='\0';
if(bytes[i]=='?')
{
i++;
int si=i;
dataCount=0;
while(bytes[i]!=' ')
{
if(bytes[i]=='&') dataCount++;
i++;
}
dataCount++;
data=(char **)malloc(sizeof(char*)*dataCount);
int *pc=(int*)malloc(sizeof(int)*dataCount);
i=si;
int j=0;
while(bytes[i]!=' ')
{
if(bytes[i]=='&')
{
pc[j]=i;
j++;
}
i++;
}
pc[j]=i;
i=si;
j=0;
int howManyToPick;
while(j<dataCount)
{
howManyToPick=pc[j]-i;
data[j]=(char*)malloc(sizeof(char*)*(howManyToPick+1));
strncpy(data[j],bytes+i,howManyToPick);
data[j][howManyToPick]='\0';
i=pc[j]+1;
j++;
}
}
}

printf("Request arrived for %s\n",resource);
REQUEST *request=(REQUEST*) malloc(sizeof(REQUEST));
request->dataCount=dataCount;
request->data=data;
request->method=(char*)malloc(sizeof(char)*strlen(method)+1);
strcpy(request->method,method);
if(resource[0]=='\0')
{
request->resource=NULL;
request->isClientSideTechnologyResource='Y';
request->mimeType=NULL;
}
else
{
request->resource=(char*)malloc((sizeof(char)*strlen(resource))+1);
strcpy(request->resource,resource);
request->isClientSideTechnologyResource=isClientSideResource(resource);
request->mimeType=getMIMEType(resource);
}
return request;
}

TMWebProjector::TMWebProjector(int portNumber)
{
this->portNumber=portNumber;
}
TMWebProjector::~TMWebProjector()
{
map<char *,void (*)(Request &,Response &),cmp_str>::iterator itr; 
for (itr = this->urlMap.begin(); itr !=urlMap.end(); ++itr)
{
if(itr->first) free(itr->first);
}
}

void TMWebProjector::onRequest(const char *url,void (*ptrOnRequest)(Request &,Response &))
{
/*
if(this->url!=NULL) delete [] this->url;
this->url=NULL;
this->ptrOnRequest=NULL;
if(url==NULL || ptrOnRequest==NULL) return;
this->url=new char[strlen(url)+1];
strcpy(this->url,url);
this->ptrOnRequest=ptrOnRequest;
*/
if(url==NULL || ptrOnRequest==NULL) return;
char *key;
if(url[0]=='/')
{
key=new char[strlen(url)];
strcpy(key,url+1);
}
else
{
key=new char[strlen(url)+1];
strcpy(key,url);
}
this->urlMap[key]=ptrOnRequest;

}

void TMWebProjector::start()
{
FILE *f;
int length;
int i,rc;
char g;
char responseBuffer[1024]; // A chunk of 1024
char requestBuffer[8192]; // maximum size of request can be 8 bytes i.e. 8192 bytes
int bytesExtracted;
WORD ver;
WSADATA wsaData;
int serverSocketDescriptor;
int clientSocketDescriptor;
int successCode=0;
int len;
struct sockaddr_in serverSocketInformation;
struct sockaddr_in clientSocketInformation;
ver=MAKEWORD(1,1);
WSAStartup(ver,&wsaData);


serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,0);
if(serverSocketDescriptor<0)
{
printf("Unable to create socket\n");
return;
}
serverSocketInformation.sin_family=AF_INET;
serverSocketInformation.sin_port=htons(this->portNumber);
serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);
successCode=bind(serverSocketDescriptor,(struct sockaddr*)&serverSocketInformation,sizeof(serverSocketInformation));
if(successCode<0)
{
printf("Unable to bind socket to port %d\n",this->portNumber);
WSACleanup();
return;
}
listen(serverSocketDescriptor,10);
len=sizeof(clientSocketInformation);
while(1)
{
printf("Server is ready and listening at port %d\n",this->portNumber);
clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr*)&clientSocketInformation,&len);
if(clientSocketDescriptor<0)
{
printf("Can not connect!!\n");
WSACleanup();
closesocket(serverSocketDescriptor);
return;
}

bytesExtracted=recv(clientSocketDescriptor,requestBuffer,8192,0);
if(bytesExtracted<0)
{
// what to do is yet not decided
}
else
if(bytesExtracted==0)
{
// what to do is yet not decided
}
else
{
requestBuffer[bytesExtracted]='\0';
REQUEST *request=parseRequest(requestBuffer);
request->clientSocketDescriptor=clientSocketDescriptor;
if(request->isClientSideTechnologyResource=='Y')
{
if(request->resource==NULL)
{
// done
f=fopen("index.html","rb");
if(f!=NULL) printf("sending index.html\n");
if(f==NULL)
{
f=fopen("index.htm","rb");
if(f!=NULL) printf("sending index.htm\n");
}
if(f==NULL)
{
printf("sending 404 page\n");
strcpy(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:164\nConnection: close\n\n<!DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM WEB PROJECTOR</title></head><body><h2 style='color:red'>Resource / not found</h2></body></html>");
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
}
else
{
fseek(f,0,2);
length=ftell(f);
fseek(f,0,0);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection: close\n\n",length);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
i=0;
while(i<length)
{
rc=length-1;
if(rc>1024) rc=1024;
fread(responseBuffer,rc,1,f);
send(clientSocketDescriptor,responseBuffer,rc,0);
i+=rc;
}
fseek(f,0,0);
fclose(f);
closesocket(clientSocketDescriptor);
}//else
}//if
else
{
f=fopen(request->resource,"rb");
if(f==NULL)
{
printf("Sending 404 page\n");
char tmp[500];
sprintf(tmp,"<!DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM WEB PROJECTOR</title></head><body><h2 style='color:red'>Resource /%s not found</h2></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection: close\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
}
else
{
printf("Sending %s\n",request->resource);
printf("%s\n",request->mimeType);
fseek(f,0,2);
length=ftell(f);
fseek(f,0,0);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:%s\nContent-Length:%d\nConnection: close\n\n",request->mimeType,length);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
i=0;
while(i<length)
{
rc=length-1;
if(rc>1024) rc=1024;
fread(responseBuffer,rc,1,f);
send(clientSocketDescriptor,responseBuffer,rc,0);
i+=rc;
}
fclose(f);
closesocket(clientSocketDescriptor);
}
}//else
}
else
{
// more changes required in context to server side resource

if(this->urlMap.size()==0)
{
printf("Sending 404 page\n");
char tmp[500];
sprintf(tmp,"<!DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM WEB PROJECTOR</title></head><body><h2 style='color:red'>Resource /%s not found</h2></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection: close\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
}
else
{
//done
void (*ptrOnRequest)(Request &,Response &);
ptrOnRequest=this->urlMap[request->resource];
if(ptrOnRequest!=NULL)
{
Request rq(request);
map<char *,void (*)(Request &,Response &),cmp_str>::iterator itr; 
for (itr = this->urlMap.begin(); itr !=urlMap.end(); ++itr)
{
rq.urlMap[itr->first]=itr->second;
}
Response rs(clientSocketDescriptor);
ptrOnRequest(rq,rs);
if(request->data!=NULL)
{
for(int k=0;k<request->dataCount;k++) if(request->data[k]!=NULL) free(request->data[k]);
free(request->data);
}
}
else
{
printf("Sending 404 page\n");
char tmp[500];
sprintf(tmp,"<!DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM WEB PROJECTOR</title></head><body><h2 style='color:red'>Resource /%s not found</h2></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection: close\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
}
}
}
}
}//while ends here
closesocket(serverSocketDescriptor);
WSACleanup();
return;
}