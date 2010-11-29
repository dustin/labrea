#ifndef LOCKS_HH
#define LOCKS_HH 1

#include <cstring>

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
        int e(0);
        pthread_mutexattr_t attr;

        if (pthread_mutexattr_init(&attr) != 0 ||
            (e = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK)) != 0) {
            std::string message = "MUTEX ERROR: Failed to initialize mutex: ";
            message.append(std::strerror(e));
            throw std::runtime_error(message);
        }

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

// Like a LockHolder, but temporarily releases the lock.
class AntiLockHolder {
public:
    AntiLockHolder(pthread_mutex_t *m) : mutex(m) {
        int e;
        if ((e = pthread_mutex_unlock(mutex)) != 0) {
            std::string message = "MUTEX_ERROR: Failed to release lock: ";
            message.append(std::strerror(e));
            throw std::runtime_error(message);
        }
    }
    ~AntiLockHolder() {
        int e;
        if ((e = pthread_mutex_lock(mutex)) != 0) {
            std::string message = "MUTEX ERROR: Failed to acquire lock: ";
            message.append(std::strerror(e));
            throw std::runtime_error(message);
        }
    }
private:
    pthread_mutex_t *mutex;
};

}

#endif /* LOCKS_HH */
