#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>

using namespace std;

/**
	Simple threadpool implementation 
*/
class thread_pool
{
private:
	// list of threads in the pool
	vector<thread> _threads;
	// the task queue 
	queue<function<void()>> _job_queue;

	// synchronization primitives
	mutex _schedule_mutex;
	condition_variable _schedule_cv, _jobs_cv;
	bool _running = true;
	unsigned int _tasks_in_progress = 0;
public:
	thread_pool(unsigned int thread_count);
	~thread_pool()
	{
		// tell the threads to stop waiting
		_running = false;
		_schedule_cv.notify_all();
		// and wait for them to finish
		for (unsigned int i = 0; i < _threads.size(); ++i)
			_threads[i].join();
	}

	unsigned int thread_count() const { return _threads.size(); }
	/**
		Insert new task into the queue
	*/
	void enqueue_job(function<void()> const& func);
	/**
		Wait until all the tasks are finished
	*/
	void wait_for_jobs();
};

#endif