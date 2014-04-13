Log.Log("Start script")

function OnTakeDamage(damage)
	Log.Log('TakeDamage ' .. body.id .. ' ' .. damage ..
		' ' .. body.pos.x .. ' ' .. body.pos.y)
	if body.id == 'Hero1' then
		--Test(body.pos)
	end
	
	body.setRotation(body.ptr, body.getRotation(body.ptr) + 0.1)
end
