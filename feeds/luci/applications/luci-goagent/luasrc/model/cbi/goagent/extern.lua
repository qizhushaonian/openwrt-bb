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

m = Map("goagent", translate("GoAgent - External Settings"))


ss = m:section(NamedSection, "extern", "extern", translate("External executable Settings"))

o = ss:option(Value, "proxy_py", translate("proxy.py Path"))

o = ss:option(Value, "proxy_pac", translate("proxy.pac Path"))

o = ss:option(Value, "proxylib_py", translate("proxylib.py Path"))

o = ss:option(Value, "dnslib", translate("dnslib Path"))

o = ss:option(Value, "cert", translate("Certificate Path"))

o = ss:option(Flag, "replace_pac", translate("Replace proxy addresses in proxy.pac"))

return m
