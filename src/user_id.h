# ifndef USER_ID_H
#define USER_ID_H
/*
	生成user id的单例工具类，只是简单的递增id
	todo：用uuid库生成全局唯一id
*/
#include <vector>
#include <string>
class user_id
{
public:
	virtual ~user_id();

	static user_id* getInstance();
	/*生成新user id*/
	std::string getNewUserId();
	/*判断是否合法user id*/
	bool isUserIdValid(std::string);

private:
	static user_id* instance;

	user_id();

	std::vector<std::string> user_id_vec;
};
#endif