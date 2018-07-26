#include "loginStreamManager.h"

LoginStreamManager * LoginStreamManager::instance = nullptr;

LoginStreamManager::LoginStreamManager(){

}

LoginStreamManager::~LoginStreamManager(){
	delete instance;
}

LoginStreamManager* LoginStreamManager::getInstance(){
	if(instance == nullptr){
		instance = new LoginStreamManager();
	}
	return instance;
}

void LoginStreamManager::update(std::string uid, int seq){
	std::lock_guard<std::mutex> lock(login_user_seq_map_mutex);
	login_user_seq_map[uid] = seq;
}

int LoginStreamManager::get(std::string uid){
	std::lock_guard<std::mutex> lock(login_user_seq_map_mutex);
	if(login_user_seq_map.count(uid)){
		return login_user_seq_map[uid];
	}else{
		return -1; //not found flag
	}

}

void LoginStreamManager::remove(std::string uid){
	//todo
}

