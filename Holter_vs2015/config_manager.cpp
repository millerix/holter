#include "config_manager.h"
#include "cJSON.h"
#include <iostream>
#include <sstream>
#ifdef _DEBUG_
#include <afxwin.h>
#endif

using namespace std;

config_manager::config_manager()
{

	configpath="F:\\holter.conf";

	fconfig.open(configpath, ifstream::in);
	
	if (fconfig.is_open()) {
		parse();
	}
	else {
#ifdef _DEBUG_
		string errmsg = "file open failed, using default value";
		AfxMessageBox(errmsg.c_str());
#endif
		server_addr = "http://172.104.78.181";
		ftp_addr = "ftp://172.104.78.181:2121";
		ftp_id = "admin";
		ftp_psw = "admin";
		login_rest = "http://172.104.78.181/client/login.do";
		upload_rest = "http://172.104.78.181/case/postcase.do";
		download_rest = "";
	}
}
void config_manager::parse()
{
	cJSON* root;

	stringstream buffers;
	buffers << fconfig.rdbuf();
	string config_json(buffers.str());

	root = cJSON_Parse(config_json.c_str());
	server_addr = cJSON_GetObjectItem(root, "server_addr")->valuestring;
	ftp_addr	= cJSON_GetObjectItem(root, "ftp_addr")->valuestring;
	ftp_addr = cJSON_GetObjectItem(root, "ftp_id")->valuestring;
	ftp_addr = cJSON_GetObjectItem(root, "ftp_psw")->valuestring;
	login_rest	= cJSON_GetObjectItem(root, "login_rest")->valuestring;
	upload_rest	= cJSON_GetObjectItem(root, "upload_rest")->valuestring;
	download_rest	= cJSON_GetObjectItem(root, "download_rest")->valuestring;
	cJSON_Delete(root);
	return;
}
config_manager::~config_manager()
{
	fconfig.close();
}