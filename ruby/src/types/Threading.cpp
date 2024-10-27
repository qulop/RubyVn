#include "threading.hpp"

namespace Ruby {
#ifdef RUBY_MSVC_USED
	unsigned long __stdcall Thread::ThreadProc(void* arg) {
#endif
		Thread* self = reinterpret_cast<Thread*>(arg);

		loop {
			threadinfo_t lostate = self->m_state & 0xF;

			self->m_state		= STATUS_THREAD_STARTING | lostate;
			self->m_processed	= self->m_processing;

			if (lostate == STATUS_THREAD_DESTROYED)
				break;
			else if (lostate == STATUS_THREAD_SUSPENDED)
				continue;

			self->m_guard.lock();

			self->m_state		= STATUS_THREAD_WORKING | lostate;
			self->m_processing	= 0U;

			TaskData* tasks = self->m_tasks;
			for (size_t ind = 0U; ind < MAXIMUM_TASKS_PER_THREAD; ind++, tasks++) {
				if (lostate <= STATUS_THREAD_SUSPENDED)
					break;

				// Checking for valid memory
				uintptr_t checksum = *reinterpret_cast<uintptr_t*>(tasks);
				if (checksum == 0U)
					continue;

				self->m_processing++;

				// Calling task's procedure
				TaskData task = *tasks;
				task.procedure(&task, task.reserved);

				if (task.lifetime)
					continue;

				std::memset(self->m_tasks + ind, 0U, sizeof(TaskData));
			}

			self->m_guard.unlock();

			self->m_state = STATUS_THREAD_ENDING | lostate;
		}

		return EXIT_SUCCESS;
	}

	Thread::Thread() {
		m_priority	= PRIORITY_THREAD_NORMAL;
		m_state		= STATUS_THREAD_SUSPENDED;

		m_processed = m_processing = 0U;
		m_id = 0U;

		// Allocating memory for tasks
		m_tasks = new TaskData[MAXIMUM_TASKS_PER_THREAD];
		std::memset(m_tasks, 0U, MAXIMUM_TASKS_PER_THREAD * sizeof(TaskData));

		m_thread = Ruby::Platform::createThread(
				&ThreadProc,
				static_cast<void*>(this),
				0,
				true,
				&m_id
			);
		if (m_thread == nullptr)
			delete this;
	}

	Thread::Thread(threadinfo_t state) {
		m_priority	= PRIORITY_THREAD_NORMAL;
		m_state		= state & 0xF;

		m_processed = m_processing = 0U;
		m_id = 0U;

		// Allocating memory for tasks
		m_tasks = new TaskData[MAXIMUM_TASKS_PER_THREAD];
		std::memset(m_tasks, 0U, sizeof(TaskData) * MAXIMUM_TASKS_PER_THREAD);

		m_thread = Ruby::Platform::createThread(
			&ThreadProc,
			static_cast<void*>(this),
			0,
			state == STATUS_THREAD_SUSPENDED,
			&m_id
		);
		if (m_thread == nullptr)
			delete this;
	}

	Thread::Thread(threadinfo_t state, threadinfo_t priority) {
		m_priority	= priority;
		m_state		= state;

		m_processed = m_processing = 0U;
		m_id = 0U;

		// Allocating memory for tasks & filling it
		m_tasks = new TaskData[MAXIMUM_TASKS_PER_THREAD];
		std::memset(m_tasks, 0U, sizeof(TaskData) * MAXIMUM_TASKS_PER_THREAD);

		m_thread = Ruby::Platform::createThread(
			&ThreadProc,
			static_cast<void*>(this),
			0,
			state == STATUS_THREAD_SUSPENDED,
			&m_id
		);
		if (m_thread == nullptr)
			delete this;

#ifdef RUBY_MSVC_USED
		// Converting our thread priority format to windows format
		int winPriority = THREAD_PRIORITY_NORMAL;
		switch (m_priority) {
		case PRIORITY_THREAD_LOW:
			winPriority = THREAD_PRIORITY_LOWEST;

			break;
		case PRIORITY_THREAD_HIGH:
			winPriority = THREAD_PRIORITY_HIGHEST;

			break;
		case PRIORITY_THREAD_CRITICAL:
			winPriority = THREAD_PRIORITY_TIME_CRITICAL;

			break;
		}
#endif

		if (!Ruby::Platform::setThreadPriority(m_thread, winPriority)) {
			delete this;

			return;
		}
	}

