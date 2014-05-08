#ifndef STATE_H
#define STATE_H

/**
 * Interface for an abstract state class
 */
class State
{
public:
    virtual ~State() = 0;
    /**
     * Process state's code
     *
     * This method must be overriden in child-calsses
     */
    virtual void process(float deltatime);
};

#endif
