#ifndef EDITORCOMMANDSSTACK_H
#define EDITORCOMMANDSSTACK_H

#include "editorcommand.h"
#include <functional>

class EditorCommandsStack
{
public:
	typedef std::function<void(EditorCommand::EffectType, bool, bool)> OnChangeFn;

public:
	template<typename T, typename... Args>
	void executeNewCommand(World* world, Args... args)
	{
		// clear old redo commands
		if (haveSomethingToRedo())
		{
			 mCommands.erase(mCommands.begin() + (mCurrentHeadIndex+1), mCommands.end());
		}

		// add and activate
		mCommands.emplace_back(new T(std::forward<Args>(args)...));
		bool forceUpdateLayout = mCommands.back()->doCommand(world);
		EditorCommand::EffectType effect = mCommands.back()->getEffectType();
		++mCurrentHeadIndex;

		// clear old commands if exceed limits
		if (mCurrentHeadIndex > mMaxHistorySize + mClearLag)
		{
			clearOldCommands();
		}

		if (mChangeHandler)
		{
			mChangeHandler(effect, true, forceUpdateLayout);
		}
	}

	bool undo(World* world);
	bool redo(World* world);
	bool haveSomethingToUndo() const;
	bool haveSomethingToRedo() const;
	void clear();
	void bindFunctionToCommandChange(OnChangeFn handler);

private:
	void clearOldCommands();

private:
	std::vector<std::shared_ptr<EditorCommand>> mCommands;
	int mCurrentHeadIndex = -1;
	static const int mMaxHistorySize = 10000;
	// how ofter commands will be cleaned after reaching the limit
	static const int mClearLag = 100;
	OnChangeFn mChangeHandler;

	Delegates::Handle mOnCommandEffectHandle;
};

#endif // EDITORCOMMANDSSTACK_H
