Log.log("Start script")

function OnTakeDamage(instigator, damageValue)
	Log.log("I'm damaged! My health is " .. body.getHealth(body.ptr))
end

local seen = 1

function OnSeeEnemy(enemy)
	if seen == 1 then
		Log.log("I'm see you!")
		body.follow(body.ptr, enemy)
		seen = 2
	end
end
