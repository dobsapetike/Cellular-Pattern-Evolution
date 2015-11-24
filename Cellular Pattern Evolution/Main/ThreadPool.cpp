#include "Headers/ThreadPool.h"

/**
	Constructor creates the number of desired thread
*/
thread_pool::thread_pool(unsigned thread_count)
{
	for (unsigned int i = 0; i < thread_count; ++i)
	{
		_threads.push_back(thread([&]{
			function<void()> task;
			while (true)
			{
				{   // acquire lock
					unique_lock<mutex> lock(_schedule_mutex);

					// look for a work item
					while (_running && _job_queue.empty())
					{
						// if there is none, wait for notification
						_schedule_cv.wait(lock);
					}

					if (!_running) // exit if the pool is stopped
						break;

					// get the task from the queue
					task = _job_queue.front();
					_job_queue.pop();
					// and note that you are working on something
					++_tasks_in_progress;
				}   // release lock

				// execute the task
				task();

				{
					// let them know the task is finished
					unique_lock<mutex> lock(_schedule_mutex);
					--_tasks_in_progress;
					_jobs_cv.notify_one();
				}
			}
		}));
	}
}

/**
	Insert new task into the queue
*/
void thread_pool::enqueue_job(function<void()> const& func)
{
	{ // acquire lock
		unique_lock<mutex> lock(_schedule_mutex);
		// and add the task
		_job_queue.push(func);
	} // release lock

	// wake up one thread
	_schedule_cv.notify_one();
}

/**
	Wait until all the tasks are finished
*/
void thread_pool::wait_for_jobs()
{
	unique_lock<mutex> lock(_schedule_mutex);
	_jobs_cv.wait(lock, [&]{ 
		return _job_queue.size() == 0 && _tasks_in_progress == 0; 
	});
}


