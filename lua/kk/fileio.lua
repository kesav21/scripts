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

function M.writeline(filename, data)
	local file = io.open(filename, 'w')
	if file then
		io.output(file)
		io.write(data)
		return io.close(file)
	else
		return false
	end
end

-- TODO: test all the functions in this module
-- eq(true, fileio.writeline(statusbar_path, "kk"))
-- eq("kk", fileio.firstline(statusbar_path))

return M
