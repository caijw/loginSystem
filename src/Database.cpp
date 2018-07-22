#include "Database.h"
Database* Database::instance = nullptr;

Database* Database::getInstance(){
	if(instance == nullptr){
		instance = new Database();
	}
	return instance;
}

Database::Database(){

}
Database::~Database(){
	/*todo close the conn*/
}
mongocxx::collection Database::getCollection(std::string str_collection){
	if(!connInit || !dbInit){
		throw "db has not yet set";
	}else{
		return db[str_collection];
	}
};
void Database::setDb(std::string str_uri, std::string str_db){
	mongocxx::instance inst{};
	mongocxx::uri uri(str_uri);
	mongocxx::client conn{uri};
	mongocxx::database db = conn[str_db];
	this->conn = std::move(conn);
	this->db = db;
	this->connInit = true;
	this->dbInit = true;
}