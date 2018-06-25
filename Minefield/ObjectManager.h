#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#pragma once

#include "Mutex.h"
#include "vector"

class Object;

const int cMaximumNumberOfObjects = 1000000;

class ObjectManager
{
public:

    static ObjectManager& GetSingleton();
	//CHANGE to unsigned int to match types
    unsigned int GetNumberOfObjects() { return m_numberOfObjects; }
	//CHANGE new function to manipulate private data
	void IncrementNumberOfObjects() { m_numberOfObjects++; }

	// not used CHANGE new function to set m_objects size
	//void SetObjectsVectorSize(int size) { (*m_objects).resize(size); }
    Object* GetObject(int aIndex) { return (*m_objects)[aIndex]; }
    Object* GetObjectByObjectId(int aObjectId);
    bool IsValidObject(Object* apObject);

	//CHANGE faster fine object
	Object* ObjectManager::RecursiveFind(std::vector<Object*> objVec,  unsigned int aObjectId);

    void RemoveObject(unsigned int aObjectId);
	//CHANGE remove the passing in of ID numbers
    void AddMineObject( float aPosition[3], int aTeam);

    int GetNextFindTargetsIndex();
	void ResetNextFindTargetIndex() { m_nextFindTargetIndex = 0; }

    Object* GetObjectWithMostEnemyTargets(int aTeam);
    int GetNumberOfObjectForTeam(int aTeam);

private:

    Mutex m_lock;
    //CHANGE to unsigned int to match types
	unsigned int m_numberOfObjects;
	//CHANGE to vector type
	std::vector<Object*> m_objects[cMaximumNumberOfObjects];

    int m_nextFindTargetIndex;

    ObjectManager();

    ~ObjectManager();
};

#endif // OBJECTMANAGER_H
