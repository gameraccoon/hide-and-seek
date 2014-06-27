Log.log("Start script 1")

local seen = 1

function OnSeeEnemy(enemy)
	Log.log("seen");
	if seen == 1 then
		gotoState(statePtr, "test1");
		seen = 2
	end
end
