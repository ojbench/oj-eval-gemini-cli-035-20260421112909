#ifndef SIMPLE_STRING_SIMPLESTRING_HPP
#define SIMPLE_STRING_SIMPLESTRING_HPP

#include <stdexcept>
#include <cstring>
#include <algorithm>

class MyString {
private:
    union {
        char* heap_ptr;
        char small_buffer[16];
    };
    size_t _size;
    size_t _capacity;

    bool is_small() const {
        return _size <= 15;
    }

    char* get_ptr() {
        return _size <= 15 ? small_buffer : heap_ptr;
    }

    const char* get_ptr() const {
        return _size <= 15 ? small_buffer : heap_ptr;
    }

public:
    MyString() : _size(0), _capacity(15) {
        small_buffer[0] = '\0';
    }

    MyString(const char* s) {
        if (s == nullptr) {
            _size = 0;
            _capacity = 15;
            small_buffer[0] = '\0';
            return;
        }
        _size = strlen(s);
        if (_size <= 15) {
            _capacity = 15;
            memcpy(small_buffer, s, _size + 1);
        } else {
            _capacity = _size;
            heap_ptr = new char[_capacity + 1];
            memcpy(heap_ptr, s, _size + 1);
        }
    }

    MyString(const MyString& other) : _size(other._size), _capacity(other._capacity) {
        if (_size <= 15) {
            memcpy(small_buffer, other.small_buffer, _size + 1);
        } else {
            heap_ptr = new char[_capacity + 1];
            memcpy(heap_ptr, other.heap_ptr, _size + 1);
        }
    }

