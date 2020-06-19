#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <mutex>

typedef unsigned char BYTE;

class ThreadPool
{
private:
	struct ThreadState
	{
		bool isWork;
		bool distroy;

		ThreadState() : isWork(false), distroy(false) {}
	};

	struct ThreadWork
	{
		ThreadState* state;
		thread* _thread;

		ThreadWork() : _thread(nullptr) {}
	};

private:
	vector<ThreadWork*> vPool;
	short ThreadNumber;
	int FileHeight;

public:
	mutex mtx;

	vector<BYTE> OrigByte;
	vector<BYTE> InitVec;
	vector<BYTE> CipByte;

public:
	ThreadPool();
	~ThreadPool();

	void SetThread(int value = thread::hardware_concurrency());		//Setting Routin Thread Number
	void release();

	bool isAllOver();
	void Cluck_AllThread();

	void ClearBeforeStart();
	void ResizeBeforeStart(size_t _size);

	inline vector<ThreadWork*> const& getThreadList() { return vPool; }
	inline int& getFileHeight() { return FileHeight; }
};
#endif