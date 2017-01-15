#include "Processes.h"

#include <exception>

Process::Process(int id)
{
	processID = id;
}

Process::~Process()
{
}

void Process::run()
{
	throw new std::exception();
}

int Process::getId()
{
	return processID;
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
        currentProcess = process;
        next = next;
    }

	~ProcessContainer()
	{
		if (currentProcess != nullptr)
		{
			delete currentProcess;
			currentProcess = nullptr;
		}
	}

    /**
     * Имя процесса
     */
    int getId()
    {
        return currentProcess->getId();
    }

    /**
     * Получить ссылку на следующий процесс
     */
    ProcessContainer* getNext()
    {
        return next;
    }

    /**
     * Запустить выполнение этого и следующих по цепочке процессов
     */
    void run()
    {
        currentProcess->run();
        if (next != nullptr)
        {
            next->run();
        }
    }

    /**
     * Удалить все экземпляры процессов с определённым именем
     */
	void remove(int id)
    {
        // если это не последний процесс в цепочке
        if (next != nullptr && id >= 0)
        {
            // если следующий процесс - то что мы хотим удалить
			if (next->getId() == id)
            {
                // удаляем следующий процесс, сохраняя связи
                ProcessContainer *processToDelete = next;
                next = next->getNext();
                delete processToDelete;  
                // рекурсивный вызов (нужен чтобы удалить дублирующиеся элементы)
				remove(id);
            }
            else
            {
                // посылаем запрос на удаление дальше по цепочке
				next->remove(id);
            }
        }
    }
};


ProcessManager::ProcessManager()
{
	first = nullptr;
}

void ProcessManager::add(Process *process)
{
	first = new ProcessContainer(first, process);
}

void ProcessManager::run()
{
	if (first != 0)
	{
		first->run();
	}
}

void ProcessManager::remove(int id)
{
    // если есть что удалять
    if (first != nullptr && id >= 0)
    {
        // если следующий процесс - то что мы хотим удалить
        if (first->getId() == id)
        {
            // удаляем следующий процесс, сохраняя связи
            ProcessContainer *processToDelete = first;
            first = first->getNext();
            delete processToDelete;
            // рекурсивный вызов (нужен чтобы удалить дублирующиеся элементы)
			remove(id);
        }
        else
        {
            // посылаем запрос на удаление дальше по цепочке
			first->remove(id);
        }
    }
}
