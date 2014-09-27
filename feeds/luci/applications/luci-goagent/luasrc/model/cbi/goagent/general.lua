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

m = Map("goagent", translate("GoAgent - General Settings"),
	translate("GoAgent is a tool based on Google App Engine that can break the Greate FireWall."))

sl = m:section(NamedSection, "listen", "listen", translate("Listening Settings"))

o = sl:option(Flag, "enabled", translate("Enabled"))
o = sl:option(Value, "ip", translate("Listen IP"))
o.datatype = "ip4addr"
o.default = "0.0.0.0"
o = sl:option(Value, "port", translate("Listen Port"))
o.datatype    = "uinteger"
o.default = "8087"

o = sl:option(Value, "extip", translate("External IP"), translate("WAN IP address used for port forwarding."))
o.datatype = "ip4addr"

o = sl:option(Value, "extport", translate("External Port"), translate("WAN Port used for port forwarding."))
o.datatype = "uinteger"

o = sl:option(Value, "username", translate("Access username"), translate("Used for proxy access authentication"))
o = sl:option(Value, "password", translate("Access password"))


sg = m:section(NamedSection, "gae", "gae", translate("Google App Engine Settings"))

o = sg:option(Value, "appid", translate("App ID"), translate("Use \"|\" to seperate each AppID."))

o = sg:option(Value, "password", translate("Password"))
o.password = true

o = sg:option(Value, "path", translate("Server Path"))
o.default = "/2"

o = sg:option(ListValue, "mode", translate("Access Mode"), translate("Whether to use secured http to access the GAE."))
o:value("http", translate("HTTP"))
o:value("https", translate("HTTPS"))
o.default = "https"

o = sg:option(ListValue, "sslversion", translate("SSL version"))
o:value("SSLv2")
o:value("SSLv3")
o:value("SSLv23")
o:value("TLSv1")
o.default = "TLSv1"

o = sg:option(ListValue, "profile", translate("Profile"),
	translate("If IPv6 is selected, the \"IPv6 Rule Lists\" will be used. Otherwise the IPv4* will be used. This will only be applied in IPv6 environment."))
o:value("ipv4", translate("IPv4"))
o:value("ipv6", translate("IPv6"))
o.default = "ipv4"

o = sg:option(Value, "window", translate("Window"), translate("Number of IPs to be used."))
o.datatype    = "uinteger"
o.default = "4"

o = sg:option(Flag, "cachesock", translate("Cache used IP connections"))

o = sg:option(Flag, "keepalive", translate("Keep alive"), translate("Keep connection alive"))

o = sg:option(Flag, "obfuscate", translate("Traffic obfuscate"))

o = sg:option(Flag, "validate", translate("Certificate validation"))

o = sg:option(Flag, "pagespeed", translate("Use pagespeed to access GAE"))

o = sg:option(Value, "maxsize", translate("Max. return size"), translate("Maximum return size in each urlfetch"))
o.datatype    = "uinteger"
o.default = "2097152"

o = sg:option(Flag, "rc4", translate("Enable RC4 Encryption"),
	translate("If enabled, the Password field must be set. It is not recommended to be enabled when Access Mode is set to HTTPS."))

o = sg:option(Value, "regions", translate("Directly-connect regions"),
	translate("Specify country regions used to connect directly. Country codes listed in http://dev.maxmind.com/geoip/legacy/codes/iso3166/"))


sp = m:section(NamedSection, "pac", "pac", translate("Proxy Auto-Config (PAC) Settings"))

o = sp:option(Flag, "enabled", translate("Enabled"),
	translate("If enabled, the PAC address is http://&lt;Server IP&gt;:&lt;Port&gt;/proxy.pac"))

o = sp:option(Value, "ip", translate("Server IP"))
o.datatype = "ip4addr"
o.default = "0.0.0.0"

o = sp:option(Value, "port", translate("Port"))
o.datatype = "uinteger"
o.default = "8086"

o = sp:option(Value, "extip", translate("External IP"), translate("WAN IP address used for port forwarding."))
o.datatype = "ip4addr"

o = sp:option(Value, "extport", translate("External Port"), translate("WAN Port used for port forwarding."))
o.datatype = "uinteger"

o = sp:option(Value, "gfwlist", translate("GFW blocked list subscription URL"))
o.template = "goagent/gfwlist"

o = sp:option(Value, "adblock", translate("AdBlock list subscription URL"))
o.template = "goagent/adblock"

o = sp:option(Value, "expired", translate("Automatic update cycle"))
o.datatype    = "uinteger"
o.default = "86400"


sil = m:section(TypedSection, "iplist", translate("IP Lists"),
	translate("IP Lists to be used to connect to the GAE. Use \"|\" to seperate each IP/Domain."))
sil.template  = "cbi/tblsection"
sil.addremove = true
sil.anonymous = true

sil:option(Value, "name", translate("Name"))

o = sil:option(Value, "ips", translate("IPs/Domains"))
o.width    = "60%"
o.template = "goagent/iplist_ips"

return m
