#include "Processes.h"

#include <Debug/DebugMethods.h>

// dummy for debugging methods
#if (!defined DEBUG) && (!defined RELEASE)
 	#define RELEASE
  	#define WARN(message)
  	#define WARN_IF(condition, message)
#endif

Process::Process(int id)
{
	this->processID = id;
}

Process::~Process()
{
}

void Process::run()
{
    WARN("Function 'Run' not redefined in some process");
}

int Process::getId()
{
	return this->processID;
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
    Process* currentProcess;

    /**
     * Процесс, который выполнится следом за данным
     */
    ProcessContainer* next;

public:
    ProcessContainer(ProcessContainer *next, Process *process)
    {
        this->currentProcess = process;
        this->next = next;
    }

	~ProcessContainer()
	{
		if (this->currentProcess != nullptr)
		{
			delete this->currentProcess;
			this->currentProcess = nullptr;
		}
	}

    /**
     * Имя процесса
     */
    int getId()
    {
        return this->currentProcess->getId();
    }

    /**
     * Получить ссылку на следующий процесс
     */
    ProcessContainer* getNext()
    {
        return this->next;
    }

    /**
     * Запустить выполнение этого и следующих по цепочке процессов
     */
    void run()
    {
        this->currentProcess->run();
        if (this->next != nullptr)
        {
            this->next->run();
        }
    }

    /**
     * Удалить все экземпляры процессов с определённым именем
     */
	void remove(int id)
    {
        // если это не последний процесс в цепочке
        if (this->next != nullptr && id >= 0)
        {
            // если следующий процесс - то что мы хотим удалить
			if (this->next->getId() == id)
            {
                // удаляем следующий процесс, сохраняя связи
                ProcessContainer *processToDelete = this->next;
                this->next = this->next->getNext();
                delete processToDelete;  
                // рекурсивный вызов (нужен чтобы удалить дублирующиеся элементы)
				this->remove(id);
            }
            else
            {
                // посылаем запрос на удаление дальше по цепочке
				this->next->remove(id);
            }
        }
    }
};


ProcessManager::ProcessManager()
{
	this->first = nullptr;
}

void ProcessManager::add(Process *process)
{
	this->first = new ProcessContainer(this->first, process);
}

void ProcessManager::run()
{
	if (this->first != 0)
	{
		this->first->run();
	}
}

void ProcessManager::remove(int id)
{
    // если есть что удалять
    if (this->first != nullptr && id >= 0)
    {
        // если следующий процесс - то что мы хотим удалить
        if (this->first->getId() == id)
        {
            // удаляем следующий процесс, сохраняя связи
            ProcessContainer *processToDelete = this->first;
            this->first = this->first->getNext();
            delete processToDelete;
            // рекурсивный вызов (нужен чтобы удалить дублирующиеся элементы)
			this->remove(id);
        }
        else
        {
            // посылаем запрос на удаление дальше по цепочке
			this->first->remove(id);
        }
    }
}
