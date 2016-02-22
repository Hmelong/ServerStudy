#pragma once

template<class T> class Singleton
{
public:
	template<typename ...Args>
	static T* Inst(Args... args)
	{
		if (instance == nullptr)
		{
			T* p(new T(std::forward<Args>(args)...));
			instance = p;
		}
		return instance;
	}

private:
	static T* instance;
};

template<typename T> 
T* Singleton<T>::instance = nullptr;