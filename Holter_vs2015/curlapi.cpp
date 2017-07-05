#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <new>
#include "curl/curl.h"
#include "cJSON.h"
#include "curlapi.h"
#include "config_manager.h"

using namespace std;

size_t getlenfuc(const void *ptr, size_t size, size_t nmemb, void *stream) {
	int r;
	long len = 0;

	/* _snscanf() is Win32 specific */
	// r = _snscanf(ptr, size * nmemb, "Content-Length: %ld\n", &len);
	r = sscanf_s((const char *)ptr, "Content-Length: %ld\n", &len);
	if (r) /* Microsoft: we don't read the specs */
		*((long *)stream) = len;

	return size * nmemb;
}

// store download files
size_t writefunc(const void *ptr, size_t size, size_t nmemb, void *stream)
{
	return fwrite(ptr, size, nmemb, (FILE *)stream);
}

//store the token in mem
size_t storetoken(const void *ptr, size_t size, size_t nmemb, string *stream)
{
	size_t sumleg = size * nmemb;
	
	if (sumleg > stream->size()) {
		try {
			stream->resize(sumleg);
		}
		catch (bad_alloc& e) {
#ifdef _DEBUG_
			cerr << "Caught: " << e.what() << endl;
			cerr << "Type: " << typeid(e).name() << endl << endl;
#endif
			return 0;
		}
	}
	copy((char*)ptr, (char*)ptr + sumleg, stream->begin());
	return sumleg;
}

// read the upload files
size_t readfuc(void *ptr, size_t size, size_t nmemb, void *stream)
{
	FILE *f = (FILE*)stream;
	size_t n;

	if (ferror(f))
		return CURL_READFUNC_ABORT;

	n = fread(ptr, size, nmemb, f) * size;

	return n;
}

curl_interface::curl_interface()
	:_config_manager()
	, token("")
{

}
string curl_interface::login(CString &userid, CString &password)
{
	CURL *curl = NULL;
	CURLcode res;
	string login_status;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl == 0) {
#ifdef _DEBUG_
		AfxMessageBox("curl init failing");
#endif
		return false;
	}

	CString postflds;
	postflds.Format("loginName=%s&password=%s", (LPSTR)(LPCSTR)userid, (LPSTR)(LPCSTR)password);
#ifdef _DEBUG_
	AfxMessageBox((LPSTR)(LPCSTR)postflds);
#endif

	curl_easy_setopt(curl, CURLOPT_URL, _config_manager.login_rest.c_str());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (LPSTR)(LPCSTR)postflds);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,storetoken);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &login_status);

	res = curl_easy_perform(curl);

	curl_easy_cleanup(curl);
	curl_global_cleanup();

	//check login status
	cJSON *root = cJSON_Parse(login_status.c_str());

	if (res != CURLE_OK) {
#ifdef _DEBUG_
		CString msg_cs;
		msg_cs.Format("res: %s", curl_easy_strerror(res));
		AfxMessageBox((LPCTSTR)msg_cs);
#endif
		exit(-1);
	}
	else if (cJSON_GetObjectItem(root, "status")->type == cJSON_True) {
		token = cJSON_GetObjectItem(root, "token")->valuestring;
#ifdef _DEBUG_
		AfxMessageBox("login PASS");
		AfxMessageBox(token.c_str());
#endif
		return token;
	}
}

int curl_interface::download(CString remotepath, CString localpath, long timeout, long tries)
{
	FILE *f;
	curl_off_t local_file_len = -1;
	long filesize = 0;
	CURL *curl;
	CURLcode r = CURLE_GOT_NOTHING;
	struct stat file_info;
	int use_resume = 0;


	if (stat((LPSTR)(LPCSTR)localpath, &file_info) == 0)
	{
		local_file_len = file_info.st_size;
		use_resume = 1;
	}
	//采用追加方式打开文件，便于实现文件断点续传工作
	f = fopen((LPSTR)(LPCSTR)localpath, "ab+");
	if (f == NULL) {
		AfxMessageBox("file open failed");
		return 0;
	}
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl == 0) {
		AfxMessageBox("curl init failing");
		return false;
	}

	curl_easy_setopt(curl, CURLOPT_URL, (LPSTR)(LPCSTR)remotepath);
	curl_easy_setopt(curl, CURLOPT_USERPWD, "admin:admin");
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout);  // 设置连接超时，单位秒 设置http 头部处理函数
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, getlenfuc);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, &filesize);
	// 设置文件续传的位置给libcurl
	curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, use_resume ? local_file_len : 0);

	curl_easy_setopt(curl, CURLOPT_WRITEDATA, f);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);

	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

	r = curl_easy_perform(curl);
	fclose(f);
	curl_easy_cleanup(curl);
	curl_global_cleanup();

	if (r == CURLE_OK) {
		return 1;
	}
	else {
		AfxMessageBox("perform download failed");
		return 0;
	}

}

