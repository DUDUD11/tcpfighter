#pragma once
#include <concurrent_unordered_set.h>
#pragma comment(lib, "winmm.lib")

template<class Key>
class Concurrent_Unordered_set
{
public:



	Concurrent_Unordered_set()
	{
		InitializeSRWLock(&srw_lock);

	}


	~Concurrent_Unordered_set()
	{

	}

	bool insert_notfind(const Key& KVal)
	{
		bool flag = false;

		AcquireSRWLockExclusive(&srw_lock);
		if (HashSet.find(KVal) == HashSet.end())
		{
			HashSet.insert(KVal);
			flag = true;
		}
		ReleaseSRWLockExclusive(&srw_lock);
		return flag;
	}

	bool find(const Key& KVal)
	{
		bool flag = false;
		AcquireSRWLockShared(&srw_lock);
		if (HashSet.find(KVal) == HashSet.end())
		{
			flag = false;
		}
		else
		{
			flag = true;
		}
		ReleaseSRWLockShared(&srw_lock);
		return flag;
	}


	int size()
	{
		AcquireSRWLockShared(&srw_lock);
		int size = HashSet.size();
		ReleaseSRWLockShared(&srw_lock);
		return size;
	}

	int erase(Key& key)
	{
		int ret;

		AcquireSRWLockExclusive(&srw_lock);
		ret = HashSet.unsafe_erase(key);
		ReleaseSRWLockExclusive(&srw_lock);

		return ret;
	}



	typename  Concurrency::concurrent_unordered_set<Key>::iterator erase(typename  Concurrency::concurrent_unordered_set<Key>::iterator iter)
	{
		AcquireSRWLockExclusive(&srw_lock);
		iter = HashSet.unsafe_erase(iter);
		ReleaseSRWLockExclusive(&srw_lock);
		return iter;
	}

	void insert(Key key)
	{

		AcquireSRWLockShared(&srw_lock);
		HashSet.insert(key);
		ReleaseSRWLockShared(&srw_lock);
	}

	//unsafe
	void Clear()
	{
		HashSet.clear();
	}

	Concurrency::concurrent_unordered_set<Key>* GetHashSet()
	{
		return &HashSet;
	}


private:
	SRWLOCK srw_lock;

	Concurrency::concurrent_unordered_set<Key> HashSet;

};