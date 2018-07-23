#include "user_id.h"
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
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

int user_id::getNewUserId(std::string &uid){

	mongocxx::collection user_id_collection = Database::getInstance()->getCollection("user_id");
	auto doc_builder = bsoncxx::builder::stream::document{};
	bsoncxx::document::view_or_value doc = doc_builder
	  << "user_id_gen" << 1
	  << bsoncxx::builder::stream::finalize;
	bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result = user_id_collection.find_one(doc);

	if(maybe_result) {
	 	auto view = maybe_result->view();
		auto iter = view.find("begin_index");
		std::string begin_index;
		if (iter != view.end() && (*iter)){
		    begin_index = std::string(iter->get_utf8().value);
		    uid = std::to_string( std::stoi(begin_index) + 1 );
		}else{
			begin_index = "0";
			uid = "1";
		}

		auto query = document{}
		  << "user_id_gen" << 1
		  << "begin_index" << begin_index
		  << finalize;
		auto update = document{}
		  << "$set"
		  << open_document 
		  << "begin_index" << uid 
		  << close_document 
		  << finalize;
		auto ret = user_id_collection.update_one(query.view(), update.view());

		if(ret && ret->modified_count() == 1){
			return 0;
		}else{
			uid = "0";
			return -1;
		}

	}else{
		uid = "1";
		auto query = document{}
		  << "user_id_gen" << 1
		  << "begin_index" << uid
		  << finalize;

		bsoncxx::stdx::optional<mongocxx::result::insert_one> insert_result = user_id_collection.insert_one(query.view());

		if(!insert_result){
			uid = "0";
			return -2;
		}else{
			return 0;
		}

	}
	return 0;
}