#!/bin/sh

TOPDIR="${0%mkrevision.sh}"

[ -d "$TOPDIR/../build" ] || {
	echo "Please execute as ./build/mkrevision.sh" >&2
	exit 1
}

(
	cd "$TOPDIR"
	if cat ../version >/dev/null 2>/dev/null; then
		revision="svn-$(cat ../version)"
	elif git log -1 >/dev/null 2>/dev/null; then
		revision="svn-r$(LC_ALL=C git log -1 | sed -ne 's/.*git-svn-id: .*@\([0-9]\+\) .*/\1/p')"
		if [ "$revision" = "svn-r" ]; then
			revision="git-$(LC_ALL=C git log -1 --pretty=%h)"
		fi
	elif svn info >/dev/null 2>/dev/null; then
		revision="svn-r$(LC_ALL=C svn info | sed -ne 's/^Revision: //p')"
	else
		revision="unknown"
	fi

	echo "$revision"
)
