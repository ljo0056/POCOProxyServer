#include "pch.h"
#include "CPOCOProxyServer.h"

#include <Poco/Net/SocketReactor.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketNotification.h>
#include <Poco/Net/SocketAcceptor.h>

using namespace Poco::Net;

////////////////////////////////////////////////////////////////////////////////////////
/// class ProxySession

class ProxySession
{
public:
	ProxySession(StreamSocket& front, SocketReactor& reactor)
		: m_front(front)
		, m_reactor(reactor)
		, m_front_buffer(nullptr)
		, m_back_buffer(nullptr)
		, m_buffer_len(1024 * 2)
		, m_service(nullptr)
		, m_back_connected(false)
		, m_closed(false)
	{		
		m_front_buffer = new byte[m_buffer_len];
		m_back_buffer = new byte[m_buffer_len];
		
        LOGI << fmt::format("Front Connection from[{}]", front_address());
	}

	~ProxySession()
	{
		UnregisterHandler();
	}

	void RegisterHandler()
	{
		//m_reactor.addEventHandler(m_front, Poco::Observer<ProxySession, WritableNotification>(*this, &ProxySession::onFrontWritable));
		m_reactor.addEventHandler(m_front, Poco::Observer<ProxySession, ReadableNotification>(*this, &ProxySession::onFrontReadable));
		m_reactor.addEventHandler(m_front, Poco::Observer<ProxySession, ShutdownNotification>(*this, &ProxySession::onFrontShutdown));
		m_reactor.addEventHandler(m_front, Poco::Observer<ProxySession, ErrorNotification>(*this, &ProxySession::onFrontError));
		//m_reactor.addEventHandler(m_front, Poco::Observer<ProxySession, IdleNotification>(*this, &ProxySession::onFrontIdle));
		//m_reactor.addEventHandler(m_front, Poco::Observer<ProxySession, TimeoutNotification>(*this, &ProxySession::onFrontTimeout));

		m_reactor.addEventHandler(m_back, Poco::Observer<ProxySession, WritableNotification>(*this, &ProxySession::onBackWritable));
		m_reactor.addEventHandler(m_back, Poco::Observer<ProxySession, ReadableNotification>(*this, &ProxySession::onBackReadable));
		m_reactor.addEventHandler(m_back, Poco::Observer<ProxySession, ShutdownNotification>(*this, &ProxySession::onBackShutdown));
		m_reactor.addEventHandler(m_back, Poco::Observer<ProxySession, ErrorNotification>(*this, &ProxySession::onBackError));
		//m_reactor.addEventHandler(m_back, Poco::Observer<ProxySession, IdleNotification>(*this, &ProxySession::onBackIdle));
		//m_reactor.addEventHandler(m_back, Poco::Observer<ProxySession, TimeoutNotification>(*this, &ProxySession::onBackTimeout));
	}

	void UnregisterHandler()
	{
		//m_reactor.removeEventHandler(m_front, Poco::Observer<ProxySession, WritableNotification>(*this, &ProxySession::onFrontWritable));
		m_reactor.removeEventHandler(m_front, Poco::Observer<ProxySession, ReadableNotification>(*this, &ProxySession::onFrontReadable));
		m_reactor.removeEventHandler(m_front, Poco::Observer<ProxySession, ShutdownNotification>(*this, &ProxySession::onFrontShutdown));
		m_reactor.removeEventHandler(m_front, Poco::Observer<ProxySession, ErrorNotification>(*this, &ProxySession::onFrontError));
		//m_reactor.removeEventHandler(m_front, Poco::Observer<ProxySession, IdleNotification>(*this, &ProxySession::onFrontIdle));
		//m_reactor.removeEventHandler(m_front, Poco::Observer<ProxySession, TimeoutNotification>(*this, &ProxySession::onFrontTimeout));

		m_reactor.removeEventHandler(m_back, Poco::Observer<ProxySession, WritableNotification>(*this, &ProxySession::onBackWritable));
		m_reactor.removeEventHandler(m_back, Poco::Observer<ProxySession, ReadableNotification>(*this, &ProxySession::onBackReadable));
		m_reactor.removeEventHandler(m_back, Poco::Observer<ProxySession, ShutdownNotification>(*this, &ProxySession::onBackShutdown));
		m_reactor.removeEventHandler(m_back, Poco::Observer<ProxySession, ErrorNotification>(*this, &ProxySession::onBackError));
		//m_reactor.removeEventHandler(m_back, Poco::Observer<ProxySession, IdleNotification>(*this, &ProxySession::onBackIdle));
		//m_reactor.removeEventHandler(m_back, Poco::Observer<ProxySession, TimeoutNotification>(*this, &ProxySession::onBackTimeout));				

		if (nullptr != m_front_buffer)
		{
			delete[] m_front_buffer;
			m_front_buffer = nullptr;
		}

		if (nullptr != m_back_buffer)
		{
			delete[] m_back_buffer;
			m_back_buffer = nullptr;
		}
	}

