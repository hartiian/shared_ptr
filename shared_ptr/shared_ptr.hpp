#ifndef SHARED_H
#define SHARED_H

#include <iostream>

template<typename T>
class weak_ptr;

template<typename T>
class shared_ptr;

template<typename T>
class control_block {
private:
    int shared_count;
    int weak_count;
    T* m_ptr;

    friend class weak_ptr<T>;
    friend class shared_ptr<T>;

public:
    control_block(T* ptr);
    control_block();
    void add_shared();
    void add_weak();
    T* get_ptr() const;
    int get_shared_count() const;
    int get_weak_count() const;
    void release_shared();
    void release_weak();
};

template <typename T>
class shared_ptr {
private:
    T* m_ptr;
    control_block<T>* block;

    friend class weak_ptr<T>;

public:
    shared_ptr(T* ptr);
    constexpr shared_ptr();
    shared_ptr(const shared_ptr<T>& other);
    shared_ptr<T>& operator=(const shared_ptr<T>& other);
    shared_ptr(shared_ptr<T>&& other) noexcept;
    shared_ptr<T>& operator=(shared_ptr<T>&& other) noexcept;
    explicit shared_ptr(const weak_ptr<T>& weak);
    shared_ptr& operator=(std::nullptr_t) noexcept;
    T* operator->() const noexcept;
    T& operator*() const noexcept;
    T* get() const noexcept;
    explicit operator bool() const;
    int use_count() const;
    ~shared_ptr();
};

template <typename T>
class weak_ptr {
private:
    T* m_ptr;
    control_block<T>* block;

    friend class shared_ptr<T>;

public:
    constexpr weak_ptr();
    weak_ptr(const shared_ptr<T>& shared);
    weak_ptr(const weak_ptr<T>& other);
    weak_ptr& operator=(const weak_ptr<T>& other);
    weak_ptr(weak_ptr&& other) noexcept;
    weak_ptr& operator=(weak_ptr&& other) noexcept;
    ~weak_ptr();
    shared_ptr<T> lock() const;
};

#include "shared.cpp"

#endif 
