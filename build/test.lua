Log.Log("Start script")

function OnTakeDamage(damageValue)
	Log.Log('TakeDamage', damageValue)
	return 1
end
--[[
function OnSpotEnemy()
{
	Log.Log('SpotEnemy');
}
--]]