#include <functional>
#include <iostream>
 
class MyTest{
using callback_t = std::function<void(const int &a, const int &b)>;
using callback_m_t = std::function<void(const std::string &message)>;
public:
    MyTest() = default;
    void doCalc(int a, int b){
        if(myCallBack!= nullptr){
            myCallBack(a,b);
        }
    }

    void doProcessMessage(std::string &message) {
        if (onNewMessageCallback != nullptr) {
            onNewMessageCallback(message);
        }
    }
 
    void setCallBackHandler(const callback_t &cb){
        myCallBack = cb;
    }

    void setOnNewMessageHandler(const callback_m_t &cb) {
        onNewMessageCallback = cb;
    }
 
private:
    callback_t myCallBack;
    callback_m_t onNewMessageCallback;
};
 
void handleCallBack(const int &a,const int &b){
    std::cout << "this is from callback handleCallBack"<<std::endl;
    std::cout << "a: " << a << " b: " <<b << std::endl;
    
}
 
int main(){
 
    MyTest t;
 
    // lamda 回调函数
    auto f= [](const int &a,const int &b){
        std::cout << "this is from callback f"<<std::endl;
        std::cout << "a: " << a << " b: " <<b << std::endl;
    };
    // register callback
    // 写法一
    t.setCallBackHandler(f);
 
    // 写法二
    t.setCallBackHandler([&f](auto &&a, auto &&b) {
        f(std::forward<decltype(a)>(a), std::forward<decltype(b)>(b));
    });
 
    // 写法三
    t.setCallBackHandler([](auto &&a, auto &&b) {
        handleCallBack(std::forward<decltype(a)>(a), std::forward<decltype(b)>(b));
    });

    // lamda onnewmessage callback
    auto on_new_message = [](const std::string &message) {
        std::cout << "on new message: " << message << std::endl;
    };

    t.setOnNewMessageHandler(on_new_message);
    std::string message = "hello world";

    // do callback
    t.doCalc(1, 2);
    t.doProcessMessage(message);
}