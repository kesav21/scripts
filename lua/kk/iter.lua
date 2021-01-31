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

return M
