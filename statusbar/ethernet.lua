local M = {}

local fileio = require('kk.fileio')

function M.bar()
	local config = os.getenv("XDG_CONFIG_HOME")
	-- print(cache)
	local ifacepath = string.format("%s/bin/ethernet_interface", config)
	-- print(ifacepath)
	local iface = fileio.firstline(ifacepath)
	-- print(iface)
	local statepath = string.format("/sys/class/net/%s/operstate", iface)
	-- print(statepath)
	local state = fileio.firstline(statepath)
	-- print(state)
	if state == "up" then
		return ""
	else
		return ""
	end
end

return M
