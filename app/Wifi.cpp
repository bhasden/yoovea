#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <AppSettings.h>
#include <Wifi.h>

BssList networks;
HttpServer server;
String network, password, currentNetwork;
Timer connectionTimer;

WifiClass::WifiClass()
{
	System.onReady(this);
}

void networkScanCompleted(bool succeeded, BssList list)
{
	if (succeeded)
	{
		for (int i = 0; i < list.count(); i++)
			if (!list[i].hidden && list[i].ssid.length() > 0)
				networks.add(list[i]);
	}

	networks.sort([](const BssInfo& a, const BssInfo& b){ return b.rssi - a.rssi; } );
}

void onIndex(HttpRequest &request, HttpResponse &response)
{
	TemplateFileStream *tmpl = new TemplateFileStream("index.html");
	auto &vars = tmpl->variables();
	response.sendTemplate(tmpl); // will be automatically deleted
}

void onIpConfig(HttpRequest &request, HttpResponse &response)
{
	if (request.getRequestMethod() == RequestMethod::POST)
	{
		AppSettings.dhcp = request.getPostParameter("dhcp") == "1";
		AppSettings.ip = request.getPostParameter("ip");
		AppSettings.netmask = request.getPostParameter("netmask");
		AppSettings.gateway = request.getPostParameter("gateway");
		debugf("Updating IP settings: %d", AppSettings.ip.isNull());
		AppSettings.save();
	}

	TemplateFileStream *tmpl = new TemplateFileStream("settings.html");
	auto &vars = tmpl->variables();

	bool dhcp = WifiStation.isEnabledDHCP();
	vars["dhcpon"] = dhcp ? "checked='checked'" : "";
	vars["dhcpoff"] = !dhcp ? "checked='checked'" : "";

	if (!WifiStation.getIP().isNull())
	{
		vars["ip"] = WifiStation.getIP().toString();
		vars["netmask"] = WifiStation.getNetworkMask().toString();
		vars["gateway"] = WifiStation.getNetworkGateway().toString();
	}
	else
	{
		vars["ip"] = "192.168.1.77";
		vars["netmask"] = "255.255.255.0";
		vars["gateway"] = "192.168.1.1";
	}

	response.sendTemplate(tmpl); // will be automatically deleted
}

void onFile(HttpRequest &request, HttpResponse &response)
{
	String file = request.getPath();
	if (file[0] == '/')
		file = file.substring(1);

	if (file[0] == '.')
		response.forbidden();
	else
	{
		response.setCache(86400, true); // It's important to use cache for better performance.
		response.sendFile(file);
	}
}

void onAjaxNetworkList(HttpRequest &request, HttpResponse &response)
{
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject& json = stream->getRoot();

	json["status"] = (bool)true;

	bool connected = WifiStation.isConnected();
	json["connected"] = connected;
	if (connected)
	{
		currentNetwork = WifiStation.getSSID();
		json["network"] = currentNetwork.c_str();
	}

	JsonArray& netlist = json.createNestedArray("available");
	for (int i = 0; i < networks.count(); i++)
	{
		//if (networks[i].hidden) continue;
		JsonObject &item = netlist.createNestedObject();
		//debugf("%s %s", test.c_str(), networks[i].ssid.c_str());
		item.add("id", (int)networks[i].getHashId());
		item.add("title", networks[i].ssid.c_str());
		item.add("signal", networks[i].rssi);
		item.add("encryption", networks[i].getAuthorizationMethodName());
	}

	response.setAllowCrossDomainOrigin("*");
	response.sendJsonObject(stream);
}

void makeConnection()
{
	WifiStation.enable(true);
	WifiStation.config(network, password);

	AppSettings.ssid = network;
	AppSettings.password = password;
	AppSettings.save();

	network = ""; // completed
}

void onAjaxConnect(HttpRequest &request, HttpResponse &response)
{
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject& json = stream->getRoot();

	String curNet = request.getPostParameter("network");
	String curPass = request.getPostParameter("password");

	bool updating = curNet.length() > 0 && (WifiStation.getSSID() != curNet || WifiStation.getPassword() != curPass);
	bool connectingNow = WifiStation.getConnectionStatus() == eSCS_Connecting || network.length() > 0;

	if (updating && connectingNow)
	{
		debugf("wrong action: %s %s, (updating: %d, connectingNow: %d)", network.c_str(), password.c_str(), updating, connectingNow);
		json["status"] = (bool)false;
		json["connected"] = (bool)false;
	}
	else
	{
		json["status"] = (bool)true;
		if (updating)
		{
			network = curNet;
			password = curPass;
			debugf("CONNECT TO: %s %s", network.c_str(), password.c_str());
			json["connected"] = false;
			connectionTimer.initializeMs(1200, makeConnection).startOnce();
		}
		else
		{
			json["connected"] = WifiStation.isConnected();
			debugf("Network already selected. Current status: %s", WifiStation.getConnectionStatusName());
		}
	}

	if (!updating && !connectingNow && WifiStation.isConnectionFailed())
		json["error"] = WifiStation.getConnectionStatusName();

	response.setAllowCrossDomainOrigin("*");
	response.sendJsonObject(stream);
}

void connectOk()
{
	debugf("Connection succeeded");
	WifiAccessPoint.enable(false);
}

void connectFail()
{
	debugf("Connection failed");

	WifiAccessPoint.config("Sprinkler Configuration " + WifiAccessPoint.getMAC().substring(WifiAccessPoint.getMAC().length() - 4), "", AUTH_OPEN);
	WifiAccessPoint.enable(true);

	WifiStation.enable(false);
	WifiStation.startScan(networkScanCompleted);
	WifiStation.waitConnection(connectOk); // Wait connection

	server.listen(80);
	server.addPath("/", onIndex);
	server.addPath("/ipconfig", onIpConfig);
	server.addPath("/ajax/get-networks", onAjaxNetworkList);
	server.addPath("/ajax/connect", onAjaxConnect);
	server.setDefaultHandler(onFile);
}

void WifiClass::start()
{
	debugf("Starting wifi...");

	if (AppSettings.exist())
	{
		WifiStation.config(AppSettings.ssid, AppSettings.password);

		if (!AppSettings.dhcp && !AppSettings.ip.isNull())
			WifiStation.setIP(AppSettings.ip, AppSettings.netmask, AppSettings.gateway);
	}

	WifiAccessPoint.enable(false);
	WifiStation.enable(true);
	WifiStation.startScan(networkScanCompleted);

	/*
	WifiStation.onConnectFail += connectFail;
    WifiStation.onConnectOk += connectOk;
    WifiStation.connectionTimeOut = 20;
    WifiStation.scanCompletedCallback += networkScanCompleted;
	*/

	WifiStation.waitConnection(connectOk, 20, connectFail); // We recommend 20+ seconds for connection timeout at start
}

void WifiClass::onSystemReady()
{
	debugf("System ready (wifi)...");
	start();
}
