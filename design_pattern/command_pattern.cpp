#include <iostream>
#include <memory>
#include <queue>

// Command 接口
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() const = 0;
};

// Light 类
class Light {
public:
    Light() {
        std::cout << "Light Construct" <<std::endl;
    }

    ~Light() {
        std::cout << "Light Deconstruct" <<std::endl;
    }

    void on() const {
        std::cout << "The light is on" <<std::endl;
    }

    void off() const {
        std::cout << "The light is off" << std::endl;
    }
};

// Light 的具体命令类
class LightOnCommand : public Command {
public:
    explicit LightOnCommand(const Light& light) : light_(light) {}

    void execute() const override {
        light_.on();
    }

private:
    const Light& light_;
};

class LightOffCommand : public Command {
public:
    explicit LightOffCommand(const Light& light) : light_(light) {}

    void execute() const override {
        light_.off();
    }

private:
    const Light& light_;
};

// Stereo 类
class Stereo {
public:
    void on() const {
        std::cout << "The stereo is on\n";
    }

    void off() const {
        std::cout << "The stereo is off\n";
    }

    void setCD() const {
        std::cout << "The stereo is set for CD input\n";
    }

    void setVolume(int volume) const {
        std::cout << "The stereo volume is set to " << volume << "\n";
    }
};

// Stereo 的具体命令类
class StereoOnWithCDCommand : public Command {
public:
    explicit StereoOnWithCDCommand(const Stereo& stereo) : stereo_(stereo) {}

    void execute() const override {
        stereo_.on();
        stereo_.setCD();
        stereo_.setVolume(11);
    }

private:
    const Stereo& stereo_;
};

class StereoOffCommand : public Command {
public:
    explicit StereoOffCommand(const Stereo& stereo) : stereo_(stereo) {
        std::cout << "StereoOffCommand construct" << std::endl;
    }

    ~StereoOffCommand() {
        std::cout << "StereoOffCommand deconstruct" << std::endl;
    }

    void execute() const override {
        stereo_.off();
    }

private:
    const Stereo& stereo_;
};

// 调用者类
class RemoteControl {
public:
    void addCommand(std::unique_ptr<Command> command) {
        commands_.push(std::move(command));
    }

    void executeCommands() {
        while (!commands_.empty()) {
            commands_.front()->execute();
            commands_.pop();
        }
    }

private:
    std::queue<std::unique_ptr<Command>> commands_;
};

// 客户端代码
void clientCode() {
    Light livingRoomLight;
    Stereo livingRoomStereo;

    auto lightOnCommand = std::make_unique<LightOnCommand>(livingRoomLight);
    auto lightOffCommand = std::make_unique<LightOffCommand>(livingRoomLight);
    auto stereoOnCommand = std::make_unique<StereoOnWithCDCommand>(livingRoomStereo);
    auto stereoOffCommand = std::make_unique<StereoOffCommand>(livingRoomStereo);

    RemoteControl remote;
    remote.addCommand(std::move(lightOnCommand));
    remote.addCommand(std::move(stereoOnCommand));
    remote.addCommand(std::move(lightOffCommand));
    remote.addCommand(std::move(stereoOffCommand));
    remote.executeCommands();
}

int main() {
    std::cout << "start clientCode" <<std::endl;
    clientCode();
    std::cout << "end clientCode" << std::endl;
    return 0;
}
