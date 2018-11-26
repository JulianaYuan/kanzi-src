#ifndef _H_SYNC_QUE_H
#define _H_SYNC_QUE_H

#include<list>
#include<mutex>
#include<thread>
#include<condition_variable>
#include<iostream>

template<typename T>
class SyncQueue
{
public:
    SyncQueue(int maxSize) :m_maxSize(maxSize), m_needStop(false)
    {
    }

    void Put(const T&x)
    {
        Add(x);
    }

    void Put(T&&x)
    {
        Add(std::forward<T>(x));
    }

    bool TryPut(const T&x)
    {
    	return TryAdd(x);
    }

    bool TryPut(T&&x)
    {
    	return TryAdd(std::forward<T>(x));
    }

    void Take(std::list<T>& list)
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_notEmpty.wait(locker, [this]{return m_needStop || NotEmpty(); });
        
        if (m_needStop)
            return;
        list = std::move(m_queue);
        m_notFull.notify_one();
    }

    void Take(T& t)
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_notEmpty.wait(locker, [this]{return m_needStop || NotEmpty(); });
        
        if (m_needStop)
            return;
        t = m_queue.front();
        m_queue.pop_front();
        m_notFull.notify_one();
    }
    bool TryTake(T& t)
    {
    	std::unique_lock<std::mutex> locker(m_mutex);
    	if (m_needStop || NotEmpty())
    	{
    		if (m_needStop)
    			return true;

    		t = m_queue.front();
    		m_queue.pop_front();
    		m_notFull.notify_one();
    		return true;
    	}
    	return false;
    }

    void Stop()
    {
        {
            std::lock_guard<std::mutex> locker(m_mutex);
            m_needStop = true;
        }
        m_notFull.notify_all();
        m_notEmpty.notify_all();
    }

    bool Empty()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.empty();
    }

    bool Full()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.size() == m_maxSize;
    }

    size_t Size()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.size();
    }

    int Count()
    {
        return m_queue.size();
    }
private:
    bool NotFull() const
    {
        bool full = m_queue.size() >= m_maxSize;
        return !full;
    }

    bool NotEmpty() const
    {
        bool empty = m_queue.empty();
        return !empty;
    }

    template<typename F>
    void Add(F&&x)
    {
        std::unique_lock< std::mutex> locker(m_mutex);
        m_notFull.wait(locker, [this]{return m_needStop || NotFull(); });
        if (m_needStop)
            return;

        m_queue.push_back(std::forward<F>(x));
        m_notEmpty.notify_one();
    }

    template<typename F>
	bool TryAdd(F&&x)
	{
		std::unique_lock< std::mutex> locker(m_mutex);
		if (m_needStop || NotFull())
		{
			if (m_needStop)
					return true;

			m_queue.push_back(std::forward<F>(x));
			m_notEmpty.notify_one();
			return true;
		}
		else
		{
			return false;
		}

	}
private:

    //缓冲区
    std::list<T> m_queue;

    //互斥量和条件变量结合起来使用
    std::mutex m_mutex;

    //不为空的条件变量
    std::condition_variable m_notEmpty;

    //没有满的条件变量
    std::condition_variable m_notFull;

    //同步队列最大的size
    int m_maxSize;

    //停止的标志
    bool m_needStop;
};

#endif
