#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>

#include <iostream>
#include <sstream>
#include <string>


using namespace std;


enum RequestType 
{
    UNKNOWN,
    POST_PAYMENT_RECORD,
    GET_PAYMENT_RECORD
};


class SA_Server : virtual public fastcgi::Component, virtual public fastcgi::Handler
{

    public:
        SA_Server(fastcgi::ComponentContext *context) :
                fastcgi::Component(context)
        {

        }

        virtual void onLoad()
        {

        }

        virtual void onUnload()
        {

        }

        virtual void handleRequest(fastcgi::Request *request, fastcgi::HandlerContext *context)
        {
                request->setContentType("text/plain");

		string URI = request->getURI();
		string path = RemoveQuery(URI);
		string method = request->getRequestMethod();

		RequestType requestType = GetRequestType(path, method);
		
                std::stringbuf buffer1("URI = " + URI + "\n");
		request->write(&buffer1);
		std::stringbuf buffer2("path = " + path + "\n");
		request->write(&buffer2);
		std::stringbuf buffer3("method = " + method + "\n");
		request->write(&buffer3);

		if (requestType == UNKNOWN)
		{
		}
		else if (requestType == POST_PAYMENT_RECORD)
		{
			std::stringbuf buffer4("requestType = POST_PAYMENT_RECORD\n");
			request->write(&buffer4);
		}
		else if (requestType == GET_PAYMENT_RECORD)
		{
			std::stringbuf buffer4("requestType = GET_PAYMENT_RECORD\n");
			request->write(&buffer4);

			string id = GetPaymentIdFromUriPath(path);
			std::stringbuf buffer5("id = " + id + "\n");
			request->write(&buffer5);
		}                
        }

	string GetPaymentIdFromUriPath(string path)
	{
		string id = path.substr (10,path.size() - 10);
		return id;
	}

	string RemoveQuery(string uri) 
	{
    		return uri.find('?') != string::npos ? uri.erase(uri.find('?'), uri.size() - 1) : uri;
	}

	RequestType GetRequestType(string path, string method)
	{
		if (!path.compare("/payments") && !method.compare("POST"))
		{
			return POST_PAYMENT_RECORD;
		}
		else if (path.compare("/payments/") && path.find("/payments/") != string::npos && !method.compare("GET"))
		{
			return GET_PAYMENT_RECORD;
		}
		else
		{
			return UNKNOWN;
		}
	}
};

FCGIDAEMON_REGISTER_FACTORIES_BEGIN()
FCGIDAEMON_ADD_DEFAULT_FACTORY("SA_Server_Factory", SA_Server)
FCGIDAEMON_REGISTER_FACTORIES_END()
