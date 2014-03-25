Log.Log("Start")

Log.Log(deltatime)

if this ~= nil then
	Log.Log("ok")
else
	Log.Warn("this == nil")
end

if body ~= nil then
	if (body.pos ~= nil) then
		Log.Log("all is ok");
		Log.Log(body.pos.x)
		Log.Log(body.pos.y)
	end
else
	Log.Warn("body == nil")
end

Log.Log("End\n")


--Log.Log("1")
--Log.Warning("2")
--Log.Error("3")