#ifndef Singleton_h_
#define Singleton_h_

template < typename T > class Singleton
{
public:
	static T* GetInstance()
	{
		static T instance;
		return &instance;
	}
private:
	Singleton();
	~Singleton();
};
#endif