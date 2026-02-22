#include <iostream>
#include <semaphore>
#include <thread>
#include <atomic>
#include <vector>

class RecursiveMutexSem {
public:
    // Initialize binary semaphore with 1 (available)
    RecursiveMutexSem() : m_sem(1), m_owner_id(), m_lock_count(0) {}

    void lock()
    {
        std::thread::id current_tid = std::this_thread::get_id();

        if (m_owner_id == current_tid)
        {
            // Recursive case: same thread already has the lock
            m_lock_count++;
            std::cout << "Lock count: " << m_lock_count << std::endl;
            return;
        }

        // New thread: must wait for the semaphore
        m_sem.acquire();
        m_owner_id = current_tid;
        m_lock_count = 1;
    }

    void unlock()
    {
        if (m_owner_id != std::this_thread::get_id())
        {
            std::cout << "Unlock failed... continue thread." << std::endl;
            return; // Only the owner can unlock
        }

        m_lock_count--;
        std::cout << "Lock count: " << m_lock_count << std::endl;
        if (m_lock_count == 0)
        {
            // Truly release the lock for other threads
            m_owner_id = std::thread::id(); // Reset ID
            m_sem.release();
        }
    }

private:
    std::binary_semaphore m_sem;
    std::thread::id m_owner_id;
    int m_lock_count;
};

RecursiveMutexSem recursive_mutex;
int shared_counter = 0;

void recursive_increment(int depth) {
    recursive_mutex.lock();
    shared_counter++;

    std::cout << "Thread " << std::this_thread::get_id() 
              << " locked at depth " << depth 
              << ". Counter: " << shared_counter << std::endl;

    if (shared_counter < depth)
    {
        recursive_increment(depth); // <--- RECURSIVE CALL
    }

    shared_counter = 0;
    recursive_mutex.unlock();
}

auto main() -> int
{
    std::vector<std::thread> workers;
    const int NUM_THREADS = 10;
    const int DEPTH = 100; // Each thread locks 3 times total (0, 1, 2)

    std::cout << "[Main] Starting recursive threads..." << std::endl;

    // Create two threads that both try to do recursive work
    // 2. Launch threads and push them into the vector
    for (int i = 0; i < NUM_THREADS; ++i) {
        workers.emplace_back(recursive_increment, DEPTH);
    }

    for (std::thread& t : workers) {
        t.join();
    }

    std::cout << "[Main] All done. Final Counter: " << shared_counter << std::endl;
    return 0;
}