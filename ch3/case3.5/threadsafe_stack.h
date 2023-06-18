#include <exception>
#include <stack>
#include <mutex>
#include <memory>

struct empty_stack : std::exception {
  const char * what() const throw();
};

template<typename T>
class threadsafe_stack {
public:
  threadsafe_stack() {}
  threadsafe_stack(const threadsafe_stack& rhs) {
    std::lock_guard<std::mutex> lg(m_);
    data_ = rhs.data_;
  }

  threadsafe_stack& operator=(const threadsafe_stack& rhs)=delete;

  void push(T value) {
    std::lock_guard<std::mutex> lg(m_);
    data_.push(std::move(value));
  }

  std::shared_ptr<T> pop() {
    std::lock_guard<std::mutex> lg(m_);
    if (data_.empty()) throw empty_stack();
    std::shared_ptr<T> const res(std::make_shared<T>(data_.top()));
    data_.pop();
    return res;
  }

  void pop(T& value) {
    std::lock_guard<std::mutex> lg(m_);
    if (data_.empty()) throw empty_stack();
    value = data_.top();
    data_.pop();
  }

  bool empty() const {
    std::lock_guard<std::mutex> lg(m_);
    return data_.empty();
  }
private:
  std::stack<T> data_;
  mutable std::mutex m_;
};

