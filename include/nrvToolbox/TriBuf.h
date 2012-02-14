#pragma once
#include <OpenThreads/Mutex>
template <class T>
class TriBuf
{
	public:
	TriBuf()
	{
		_d[0] = new T();
		_d[1] = new T();
		_d[2] = new T();
		_status[0] = _status[1] = _status[2] = WRITABLE;
	}

	~TriBuf()
	{
		delete[] _d[0];
		delete[] _d[1];
		delete[] _d[2];
	}
	void setData(T& s)
	{		
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_writeLock);
		int writeIndex = getWritable();
		if(writeIndex>=0)
		{
			*(_d[writeIndex]) = s;
			statusSwapPostWrite(writeIndex);
		}
		//else printf("%i %i %i (Wa-Wg-Ra-Rg) setData failed: no WRITABLE object\n", _status[0], _status[1], _status[2]);
		
	}
	void getData(T& s)
	{		
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_readLock);
		int readIndex = getReadable();
		if(readIndex>=0)
		{
			s = (*_d[readIndex]);
			statusSwapPostRead(readIndex);
		}
		//else printf("%i %i %i (Wa-Wg-Ra-Rg) getData failed: no READABLE object\n", _status[0], _status[1], _status[2]);
	}
	T getData()
	{		
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_readLock);
		T s;
		int readIndex = getReadable();
		if(readIndex>=0)
		{
			s = (*_d[readIndex]);
			statusSwapPostRead(readIndex);
		}
		//else printf("%i %i %i (Wa-Wg-Ra-Rg) getData failed: no READABLE object\n", _status[0], _status[1], _status[2]);
		return s;
	}
protected:
	T *_d[3];
	typedef enum
	{
		WRITABLE,
		WRITING,
		READABLE,
		READING
	} BufferStatus;
	enum AlterStatus
	{
		ALTER,
		DO_NOT_ALTER
	};
	BufferStatus _status[3];
	OpenThreads::Mutex _lock, _readLock, _writeLock;

	int getReadable ()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);
		int index = -1;
		for(int i=0; i<3; ++i)
		{
			if(_status[i]==READABLE)
			{
				index = i;
				_status[i]=READING;
				break;
			}
		}
		return index;
	}
	int getWritable ()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);
		int index = -1;
		for(int i=0; i<3; ++i)
		{
			if(_status[i]==WRITABLE)
			{
				index = i;
				_status[i]=WRITING;
				break;
			}
		}
		return index;
	}
	void statusSwapPostRead(int index)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);
		bool readableAvailable = false;
		for(int i=0; i<3; ++i)
		{
			if(_status[i]==READABLE) 
			{
				readableAvailable = true;//another READABLE is available, so this one(index) can be set to WRITABLE
				break;
			}
		}
		if(readableAvailable) _status[index] = WRITABLE;
		else _status[index] = READABLE;//reset index to READABLE so there's always a readable available
	}
	void statusSwapPostWrite(int index)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);
		//the just-written index will be set to READABLE, all other READABLES can be set to WRITABLE
		for(int i=0; i<3; ++i)
		{
			if(_status[i]==READABLE)_status[i]=WRITABLE;
		}
		_status[index] = READABLE;
	}
};