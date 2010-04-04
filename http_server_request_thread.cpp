
#include "http_server_request_thread.h"

#include "http_server_request.h"
#include "http_server_responses.h"

#include "http_engine.h"

HTTPServerRequestThread::HTTPServerRequestThread(Socket *socket, const std::string &webContentPath, SQLiteDB *pMainDB)
	: m_pSocket(socket), m_webContentPath(webContentPath), m_pMainDB(pMainDB)
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
		if (request.getPath() == "/inline_simple" && request.hasParams())
		{
			std::string url = request.getParam("url");
			
			std::string content;
			
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
			
			HTTPServerResponse resp(200, content);
			response = resp.responseString();
		}
		else
		{
			std::string requestedPath = request.getPath();
			if (requestedPath.size() > 1 && requestedPath.substr(0, 1) == "/") // an actual relative path was specified
			{
				requestedPath = requestedPath.substr(1); // knock off the leading slash
			}
			else if (requestedPath.size() == 1 && requestedPath == "/")
			{
				// default is index.html
				requestedPath = "index.html";
			}
			
			if (m_webContentPath.empty())
			{
				std::string content = "<html>\n<head><title>Sitemon Web Interface</title></head>\n<body>\n";
				content += "<h3>Sitemon Web Interface</h3>\nError: Web Content Path not configured.\n</body>\n</html>\n";
				
				HTTPServerResponse resp(500, content);
				response = resp.responseString();
			}
			else
			{
				std::string filePath = m_webContentPath + requestedPath;
			
				HTTPServerFileResponse resp(filePath);
				response = resp.responseString();
			}
		}
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
