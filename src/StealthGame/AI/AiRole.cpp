#include "AiRole.h"

#include <Debug/Log.h>

#include <LuaInterface/LuaAiState.h>

#include <SqliteInterface/SqliteConnection.h>

AiRole::AiRole(World* world, IBody *body) : Role(world, body)
{
	SqlDataReader::Ptr conn = SqliteConnection("testdb.db").execSql("select * from test");

	std::string firstStateName;
	if (conn->next())
	{
		firstStateName = conn->getValueByName("test")->asString();
	}
	else
	{
		throw std::runtime_error("can't read from table 'test' of database 'testdb'");
	}
	this->states.push(new LuaAiState(world, body, this, firstStateName));
}

AiRole::~AiRole(void)
{
	while (this->states.size() > 0)
	{
		IAiState *stateToDelete = this->states.top();
		this->states.pop();
		delete stateToDelete;
	}
}

void AiRole::update(float deltaTime)
{
	this->checkStates();
}

void AiRole::onTakeDamage(IActor* instigator, float damageValue, Vector2D impulse)
{
	this->states.top()->onTakeDamage(instigator, damageValue, impulse);
	this->checkStates();
}

void AiRole::onSeeEnemy(IActor *enemy)
{
	this->states.top()->onSeeEnemy(enemy);
	this->checkStates();
}

void AiRole::onHearNoise(SoundVolume *sound)
{
	this->states.top()->onHearNoise(sound);
	this->checkStates();
}

void AiRole::gotoState(std::string stateName)
{
	this->states.push(new LuaAiState(this->getWorld(), this->getBody(), this, stateName));
}

void AiRole::endCurrentState()
{
	if (this->states.size() > 1)
	{
		this->states.top()->setEnded(true);
	}
	else
	{
		Log::Instance().writeWarning("Trying to delete the last state");
	}
}

void AiRole::checkStates()
{
	if (this->states.top()->isEnded())
	{
		IAiState * stateToDelete = this->states.top();
		this->states.pop();
		delete stateToDelete;
		this->getBody()->moveTo(Vector2D(0.0f, 0.0f));
	}
}