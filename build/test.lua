Log.log("Start script")

function OnTakeDamage(instigator, damageValue)
	Log.log("I'm damaged! My health is " .. body.getHealth(body.ptr));
end

function OnSeeEnemy(enemy)
	Log.log("I'm see you!")
	body.follow(body.ptr, enemy);
end
