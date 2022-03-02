#include<pthread.h>

#pragma once

class CLock  
{
private:
	pthread_mutex_t m;
	pthread_cond_t sync_cond;
public:
	CLock() 
	{
		// 뮤텍스 속성 변수 생성
		pthread_mutexattr_t mAttr;
		// 뮤텍스 속성에 대한 재귀 뮤텍스 설정
		pthread_mutexattr_settype (& mAttr, PTHREAD_MUTEX_RECURSIVE_NP);
		// mutex 속성을 사용하여 mutex 생성
		pthread_mutex_init (& m, & mAttr);
		// 뮤텍스 변수를 초기화 한 후 뮤텍스 속성을 삭제할 수 있습니다.
		pthread_mutexattr_destroy (& mAttr);
	}
   	~CLock()
	{
		   pthread_mutex_destroy (& m);
	}

	inline void Lock()
	{
		pthread_mutex_lock (&m);
	}
	inline void Unlock() 
	{
		pthread_mutex_unlock(&m);
	}
};