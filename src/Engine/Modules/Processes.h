#ifndef PROCESSES_H
#define PROCESSES_H

// dummy for debugging methods
#if (!defined DEBUG) && (!defined RELEASE)
	#define RELEASE
	#define WARN(message)
	#define WARN_IF(condition, message)
#endif

/**
 * Abstract class for some process
 */
class Process
{
public:
	/**
	 * Initialize the process with some ID
	 */
    Process(int id);

	virtual ~Process();

    /**
     * Функция, вызываемая при необходимости обработать процесс
     *
     * It should be overridden in the the child class.
     */
	virtual void run() = 0;

	/**
	 * Get the process ID
	 */
    int getId();
private:
	/** ID of the process */
	int processID;
};

/**
 * Container for the process
 *
 * Need to hide the implementation details of PracessManager.
 */
class ProcessContainer;

/**
 * Process manager
 *
 * Последовательно выполняет группу процессов.
 * Для корректной работы необходимо, чтобы процессы не влияли друг на друга.
 */
class ProcessManager
{
public:
	ProcessManager();
    /**
     * Добавить новый процесс
     */
    void add(Process *process);

    /**
     * Запустить выполнение всех процессов
     */
    void run();

    /**
	 * Удалить все экземпляры процессов с определённым ID
     */
	void remove(int id);
private:
    /**
     * Вершина стека
     */
    ProcessContainer *first;
};

#endif
