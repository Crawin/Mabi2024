constexpr int BUFSIZE = 512;

enum OVER_TYPE
{
	ACCEPT,
	SEND,
	RECV
};

class OVER
{
public:
	WSAOVERLAPPED m_wsaover;
	WSABUF m_wsabuf;
	char m_sendbuf[BUFSIZE];
	OVER_TYPE type;
public:
	OVER() {
		m_wsabuf.buf = m_sendbuf;
		m_wsabuf.len = BUFSIZE;
		type = RECV;
		ZeroMemory(&m_wsaover, sizeof(m_wsaover));
	}

	OVER(const OVER_TYPE& t) {
		m_wsabuf.buf = m_sendbuf;
		m_wsabuf.len = BUFSIZE;
		type = t;
		ZeroMemory(&m_wsaover, sizeof(m_wsaover));
	}
};