int curl_interface::upload( CString localpath, long timeout, long tries)
{
	FILE *f;
	curl_off_t local_file_len = -1;
	long filesize = 0;
	CURL *curl;
	CURLcode r = CURLE_GOT_NOTHING;
	struct stat file_info;
	int use_resume = 0;


	if (stat((LPSTR)(LPCSTR)localpath, &file_info) == 0)
	{
		local_file_len = file_info.st_size;
		use_resume = 1;
	}
	//采用追加方式打开文件，便于实现文件断点续传工作
	f = fopen((LPSTR)(LPCSTR)localpath, "ab+");
	if (f == NULL) {
#ifdef _DEBUG_
		AfxMessageBox("open file fiailed");
#endif
		return 0;
	}

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl == 0) {
#ifdef _DEBUG_
		AfxMessageBox("curl init failing");
#endif
		return false;
	}
	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
	string userpsw = _config_manager.ftp_id + _config_manager.ftp_psw;
	curl_easy_setopt(curl, CURLOPT_USERPWD, userpsw.c_str());
	// 上传文件的命名？
	curl_easy_setopt(curl, CURLOPT_URL, _config_manager.upload_rest);
	// 设置连接超时，单位秒
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout);  
	//设置http 头部处理函数
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, getlenfuc);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, &filesize);
	// 设置文件续传的位置给libcurl
	curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, use_resume ? local_file_len : 0);

	curl_easy_setopt(curl, CURLOPT_READFUNCTION, readfuc);
	curl_easy_setopt(curl, CURLOPT_READDATA, f);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);


	r = curl_easy_perform(curl);

	curl_easy_cleanup(curl);
	curl_global_cleanup();
	fclose(f);

	if (r == CURLE_OK) {

		return 1;
	}
	else {
		AfxMessageBox("uploading perform failed");
		return 0;
	}
}

int curl_interface::refresh_remotelist()
{
	FILE *fremotelist;
	CString remotepath = "http://172.104.78.181/client/getCommonList.do";
	CURL *curl;
	CURLcode r = CURLE_GOT_NOTHING;

	string fremotelistpath = "F:\\remotelist.json";
	fremotelist = fopen(fremotelistpath.c_str(), "w+");
	if (fremotelist == NULL) {
		AfxMessageBox("open fremotelist file fiailed");
		return 0;
	}

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl == 0) {
		AfxMessageBox("curl init failing");
		return false;
	}



	curl_easy_setopt(curl, CURLOPT_URL, (LPSTR)(LPCSTR)remotepath);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "status=1&pgType=心电");

	curl_slist *plist = curl_slist_append(NULL,
		"Accept:application/json;Content-Type:application/json;charsets:UTF-8");
	string tokenhead = "token:" + token;
	plist = curl_slist_append(plist, tokenhead.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, plist);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fremotelist);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);


	r = curl_easy_perform(curl);

	curl_easy_cleanup(curl);
	curl_global_cleanup();
	fclose(fremotelist);

	if (r == CURLE_OK) {

		return 1;
	}
	else {
		AfxMessageBox("uploading perform failed");
		return 0;
	}
}

int curl_interface::postjson(CString remotepath, CString json_info, long timeout, long tries)
{

	curl_off_t local_file_len = -1;
	long filesize = 0;
	CURL *curl;
	CURLcode r = CURLE_GOT_NOTHING;
	FILE* fp;
	CString para = "caseJson=";
	CString Jsonpost = para + json_info;
	int use_resume = 0;

	fp = fopen("F:\\postjson.log", "ab");
	if (!fp) {
		AfxMessageBox("open file fiailed");
		return 0;
	}

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl == 0) {
		AfxMessageBox("curl init failing");
		return false;
	}

	curl_easy_setopt(curl, CURLOPT_URL, (LPSTR)(LPCSTR)remotepath);
	curl_slist *plist = curl_slist_append(NULL,
		"Accept:application/json;Content-Type:application/json;charsets:UTF-8");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, plist);

	// 设置要POST的JSON数据  
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (LPSTR)(LPCSTR)Jsonpost);
	//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_buf_str.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout);  // 设置连接超时，单位秒
															  //设置http 头部处理函数
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,writefunc);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "F:\\cookie.txt");


	r = curl_easy_perform(curl);

	curl_easy_cleanup(curl);
	curl_global_cleanup();
	fclose(fp);

	if (r == CURLE_OK) {

		return 1;
	}
	else {
		AfxMessageBox("postjson perform failed");
		return 0;
	}

}

CString curl_interface::info2json()
{

	fstream info_json;

	info_json.open("D:\\HolterDate\\Data\\Save005\\info.json", ios::in);
	if (!info_json.is_open()) {
		cout << "info_json file open failed" << endl;
		getchar();
		exit(0);
	}

	stringstream buffers;
	buffers << info_json.rdbuf();
	string json_buf_str(buffers.str());


	return json_buf_str.c_str();
}


