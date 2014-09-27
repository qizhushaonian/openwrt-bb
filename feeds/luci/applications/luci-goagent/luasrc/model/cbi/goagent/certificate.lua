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
	translate("GoAgent - Certificate Settings"),
	translate("GoAgent uses a fake root certificate, which used to generate website certs to replace the original certs. This is used to cheat browsers to avoid certificate warnings. This certificate will be applied the next time GoAgent starts. The certificate locates at /etc/goagent/CA.crt"))

local o = f:field(Value, "_fakecarootcert")

o.template = "cbi/tvalue"
o.rows = 40

function o.cfgvalue(self, section)
	return fs.readfile("/etc/goagent/CA.crt")
end

function o.write(self, section, value)
	value = value:gsub("\r\n?", "\n")
	fs.writefile("/etc/goagent/CA.crt", value)
end

return f
