#include "AI/AiRole.h"

#include <Log.h>

#include <SqliteConnection.h>

AiRole::AiRole(World* world, IBody *body) : Role(world, body)
{
	SqliteConnection connection("testdb.db");
	SqlDataReader::Ptr conn = connection.execQuery("select * from test");

	std::string firstStateName;
	if (conn->next())
	{
		firstStateName = conn->getValueByName("test")->asString();
	}
	else
	{
		throw std::runtime_error("can't read from table 'test' of database 'testdb'");
	}
	//states.push(new LuaAiState(world, body, this, firstStateName));
}

AiRole::~AiRole()
{
	while (mStates.size() > 0)
	{
		IAiState *stateToDelete = mStates.top();
		mStates.pop();
		delete stateToDelete;
	}
}

void AiRole::update(float )
{
	checkStates();
}

void AiRole::onTakeDamage(IActor* instigator, float damageValue, Vector2D impulse)
{
	mStates.top()->onTakeDamage(instigator, damageValue, impulse);
	checkStates();
}

void AiRole::onSeeEnemy(IActor *enemy)
{
	mStates.top()->onSeeEnemy(enemy);
	checkStates();
}

void AiRole::onHearNoise(SoundVolume *sound)
{
	mStates.top()->onHearNoise(sound);
	checkStates();
}

void AiRole::gotoState(std::string stateName)
{
	//states.push(new LuaAiState(getWorld(), getBody(), this, stateName));
}

void AiRole::endCurrentState()
{
	if (mStates.size() > 1)
	{
		mStates.top()->setEnded(true);
	}
	else
	{
		Log::Instance().writeWarning("Trying to delete the last state");
	}
}

void AiRole::checkStates()
{
	if (mStates.top()->isEnded())
	{
		IAiState * stateToDelete = mStates.top();
		mStates.pop();
		delete stateToDelete;
	}
}