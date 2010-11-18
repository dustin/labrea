#ifndef LOCKS_HH
#define LOCKS_HH 1

#include <pthread.h>

namespace labrea {

class LockHolder {
public:
    LockHolder(pthread_mutex_t *m) : mutex(m), locked(false) {
        lock();
    }
    ~LockHolder() {
        unlock();
    }
    void lock() {
        int e;
        if ((e = pthread_mutex_lock(mutex)) != 0) {
            std::string message = "MUTEX ERROR: Failed to acquire lock: ";
            message.append(std::strerror(e));
            throw std::runtime_error(message);
        }
        locked = true;
    }
    void unlock() {
        if (locked) {
            locked = false;
            int e;
            if ((e = pthread_mutex_unlock(mutex)) != 0) {
                std::string message = "MUTEX_ERROR: Failed to release lock: ";
                message.append(std::strerror(e));
                throw std::runtime_error(message);
            }
        }
    }
private:
    pthread_mutex_t *mutex;
    bool locked;
};

}

#endif /* LOCKS_HH */
