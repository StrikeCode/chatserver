#include "chatserver.hpp"
#include "json.hpp"
#include "chatservice.hpp"

#include <iostream>
#include <functional>
#include <string>
using namespace std;
using namespace placeholders;
using json = nlohmann::json;

ChatServer::ChatServer(EventLoop* loop,
            const InetAddress& listenAddr,
            const string& nameArg)
    : _server(loop, listenAddr, nameArg), _loop(loop)
{
    // 注册连接回调
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));

    // 注册消息到来回调
    _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));
    
    // 设置subLoop 的数量
    _server.setThreadNum(4);
}
void ChatServer::start()
{
    _server.start();
}

void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
    if(!conn->connected())
    {
        ChatService::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}

void ChatServer::onMessage(const TcpConnectionPtr &conn,
                    Buffer *buffer,
                    Timestamp time)
{
    string buf = buffer->retrieveAllAsString();

    // 添加json打印代码
    cout << buf << endl;

    // 数据的反序列化
    json js = json::parse(buf);

    // 达到的目的：完全解耦网络模块的代码和业务模块的代码
    // 通过js["msgid"]获取 对应业务的handler -> conn js time
    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());
    // 回调指定的绑定好的事件处理器，来执行相应的业务处理
    msgHandler(conn, js, time);

}
        
