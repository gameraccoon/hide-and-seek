#ifndef EDITORCOMMANDSSTACK_H
#define EDITORCOMMANDSSTACK_H

#include "editorcommand.h"
#include <functional>

class EditorCommandsStack
{
public:
	template<typename T, typename... Args>
	void executeNewCommand(World* world, MainWindow* editorWindow, Args... args)
	{
		// clear old redo commands
		if (haveSomethingToRedo())
		{
			 mCommands.erase(mCommands.begin() + (mCurrentHeadIndex+1), mCommands.end());
		}

		// add and activate
		mCommands.emplace_back(new T(std::forward<Args>(args)...));
		mCommands.back()->doCommand(world, editorWindow);
		++mCurrentHeadIndex;

		// clear old commands if exceed limits
		if (mCurrentHeadIndex > mMaxHistorySize + mClearLag)
		{
			clearOldCommands();
		}

		if (mChangeHandler)
		{
			mChangeHandler();
		}
	}

	void undo(World* world, MainWindow* editorWindow);
	void redo(World* world, MainWindow* editorWindow);
	bool haveSomethingToUndo() const;
	bool haveSomethingToRedo() const;
	void clear();
	void bindFunctionToCommandChange(std::function<void()> handler);

private:
	void clearOldCommands();

private:
	std::vector<std::shared_ptr<EditorCommand>> mCommands;
	int mCurrentHeadIndex = -1;
	static const int mMaxHistorySize = 10000;
	// how ofter commands will be cleaned after reaching the limit
	static const int mClearLag = 100;
	std::function<void()> mChangeHandler;
};

#endif // EDITORCOMMANDSSTACK_H
