#pragma once
#include <functional>
#include <list>
#include <mutex>
#include "PoolableThread.h"

class JobManager
{
public:

	JobManager() = delete;
	JobManager(int jobQueueSize);
	~JobManager();


	class Job
	{
	public:
		Job() = delete;
		Job(std::function<void()> func) : dataProcessingFunction(func) {};

		std::function<void()> dataProcessingFunction;
	};

	void AddJobToQueue(Job job);
	void ProcessJobs();

private:
	std::mutex _jobQueueMutex;
	std::list<Job> _jobQueue;
	std::vector<std::unique_ptr<PoolableThread>> _threads;
};

