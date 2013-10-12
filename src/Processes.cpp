#include "BaseProperties.h"
#include "Processes.h"

Process::Process(int id)
{
	ProcessID = id;
}

Process::~Process()
{
}

void Process::Run()
{
    WARN("Function 'Run' not redefined in some process");
}

int Process::GetId()
{
	return ProcessID;
}

/**
 * Контейнер для хранения процесса
 *
 * Нужен для скрытия деталей реализации менеджера процессов
 */
class ProcessContainer
{               
protected:

    /**
     * Процесс, который хранится в этом контейнере
     */
    Process* CurrentProcess;

    /**
     * Процесс, который выполнится следом за данным
     */
    ProcessContainer* Next;

public:
    ProcessContainer(ProcessContainer *next, Process *process)
    {
        CurrentProcess = process;
        Next = next;
    }

	~ProcessContainer()
	{
		delete CurrentProcess;
	}

    /**
     * Имя процесса
     */
    int GetId()
    {
        return CurrentProcess->GetId();
    }

    /**
     * Получить ссылку на следующий процесс
     */
    ProcessContainer* GetNext()
    {
        return Next;
    }

    /**
     * Запустить выполнение этого и следующих по цепочке процессов
     */
    void Run()
    {
        CurrentProcess->Run();
        if (Next != NULL)
        {
            Next->Run();
        }
    }

    /**
     * Удалить все экземпляры процессов с определённым именем
     */
	void Remove(int id)
    {
        // если это не последний процесс в цепочке
        if (Next != NULL && id >= 0)
        {
            // если следующий процесс - то что мы хотим удалить
			if (Next->GetId() == id)
            {
                // удаляем следующий процесс, сохраняя связи
                ProcessContainer *processToDelete = Next;
                Next = Next->GetNext();
                delete processToDelete;  
                // рекурсивный вызов (нужен чтобы удалить дублирующиеся элементы)
				this->Remove(id);
            }
            else
            {
                // посылаем запрос на удаление дальше по цепочке
				Next->Remove(id);
            }
        }
    }
};


ProcessManager::ProcessManager()
{
	First = NULL;
}

void ProcessManager::Add(Process *process)
{
	First = new ProcessContainer(First, process);
}

void ProcessManager::Run()
{
	if (First != 0)
	{
		First->Run();
	}
}

void ProcessManager::Remove(int id)
{
    // если есть что удалять
    if (First != NULL && id >= 0)
    {
        // если следующий процесс - то что мы хотим удалить
        if (First->GetId() == id)
        {
            // удаляем следующий процесс, сохраняя связи
            ProcessContainer *processToDelete = First;
            First = First->GetNext();
            delete processToDelete;
            // рекурсивный вызов (нужен чтобы удалить дублирующиеся элементы)
			this->Remove(id);
        }
        else
        {
            // посылаем запрос на удаление дальше по цепочке
			First->Remove(id);
        }
    }
}
