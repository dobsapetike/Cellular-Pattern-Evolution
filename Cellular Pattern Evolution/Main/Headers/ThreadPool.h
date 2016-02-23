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
	vector<thread> threads;
	// the task queue 
	queue<function<void()>> job_queue;

	// synchronization primitives
	mutex schedule_mutex;
	condition_variable schedule_cv, jobs_cv;
	bool running = true;
	unsigned int tasks_in_progress = 0;
public:
	thread_pool(unsigned int thread_count);
	~thread_pool()
	{
		// tell the threads to stop waiting
		running = false;
		schedule_cv.notify_all();
		// and wait for them to finish
		for (unsigned int i = 0; i < threads.size(); ++i)
			threads[i].join();
	}

	unsigned int thread_count() const { return threads.size(); }
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