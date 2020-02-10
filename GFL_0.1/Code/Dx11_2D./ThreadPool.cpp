#include "stdafx.h"
#include "ThreadPool.h"

ThreadPool::ThreadPool()
{
	ThreadNumber = 0;
}

ThreadPool::~ThreadPool()
{
}

void ThreadPool::SetThread(int value)
{
	//√÷√  1»∏
	if (ThreadNumber == 0)
	{
		//Create Total Thread
		ThreadNumber = value;
		vPool.reserve(value);

		for (int i = 0; i < value;)
		{
			vPool.push_back(new ThreadWork);
			vPool[vPool.size() - 1]->state = new ThreadState;
			vPool[vPool.size() - 1]->_thread = new thread([&, i]()
			{
				mtx.lock();

				ThreadState* state = vPool[i]->state;
				state->isWork = false;
				state->distroy = false;

				int startP = i * 4;
				int lineNum = 0;
				int* _height = &FileHeight;
				BYTE value = 0;

				mtx.unlock();

				while (!state->distroy)
				{
					if (!state->isWork)
						this_thread::yield();
					else
					{
						while (state->isWork)
						{
							// 0
							value = CipByte[startP + (lineNum * 32)] ^ InitVec[startP];
							value ^= 255;
							OrigByte[startP + (lineNum * 32)] = value;
							InitVec[startP] = value;

							// 1
							value = CipByte[startP + (lineNum * 32) + 1] ^ InitVec[startP + 1];
							value ^= 255;
							OrigByte[startP + (lineNum * 32) + 1] = value;
							InitVec[startP + 1] = value;

							// 2
							value = CipByte[startP + (lineNum * 32) + 2] ^ InitVec[startP + 2];
							value ^= 255;
							OrigByte[startP + (lineNum * 32) + 2] = value;
							InitVec[startP + 2] = value;

							// 3
							value = CipByte[startP + (lineNum * 32) + 3] ^ InitVec[startP + 3];
							value ^= 255;
							OrigByte[startP + (lineNum * 32) + 3] = value;
							InitVec[startP + 3] = value;

							++lineNum;

							if (lineNum >= *_height)
							{
								mtx.lock();

								lineNum = 0;
								value = 0;
								state->isWork = false;

								mtx.unlock();
							}
						}
					}

				}

			});

			mtx.lock();
			++i;
			mtx.unlock();

			vPool[vPool.size() - 1]->_thread->detach();
		}
	}
}

void ThreadPool::release()
{
	for (int i = 0; i < ThreadNumber; ++i)
	{
		if (vPool[i] != nullptr)
		{
			vPool[i]->state->distroy = true;
			vPool[i]->state->isWork = false;

			if (vPool[i]->_thread != nullptr)
			{
				delete vPool[i]->_thread;
				vPool[i]->_thread = nullptr;
			}

			if (vPool[i]->state != nullptr)
			{
				delete vPool[i]->state;
				vPool[i]->state = nullptr;
			}

			delete vPool[i];
			vPool[i] = nullptr;
		}
	}
	vPool.clear();

	_sleep(500);
}

bool ThreadPool::isAllOver()
{
	mtx.lock();
	for (auto& it : vPool)
	{
		if (it->state->isWork)
		{
			mtx.unlock();
			return false;
		}
	}
	mtx.unlock();
	return true;
}

void ThreadPool::Cluck_AllThread()
{
	mtx.lock();
	{
		for (auto& it : vPool)
			it->state->isWork = true;
	}
	mtx.unlock();
}

void ThreadPool::ClearBeforeStart()
{
	OrigByte.clear();
	CipByte.clear();
	InitVec.clear();
}

void ThreadPool::ResizeBeforeStart(size_t _size)
{
	InitVec.resize(32, 0);
	CipByte.resize(_size, 0);
	OrigByte.resize(_size, 0);
}