#include <muduo/net/TcpServer.h>
#include <muduo/base/Logging.h>
#include <functional>
#include <muduo/net/EventLoop.h>
#include <iostream>

using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;

/*
muduo网络库给用户提供了两个主要的类
TcpServer
TcpClient
*/

/*
基于muduo网络库开发服务器程序
    1. 组合TcpServer对象
    2. 创建EventLoop事件循环对象的指针（相当于epoll）
    3. 明确TcpServer构造函数需要什么参数，输出ChatServer构造
    4. 在当前服务器类的构造函数中，注册处理连接的回调函数和处理读写事件的回调函数
    5. 设置合适的服务端线程数量，muduo会自己划分I/O线程和worker线程
*/

// 用muduo库实现一个回声服务器
class EchoServer
{
public:
    EchoServer(EventLoop *loop,               // 事件循环
               const InetAddress &listenAddr, // IP Port
               const string &nameArg)         // 服务器名字
        : _server(loop, listenAddr, nameArg), _loop(loop)
    {
        // 给服务器注册用户连接和断开的回调
        _server.setConnectionCallback(std::bind(&EchoServer::onConnection, this, _1));

        // 给服务器注册用户读写事件的回调
        _server.setMessageCallback(std::bind(&EchoServer::onMessage, this, _1, _2, _3));

        // 设置服务端的线程数量，内部分一个给IO线程，其他为业务处理线程
        // 1个IO线程，3个Worker线程
        _server.setThreadNum(4);
    }

    // 开启事件循环
    void start()
    {
        _server.start();
    }

private:
    // 专门处理用户的连接创建和断开 epoll listenfd accept
    void onConnection(const TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            LOG_INFO << conn->peerAddress().toIpPort() << " -> "
                     << conn->localAddress().toIpPort() << " state:online";
        }
        else
        {
            LOG_INFO << conn->peerAddress().toIpPort() << " -> "
                     << conn->localAddress().toIpPort() << " state:offline";
            conn->shutdown();
        }
        // LOG_INFO << conn->peerAddress().toIpPort() << " -> "
        //          << conn->localAddress().toIpPort() << " is "
        //          << (conn->connected() ? "UP" : "DOWN");
    }
    // 专门处理用户的读写事件
    void onMessage(const TcpConnectionPtr &conn, // 连接
                   Buffer *buf,                  // 缓冲区
                   Timestamp time)               // 时间戳
    {
        muduo::string msg(buf->retrieveAllAsString());
        LOG_INFO << conn->name() << " echo " << msg.size() << "bytes, "
                 << "data recived at " << time.toString();
        conn->send(msg); // echo
    }

    TcpServer _server; // #1
    EventLoop *_loop;  // #2 epoll
};

int main()
{
    EventLoop loop; // epoll
    InetAddress addr("127.0.0.1", 6000);
    EchoServer server(&loop, addr, "EchoServer");

    server.start(); // listen , 将listenfd传入epoll_ctl,上epoll内核事件表
    loop.loop();    // epoll_wait 阻塞方式等待新用户连接，或已连接用户的读写事件
    return 0;
}