	void Close()
	{
		if (m_closed.exchange(true)) 
			return;

		LOGI << fmt::format("Disconnection front[{}] back[{}]", front_address(), back_address());

		m_back_connected = false;

		m_back.close();
		m_front.close();

		if (nullptr != m_service)
		{
			m_service->OnFrontDisconnect();
			m_service->OnBackDisconnect();
			delete m_service;
		}

		delete this;
	}	

	void SetService(CPOCOProxyHandler* service, SocketAddress& addr)
	{
		m_service = service;

		LOGI << fmt::format("POCO Try back connection Sesseion [{}] ", addr.toString());
		m_back.connectNB(addr);

		RegisterHandler();

		if (m_service)
			m_service->OnFrontConnect();
	}

	void onFrontWritable(Poco::Net::WritableNotification* pNf)
	{
		pNf->release();
	}

	void onFrontReadable(Poco::Net::ReadableNotification* pNf)
	{
		pNf->release();

		try
		{
			if (false == m_back_connected)
				return;

			int n = m_front.receiveBytes(m_front_buffer, m_buffer_len);	
			if (n <= 0)
			{
				Close();
				return;
			}			
			
			if (m_service)
				m_service->OnFrontReadable(m_front_buffer, n);
		}
		catch (Poco::Exception& e)
		{
			LOGE << fmt::format("POCO Sesseion [{}] error : [{}]", front_address(), e.displayText());
			Close();
		}
	}

	void onFrontShutdown(Poco::Net::ShutdownNotification* pNf)
	{
		pNf->release();
		LOGI << fmt::format("onShutdown from[{}]", front_address());

		Close();		
	}

	void onFrontError(Poco::Net::ErrorNotification* pNf)
	{
		pNf->release();
		LOGI << fmt::format("onError from[{}]", front_address());

		Close();
	}

	void onFrontTimeout(Poco::Net::TimeoutNotification* pNf)
	{
		pNf->release();		
	}

	void onFrontIdle(Poco::Net::IdleNotification* pNf)
	{
		pNf->release();		
	}

	void onBackWritable(Poco::Net::WritableNotification* pNf)
	{
		pNf->release();

		try
        {
			if (false == m_back_connected)
			{
				m_back_connected = true;
				if (m_service)
					m_service->OnBackConnect();

				LOGI << fmt::format("Back Connection from[{}]", back_address());

				//m_reactor.removeEventHandler(m_back, Poco::Observer<ProxySession, WritableNotification>(*this, &ProxySession::onBackWritable));
            }
		}
		catch (Poco::Exception& e)
		{
			LOGE << fmt::format("POCO Sesseion [{}] error : [{}]", front_address(), e.displayText());
			Close();
		}
	}

	void onBackReadable(Poco::Net::ReadableNotification* pNf)
	{
		pNf->release();

		try
		{
			if (false == m_back_connected)
				return;

			int n = m_back.receiveBytes(m_back_buffer, m_buffer_len);			
			if (n <= 0)
			{
				Close();
				return;
            }
			
			if (m_service)
				m_service->OnBackReadable(m_back_buffer, n);
		}
		catch (Poco::Exception& e)
		{
			LOGE << fmt::format("POCO Sesseion [{}] error : [{}]", front_address(), e.displayText());
			Close();
		}
	}

	void onBackShutdown(Poco::Net::ShutdownNotification* pNf)
	{
		pNf->release();
		LOGI << fmt::format("onShutdown from[{}]", front_address());

		Close();
	}

	void onBackError(Poco::Net::ErrorNotification* pNf)
	{
		pNf->release();
		LOGI << fmt::format("onError from[{}]", front_address());

		Close();
	}

	void onBackTimeout(Poco::Net::TimeoutNotification* pNf)
	{
		pNf->release();
	}

	void onBackIdle(Poco::Net::IdleNotification* pNf)
	{
		pNf->release();
	}

	StreamSocket& front() { return m_front; }
	StreamSocket& back()  { return m_back;  }

	const std::string front_address() {  return m_front.peerAddress().toString();  }
	const std::string back_address()  {  return m_back.peerAddress().toString();   }

private:
	StreamSocket		        m_front;
	StreamSocket		        m_back;
	SocketReactor&	            m_reactor;
	CPOCOProxyHandler*			m_service;
	byte*						m_front_buffer;
	byte*						m_back_buffer;
	const int				    m_buffer_len;
	bool						m_back_connected;
	std::atomic<bool>			m_closed;
};

