
#include "http_server_request_thread.h"

#include "http_server_request.h"
#include "http_server_responses.h"

#include "http_engine.h"

#include "http_server_db_helpers.h"
#include "http_server_html_formatters.h"

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
	
	std::string recvData;
	m_pSocket->recv(recvData);
	
	HTTPServerRequest request(recvData);
	std::string response;
	
	if (request.parse())
	{
		if (request.getPath() == "/inline_simple" && request.hasParams())
		{
			std::string url = request.getParam("url");
			std::string acceptCompressed = request.getParam("accept_compressed");
			
			std::string content;
			
			HTTPEngine engine;
			HTTPRequest httpTestRequest(url);
			if (acceptCompressed == "1")
			{
				httpTestRequest.setAcceptCompressed(true);
			}
			HTTPResponse httpTestResponse;

			if (engine.performRequest(httpTestRequest, httpTestResponse))
			{
				formatResponseToHTMLDL(httpTestResponse, content);
			}
			else
			{
				content += "Couldn't perform test: " + httpTestResponse.errorString + "<br>\n";
			}

			addResponseToSingleTestHistoryTable(m_pMainDB, httpTestResponse);
			
			HTTPServerResponse resp(200, content);
			response = resp.responseString();
		}
		else if (request.getPath() == "/history")
		{
			int offset = 0;
			if (request.hasParams())
			{
				offset = atoi(request.getParam("start").c_str());
			}
			
			std::string filePath = m_webContentPath + "history.tplt";
			
			std::string dataContent;
			getSingleTestHistoryList(m_pMainDB, dataContent, offset);
			
			HTTPServerTemplateFileResponse resp(filePath, dataContent);
			response = resp.responseString();
		}
		else if (request.getPath() == "/single_details" && request.hasParams())
		{
			long runID = atoi(request.getParam("runid").c_str());
			
			std::string filePath = m_webContentPath + "single_details.tplt";
			
			std::string dataContent;
			formatDBSingleTestResponseToHTMLDL(m_pMainDB, runID, dataContent);
			
			HTTPServerTemplateFileResponse resp(filePath, dataContent);
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
				if (requestedPath.find("..") != -1) // try and guard against obvious exploits
				{
					HTTPServerResponse resp1(500, "<h3>Error occured.</h3>");
					
					response = resp1.responseString();
				}
				else
				{
					std::string filePath = m_webContentPath + requestedPath;
			
					HTTPServerFileResponse resp(filePath);
					response = resp.responseString();
				}
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
