#include "stdafx.h"

#include "ObjectManager.h"
#include "Object.h"
#include "Mine.h"

ObjectManager::ObjectManager()
    : m_numberOfObjects(0)
    , m_nextFindTargetIndex(0)
{
}

ObjectManager::~ObjectManager()
{
}

static ObjectManager* s_pObjectManager;

ObjectManager& ObjectManager::GetSingleton()
{
    static ObjectManager* pSingetonInstance = new ObjectManager(); 
    return *pSingetonInstance; 
}


void ObjectManager::AddMineObject(float aPosition[3], int aTeam)//unsigned int aObjectId, float aPosition[3], int aTeam)
{
    MutexLock lock(m_lock);
	
	for(unsigned int i = 0; i < m_numberOfObjects; i++)
    {
		/*
        if((*m_objects)[i]->GetObjectId() == aObjectId)
        {
            // If objectId matches an existing entry then just consider it as getting updated (even potentially switched to a new team)
            Mine* pMineObject = new Mine();
            pMineObject->m_objectId = (*m_objects)[i]->m_objectId;
            pMineObject->m_team = (*m_objects)[i]->m_team;
            pMineObject->m_bitFlags = (*m_objects)[i]->m_bitFlags;
            for(int j = 0; j < 3; j++)
                pMineObject->m_position[j] = aPosition[j];
            pMineObject->m_destructiveRadius = static_cast<Mine*>((*m_objects)[i])->m_destructiveRadius;

            delete (*m_objects)[i];
            (*m_objects)[i] = pMineObject;

            return;
        }
		*/
    }

	if(m_numberOfObjects == cMaximumNumberOfObjects)
		return; 
    
	
	//(*m_objects)[m_numberOfObjects] = 
	m_objects->push_back(new Mine());
	(*m_objects)[m_numberOfObjects]->m_objectId = m_numberOfObjects; //aObjectId;
    (*m_objects)[m_numberOfObjects]->m_team = aTeam;
    (*m_objects)[m_numberOfObjects]->SetPosition(aPosition);
    static_cast<Mine*>((*m_objects)[m_numberOfObjects])->m_destructiveRadius = GetRandomFloat32_Range(10.0f, 100.0f);
    
    bool active = (GetRandomFloat32() < 0.95f);
    (*m_objects)[m_numberOfObjects]->SetActive(active);

    if(GetRandomFloat32() < 0.1f)
	{
		//CHANGE to an assignment: `=` in stead of `==`
        (*m_objects)[m_numberOfObjects]->m_bitFlags = Object::OBF_INVULNERABLE;
    }

    //CHANGE m_numberOfObjects++; to:
	ObjectManager::IncrementNumberOfObjects();

	return;
}

void ObjectManager::RemoveObject(unsigned int aObjectId)
{
    for(unsigned int i = 0; i < m_numberOfObjects; i++)
    {
		if ((*m_objects)[i]->m_objectId == aObjectId)
		{
			printf("does this happen?\n");
            delete (*m_objects)[i];

            // Do a fast remove and replace this location with object currently at end
            (*m_objects)[i] = (*m_objects)[m_numberOfObjects - 1];
            (*m_objects)[m_numberOfObjects - 1] = NULL;
        }
    }

    if(m_numberOfObjects % 100 == 0)
        printf("Number of objects in system %u\n", m_numberOfObjects);

    return;
}

//CHANGE use recursive find
Object* ObjectManager::GetObjectByObjectId(int aObjectId)
{
	/*
    for(unsigned int i = 0; i < m_numberOfObjects; i++)
    {
        if((*m_objects)[i]->m_objectId == aObjectId)
        {
            return (*m_objects)[i];
        }
    }
	*/
	return ObjectManager::RecursiveFind((*m_objects), aObjectId);
}

//CHANGE faster finding mine index
Object* ObjectManager::RecursiveFind(std::vector<Object*> objVec, unsigned int aObjectId)
{
	size_t low = 0;
	size_t high = objVec.size();
	size_t mid = (high - low) / 2;
	if (high < 2)
	{
		if (aObjectId == (*m_objects)[0]->m_objectId)
		{
			return (*m_objects)[0];
		}
	}
	std::vector<Object*> lowObjVec(objVec.begin(), objVec.begin() + mid);
	std::vector<Object*> highObjVec(objVec.begin() + mid, objVec.end());
	ObjectManager::RecursiveFind(lowObjVec, aObjectId);
	ObjectManager::RecursiveFind(highObjVec, aObjectId);

	return NULL;
}

bool ObjectManager::IsValidObject(Object* apObject)
{
    for(unsigned int i = 0; i < m_numberOfObjects; i++)
    {
        if((*m_objects)[i] == apObject)
        {
            return true;
        }
    }

    return false;
}

int ObjectManager::GetNextFindTargetsIndex()
{
    MutexLock lock(m_lock);

    int index = m_nextFindTargetIndex;
    m_nextFindTargetIndex++;

    return index;
}

Object* ObjectManager::GetObjectWithMostEnemyTargets(int aTeam)
{
    Object* pBestObject = NULL;
    for(unsigned int i = 0; i < m_numberOfObjects; i++)
    {
        if((*m_objects)[i]->m_team == aTeam)
        {
            if(pBestObject == NULL)
            {
                pBestObject = (*m_objects)[i];
            }
            else if(static_cast<Mine*>((*m_objects)[i])->GetNumberOfEnemyTargets() > static_cast<Mine*>(pBestObject)->GetNumberOfEnemyTargets())
            {
                pBestObject = (*m_objects)[i];
            }
        }
    }

    return pBestObject;
}

int ObjectManager::GetNumberOfObjectForTeam(int aTeam)
{
    int count = 0;
    for(unsigned int i = 0; i < m_numberOfObjects; i++)
    {
        if((*m_objects)[i]->m_team == aTeam)
        {
            count++;
        }
    }

    return count;
}

