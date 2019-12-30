#!/bin/awk -f
#
# parameter: sink, the sink to retrieve

BEGIN { atsink="no" }

# if we're at the right sink, set flag
/^Sink #[0-9]$/ {
	gsub(/[^0-9]/, "", $2)
	if (sink == $2) { atsink = "yes" }
}

# atsink == "yes" && /^\s*Mute:/ { print }

# if flag, get volume
atsink == "yes" && /^\s*Mute:/ {
	print $2
	exit
}

