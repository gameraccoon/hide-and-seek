Log.Log("Start script")

function OnTakeDamage(damage)
	Log.Log('it works!')
	body.follow(body.ptr, instigator);
end
