#include "Headers/ThreadPool.h"

/**
	Constructor creates the number of desired thread
*/
thread_pool::thread_pool(unsigned thread_count)
{
	for (unsigned int i = 0; i < thread_count; ++i)
	{
		threads.push_back(thread([&]{
			function<void()> task;
			while (true)
			{
				{   // acquire lock
					unique_lock<mutex> lock(schedule_mutex);

					// look for a work item
					while (running && job_queue.empty())
					{
						// if there is none, wait for notification
						schedule_cv.wait(lock);
					}

					if (!running) // exit if the pool is stopped
						break;

					// get the task from the queue
					task = job_queue.front();
					job_queue.pop();
					// and note that you are working on something
					++tasks_in_progress;
					//task();
				}   // release lock

				// execute the task
				task();

				{
					// let them know the task is finished
					unique_lock<mutex> lock(schedule_mutex);
					--tasks_in_progress;
					jobs_cv.notify_one();
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
		unique_lock<mutex> lock(schedule_mutex);
		// and add the task
		job_queue.push(func);
	} // release lock

	// wake up one thread
	schedule_cv.notify_one();
}

/**
	Wait until all the tasks are finished
*/
void thread_pool::wait_for_jobs()
{
	unique_lock<mutex> lock(schedule_mutex);
	jobs_cv.wait(lock, [&]{ 
		return job_queue.size() == 0 && tasks_in_progress == 0; 
	});
}


