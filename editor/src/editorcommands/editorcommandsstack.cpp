#include "editorcommandsstack.h"


void EditorCommandsStack::undo(World* world, MainWindow* editorWindow)
{
	if (haveSomethingToUndo())
	{
		mCommands[static_cast<size_t>(mCurrentHeadIndex)]->undoCommand(world, editorWindow);
		--mCurrentHeadIndex;

		if (mChangeHandler)
		{
			mChangeHandler();
		}
	}
}

void EditorCommandsStack::redo(World* world, MainWindow* editorWindow)
{
	if (haveSomethingToRedo())
	{
		++mCurrentHeadIndex;
		mCommands[static_cast<size_t>(mCurrentHeadIndex)]->doCommand(world, editorWindow);

		if (mChangeHandler)
		{
			mChangeHandler();
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

void EditorCommandsStack::bindFunctionToCommandChange(std::function<void ()> handler)
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
		mChangeHandler();
	}
}
