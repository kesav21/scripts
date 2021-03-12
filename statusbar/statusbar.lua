local M = {}

--
-- detect if being loaded as a library or being run
-- https://stackoverflow.com/questions/4521085/main-function-in-lua
--
-- if not pcall(debug.getlocal, 4, 1) then
--     print(M.bar())
-- end
--
-- if not debug.getinfo(3) then
--     print(M.bar())
-- end
--

local wm = require('wm')
local utils = require('utils')
local volume = require('volume')
local ethernet = require('ethernet')
local bluetooth = require('bluetooth')

function M.bar()
	local cache = os.getenv("XDG_CACHE_HOME")
	local monitors_path = string.format("%s/bin/statusbar.monitors", cache)
	local monitors = utils.file_as_table(monitors_path)

	local volume_bar = volume.bar()
	local bluetooth_bar = bluetooth.bar()
	local ethernet_bar = ethernet.bar()
	local clock_bar = os.date(" %I:%M %p")
	local calendar_bar = os.date(" %a, %b %d")

	local t = {}
	for key, monitor_name in pairs(monitors) do
		table.insert(t, string.format(
			"%%{S%s}%%{l}%s%%{r}%s  %s  %s  %s  %s ",
			key - 1, -- lemonbar starts counting monitors at 0
			wm.bar(monitor_name),
			volume_bar,
			bluetooth_bar,
			ethernet_bar,
			clock_bar,
			calendar_bar
		))
	end
	return table.concat(t, "")
end

return M
