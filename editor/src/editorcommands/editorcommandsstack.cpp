#include "editorcommandsstack.h"


bool EditorCommandsStack::undo(World* world)
{
	if (haveSomethingToUndo())
	{
		bool forceUpdateLayout = mCommands[static_cast<size_t>(mCurrentHeadIndex)]->undoCommand(world);
		EditorCommand::EffectType effect = mCommands[static_cast<size_t>(mCurrentHeadIndex)]->getEffectType();
		--mCurrentHeadIndex;

		if (mChangeHandler)
		{
			mChangeHandler(effect, false, forceUpdateLayout);
		}
	}
	return false;
}

bool EditorCommandsStack::redo(World* world)
{
	if (haveSomethingToRedo())
	{
		++mCurrentHeadIndex;
		bool forceUpdateLayout = mCommands[static_cast<size_t>(mCurrentHeadIndex)]->doCommand(world);
		EditorCommand::EffectType effect = mCommands[static_cast<size_t>(mCurrentHeadIndex)]->getEffectType();

		if (mChangeHandler)
		{
			mChangeHandler(effect, false, forceUpdateLayout);
		}
	}
	return false;
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
		mChangeHandler(EditorCommand::EffectType::CommandsStack, false, false);
	}
}
