#ifndef STATE_LOADER_H
#define STATE_LOADER_H

#include <string>


/**
 * Class able to read Textures and create and sprites.
 *
 * If texture load once that it unload only after destruction.
 */
class StateLoader
{
public:
	StateLoader(std::string databaseFileName);
	~StateLoader(void);
	/**
	 * @throws TextureNotFoundException
	 */
	std::string getStateName(std::string actorName);

private:
	std::string databaseFileName;
};

#endif