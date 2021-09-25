#include <windows.h>
#include <string>
#include <stdio.h>
//#include "HttpClient.h"

using std::string;

#pragma comment(lib,"ws2_32.lib")



class HttpClient
{
    HINSTANCE hInst;
    WSADATA wsaData;
   /* void mParseUrl(char* mUrl, string& serverName, string& filepath, string& filename);
    SOCKET connectToServer(char* szServerName, WORD portNum);
    int getHeaderLength(char* content);
    char* readUrl2(char* szUrl, long& bytesReturnedOut, char** headerOut);*/
public:
    void getRequest(string hostname, string type, string name, string location,int port) {
        const int bufLen = 1024;
        string path = hostname + "/" + type + "/" +  name + "/" + location;
        const char* szUrl_ = path.c_str();
        //const char* szUrl_ = "http://localhost";
        char* szUrl = const_cast<char*>(szUrl_);
        long fileSize;
        char* memBuffer, * headerBuffer;
        FILE* fp;

        memBuffer = headerBuffer = NULL;

        if (WSAStartup(0x101, &wsaData) != 0)
            return;


        memBuffer = readUrl2(szUrl, fileSize, &headerBuffer,port);
        printf("returned from readUrl\n");
        printf("data returned:\n%s", memBuffer);
        if (fileSize != 0)
        {
            printf("Got some data\n");
            fp = fopen("downloaded.file", "wb");
            fwrite(memBuffer, 1, fileSize, fp);
            fclose(fp);
            delete(memBuffer);
            delete(headerBuffer);
        }

        WSACleanup();
    }

    void mParseUrl(char* mUrl, string& serverName, string& filepath, string& filename)
    {
        string::size_type n;
        string url = mUrl;

        if (url.substr(0, 7) == "http://")
            url.erase(0, 7);

        if (url.substr(0, 8) == "https://")
            url.erase(0, 8);

        n = url.find('/');
        if (n != string::npos)
        {
            serverName = url.substr(0, n);
            filepath = url.substr(n);
            n = filepath.rfind('/');
            filename = filepath.substr(n + 1);
        }

        else
        {
            serverName = url;
            filepath = "/v1/reportAlarm/1/%E4%BA%8C%E5%8F%B7%E6%9C%BA/%E7%A7%91%E7%A0%94%E6%A5%BC%E4%BA%8C%E5%B1%82%E4%B8%9C";
            filename = "";
        }
    }

    SOCKET connectToServer(char* szServerName, WORD portNum)
    {
        struct hostent* hp;
        unsigned int addr;
        struct sockaddr_in server;
        SOCKET conn;

        conn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (conn == INVALID_SOCKET)
            return NULL;

        if (inet_addr(szServerName) == INADDR_NONE)
        {
            hp = gethostbyname(szServerName);
        }
        else
        {
            addr = inet_addr(szServerName);
            hp = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
        }

        if (hp == NULL)
        {
            closesocket(conn);
            return NULL;
        }

        server.sin_addr.s_addr = *((unsigned long*)hp->h_addr);
        server.sin_family = AF_INET;
        server.sin_port = htons(portNum);
        if (connect(conn, (struct sockaddr*)&server, sizeof(server)))
        {
            closesocket(conn);
            return NULL;
        }
        return conn;
    }

    int getHeaderLength(char* content)
    {
        const char* srchStr1 = "\r\n\r\n", * srchStr2 = "\n\r\n\r";
        char* findPos;
        int ofset = -1;

        findPos = strstr(content, srchStr1);
        if (findPos != NULL)
        {
            ofset = findPos - content;
            ofset += strlen(srchStr1);
        }

        else
        {
            findPos = strstr(content, srchStr2);
            if (findPos != NULL)
            {
                ofset = findPos - content;
                ofset += strlen(srchStr2);
            }
        }
        return ofset;
    }

    char* readUrl2(char* szUrl, long& bytesReturnedOut, char** headerOut,int port)
    {
        const int bufSize = 512;
        char readBuffer[bufSize], sendBuffer[bufSize], tmpBuffer[bufSize];
        char* tmpResult = NULL, * result;
        SOCKET conn;
        string server, filepath, filename;
        long totalBytesRead, thisReadSize, headerLen;

        mParseUrl(szUrl, server, filepath, filename);

        ///////////// step 1, connect //////////////////////
        conn = connectToServer((char*)server.c_str(), port);

        ///////////// step 2, send GET request /////////////
        sprintf(tmpBuffer, "GET %s HTTP/1.0", filepath.c_str());
        strcpy(sendBuffer, tmpBuffer);
        strcat(sendBuffer, "\r\n");
        sprintf(tmpBuffer, "Host: %s", server.c_str());
        strcat(sendBuffer, tmpBuffer);
        strcat(sendBuffer, "\r\n");
        strcat(sendBuffer, "\r\n");
        send(conn, sendBuffer, strlen(sendBuffer), 0);

        //    SetWindowText(edit3Hwnd, sendBuffer);
        printf("Buffer being sent:\n%s", sendBuffer);

        ///////////// step 3 - get received bytes ////////////////
        // Receive until the peer closes the connection
        totalBytesRead = 0;
        while (1)
        {
            memset(readBuffer, 0, bufSize);
            thisReadSize = recv(conn, readBuffer, bufSize, 0);

            if (thisReadSize <= 0)
                break;

            tmpResult = (char*)realloc(tmpResult, thisReadSize + totalBytesRead);

            memcpy(tmpResult + totalBytesRead, readBuffer, thisReadSize);
            totalBytesRead += thisReadSize;
        }

        headerLen = getHeaderLength(tmpResult);
        long contenLen = totalBytesRead - headerLen;
        result = new char[contenLen + 1];
        memcpy(result, tmpResult + headerLen, contenLen);
        result[contenLen] = 0x0;
        char* myTmp;

        myTmp = new char[headerLen + 1];
        strncpy(myTmp, tmpResult, headerLen);
        myTmp[headerLen] = NULL;
        delete(tmpResult);
        *headerOut = myTmp;

        bytesReturnedOut = contenLen;
        closesocket(conn);
        return(result);
    }

};

