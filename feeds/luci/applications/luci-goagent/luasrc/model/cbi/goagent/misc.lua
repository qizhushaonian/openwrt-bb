--[[
LuCI - Lua Configuration Interface

Copyright 2008 Steven Barth <steven@midlink.org>
Copyright 2014 HackPascal <hackpascal@gmail.com>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

$Id$
]]--

m = Map("goagent", translate("GoAgent - Miscellaneous Settings"))


ss = m:section(NamedSection, "proxy", "proxy", translate("Secondary Proxy Settings"))

o = ss:option(Flag, "enabled", translate("Enabled"))

o = ss:option(Value, "host", translate("Host IP"))
o.datatype = "ip4addr"

o = ss:option(Value, "port", translate("Port"))
o.datatype = "uinteger"
o.default = "8080"

o = ss:option(Value, "username", translate("Username"))

o = ss:option(Value, "password", translate("Password"))
o.password = true


su = m:section(NamedSection, "useragent", "useragent", translate("User-Agent Settings"))

o = su:option(Flag, "enabled", translate("Enabled"))

o = su:option(Value, "string", translate("String"))
o.template = "goagent/useragent"


sa = m:section(NamedSection, "autorange", "autorange", translate("Auto range download"),
	translate("Auto range download is used to split big data into small pieces, i.e. multithread downloading. The hosts must support HTTP Range header."))

o = sa:option(Value, "hosts", translate("Host List"), translate("Use \"|\" to seperate each host."))
o.template = "goagent/autorange_hosts"

o = sa:option(Value, "endswith", translate("Allowed Extensions"))
o.template = "goagent/autorange_endswith"

o = sa:option(Value, "noendswith", translate("Disallowed Extensions"))
o.template = "goagent/autorange_noendswith"

o = sa:option(Value, "threads", translate("Threads"))
o.datatype = "uinteger"
o.default = "3"

o = sa:option(Value, "maxsize", translate("Max packet size"))
o.datatype = "uinteger"
o.default = "1048576"

o = sa:option(Value, "waitsize", translate("First R/W size"))
o.datatype = "uinteger"
o.default = "524288"

o = sa:option(Value, "bufsize", translate("Follow-up R/W size"))
o.datatype = "uinteger"
o.default = "8192"

return m
