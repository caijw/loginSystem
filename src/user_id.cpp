#include "user_id.h"

user_id * user_id::instance = nullptr;

user_id::user_id(){

}

user_id::~user_id(){
	delete instance;
}

user_id* user_id::getInstance(){
	if(instance == nullptr){
		instance = new user_id();
	}
	return instance;
}

std::string user_id::getNewUserId(){
	auto size = user_id_vec.size();
	std::string uid = std::to_string(size + 1);
	user_id_vec.push_back(uid);
	return uid;
}
bool user_id::isUserIdValid(std::string uid){
	bool valid = false;
	for(auto ite = user_id_vec.begin(); ite != user_id_vec.end(); ++ite){
		if(uid == *ite){
			valid = true;
			break;
		}
	}
	return valid;
}