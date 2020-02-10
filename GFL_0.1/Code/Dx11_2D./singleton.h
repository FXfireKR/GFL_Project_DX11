#pragma once
template<typename T>
class singleton 
{
protected:
	static T* inst;

public:
	static T* getInstance();
	void delInstance();
};

template<typename T> T* singleton<T>::inst = nullptr;

template<typename T> inline T* singleton<T>::getInstance()
{
	if (inst == nullptr)
		inst = new T;
	return inst;
}

template<typename T> inline void singleton<T>::delInstance()
{
	if (inst != nullptr) {
		delete inst;
		inst = nullptr;
	}
}