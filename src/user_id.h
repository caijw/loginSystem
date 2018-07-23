# ifndef USER_ID_H
#define USER_ID_H
/*
	简单生成user id的单例工具类，只是简单的递增id
	todo：用uuid库生成全局唯一id
*/
#include <vector>
#include <string>
#include "Database.h"
#include <iostream>
#include <plog/Log.h> 
class user_id
{
public:
	virtual ~user_id();
	static user_id* getInstance();
	/*生成新user id*/
	int getNewUserId(std::string &uid);

private:
	static user_id* instance;

	user_id();

	std::vector<std::string> user_id_vec;
};
#endif