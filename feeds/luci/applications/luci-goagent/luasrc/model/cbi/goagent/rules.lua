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

m = Map("goagent", translate("GoAgent - Rule Settings"))

sihl4 = m:section(TypedSection, "profile", translate("Rule Lists"),
	translate("The following items specified the forwarding rules. For example, using crlf will lead GoAgent to use CrLf; using no forcehttps will lead GoAgent not to use https."))
sihl4.template  = "cbi/tblsection"
sihl4.addremove = true
sihl4.anonymous = true

o = sihl4:option(Value, "host", translate("Host/RegExp"))
o.width    = "50%"
o.template = "goagent/rule_host"

o = sihl4:option(Value, "items", translate("Rules"))
o.width    = "30%"
o.template = "goagent/rule_items"

return m
