template<typename T>
control_block<T>::control_block(T* ptr) : m_ptr(ptr), shared_count(1), weak_count(0) {}

template<typename T>
control_block<T>::control_block() : m_ptr(nullptr), shared_count(0), weak_count(0) {}

template<typename T>
void control_block<T>::add_shared() {
    ++shared_count;
}

template<typename T>
void control_block<T>::add_weak() {
    ++weak_count;
}

template<typename T>
T* control_block<T>::get_ptr() const {
    return m_ptr;
}

template<typename T>
int control_block<T>::get_shared_count() const {
    return shared_count;
}

template<typename T>
int control_block<T>::get_weak_count() const {
    return weak_count;
}

template<typename T>
void control_block<T>::release_shared() {
    if (shared_count) {
        if ((--shared_count) == 0) {
            delete m_ptr;
            m_ptr = nullptr;
            if (weak_count == 0) {
                delete this;
            }
        }
    }
}

template<typename T>
void control_block<T>::release_weak() {
    if ((--weak_count) == 0 && shared_count == 0) {
        delete this;
    }
}

template <typename T>
shared_ptr<T>::shared_ptr(T* ptr) : m_ptr(ptr), block(nullptr) {
    if (m_ptr) {
        block = new control_block<T>(ptr);
    }
}

template <typename T>
constexpr shared_ptr<T>::shared_ptr() : m_ptr(nullptr), block(nullptr) {}

template <typename T>
shared_ptr<T>::shared_ptr(const shared_ptr<T>& other) : m_ptr(other.m_ptr), block(other.block) {
    if (block) {
        block->add_shared();
    }
}

template <typename T>
shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr<T>& other) {
    if (this != &other) {
        if (block) {
            block->release_shared();
        }
        m_ptr = other.m_ptr;
        block = other.block;
        if (block) {
            block->add_shared();
        }
    }
    return *this;
}

template <typename T>
shared_ptr<T>::shared_ptr(shared_ptr<T>&& other) noexcept : m_ptr(other.m_ptr), block(other.block) {
    other.m_ptr = nullptr;
    other.block = nullptr;
}

template <typename T>
shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr<T>&& other) noexcept {
    if (this != &other) {
        if (block) {
            block->release_shared();
        }
        m_ptr = other.m_ptr;
        block = other.block;
        other.m_ptr = nullptr;
        other.block = nullptr;
    }
    return *this;
}

template <typename T>
shared_ptr<T>::shared_ptr(const weak_ptr<T>& weak) : m_ptr(weak.m_ptr), block(weak.block) {
    if (block) {
        block->add_shared();
    }
}

template <typename T>
shared_ptr<T>& shared_ptr<T>::operator=(std::nullptr_t) noexcept {
    if (block) {
        block->release_shared();
    }
    m_ptr = nullptr;
    block = nullptr;
    return *this;
}

template <typename T>
T* shared_ptr<T>::operator->() const noexcept {
    return m_ptr;
}

template <typename T>
T& shared_ptr<T>::operator*() const noexcept {
    return *m_ptr;
}

template <typename T>
T* shared_ptr<T>::get() const noexcept {
    return m_ptr;
}

template <typename T>
shared_ptr<T>::operator bool() const {
    return m_ptr != nullptr;
}

template <typename T>
int shared_ptr<T>::use_count() const {
    return block ? block->get_shared_count() : 0;
}

template <typename T>
shared_ptr<T>::~shared_ptr() {
    if (block) {
        block->release_shared();
    }
}

template <typename T>
constexpr weak_ptr<T>::weak_ptr() : m_ptr(nullptr), block(nullptr) {}

template <typename T>
weak_ptr<T>::weak_ptr(const shared_ptr<T>& shared) : m_ptr(shared.m_ptr), block(shared.block) {
    if (block) {
        block->add_weak();
    }
}

template <typename T>
weak_ptr<T>::weak_ptr(const weak_ptr<T>& other) : m_ptr(other.m_ptr), block(other.block) {
    if (block) {
        block->add_weak();
    }
}

template <typename T>
weak_ptr<T>& weak_ptr<T>::operator=(const weak_ptr<T>& other) {
    if (this != &other) {
        if (block) {
            block->release_weak();
        }
        m_ptr = other.m_ptr;
        block = other.block;
        if (block) {
            block->add_weak();
        }
    }
    return *this;
}

template <typename T>
weak_ptr<T>::weak_ptr(weak_ptr<T>&& other) noexcept : m_ptr(other.m_ptr), block(other.block) {
    other.m_ptr = nullptr;
    other.block = nullptr;
}

template <typename T>
weak_ptr<T>& weak_ptr<T>::operator=(weak_ptr<T>&& other) noexcept {
    if (this != &other) {
        if (block) {
            block->release_weak();
        }
        m_ptr = other.m_ptr;
        block = other.block;
        other.m_ptr = nullptr;
        other.block = nullptr;
    }
    return *this;
}

template <typename T>
weak_ptr<T>::~weak_ptr() {
    if (block) {
        block->release_weak();
    }
}

template <typename T>
shared_ptr<T> weak_ptr<T>::lock() const {
    if (block && block->get_shared_count() > 0) {
        return shared_ptr<T>(*this);
    }
    return shared_ptr<T>();
}
