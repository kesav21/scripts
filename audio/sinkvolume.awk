#!/bin/awk -f
#
# parameter: sink, the sink to retrieve

BEGIN {
	FS="/"
	atsink="no"
}

# search for the given sink
/^Sink #([0-9])$/ {
	# retrieve only numbers from field 1
	gsub(/[^0-9]/, "", $1)
	# if we're at the right sink, set flag
	if (sink == $1) { atsink = "yes" }
}

# if flag, search for volume
atsink == "yes" && /^\s*Volume/ {
	# retrieve only numbers from field 2
	gsub(/[^0-9]/, "", $2)
	print $2
	# do not search any other sinks
	exit
}

