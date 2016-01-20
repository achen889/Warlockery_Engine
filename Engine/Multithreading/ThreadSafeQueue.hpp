//==============================================================================================================
//ThreadSafeQueue.hpp
//by Albert Chen Nov-3-2015.
//==============================================================================================================

#pragma once

#ifndef _included_ThreadSafeQueue__
#define _included_ThreadSafeQueue__

#include <queue>
#include "CriticalSection.hpp"

//===========================================================================================================

template <typename Tx>

//===========================================================================================================

struct ThreadSafeQueue{

public:
	//enQ/deQ
	template <typename Tx>
	void Enqueue(Tx& item){

		criticalSection.Enter();
		queue.push(item);
		criticalSection.Exit();
	}

	//-----------------------------------------------------------------------------------------------------------

	template <typename Tx>
	bool Dequeue(Tx* out){

		bool success = false;
		criticalSection.Enter();
		if (queue.size() > 0) {
			*out = queue.front();
			queue.pop();
			success = true;
		}
		criticalSection.Exit();
		return success;
	}

	//-----------------------------------------------------------------------------------------------------------

	size_t Size() const {
		size_t size = 0;
		criticalSection.Enter();
		size = queue.size();
		criticalSection.Exit();

		return size;
	}

	bool Empty(){
		return queue.empty();
	}

private:
	CriticalSection criticalSection;
	//queue of stuff
	std::queue<Tx> queue;

};


//===========================================================================================================

#endif //__includedThreadSafeQueue__