	Thread::~Thread() {
		if (m_thread != nullptr) {
			m_state = STATUS_THREAD_DESTROYED;

#ifdef RUBY_MSVC_USED
			WaitForSingleObject(m_thread, INFINITE);

			CloseHandle(m_thread);
#endif
		}

		if (m_tasks != 0 && m_tasks != nullptr)
			delete[] m_tasks;
	}

	const threadinfo_t Thread::Priority() const {
		return m_priority;
	}

	const threadinfo_t Thread::Priority(threadinfo_t priority) {
		m_priority = priority;

#ifdef RUBY_MSVC_USED
		// Converting our thread priority format to windows format
		int winPriority = THREAD_PRIORITY_NORMAL;
		switch (m_priority) {
		case PRIORITY_THREAD_LOW:
			winPriority = THREAD_PRIORITY_LOWEST;

			break;
		case PRIORITY_THREAD_HIGH:
			winPriority = THREAD_PRIORITY_HIGHEST;

			break;
		case PRIORITY_THREAD_CRITICAL:
			winPriority = THREAD_PRIORITY_TIME_CRITICAL;

			break;
		}
#endif

		return Ruby::Platform::setThreadPriority(m_thread, m_priority);
	}

	const threadinfo_t Thread::State() const {
		return m_state;
	}

	const size_t Thread::Processed() const {
		return m_processed;
	}

	const size_t Thread::Processing() const {
		return m_processing;
	}

	const TaskData*Thread::Tasks() const {
		return m_tasks;
	}

	bool Thread::AddTask(TaskData task) {
		// Invalid task
		uintptr_t checksum = *reinterpret_cast<uintptr_t*>(&task);
		if (checksum == 0U)
			return false;

		m_guard.lock();

		bool added = false;
		for (size_t ind = 0U; ind < MAXIMUM_TASKS_PER_THREAD; ind++) {
			// Finding empty(not filled) memory
			checksum = *reinterpret_cast<uintptr_t*>(m_tasks + ind);
			if (checksum != 0U)
				continue;

			// Copying task's memory(Filling)
			m_tasks[ind] = task;

			added = true;
			break;
		}

		m_guard.unlock();

		return added;
	}

	bool Thread::RemoveTask(TaskData task) {
		// Invalid task
		uintptr_t checksum = *reinterpret_cast<uintptr_t*>(&task);
		if (checksum == 0U)
			return false;

		m_guard.lock();

		bool removed = false;
		for (size_t ind = 0U; ind < MAXIMUM_TASKS_PER_THREAD; ind++) {
			// Finding similar checksum
			uintptr_t tchecksum = *reinterpret_cast<uintptr_t*>(m_tasks + ind);
			if (tchecksum != checksum)
				continue;

			// Filling with zeros to remove
			std::memset(m_tasks + ind, 0U, sizeof(TaskData));

			removed = true;
			break;
		}

		m_guard.unlock();

		return removed;
	}

	bool Thread::Suspend() {
		threadinfo_t lostate = m_state & 0xF;
		if (lostate == STATUS_THREAD_SUSPENDED)
			return true;

		// Changing only low bits
		m_state = (m_state & 0xF0) | STATUS_THREAD_SUSPENDED;

		// Checking for running not joined
		if (lostate == STATUS_THREAD_RUNNING) {
			// Waiting for thread suspended
			while (m_state & STATUS_THREAD_WORKING)
				continue;

			// Suspending thread internally
			Ruby::Platform::suspendThread(m_thread);
		}

		return true;
	}

	bool Thread::Resume() {
		if ((m_state & 0xF) > STATUS_THREAD_SUSPENDED)
			return true;

		// Changing only low bits
		m_state = (m_state & 0xF0) | STATUS_THREAD_RUNNING;

		// Resuming thread internally
		return Ruby::Platform::resumeThread(m_thread);
	}

	bool Thread::Wait() {
		// Locking will wait for iteration end, as thread unlocking guard at iteration end
		m_guard.lock();
		m_guard.unlock();

		return true;
	}

