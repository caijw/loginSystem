# ifndef LOGIN_STREAM_MANAGER_H
#define LOGIN_STREAM_MANAGER_H
/*
	login模块的server端stream管理单例类
*/
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <mutex>
#include <thread>



class LoginStreamManager
{
public:
	virtual ~LoginStreamManager();

	static LoginStreamManager* getInstance();
	/*更新接口*/
	void update(std::string uid, int seq);
	/*获取接口*/
	int get(std::string uid);
	/*移除接口*/
	void remove(std::string uid);

private:
	static LoginStreamManager* instance;

	LoginStreamManager();

	std::mutex login_user_seq_map_mutex;

	std::map<std::string, int> login_user_seq_map;
};
#endif