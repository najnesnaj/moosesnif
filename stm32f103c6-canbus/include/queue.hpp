#ifndef CONTROLLER_QUEUE_H
#define CONTROLLER_QUEUE_H

template <typename T, int size> class Queue {
public:
  bool push(T t) {
    if (full())
      return false;
    _buffer[_write_index++] = t;
    _write_index = wrap(_write_index);
    return true;
  }
  bool pop(T &out) {
    if (!available())
      return false;
    out = _buffer[_read_index++];
    _read_index = wrap(_read_index);
    return true;
  }
  bool available() { return _write_index != _read_index; }
  bool full() { return wrap(_write_index + 1) == _read_index; }

private:
  T _buffer[size];
  volatile int _read_index = 0;
  volatile int _write_index = 0;
  int wrap(int index) { return index >= size ? 0 : index; }
};

#endif // ifndef CONTROLLER_QUEUE_H
