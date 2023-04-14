#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
using namespace muduo;
using namespace muduo::net;

class ChatServer
{
public:
    // init
    ChatServer(EventLoop* loop,
            const InetAddress& listenAddr,
            const string& nameArg);
    
    // 启动服务
    void start();
private:
    // 连接到来时的回调函数
    void onConnection(const TcpConnectionPtr&);

    // 上报读写事件相关信息的回调函数
    void onMessage(const TcpConnectionPtr&,
                    Buffer*,
                    Timestamp);

    TcpServer _server;
    EventLoop *_loop; // 我选择引用和星号根在变量名后,baseLoop
};

#endif