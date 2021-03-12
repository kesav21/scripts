local M = {}

local utils = require('utils')

local function list_desktops(monitor_name)
	local cmd = string.format(
		"bspc query --desktops --monitor %s --names",
		monitor_name
	)
	local t = {}
	for _, v in pairs(utils.lines(utils.execute(cmd))) do
		table.insert(t, tonumber(v))
	end
	return t
end

local function get_focused_desktop(monitor_name)
	local cmd = string.format(
		"bspc query --desktops --monitor %s --desktop .active --names",
		monitor_name
	)
	return tonumber(utils.execute(cmd))
end

local function bar(desktops, focused)
	local t = {}
	for _, desktop in pairs(desktops) do
		if desktop == focused then
			table.insert(t, string.format("%%{R} %s %%{R}", desktop))
		else
			table.insert(t, string.format(" %s ", desktop))
		end
	end
	return table.concat(t, " ")
end

function M.bar(monitor_name)
	return bar(list_desktops(monitor_name), get_focused_desktop(monitor_name))
end

return M
