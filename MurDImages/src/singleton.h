#pragma once
//#include <cstddef>

template <typename T>
class singleton
{
protected:
	singleton() { }
	~singleton() { }

public:
	static T *getInstance()
	{
		if(nullptr == _singleton)
		{
			_singleton = new T;
		}
		return (static_cast<T*>(_singleton));
	}

	static void kill()
	{
		if(nullptr != _singleton)
		{
			delete _singleton;
			_singleton = nullptr;
		}
	}

private:
	static T *_singleton;
};

template <typename T>
T *singleton<T>::_singleton = nullptr;