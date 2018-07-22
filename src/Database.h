#ifndef DATABASE_H
#define DATABASE_H
/*
	数据库管理类
*/
#include <string>
#include <utility>
/*mongodb headers begin*/
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
/*mongodb headers end*/

class Database
{
public:
	static Database* getInstance();
	mongocxx::collection getCollection(std::string str_collection);
	void setDb(std::string str_uri, std::string str_db);
	virtual ~Database();
private:
	static Database* instance;
	Database();
	mongocxx::client conn;
	mongocxx::database db;
	bool connInit;
	bool dbInit;
};
#endif