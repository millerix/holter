#ifndef CURLAPI_H
#define CURLAPI_H

#undef _WINDOWS_
#include <afxwin.h>   
#include <iostream>
#include "config_manager.h"

using namespace std;

size_t getlenfuc(const void *ptr, size_t size, size_t nmemb, void *stream);
size_t writefunc(const void *ptr, size_t size, size_t nmemb, void *stream);
size_t storetoken(const void *ptr, size_t size, size_t nmemb, string *stream);
size_t readfuc(void *ptr, size_t size, size_t nmemb, void *stream);

class curl_interface
{
public:
	curl_interface();

	string login(CString &userid, CString &password);
	int download(CString remotepath, CString localpath, long timeout, long tries);
	int upload(CString localpath, long timeout, long tries);
	int refresh_remotelist();
	int postjson(CString remotepath, CString json_info, long timeout, long tries);
	CString info2json();


public:
	config_manager _config_manager;
	string token;
};
#endif