local M = {}

local utils = require('utils')

function M.bar()
	local output = utils.execute("btconnected")
	if #output == 0 then
		return string.format("")
	else
		return string.format(" %s", table.concat(utils.lines(output), ","))
	end
end

return M
