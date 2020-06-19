#ifndef SINGLETON_HPP
#define SINGLETON_HPP

template<typename T>
class singleton {
protected:
	static T* inst;

public:
	singleton() {}
	singleton(const singleton& other) = delete;
	singleton operator= (const singleton& other) = delete;

	~singleton() {}

	inline static T* getInstance() {
		if (inst == nullptr)
			inst = new T;
		return inst;
	}

	inline static void releaseInstance() {
		if (inst != nullptr) {
			delete inst;
			inst = nullptr;
		}
	}
};

template<typename T>
T* singleton<T>::inst = nullptr;

#endif