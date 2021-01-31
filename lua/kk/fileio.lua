local M = {}

-- assumption: if the file can be opened, it exists
function M.exists(filename)
	local file = io.open(filename, 'r')
	if file then
		io.close(file)
		return true
	else
		return false
	end
end

function M.firstline(filename)
	local file = io.open(filename, 'r')
	if file then
		local lines = io.lines(filename)
		io.close(file)
		return lines() -- get first line
	end
end

return M
