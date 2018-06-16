#ifndef MUTEX_H
#define MUTEX_H

#include "stdafx.h"
#include <intrin.h>

#pragma once
#pragma intrinsic(_InterlockedExchange)
#pragma intrinsic(_InterlockedCompareExchange)

class Mutex
{
public:
    enum LockState
    {
        LS_LOCK_IS_FREE = 0,
        LS_LOCK_IS_TAKEN = 1
    };

    Mutex() : m_spinLock(LS_LOCK_IS_FREE)
    {
    }

    ~Mutex()
    {
    }

    void Lock()
    {
        while(_InterlockedCompareExchange(&m_spinLock, LS_LOCK_IS_TAKEN, LS_LOCK_IS_FREE));
    }

    void Unlock()
    {
        _InterlockedExchange(&m_spinLock, LS_LOCK_IS_FREE);
    }

    volatile long m_spinLock;
};

class MutexLock
{
public:
    MutexLock(Mutex* aLock)
        : m_lock(aLock)
    {
        m_lock->Lock();
    }

    MutexLock(Mutex& aLock)
        : m_lock(&aLock)
    {
        m_lock->Lock();
    }

    ~MutexLock()
    {
        m_lock->Unlock();
    }

    Mutex* m_lock;
};

#endif // MUTEX_H
