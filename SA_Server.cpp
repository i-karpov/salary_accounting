#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>

#include <iostream>
#include <sstream>
#include <string>

//#include <json/json.h>
#include "mongo/bson/bson.h"
#include "mongo/client/dbclient.h"


using namespace std;
using namespace mongo;


#define PAYMENTS_COLLECTION_NAMESPASE "SA.payments"


enum RequestType 
{
    UNKNOWN,
    POST_PAYMENT_RECORD,
    GET_PAYMENT_RECORD
};


class SA_Server : virtual public fastcgi::Component, virtual public fastcgi::Handler
{
	DBClientConnection db;

    public:
        SA_Server(fastcgi::ComponentContext *context) :
                fastcgi::Component(context)
        {

        }

        virtual void onLoad()
        {
		mongo::client::initialize();
		db.connect("localhost");
        }

        virtual void onUnload()
        {

        }

        virtual void handleRequest(fastcgi::Request *request, fastcgi::HandlerContext *context)
        {
                

		string URI = request->getURI();
		string path = RemoveQuery(URI);
		string method = request->getRequestMethod();

		RequestType requestType = GetRequestType(path, method);
		

		if (requestType == UNKNOWN)
		{
			request->setContentType("text/plain");
			request->setStatus(400);
		}
		else if (requestType == POST_PAYMENT_RECORD)
		{
			std::string body;
			request->requestBody().toString(body);

			int amount = atoi(body.c_str());

			string newPaymentId = SavePaymentJson(amount);


			request->setContentType("application/json");
			std::stringbuf buffer4("{ \"newId\": " + newPaymentId + " }");
			request->write(&buffer4);
			request->setStatus(200);
		}
		else if (requestType == GET_PAYMENT_RECORD)
		{
			/*request->setContentType("application/json");
			std::stringbuf buffer4("requestType = GET_PAYMENT_RECORD\n");
			request->write(&buffer4);*/

			string id = GetPaymentIdFromUriPath(path);
			string paymentJson = GetPaymentJson(id);

			if (!paymentJson.compare(""))
			{
				request->setStatus(404);
			}
			else
			{
				std::stringbuf bufferResults(paymentJson);
				request->write(&bufferResults);
				request->setStatus(200);
			}
		}                
        }

	string SavePaymentJson(int amount)
	{
		//BSONObj paymentBSON = mongo::fromjson(newPyamentJson);

		BSONObj paymentBSON = BSON(GENOID 
			<< "PayedToUserId" << 8888
			<< "PayedDate" << "2015-01-25 12:00:00"
			<< "PayedPeriodStartDate" << "2015-01-01 00:00:00"
			<< "PayedPeriodEndDate" << "2015-01-29 23:59:59" 
			<< "Amount" << amount);

		db.insert(PAYMENTS_COLLECTION_NAMESPASE, paymentBSON);

		BSONElement oi;
		paymentBSON.getObjectID(oi);
		OID oid = oi.__oid();
		return oid.toString(); 
	}

	string GetPaymentJson(string id)
	{
		
		auto_ptr<DBClientCursor> cursor = db.query(PAYMENTS_COLLECTION_NAMESPASE, MONGO_QUERY("_id" << OID(id)));
		if (cursor->more()) 
		{
			
			BSONObj payment = cursor->next();
			string paymentJson = payment.jsonString();
			return paymentJson;
			//Json::Value paymentJson;
			//Json::Reader reader;
			//reader.parse(payment.jsonString(), paymentJson);

			//return video.toStyledString();
		}
	    return "";
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
