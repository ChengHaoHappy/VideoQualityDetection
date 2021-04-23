#include "Camera.h"


int Camera::getId() const
{
    return id;
}

void Camera::setId(int id)
{
    this->id = id;
}

string Camera::getName() const
{
    return name;
}

void Camera::setName(string name)
{
    this->name = name;
}

int Camera::getType() const
{
    return type;
}

void Camera::setType(int type)
{
    this->type = type;
}

int Camera::getNumber() const
{
    return number;
}

void Camera::setNumber(int number)
{
    this->number = number;
}

string Camera::getIp() const
{
    return ip;
}

void Camera::setIp(string ip)
{
    this->ip = ip;
}

string Camera::getPort() const
{
    return port;
}

void Camera::setPort(string port)
{
    this->port = port;
}

int Camera::getStatus() const
{
    return status;
}

void Camera::setStatus(int status)
{
    this->status = status;
}

string Camera::getLocation() const
{
    return location;
}

void Camera::setLocation(string location)
{
    this->location = location;
}

int Camera::getCreatorId() const
{
    return creatorId;
}

void Camera::setCreatorId(int creatorId)
{
    this->creatorId = creatorId;
}

string Camera::getCreatorName() const
{
    return creatorName;
}

void Camera::setCreatorName(string creatorName)
{
    this->creatorName = creatorName;
}

string Camera::getCreatedTime() const
{
    return createdTime;
}

void Camera::setCreatedTime(string createdTime)
{
    this->createdTime = createdTime;
}

