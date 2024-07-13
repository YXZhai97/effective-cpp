
#ifndef SIGNAL_SLOT_H_
#define SIGNAL_SLOT_H_

#include <algorithm>
#include <atomic>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <utility>

/**
 * c++信号槽实现
*/

namespace design_pattern {

template <typename... Args>
class Slot;

template <typename... Args>
class Connection;

template <typename... Args>
class Signal {
 public:
  using Callback = std::function<void(Args...)>;
  using SlotPtr = std::shared_ptr<Slot<Args...>>;
  using SlotList = std::list<SlotPtr>;
  using ConnectionType = Connection<Args...>;

  Signal() {}
  virtual ~Signal() { DisconnectAllSlots(); }

  void operator()(Args... args) {
    SlotList local;
    {
      std::lock_guard<std::recursive_mutex> lock(mutex_);
      local.assign(slots_.begin(), slots_.end());
    }

    if (!local.empty()) {
      for (auto& slot : local) {
        (*slot)(args...);
      }
    }

    ClearDisconnectedSlots();
  }

  ConnectionType Connect(const Callback& cb) {
    auto slot = std::make_shared<Slot<Args...>>(cb);
    {
      std::lock_guard<std::recursive_mutex> lock(mutex_);
      slots_.emplace_back(slot);
    }

    return ConnectionType(slot, this);
  }

  bool Disconnect(const ConnectionType& conn) {
    bool find = false;
    {
      std::lock_guard<std::recursive_mutex> lock(mutex_);
      for (auto& slot : slots_) {
        if (conn.HasSlot(slot)) {
          find = true;
          slot->Disconnect();
        }
      }
    }

    if (find) {
      ClearDisconnectedSlots();
    }
    return find;
  }

  void DisconnectAllSlots() {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    for (auto& slot : slots_) {
      slot->Disconnect();
    }
    slots_.clear();
  }

  bool Empty() const {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return slots_.empty();
  }

 private:
  Signal(const Signal&) = delete;
  Signal& operator=(const Signal&) = delete;

  void ClearDisconnectedSlots() {
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    slots_.erase(
        std::remove_if(slots_.begin(), slots_.end(),
                       [](const SlotPtr& slot) { return !slot->connected(); }),
        slots_.end());
  }

  SlotList slots_;
  mutable std::recursive_mutex mutex_;
};

template <typename... Args>
class Connection {
 public:
  using SlotPtr = std::shared_ptr<Slot<Args...>>;
  using SignalPtr = Signal<Args...>*;

  Connection() : slot_(nullptr), signal_(nullptr) {}
  Connection(const SlotPtr& slot, const SignalPtr& signal)
      : slot_(slot), signal_(signal) {}
  Connection(const Connection& another)
      : slot_(another.slot_), signal_(another.signal_) {}
  Connection(Connection&& another)
      : slot_(std::move(another.slot_)), signal_(std::move(another.signal_)) {}
  virtual ~Connection() {
    slot_ = nullptr;
    signal_ = nullptr;
  }

  Connection& operator=(const Connection& another) {
    if (this != &another) {
      this->slot_ = another.slot_;
      this->signal_ = another.signal_;
    }
    return *this;
  }

  Connection& operator=(Connection&& another) {
    this->slot_ = std::move(another.slot_);
    this->signal_ = std::move(another.signal_);
    return *this;
  }

  bool HasSlot(const SlotPtr& slot) const {
    if (slot != nullptr && slot_ != nullptr) {
      return slot_.get() == slot.get();
    }
    return false;
  }

  bool IsConnected() const {
    if (slot_) {
      return slot_->connected();
    }
    return false;
  }

  bool Disconnect() {
    if (signal_ && slot_) {
      return signal_->Disconnect(*this);
    }
    return false;
  }

 private:
  SlotPtr slot_;
  SignalPtr signal_;
};

template <typename... Args>
class Slot {
 public:
  using Callback = std::function<void(Args...)>;
  Slot(const Slot& another)
      : cb_(another.cb_), connected_({another.connected_.load()}) {}
  explicit Slot(const Callback& cb, bool connected = true)
      : cb_(cb), connected_({connected}) {}
  virtual ~Slot() {}

  void operator()(Args... args) {
    if (connected_.load(std::memory_order_acquire) && cb_) {
      cb_(args...);
    }
  }

  void Disconnect() { connected_.store(false, std::memory_order_release); }
  bool connected() const { return connected_; }

 private:
  Callback cb_;
  std::atomic<bool> connected_ = {true};
};

}  // namespace design_pattern

#endif  // SIGNAL_SLOT_H_
