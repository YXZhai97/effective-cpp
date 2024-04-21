#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <chrono>
#include <iomanip>
#include <ctime>

std::mutex g_mutex;

class MessageSender {
public:
    using callback_t = std::function<std::string(const std::string &message)>;
    MessageSender() = default;

    void sendMessage(const std::string &message) {
        std::lock_guard<std::mutex> lock(g_mutex);
        std::string send_mg = message;
        if (send_callback_ != nullptr) {
            send_mg = send_callback_(message);
        } 
        messages_.push(send_mg);
        std::cout << "sender sendMessage " << send_mg << std::endl;
    }

    bool hasMessages() const {
        std::lock_guard<std::mutex> lock(g_mutex);
        return !messages_.empty();
    }

    std::string receiveMessage() {
        std::lock_guard<std::mutex> lock(g_mutex);
        if (!messages_.empty()) {
            std::string message = messages_.front();
            messages_.pop();
            return message;
        }
        return "";
    }

    void setMessageSenderFunc(const callback_t &cb) {
        send_callback_ = cb;
    }


    void startSending(std::string &message) {
        sendingThread_ = std::thread([this, &message] {
            while (true) {
                sendMessage(message);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        });
    }

    void joinSendingThread() {
        sendingThread_.join();
    }

private:
    std::queue<std::string> messages_;
    // std::mutex mutex_;
    std::thread sendingThread_;
    callback_t send_callback_;

};

class MessageProcessor {
public:
    using callback_t = std::function<void(const std::string &message)>;

    MessageProcessor() = default;

    void processMessages(MessageSender &sender) {
        while (true) {
            if (sender.hasMessages()) {
                std::string message = sender.receiveMessage();
                if (!message.empty()) {
                    if (onNewMessageCallback_ != nullptr) {
                        onNewMessageCallback_(message);
                    }
                } else {
                    std::cout << "message not valid" << std::endl;
                }
            } else {
                std::cout << "message not ready" << std::endl;
            }
            // Add some delay to avoid busy-waiting
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void setOnNewMessageHandler(const callback_t &cb) {
        onNewMessageCallback_ = cb;
    }

    void startProcessing(MessageSender &sender) {
        processingThread_ = std::thread([this, &sender] {
            processMessages(sender);
        });
    }

    void joinProcessingThread() {
        if (processingThread_.joinable()){
            processingThread_.join();
        }
    }

private:
    callback_t onNewMessageCallback_;
    std::thread processingThread_;
};

int main() {
    MessageSender sender;
    MessageProcessor processor;

    auto f= [](const std::string& message ){
        std::cout << "this is from sender "<<std::endl;
        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::string new_message = message + std::to_string(currentTime);
        return new_message;
    };
    sender.setMessageSenderFunc(f);

    // Register callback for new messages
    processor.setOnNewMessageHandler([](const std::string &message) {
        std::cout << "Processor Received message: " << message << std::endl;
    });

    // Start sending and processing threads
    std::string message = "hello from sender";
    sender.startSending(message);
    processor.startProcessing(sender);

    // Join the threads
    sender.joinSendingThread();
    processor.joinProcessingThread();

    return 0;
}
