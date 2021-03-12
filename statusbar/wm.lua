local M = {}

local utils = require('utils')

local function as_numlist(input)
	local t = {}
	for _, v in pairs(input) do
		table.insert(t, tonumber(v))
	end
	return t
end

local function as_bitlist(all, positives)
	local t = {}
	for _, v in pairs(all) do
		t[v] = 0
	end
	for _, v in pairs(positives) do
		t[v] = 1
	end
	return t
end

local function list_all_desktops(monitor_name)
	local cmd = string.format(
		"bspc query --desktops --monitor %s --names",
		monitor_name
	)
	return as_numlist(utils.lines(utils.execute(cmd)))
end

local function get_isfocused(monitor_name, all)
	local cmd = string.format(
		"bspc query --desktops --monitor %s --desktop .focused --names",
		monitor_name
	)
	local bitlist = as_bitlist(all, as_numlist(utils.lines(utils.execute(cmd))))
	return function(desktop)
		return bitlist[desktop] == 1
	end
end

local function get_isactive(monitor_name, all)
	local cmd = string.format(
		"bspc query --desktops --monitor %s --desktop .active --names",
		monitor_name
	)
	local bitlist = as_bitlist(all, as_numlist(utils.lines(utils.execute(cmd))))
	return function(desktop)
		return bitlist[desktop] == 1
	end
end

local function get_isoccupied(monitor_name, all)
	local cmd = string.format(
		"bspc query --desktops --monitor %s --desktop .occupied --names",
		monitor_name
	)
	local bitlist = as_bitlist(all, as_numlist(utils.lines(utils.execute(cmd))))
	return function(desktop)
		return bitlist[desktop] == 1
	end
end

local function get_styler(isfocused, isactive, isoccupied)
	return function(desktop)
		local template = "%%{F%s}%%{B%s} %s %%{F-}%%{B-}"
		if isfocused(desktop) then
			return string.format(template, "#1d2021", "#ebdbb2", desktop)
		elseif isactive(desktop) then
			return string.format(template, "#1d2021", "#928374", desktop)
		elseif isoccupied(desktop) then
			return string.format(template, "#bdae93", "#3c3836", desktop)
		else
			return string.format(template, "-", "-", desktop)
		end
	end
end

function M.bar(monitor_name)
	local all = list_all_desktops(monitor_name)
	local style = get_styler(
		get_isfocused(monitor_name, all),
		get_isactive(monitor_name, all),
		get_isoccupied(monitor_name, all)
	)

	local t = {}
	for _, desktop in pairs(all) do
		table.insert(t, style(desktop))
	end
	return table.concat(t, "")
end

return M
