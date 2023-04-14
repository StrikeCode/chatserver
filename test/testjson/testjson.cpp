#include "json.hpp"
using json = nlohmann::json;

#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

// 序列化
string func1()
{
    // 可以认为底层用链式哈希表存储，输出后不一定顺序
    json js;
    js["msg_type"] = 2;
    js["from"] = "zhang san";
    js["to"] = "wang wu";
    js["msg"] = "hello, what are you doing";

    string sendBuf = js.dump();

    // cout << sendBuf.c_str() << endl;
    return sendBuf;
}

// 序列化
void func2()
{
    json js;
    // 添加数组
    js["id"] = {1, 2, 3, 4, 5};
    // 添加key-value
    js["name"] = "zhangsan";
    // 添加对象
    js["msg"]["zhangsan"] = "hello world";
    js["msg"]["liu shuo"] = "hello Beijing";
    // 上面等同于下面一次性添加数组对象
    js["msg"] = {{"zhangsan", "hello world"}, {"liu shuo", "hello Beijing"}};
    cout << js << endl;
    // return js.dump();
}

// 容器序列化
string func3()
{
    json js;
    vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(6);
    js["list"] = vec;

    // 序列化一个map容器
    map<int, string> m;
    m.insert({1, "黄山"});
    m.insert({2, "华山"});
    m.insert({3, "泰山"});
    js["path"] = m;

    string sendBuf = js.dump(); // json数据对象 -> 序列化 json字符串
    return sendBuf;
    // cout << sendBuf.c_str() << endl;
}

int main()
{
    // func1();
    func2();
    // func3();

    // string recvBuf = func3();
    // json jsbuf = json::parse(recvBuf);
    // cout << jsbuf["msg_type"] << endl;
    // cout << jsbuf["from"] << endl;
    // cout << jsbuf["to"] << endl;
    // cout << jsbuf["msg"] << endl;

    // cout << jsbuf["id"] << endl;
    // auto arr = jsbuf["id"];
    // cout << arr[2] << endl;

    // auto msgjs = jsbuf["msg"];
    // cout << msgjs["zhangsan"] << endl;
    // cout << msgjs["liu shuo"] << endl;

    // vector<int> vec = jsbuf["list"];
    // for (int &v : vec)
    // {
    //     cout << v << " ";
    // }
    // cout << endl;

    // map<int, string> mymap = jsbuf["path"];
    // for(auto &p : mymap)
    // {
    //     cout << p.first << " " << p.second << endl;
    // }
    return 0;
}