	bool Thread::Join() {
		while ((m_state & 0xF) == STATUS_THREAD_RUNNING_JOINED) {
			threadinfo_t lostate = m_state & 0xF;

			m_state		= STATUS_THREAD_STARTING | lostate;
			m_processed	= m_processing;

			if (lostate == STATUS_THREAD_DESTROYED)
				break;
			else if (lostate == STATUS_THREAD_SUSPENDED)
				continue;

			m_guard.lock();

			m_state		= STATUS_THREAD_WORKING | lostate;
			m_processing	= 0U;

			TaskData* tasks = m_tasks;
			for (size_t ind = 0U; ind < MAXIMUM_TASKS_PER_THREAD; ind++, tasks++) {
				if (lostate <= STATUS_THREAD_SUSPENDED)
					break;

				// Checking for valid memory
				uintptr_t checksum = *reinterpret_cast<uintptr_t*>(tasks);
				if (checksum == 0U)
					continue;

				m_processing++;

				// Calling task's procedure
				TaskData task = *tasks;
				task.procedure(&task, task.reserved);

				if (task.lifetime)
					continue;

				std::memset(m_tasks + ind, 0U, sizeof(TaskData));
			}

			m_guard.unlock();

			m_state = STATUS_THREAD_ENDING | lostate;
		}

		return true;
	}

	namespace Threading {
		Thread* Create(threadinfo_t state, threadinfo_t priority) {
			// Allocating memory for threads
			if (Threads == nullptr) {
				Threads = static_cast<Thread**>(Ruby::Platform::virtualAlloc(MAXIMUM_THREADS * sizeof(Thread)));

				std::memset(Threads, 0U, MAXIMUM_THREADS * sizeof(Thread));
			}

			// Copying new thread's memory into free memory
			for (size_t ind = 0U; ind < MAXIMUM_THREADS; ind++) {
				uintptr_t checksum = *reinterpret_cast<uintptr_t*>(Threads + ind);
				if (checksum != 0U)
					continue;

				Threads[ind] = new Thread(state, priority);
				return Threads[ind];
			}

			return nullptr;
		}

		bool Create(size_t amount) {
			if (amount > MAXIMUM_THREADS)
				return false;

			// Allocating memory for threads
			if (Threads == nullptr) {
				Threads = static_cast<Thread**>(Ruby::Platform::virtualAlloc(MAXIMUM_THREADS * sizeof(Thread)));

				std::memset(Threads, 0U, MAXIMUM_THREADS * sizeof(Thread));
			}

			// Copying new threads memory into free memory
			size_t created = 0U;
			for (size_t ind = 0U; ind < MAXIMUM_THREADS; ind++) {
				if (created >= amount)
					break;

				uintptr_t checksum = *reinterpret_cast<uintptr_t*>(Threads + ind);
				if (checksum != 0U)
					continue;

				uintptr_t size = sizeof(Thread*);

				Threads[ind] = new Thread();
				created++;
			}

			return created == amount;
		}

		bool Add(TaskData task) {
			size_t tasks = MAXIMUM_TASKS_PER_THREAD,
				   index = 0U;
			for (size_t ind = 0U; ind < MAXIMUM_THREADS; ind++) {
				uintptr_t checksum = *reinterpret_cast<uintptr_t*>(Threads + ind);
				if (checksum == 0U)
					continue;

				Thread* thread = Threads[ind];

				// Saving less busy thread
				if (thread->Processed() <= tasks)
					index = ind;
			}

			Thread* thread = Threads[index];
			if (thread->State() & STATUS_THREAD_SUSPENDED)
				thread->Resume();

			return thread->AddTask(task);
		}

		bool Remove(TaskData task) {
			for (size_t ind = 0U; ind < MAXIMUM_THREADS; ind++) {
				uintptr_t checksum = *reinterpret_cast<uintptr_t*>(Threads + ind);
				if (checksum == 0U)
					continue;

				Thread* thread = Threads[ind];
				if (thread->RemoveTask(task))
					return true;
			}

			return false;
		}

		bool Destroy(Thread* thread) {
			if (Threads == nullptr)
				return true;

			for (size_t ind = 0U; ind < MAXIMUM_THREADS; ind++) {
				uintptr_t checksum = *reinterpret_cast<uintptr_t*>(Threads + ind);
				if (checksum != 0U)
					continue;

				if (Threads[ind] == thread) {
					delete Threads[ind];

					std::memset(Threads + ind, 0U, sizeof(Thread));

					return true;
				}
			}

			return false;
		}

		void Destroy() {
			for (size_t ind = 0U; ind < MAXIMUM_THREADS; ind++) {
				delete Threads[ind];

				Ruby::Platform::virtualFree(Threads);
			}

			Threads = nullptr;
		}
	}
}