////////////////////////////////////////////////////////////////////////////////////////
/// class CPOCOProxyHandler

CPOCOProxyHandler::CPOCOProxyHandler(StreamSocket& front, StreamSocket& back)
	: m_front(front)
	, m_back(back)
{
}

CPOCOProxyHandler::~CPOCOProxyHandler()
{
}

////////////////////////////////////////////////////////////////////////////////////////
/// class CPOCOProxyByPassHandler

CPOCOProxyByPassHandler::CPOCOProxyByPassHandler(StreamSocket& front, StreamSocket& back)
	: CPOCOProxyHandler(front, back)
{

}

CPOCOProxyByPassHandler::~CPOCOProxyByPassHandler()
{

}

void CPOCOProxyByPassHandler::OnFrontReadable(byte* buffer, int length)
{	
	back().sendBytes(buffer, length);	
}

void CPOCOProxyByPassHandler::OnBackReadable(byte* buffer, int length)
{
	front().sendBytes(buffer, length);	
}

////////////////////////////////////////////////////////////////////////////////////////
/// class CPOCOProxyAcceptor

class CPOCOProxyAcceptor : public Poco::Net::SocketAcceptor<ProxySession>
{
public:
	CPOCOProxyAcceptor(ServerSocket& socket, SocketReactor& reactor, SocketAddress& back_addr, HandlerCreator creator)
		: Poco::Net::SocketAcceptor<ProxySession>(socket, reactor)
		, m_creator(creator)
		, m_back_addr(back_addr)
	{
	}

	virtual ProxySession* createServiceHandler(StreamSocket& socket)
	{				
		ProxySession* session = new ProxySession(socket, *reactor());
		CPOCOProxyHandler* service = m_creator(session->front(), session->back());
		session->SetService(service, m_back_addr);

		return session;
	}

public:
	HandlerCreator				m_creator;
	Poco::Net::SocketAddress	m_back_addr;
};

////////////////////////////////////////////////////////////////////////////////////////
/// class CPOCOProxyServer

CPOCOProxyServer::CPOCOProxyServer()
	: m_front_port(1470)
	, m_back_ip("localhost")
	, m_back_port(4242)
	, m_thread_running(false)	
{	
}

CPOCOProxyServer::~CPOCOProxyServer()
{
	Deactivate();
}

bool CPOCOProxyServer::Activate(int front_port, const std::string& back_ip, int back_port)
{
	auto handler = [](StreamSocket& front, StreamSocket& back)
	{
		return new CPOCOProxyHandler(front, back);
	};
	return Activate(front_port, back_ip, back_port, handler);
}

bool CPOCOProxyServer::Activate(int front_port, const std::string& back_ip, int back_port, HandlerCreator creator)
{
	LOGI << fmt::format("POCO Rroxy Server Activate FrontPort[{}] BackAddr[{}:{}]", front_port, back_ip, back_port);

	if (true == m_thread_running)
		return false;

	m_front_port = front_port;
	m_back_ip = back_ip;
	m_back_port = back_port;
	m_creator = creator;

	if (m_thread.joinable())
		m_thread.join();

	m_thread = std::thread([this]()
		{
			m_thread_running = true;
			ThreadLoop();
			m_thread_running = false;
		});

	return true;
}

bool CPOCOProxyServer::Deactivate()
{
	LOGI << fmt::format("POCO Rroxy Server Deactivate Port[{}]", m_front_port);

	if (m_reactor)
		m_reactor->stop();

	if (m_thread.joinable())
		m_thread.join();

	return true;
}

void CPOCOProxyServer::ThreadLoop()
{
	LOGI << fmt::format("POCO Rroxy Server Thread Start");

	try
	{
		Poco::Timespan timeout(10, 0);
		Poco::Net::SocketAddress back_addr(m_back_ip, m_back_port);

		m_reactor = std::make_unique<Poco::Net::SocketReactor>();
		m_server_socket = std::make_unique<Poco::Net::ServerSocket>(m_front_port);
		m_acceptor = std::make_unique<CPOCOProxyAcceptor>(*m_server_socket, *m_reactor, back_addr, m_creator);

		m_reactor->run();
	}
	catch (Poco::Exception& e)
	{
		LOGE << fmt::format("POCO Server error : [{}]", e.displayText());
	}

	m_acceptor.reset();
	m_reactor.reset();
	m_server_socket.reset();

	LOGI << fmt::format("POCO Server Thread End");
}

