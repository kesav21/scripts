local M = {}

function M.file_as_table(filename)
	local file = io.open(filename, 'r')
	if file then
		local t = {}
		for line in io.lines(filename) do
			table.insert(t, line)
		end
		io.close(file)
		return t
	end
end

function M.execute(cmd)
	local f = assert(io.popen(cmd, "r"))
	local s = assert(f:read("*a"))
	-- local s = assert(io.read("*a"))
	io.close(f)
	return s
end

-- function M.trim_whitespace(s)
-- 	-- remove leading whitespace
-- 	local a = string.gsub(s, "^%s+", "")
-- 	-- remove trailing whitespace
-- 	local b = string.gsub(a, "%s+$", "")
-- 	-- replace newlines with spaces
-- 	local c = string.gsub(b, "[\n\r]+", " ")
-- 	return c
-- end

function M.lines(input)
	local t = {}
	for s in string.gmatch(input, "[^\r\n]+") do
		table.insert(t, s)
	end
	return t
end

-- function M.words(input)
-- 	local t = {}
-- 	for s in string.gmatch(input, "[^%s]+") do
-- 		table.insert(t, s)
-- 	end
-- 	return t
-- end

return M
