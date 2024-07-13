#include <iostream>
#include "signal_slot.h"

struct Message {
    std::string content;
};

void slot_cb1(const Message& msg) {
    std::cout << "Slot 1 received message: " << msg.content << std::endl;
}

void slot_cb2(const Message& msg) {
    std::cout << "Slot 2 received message: " << msg.content << std::endl;
}

void slot_cb3(const Message& msg, int count) {
    std::cout << "Slot 3 receive message: " << msg.content << "count: " << count << std::endl;
}

int main() {
    design_pattern::Signal<Message> signal;

    auto conn1 = signal.Connect(slot_cb1);
    auto conn2 = signal.Connect(slot_cb2);
    Message msg{"Hello, Signal-Slot!"};
    signal(msg);

    design_pattern::Signal<Message, int> signal2;
    auto con3 = signal2.Connect(slot_cb3);
    signal2(msg, 10);

    // 断开连接
    conn1.Disconnect();

    // 再次触发信号，只有 slot2 应该会收到消息
    signal(msg);

    return 0;
}
