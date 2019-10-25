#pragma once
template <typename T>

class singleton
{
private:
	static T* instance;

public:
	singleton() {}

	~singleton() {}

	static T* getInstance() { return instance; }

	void delInstance() 
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	}

};

template <typename T>
T* singleton<T>::instance = new T;