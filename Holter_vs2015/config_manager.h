#pragma once
#include <fstream>
using namespace std;



class config_manager
{
public:
	config_manager();
	void parse();
	~config_manager();

	string configpath;
	ifstream fconfig;
	string server_addr;
	string ftp_addr;
	string ftp_id;
	string ftp_psw;
	string login_rest;
	string upload_rest;
	string download_rest;
};