local M = {}

function M.takeUntil(predicate, iterator)
	return function()
		local element = iterator()
		if predicate(element) then
			return element
		end
	end
end

function M.insert(value, iterator)
	local firstTime = true
	return function()
		if firstTime then
			firstTime = false
			return value
		else
			return iterator()
		end
	end
end

function M.append(value, iterator)
	local firstTime = true
	return function()
		local element = iterator()
		if element then
			return element
		elseif firstTime then
			firstTime = false
			return value
		end
	end
end

function M.map(transformation, iterator)
	return function()
		local element = iterator()
		if element then
			return transformation(element)
		end
	end
end

-- why did i create this?
-- because applying a function can be a bit cryptic,
-- but using this function clarifies what is happening
function M.first(iterator)
	return iterator()
end

function M.filter(predicate, iterator)
	local function next_match()
		local element = iterator()
		if element then
			if predicate(element) then
				return element
			else
				return next_match()
			end
		end
	end
	return next_match
end

function M.to_table(iterator)
	local t = {}
	for i in iterator do
		table.insert(t, i)
	end
	return t
end

-- https://stackoverflow.com/questions/22724092/difference-between-stateful-and-stateless-iterators-in-lua
function M.values(s)
	local f, _, var = pairs(s)
	return function()
		local i, v = f(s, var)
		var = i
		return v
	end
end

function M.filter_keys(predicate, s)
	local f, _, var = pairs(s)
	local function next_match()
		local i, v = f(s, var)
		var = i
		if v then
			if predicate(v) then
				return i
			else
				return next_match()
			end
		end
	end
	return next_match
end

function M.length(iterator)
	local count = 0
	for _ in iterator do
		count = count + 1
	end
	return count
end

return M
