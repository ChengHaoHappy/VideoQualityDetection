#pragma once
#include<iostream>
#include <string>
#include <ctime>

using namespace std;

class Camera
{
private:
    int id;
    string name;
    int type;
    int number;
    string ip;
    string port;
    int status;
    string location;
    int creatorId;
    string creatorName;
    string createdTime;
public:
    Camera() {}

    Camera(int id, string name, int type, int number, string ip, string port, int status, string location, int creatorId, string creatorName, string createdTime) {
        this->id = id;
        this->name = name;
        this->type = type;
        this->number = number;
        this->ip = ip;
        this->port = port;
        this->status = status;
        this->location = location;
        this->creatorId = creatorId;
        this->creatorName = creatorName;
        this->createdTime = createdTime;
    }

    int getId() const;
    void setId(int id);

    string getName() const;
    void setName(string name);

    int getType() const;
    void setType(int type);

    int getNumber() const;
    void setNumber(int number);

    string getIp() const;
    void setIp(string ip);

    string getPort() const;
    void setPort(string port);

    int getStatus() const;
    void setStatus(int status);

    string getLocation() const;
    void setLocation(string location);

    int getCreatorId() const;
    void setCreatorId(int creatorId);

    string getCreatorName() const;
    void setCreatorName(string creatorName);

    string getCreatedTime() const;
    void setCreatedTime(string createdTime);

};

