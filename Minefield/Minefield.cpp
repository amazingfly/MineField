//
// ZOS Software Engineer Applicant Test 2.1.1
//
// Submitted by: 
//
// Date:
//
// Time taken:
//
// Notes:
//
//
//
// 
//
#include "stdafx.h"

#include "Windows.h"
#include "ObjectManager.h"
#include "Object.h"
#include "Mine.h"
#include <process.h>

int g_numberOfTeams = 3;
int g_numberOfMinesPerTeam = 1500;

class QueryPerformanceTimer
{
public:
    QueryPerformanceTimer()
    {
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);

        m_inverseFrequency = 1000000.0 / (double)frequency.QuadPart;
    }

    void Start()
    {
        QueryPerformanceCounter(&m_start);
    }

    double Get()
    {
        QueryPerformanceCounter(&m_stop);

        double time = (double)(m_stop.QuadPart - m_start.QuadPart) * m_inverseFrequency;

        m_start = m_stop;

        // time value is in micro seconds
        return time;
    }

    LARGE_INTEGER m_start;
    LARGE_INTEGER m_stop;
    double m_inverseFrequency;
};

class ScopedQueryPerformanceTimer
{
public:
    ScopedQueryPerformanceTimer(const char*	aMsg = NULL)
    {
        m_msg = aMsg;
        m_timer.Start();
    }

    ~ScopedQueryPerformanceTimer()
    {
        double timeUsed = m_timer.Get();
        printf("%s %f\n", m_msg, timeUsed / 1000.0);
    }

    QueryPerformanceTimer m_timer;
    const char*	m_msg;
};

static int s_numberOfWorkerThreadsActive = 0;
static int s_numberOfWorkerThreadsStarted = 0;
static Mutex s_lock;

namespace
{
    void FindTargets(void* aIgnored)
    {
        {
            MutexLock lock(s_lock);
            s_numberOfWorkerThreadsActive++;
            s_numberOfWorkerThreadsStarted++;
        }
        bool done = false;
        while(!done)
        {
			//CHANGE to unsigned into to match types
            unsigned int index = ObjectManager::GetSingleton().GetNextFindTargetsIndex();
            if(index < ObjectManager::GetSingleton().GetNumberOfObjects())
            {
                Mine* pMineObject = static_cast<Mine*>(ObjectManager::GetSingleton().GetObject(index));
                pMineObject->FindCurrentTargets();
            }
            else
            {
                done = true;
            }
        }
        {
            MutexLock lock(s_lock);
            s_numberOfWorkerThreadsActive--;
        }
    }
}

class WorkerThread
{
public:

    WorkerThread()
    {
    }

    ~WorkerThread()
    {
    }

    void FindTargetsForAllMines()
    {
        _beginthread(FindTargets, 0, NULL);
    }
};

int main(int aArgc, char* aArgv[])
{
    int numberOfWorkerThreads = 16;
    int randomSeed = 0;
    if(aArgc > 1)
    {
        randomSeed = atoi(aArgv[1]);
        SetRandomSeed(randomSeed);
    }
    if(aArgc > 2)
    {
        numberOfWorkerThreads = atoi(aArgv[2]);
    }
    if(aArgc > 3)
    {
        g_numberOfTeams = atoi(aArgv[3]);
    }
    if(aArgc > 4)
    {
        g_numberOfMinesPerTeam = atoi(aArgv[4]);
    }

    printf("Random seed: %d\n", randomSeed);
    printf("Number of worker threads: %d\n", numberOfWorkerThreads);
    printf("Number of teams: %d  \n", g_numberOfTeams);
    printf("Number of mines per team: %d\n", g_numberOfMinesPerTeam);


    {
        ScopedQueryPerformanceTimer timer("Time taken in milliseconds:");

        ObjectManager::GetSingleton();

        // Let's add lots of mine objects to the system before starting things up
        for(int i = 0; i < g_numberOfTeams; i++)
        {
            for(int j = 0; j < g_numberOfMinesPerTeam; j++)
            {
                float position[3];
                for(int i = 0; i < 3; i++)
                    position[i] = GetRandomFloat32_Range(-1000.0f, 1000.0f);

                unsigned int objectId = GetRandomUInt32() % (g_numberOfMinesPerTeam * 10);
                ObjectManager::GetSingleton().AddMineObject(objectId, position, i);
            }
        }

		for(int i = 0; i < 10; i++)
		{
			Object* pObject = ObjectManager::GetSingleton().GetObject(i);
			if(pObject)
			{
				float* pPosition = pObject->GetPosition();
				printf("Object id %d position (%0.3f, %0.3f, %0.3f) active %s invulnerable %s\n", pObject->GetObjectId(),
					pPosition[0], pPosition[1], pPosition[2], pObject->GetActive() ? "Y" : "N", pObject->GetInvulnerable() ? "Y" : "N");
			}
		}

        printf("Number of objects in system %u\n", ObjectManager::GetSingleton().GetNumberOfObjects());

        std::vector<WorkerThread*> workerThreadList;
        for(int i = 0; i < numberOfWorkerThreads; i++)
        {
            workerThreadList.push_back(new WorkerThread());
        }

        int numberOfTurns = 0;

        bool targetsStillFound = true;
        while(targetsStillFound)
        {
            numberOfTurns++;
            targetsStillFound = false;
            ObjectManager::GetSingleton().ResetNextFindTargetIndex();
            s_numberOfWorkerThreadsStarted = 0;

            for(int i = 0; i < numberOfWorkerThreads; i++)
            {
                workerThreadList[i]->FindTargetsForAllMines();
            }

            do
            {
                // sleep until all worker threads have finished doing their thing
                Sleep(1);
            } while(s_numberOfWorkerThreadsActive > 0 || s_numberOfWorkerThreadsStarted == 0);

            for(int i = 0; i < g_numberOfTeams; i++)
            {
                Mine* pMine = static_cast<Mine*>(ObjectManager::GetSingleton().GetObjectWithMostEnemyTargets(i));
                if(pMine->GetNumberOfEnemyTargets() > 0)
                {
                    targetsStillFound = true;
                }
                pMine->Explode();
            }
        }

        int winningTeam = 0;
        int winningObjectCount = 0;
        for(int i = 0; i < g_numberOfTeams; i++)
        {
            printf("Team %d has %d mines remaining\n", i, ObjectManager::GetSingleton().GetNumberOfObjectForTeam(i));

            if(ObjectManager::GetSingleton().GetNumberOfObjectForTeam(i) > winningObjectCount)
            {
                winningObjectCount = ObjectManager::GetSingleton().GetNumberOfObjectForTeam(i);
                winningTeam = i;
            }
        }

        printf("Team %d WINS after %d turns!!\n", winningTeam, numberOfTurns);
    }

    Sleep(-1);

    return 0;
}

