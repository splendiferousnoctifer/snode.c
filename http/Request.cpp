#include <sstream>

#include "Request.h"
#include "HTTPContext.h"


std::map<std::string, std::string>& Request::header() {
    return this->httpContext->requestHeader;
}

const char* Request::body() {
    return this->httpContext->bodyData;
}

int Request::bodySize() {
    return this->httpContext->bodyLength;
}

bool Request::isGet() {
    return this->httpContext->requestLine.find("GET") !=  std::string::npos;
}

bool Request::isPost() {
    return this->httpContext->requestLine.find("POST") != std::string::npos;
}

bool Request::isPut() {
    return this->httpContext->requestLine.find("PUT") != std::string::npos;
}

const std::string Request::requestURI() {
    std::string method;
    std::string uri;
    
    std::istringstream tokenStream(this->httpContext->requestLine);
    std::getline(tokenStream, method, ' ');
    std::getline(tokenStream, uri, ' ');
    
    return uri;
}
