#include "config_manager.h"
#include "cJSON.h"
#include <iostream>
#include <sstream>
using namespace std;

config_manager::config_manager()
{
	configpath="F:\\holter.conf";
	try {
		fconfig.open(configpath, ifstream::in);
	}
	catch (exception &e) {
	//TRACE()
	//	TRACE(e.what());
		exit(-1);
	}
	if (fconfig.is_open()) {
		parse();
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
	upload_rest_base	= cJSON_GetObjectItem(root, "upload_rest")->valuestring;
	download_rest_base	= cJSON_GetObjectItem(root, "download_rest")->valuestring;
	cJSON_Delete(root);
	return;
}
config_manager::~config_manager()
{
	fconfig.close();
}