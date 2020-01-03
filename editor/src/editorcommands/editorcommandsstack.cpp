#include "editorcommandsstack.h"


void EditorCommandsStack::undo(World* world)
{
	if (haveSomethingToUndo())
	{
		mCommands[static_cast<size_t>(mCurrentHeadIndex)]->undoCommand(world);
		EditorCommand::EffectBitset effects = mCommands[static_cast<size_t>(mCurrentHeadIndex)]->getEffects();
		--mCurrentHeadIndex;

		if (mChangeHandler)
		{
			mChangeHandler(effects, false);
		}
	}
}

void EditorCommandsStack::redo(World* world)
{
	if (haveSomethingToRedo())
	{
		++mCurrentHeadIndex;
		mCommands[static_cast<size_t>(mCurrentHeadIndex)]->doCommand(world);
		EditorCommand::EffectBitset effects = mCommands[static_cast<size_t>(mCurrentHeadIndex)]->getEffects();

		if (mChangeHandler)
		{
			mChangeHandler(effects, false);
		}
	}
}

bool EditorCommandsStack::haveSomethingToUndo() const
{
	return mCurrentHeadIndex >= 0;
}

bool EditorCommandsStack::haveSomethingToRedo() const
{
	return mCurrentHeadIndex < static_cast<int>(mCommands.size()) - 1;
}

void EditorCommandsStack::clear()
{
	mCommands.clear();
	mCurrentHeadIndex = -1;
}

void EditorCommandsStack::bindFunctionToCommandChange(OnChangeFn handler)
{
	mChangeHandler = handler;
}

void EditorCommandsStack::clearOldCommands()
{
	mCommands.erase(mCommands.begin(), mCommands.begin() + mClearLag);
	mCurrentHeadIndex -= mClearLag;

	if (mCurrentHeadIndex < -1)
	{
		mCurrentHeadIndex = -1;
	}

	if (mChangeHandler)
	{
		mChangeHandler(EditorCommand::EffectType::CommandStack, false);
	}
}
