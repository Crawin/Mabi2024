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
	SOCKET sock;
public:
	OVER() {
		m_wsabuf.buf = m_sendbuf;
		m_wsabuf.len = BUFSIZE;
		type = RECV;
		sock = NULL;
		ZeroMemory(m_sendbuf, BUFSIZE);
		ZeroMemory(&m_wsaover, sizeof(m_wsaover));
	}

	OVER(const OVER_TYPE& t, const SOCKET& s) {
		m_wsabuf.buf = m_sendbuf;
		m_wsabuf.len = BUFSIZE;
		type = t;
		sock = s;
		ZeroMemory(m_sendbuf, BUFSIZE);
		ZeroMemory(&m_wsaover, sizeof(m_wsaover));
	}
};