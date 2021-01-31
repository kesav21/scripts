local M = {}

function M.takeUntil(predicate, Mator)
	return function()
		local element = Mator()
		if predicate(element) then
			return element
		end
	end
end

function M.insert(value, Mator)
	local firstTime = true
	return function()
		if firstTime then
			firstTime = false
			return value
		else
			return Mator()
		end
	end
end

function M.append(value, Mator)
	local firstTime = true
	return function()
		local element = Mator()
		if element then
			return element
		elseif firstTime then
			firstTime = false
			return value
		end
	end
end

function M.map(transformation, Mator)
	return function()
		local element = Mator()
		if element then
			return transformation(element)
		end
	end
end

-- why did i create this?
-- because applying a function can be a bit cryptic,
-- but using this function clarifies what is happening
function M.first(Mator)
	return Mator()
end

function M.filter(predicate, Mator)
	local function next_match()
		local element = Mator()
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
