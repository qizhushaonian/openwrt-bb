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
	translate("GoAgent - Custom PAC file"),
	translate("The \"PROXY 127.0.0.1:8087\" and \"PROXY 127.0.0.1:8086\" will be replaced with External IP and Port. This file will be applied the next time GoAgent starts."))

local o = f:field(Value, "_custompac")

o.template = "cbi/tvalue"
o.rows = 20

function o.cfgvalue(self, section)
	return fs.readfile("/etc/goagent/proxy.pac")
end

function o.write(self, section, value)
	value = value:gsub("\r\n?", "\n")
	fs.writefile("/etc/goagent/proxy.pac", value)
end

return f
