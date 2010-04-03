
#include "http_server_request_thread.h"

#include "http_server_request.h"
#include "http_server_responses.h"

#include "http_engine.h"

HTTPServerRequestThread::HTTPServerRequestThread(Socket *socket, std::string webContentPath) : m_pSocket(socket), m_webContentPath(webContentPath)
{
}

HTTPServerRequestThread::~HTTPServerRequestThread()
{
}

void HTTPServerRequestThread::run()
{
	if (!m_pSocket)
		return;
	
	std::string clientAddress = m_pSocket->getClientAddress();
	
	printf("Web connection accepted: %s...\n", clientAddress.c_str());
	
	std::string recvData;
	m_pSocket->recv(recvData);
	
	HTTPServerRequest request(recvData);
	std::string response;
	
	if (request.parse())
	{
		std::string content = "<html>\n<body>\n";
		
		if (request.getPath() == "/run" && request.hasParams())
		{
			std::string url = request.getParam("url");
			content += "<p>\nTesting: ";
			content += url;
			content += "<br><br>\n";
			
			HTTPEngine engine;
			HTTPRequest httpTestRequest(url);
			HTTPResponse httpTestResponse;
			if (engine.performRequest(httpTestRequest, httpTestResponse))
			{
				char szData[128];
				memset(szData, 0, 128);
				sprintf(szData, "Response code: %ld<br>\nTotal time: %f<br>\nDownload size: %ld<br>\n", httpTestResponse.responseCode, httpTestResponse.totalTime,
						httpTestResponse.downloadSize);
				
				content += "Results:<br>\n";
				content.append(szData);				
			}
			else
			{
				content += "Couldn't perform test.<br>\n";				
			}			
		}
		else // hard coded html for simple form for the moment
		{
			content += "<h3>Sitemon Web Interface</h3>\n<form action=\"run\" method=\"get\">\n";
			content += "<p><label for=\"url\"><small>URL</small></label>\n";
			content += "<input type=\"text\" name=\"url\" id=\"url\" value=\"http://\" size=\"40\"/></p>\n";
			content += "<p><input type=\"submit\" value=\"Run test\"/>\n";
			content += "</form>\n";		
		}
		
		content += "\n</body>\n</html>\n";
		
		HTTPServerResponse resp(200, content);
		response = resp.responseString();
	}
	else
	{
		HTTPServerResponse resp(500, "<html>\n<body>\nCouldn't understand request\n</body>\n</html>");
		response = resp.responseString();
	}
	
	m_pSocket->send(response);
	
	m_pSocket->close();
	
	delete m_pSocket;
}
