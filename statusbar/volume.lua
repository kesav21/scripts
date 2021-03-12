local M = {}

local fileio = require('kk.fileio')

local function get_mute_icon(mute)
	if mute == 0 then
		return "墳"
	elseif mute == 1 then
		return "ﱝ"
	else
		return ""
	end
end

function M.bar()
	local cache = os.getenv("XDG_CACHE_HOME")
	local newest_path = string.format('%s/bin/sinkmon.newest_sink_index', cache)
	local newest = tonumber(fileio.firstline(newest_path))
	local mute_path = string.format('%s/bin/sinkmon.sinks.%s.mute', cache, newest)
	local mute_icon = get_mute_icon(tonumber(fileio.firstline(mute_path)))
	local volume_path = string.format('%s/bin/sinkmon.sinks.%s.volume', cache, newest)
	local volume = tonumber(fileio.firstline(volume_path))
	return string.format('%s %s%%', mute_icon, volume)
end

return M
