--[[
LuCI - Lua Configuration Interface

Copyright 2014 HackPascal <hackpascal@gmail.com>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

$Id$
]]--

local fs = require "nixio.fs"

local f = SimpleForm("goagent",
	translate("GoAgent - Hosts File"),
	translate("This is the content of /etc/hosts."))

local o = f:field(Value, "_hosts")

o.template = "cbi/tvalue"
o.rows = 20

function o.cfgvalue(self, section)
	return fs.readfile("/etc/hosts")
end

function o.write(self, section, value)
	value = value:gsub("\r\n?", "\n")
	fs.writefile("/etc/hosts", value)
end

return f
