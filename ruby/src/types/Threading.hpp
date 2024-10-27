#pragma once

#include <cstring>	
#include <mutex>
#include <platform\Platform.hpp>

#define MAXIMUM_TASKS_PER_THREAD 64
#define MAXIMUM_THREADS 32

#define PRIORITY_THREAD_LOW 0x0
#define PRIORITY_THREAD_NORMAL 0x1
#define PRIORITY_THREAD_HIGH 0x2
#define PRIORITY_THREAD_CRITICAL 0x3

#define STATUS_THREAD_DESTROYED 0x0
#define STATUS_THREAD_SUSPENDED 0x1
#define STATUS_THREAD_RUNNING 0x2
#define STATUS_THREAD_RUNNING_JOINED 0x4
#define STATUS_THREAD_STARTING (0x1 << 4)
#define STATUS_THREAD_ENDING (0x2 << 4)
#define STATUS_THREAD_WORKING (0x4 << 4)

namespace Ruby {
	using threadinfo_t = unsigned char;
	using taskprocedure_t = void(*)(void*, void*);

	struct TaskData {
		bool lifetime;

		void* reserved;

		taskprocedure_t procedure;
	};

	class Thread {
#ifdef _WIN32
		static unsigned long __stdcall ThreadProc(void*);
#endif

	public:
		// Constructors/Destructors
		Thread();
		Thread(threadinfo_t);
		Thread(threadinfo_t, threadinfo_t);

		~Thread();

		// Functions
		const threadinfo_t Priority() const;
		const threadinfo_t Priority(threadinfo_t);

		const threadinfo_t State() const;

		const size_t Processed() const;
		const size_t Processing() const;

		const TaskData* Tasks() const;


		bool AddTask(TaskData);
		bool RemoveTask(TaskData);

		bool Suspend();
		bool Resume();

		bool Wait();
		bool Join();

	private:
		threadinfo_t m_priority;
		threadinfo_t m_state;

		unsigned long m_id;

		size_t m_processed;
		size_t m_processing;

		void* m_thread;

		TaskData* m_tasks;

		std::mutex m_guard;
	};

	namespace Threading {
		// Variables
		inline Thread** Threads = nullptr;

		// Functions
		Thread* Create(threadinfo_t, threadinfo_t);
		bool Create(size_t);

		bool Add(TaskData);
		bool Remove(TaskData);

		bool Destroy(Thread*);
		bool Destroy(size_t);
	}
}