    MyString(MyString&& other) noexcept : _size(other._size), _capacity(other._capacity) {
        if (other._size <= 15) {
            memcpy(small_buffer, other.small_buffer, _size + 1);
        } else {
            heap_ptr = other.heap_ptr;
        }
        other._size = 0;
        other._capacity = 15;
        other.small_buffer[0] = '\0';
    }

    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            if (_size > 15) {
                delete[] heap_ptr;
            }
            _size = other._size;
            _capacity = other._capacity;
            if (other._size <= 15) {
                memcpy(small_buffer, other.small_buffer, _size + 1);
            } else {
                heap_ptr = other.heap_ptr;
            }
            other._size = 0;
            other._capacity = 15;
            other.small_buffer[0] = '\0';
        }
        return *this;
    }

    MyString& operator=(const MyString& other) {
        if (this != &other) {
            if (other._size <= 15) {
                if (_size > 15) {
                    delete[] heap_ptr;
                }
                _size = other._size;
                _capacity = 15;
                memcpy(small_buffer, other.small_buffer, _size + 1);
            } else {
                if (_size > 15 && _capacity >= other._size) {
                    _size = other._size;
                    memcpy(heap_ptr, other.heap_ptr, _size + 1);
                } else {
                    if (_size > 15) {
                        delete[] heap_ptr;
                    }
                    _size = other._size;
                    _capacity = other._capacity;
                    heap_ptr = new char[_capacity + 1];
                    memcpy(heap_ptr, other.heap_ptr, _size + 1);
                }
            }
        }
        return *this;
    }

    ~MyString() {
        if (_size > 15) {
            delete[] heap_ptr;
        }
    }

    const char* c_str() const {
        return get_ptr();
    }

    size_t size() const {
        return _size;
    }

    size_t capacity() const {
        return _size <= 15 ? 15 : _capacity;
    }

    void reserve(size_t new_capacity) {
        if (new_capacity <= _capacity) return;
        if (_size > 15) {
            char* new_ptr = new char[new_capacity + 1];
            memcpy(new_ptr, heap_ptr, _size + 1);
            delete[] heap_ptr;
            heap_ptr = new_ptr;
        }
        _capacity = new_capacity;
    }

    void resize(size_t new_size) {
        if (new_size > _capacity) {
            reserve(std::max(new_size, _capacity * 2));
        }
        
        if (_size <= 15 && new_size > 15) {
            char* new_ptr = new char[_capacity + 1];
            memcpy(new_ptr, small_buffer, _size);
            if (new_size > _size) {
                memset(new_ptr + _size, '\0', new_size - _size);
            }
            new_ptr[new_size] = '\0';
            heap_ptr = new_ptr;
        } else if (_size > 15 && new_size <= 15) {
            char* old_ptr = heap_ptr;
            memcpy(small_buffer, old_ptr, new_size);
            small_buffer[new_size] = '\0';
            delete[] old_ptr;
            _capacity = 15;
        } else if (_size > 15) {
            if (new_size > _size) {
                memset(heap_ptr + _size, '\0', new_size - _size);
            }
            heap_ptr[new_size] = '\0';
        } else {
            if (new_size > _size) {
                memset(small_buffer + _size, '\0', new_size - _size);
            }
            small_buffer[new_size] = '\0';
        }
        _size = new_size;
    }

    char& operator[](size_t index) {
        if (index >= _size) throw std::out_of_range("Index out of range");
        return get_ptr()[index];
    }

    const char& operator[](size_t index) const {
        if (index >= _size) throw std::out_of_range("Index out of range");
        return get_ptr()[index];
    }

    MyString operator+(const MyString& rhs) const {
        MyString res(*this);
        res.append(rhs.c_str());
        return res;
    }

    void append(const char* str) {
        if (str == nullptr) return;
        size_t len = strlen(str);
        size_t new_size = _size + len;
        if (new_size > _capacity) {
            reserve(std::max(new_size, _capacity * 2));
        }
        
        if (_size <= 15 && new_size > 15) {
            char* new_ptr = new char[_capacity + 1];
            memcpy(new_ptr, small_buffer, _size);
            memcpy(new_ptr + _size, str, len + 1);
            heap_ptr = new_ptr;
        } else if (_size > 15) {
            memcpy(heap_ptr + _size, str, len + 1);
        } else {
            memcpy(small_buffer + _size, str, len + 1);
        }
        _size = new_size;
    }

    const char& at(size_t pos) const {
        if (pos >= _size) throw std::out_of_range("Index out of range");
        return get_ptr()[pos];
    }

    class const_iterator;

    class iterator {
    private:
        char* ptr;
    public:
        iterator(char* p) : ptr(p) {}
        iterator& operator++() {
            ++ptr;
            return *this;
        }
        iterator operator++(int) {
            iterator tmp = *this;
            ++ptr;
            return tmp;
        }
        iterator& operator--() {
            --ptr;
            return *this;
        }
        iterator operator--(int) {
            iterator tmp = *this;
            --ptr;
            return tmp;
        }
        char& operator*() const {
            return *ptr;
        }
        bool operator==(const iterator& other) const {
            return ptr == other.ptr;
        }
        bool operator!=(const iterator& other) const {
            return ptr != other.ptr;
        }
        bool operator==(const const_iterator& other) const;
        bool operator!=(const const_iterator& other) const;
        
        char* get_ptr() const { return ptr; }
    };

    class const_iterator {
    private:
        const char* ptr;
    public:
        const_iterator(const char* p) : ptr(p) {}
        const_iterator& operator++() {
            ++ptr;
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++ptr;
            return tmp;
        }
        const_iterator& operator--() {
            --ptr;
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            --ptr;
            return tmp;
        }
        const char& operator*() const {
            return *ptr;
        }
        bool operator==(const const_iterator& other) const {
            return ptr == other.ptr;
        }
        bool operator!=(const const_iterator& other) const {
            return ptr != other.ptr;
        }
        bool operator==(const iterator& other) const {
            return ptr == other.get_ptr();
        }
        bool operator!=(const iterator& other) const {
            return ptr != other.get_ptr();
        }
        const char* get_ptr() const { return ptr; }
    };

    iterator begin() {
        return iterator(get_ptr());
    }

    iterator end() {
        return iterator(get_ptr() + _size);
    }

    const_iterator cbegin() const {
        return const_iterator(get_ptr());
    }

    const_iterator cend() const {
        return const_iterator(get_ptr() + _size);
    }
};

inline bool MyString::iterator::operator==(const const_iterator& other) const {
    return ptr == other.get_ptr();
}

inline bool MyString::iterator::operator!=(const const_iterator& other) const {
    return ptr != other.get_ptr();
}

#endif