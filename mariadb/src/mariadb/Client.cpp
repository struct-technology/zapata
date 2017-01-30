/*
The MIT License (MIT)

Copyright (c) 2014 n@zgul <naazgull@dfz.pt>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <zapata/mariadb/Client.h>

zpt::mariadb::ClientPtr::ClientPtr(zpt::mariadb::Client * _target) : std::shared_ptr<zpt::mariadb::Client>(_target) {
}

zpt::mariadb::ClientPtr::ClientPtr(zpt::json _options, std::string _conf_path) : std::shared_ptr<zpt::mariadb::Client>(new zpt::mariadb::Client(_options, _conf_path)) {
}

zpt::mariadb::ClientPtr::~ClientPtr() {
}

zpt::mariadb::Client::Client(zpt::json _options, std::string _conf_path) : __options( _options), __mariadb_conf(_options->getPath(_conf_path)), __conf_path(_conf_path), __conn(sql::mysql::get_mysql_driver_instance()->connect(string("tcp://") + __mariadb_conf["bind"]->str(), std::string(__mariadb_conf["user"]), std::string(__mariadb_conf["passwd"]))) {
}

zpt::mariadb::Client::~Client() {
}

auto zpt::mariadb::Client::name() -> std::string {
	return std::string("mariadb://") + ((std::string) this->__mariadb_conf["bind"]) + std::string("/") + ((std::string) this->__mariadb_conf["db"]);
}

auto zpt::mariadb::Client::options() -> zpt::json {
	return this->__options;
}

auto zpt::mariadb::Client::events(zpt::ev::emitter _emitter) -> void {
	this->__events = _emitter;
}

auto zpt::mariadb::Client::events() -> zpt::ev::emitter {
	return this->__events;
}

auto zpt::mariadb::Client::mutations(zpt::mutation::emitter _emitter) -> void {
}

auto zpt::mariadb::Client::mutations() -> zpt::mutation::emitter {
	return this->__events->mutations();
}

auto zpt::mariadb::Client::connect(zpt::json _opts) -> void {
}

auto zpt::mariadb::Client::reconnect() -> void {
}

auto zpt::mariadb::Client::insert(std::string _collection, std::string _href_prefix, zpt::json _document, zpt::json _opts) -> std::string {	
	assertz(_document->ok() && _document->type() == zpt::JSObject, "'_document' must be of type JSObject", 412, 0);
	std::lock_guard< std::mutex > _lock(this->__mtx);
	std::unique_ptr<sql::Statement> _stmt(this->__conn->createStatement());
	_stmt->execute(string("USE ") + this->__mariadb_conf["db"]->str());

	if (!_document["id"]->ok()) {
		uuid _uuid;
		_uuid.make(UUID_MAKE_V1);
		_document << "id" << _uuid.string();
	}
	if (!_document["href"]->ok() && _href_prefix.length() != 0) {
		_document << "href" << (_href_prefix + (_href_prefix.back() != '/' ? std::string("/") : std::string("")) + _document["id"]->str());
	}
	
	std::string _expression("INSERT INTO ");
	_expression += _collection;
	_expression += std::string("(");
	std::string _columns;
	std::string _values;
	for (auto _c : _document->obj()){
		if (_columns.length() != 0) {
			_columns += std::string(",");
		}
		_columns += _c.first;
		if (_values.length() != 0) {
			_values += std::string(",");
		}
		std::string _val;
		_c.second->stringify(_val);
		_values += _val;
	}

	_expression += _columns + std::string(") VALUES (") + _values + (")");
	try {
		_stmt->execute(_expression);
	}
	catch(std::exception& _e) {
		assertz(false, _e.what(), 412, 0);
	}

	zpt::Connector::insert(_collection, _href_prefix, _document, _opts);
	return _document["id"]->str();
}

auto zpt::mariadb::Client::save(std::string _collection, std::string _href, zpt::json _document, zpt::json _opts) -> int {	
	assertz(_document->ok() && _document->type() == zpt::JSObject, "'_document' must be of type JSObject", 412, 0);
	std::lock_guard< std::mutex > _lock(this->__mtx);
	std::unique_ptr<sql::Statement> _stmt(this->__conn->createStatement());
	_stmt->execute(string("USE ") + this->__mariadb_conf["db"]->str());

	std::string _expression("UPDATE ");
	_expression += _collection;
	_expression += string(" SET ");
	std::string _sets;
	for (auto _c : _document->obj()){
		if (_sets.length() != 0) {
			_sets += string(",");
		}
		std::string _val;
		_c.second->stringify(_val);
		_sets += _c.first + string("=") + _val;
	}
	_expression += _sets;

	zpt::json _splited = zpt::split(_href, "/");
	_expression += string(" WHERE id=") + zpt::mariadb::escape(_splited->arr()->back()->str());

	try {
		_stmt->execute(_expression);
	}
	catch(std::exception& _e) {}

	zpt::Connector::save(_collection, _href, _document, _opts);
	return 1;
}

auto zpt::mariadb::Client::set(std::string _collection, std::string _href, zpt::json _document, zpt::json _opts) -> int {
	assertz(_document->ok() && _document->type() == zpt::JSObject, "'_document' must be of type JSObject", 412, 0);
	std::lock_guard< std::mutex > _lock(this->__mtx);
	std::unique_ptr<sql::Statement> _stmt(this->__conn->createStatement());
	_stmt->execute(string("USE ") + this->__mariadb_conf["db"]->str());

	std::string _expression("UPDATE ");
	_expression += _collection;
	_expression += string(" SET ");
	std::string _sets;
	for (auto _c : _document->obj()){
		if (_sets.length() != 0) {
			_sets += string(",");
		}
		std::string _val;
		_c.second->stringify(_val);
		_sets += _c.first + string("=") + _val;
	}
	_expression += _sets;

	zpt::json _splited = zpt::split(_href, "/");
	_expression += string(" WHERE id=") + zpt::mariadb::escape(_splited->arr()->back()->str());

	try {
		_stmt->execute(_expression);
	}
	catch(std::exception& _e) {}

	zpt::Connector::save(_collection, _href, _document, _opts);
	return 1;
}

auto zpt::mariadb::Client::set(std::string _collection, zpt::json _pattern, zpt::json _document, zpt::json _opts) -> int {
	assertz(_document->ok() && _document->type() == zpt::JSObject, "'_document' must be of type JSObject", 412, 0);
	std::lock_guard< std::mutex > _lock(this->__mtx);
	std::unique_ptr<sql::Statement> _stmt(this->__conn->createStatement());
	_stmt->execute(string("USE ") + this->__mariadb_conf["db"]->str());

	std::string _expression("UPDATE ");
	_expression += _collection;
	_expression += string(" SET ");
	std::string _sets;
	for (auto _c : _document->obj()){
		if (_sets.length() != 0) {
			_sets += string(",");
		}
		std::string _val;
		_c.second->stringify(_val);
		_sets += _c.first + string("=") + _val;
	}
	_expression += _sets;

	if (_pattern->ok() && _pattern->type() == zpt::JSObject) {
		std::string _where;
		zpt::mariadb::get_query(_pattern, _where);
		_expression += std::string(" WHERE ") + _where;
	}
	zpt::mariadb::get_query(_opts, _expression);
	
	int _size = 0;
	try {
		_size = _stmt->execute(_expression);
	}
	catch(std::exception& _e) {}

	zpt::Connector::set(_collection, _pattern, _document, _opts);
	return _size;
}

auto zpt::mariadb::Client::unset(std::string _collection, std::string _href, zpt::json _document, zpt::json _opts) -> int {
	assertz(_document->ok() && _document->type() == zpt::JSObject, "'_document' must be of type JSObject", 412, 0);
	std::lock_guard< std::mutex > _lock(this->__mtx);
	std::unique_ptr<sql::Statement> _stmt(this->__conn->createStatement());
	_stmt->execute(string("USE ") + this->__mariadb_conf["db"]->str());

	std::string _expression("UPDATE ");
	_expression += _collection;
	_expression += string(" SET ");
	std::string _sets;
	for (auto _c : _document->obj()){
		if (_sets.length() != 0) {
			_sets += string(",");
		}
		_sets += _c.first + string("=NULL");
	}
	_expression += _sets;

	zpt::json _splited = zpt::split(_href, "/");
	_expression += string(" WHERE id=") + zpt::mariadb::escape(_splited->arr()->back()->str());

	try {
		_stmt->execute(_expression);
	}
	catch(std::exception& _e) {}

	zpt::Connector::unset(_collection, _href, _document, _opts);
	return 1;
}

auto zpt::mariadb::Client::unset(std::string _collection, zpt::json _pattern, zpt::json _document, zpt::json _opts) -> int {
	assertz(_document->ok() && _document->type() == zpt::JSObject, "'_document' must be of type JSObject", 412, 0);
	std::lock_guard< std::mutex > _lock(this->__mtx);
	std::unique_ptr<sql::Statement> _stmt(this->__conn->createStatement());
	_stmt->execute(string("USE ") + this->__mariadb_conf["db"]->str());

	std::string _expression("UPDATE ");
	_expression += _collection;
	_expression += string(" SET ");
	std::string _sets;
	for (auto _c : _document->obj()){
		if (_sets.length() != 0) {
			_sets += string(",");
		}
		_sets += _c.first + string("=NULL");
	}
	_expression += _sets;

	if (_pattern->ok() && _pattern->type() == zpt::JSObject) {
		std::string _where;
		zpt::mariadb::get_query(_pattern, _where);
		_expression += std::string(" WHERE ") + _where;
	}
	zpt::mariadb::get_query(_opts, _expression);
	
	int _size = 0;
	try {
		_size = _stmt->execute(_expression);
	}
	catch(std::exception& _e) {}

	zpt::Connector::unset(_collection, _pattern, _document, _opts);
	return _size;
}

auto zpt::mariadb::Client::remove(std::string _collection, std::string _href, zpt::json _opts) -> int {
	std::lock_guard< std::mutex > _lock(this->__mtx);
	std::unique_ptr<sql::Statement> _stmt(this->__conn->createStatement());
	_stmt->execute(string("USE ") + this->__mariadb_conf["db"]->str());

	std::string _expression("DELETE FROM ");
	_expression += _collection;

	zpt::json _splited = zpt::split(_href, "/");
	_expression += string(" WHERE id=") + zpt::mariadb::escape(_splited->arr()->back()->str());

	try {
		_stmt->execute(_expression);
	}
	catch(std::exception& _e) {}

	zpt::Connector::remove(_collection, _href, _opts);
	return 1;
}

auto zpt::mariadb::Client::remove(std::string _collection, zpt::json _pattern, zpt::json _opts) -> int {
	std::lock_guard< std::mutex > _lock(this->__mtx);
	std::unique_ptr<sql::Statement> _stmt(this->__conn->createStatement());
	_stmt->execute(string("USE ") + this->__mariadb_conf["db"]->str());

	std::string _expression("DELETE FROM ");
	_expression += _collection;

	if (_pattern->ok() && _pattern->type() == zpt::JSObject) {
		std::string _where;
		zpt::mariadb::get_query(_pattern, _where);
		_expression += std::string(" WHERE ") + _where;
	}
	zpt::mariadb::get_query(_opts, _expression);
	
	int _size = 0;
	try {
		_size = _stmt->execute(_expression);
	}
	catch(std::exception& _e) {}

	zpt::Connector::remove(_collection, _pattern, _opts);
	return _size;
}

auto zpt::mariadb::Client::query(std::string _collection, std::string _pattern, zpt::json _opts) -> zpt::json {
	std::lock_guard< std::mutex > _lock(this->__mtx);
	zpt::json _elements = zpt::json::array();
	std::unique_ptr<sql::Statement> _stmt(this->__conn->createStatement());
	_stmt->execute(string("USE ") + this->__mariadb_conf["db"]->str());

	std::shared_ptr<sql::ResultSet> _result(_stmt->executeQuery(_pattern));
	for (; _result->next(); ) {
		_elements << zpt::mariadb::fromsql_r(_result);
	}

	return _elements;
}

auto zpt::mariadb::Client::query(std::string _collection, zpt::json _pattern, zpt::json _opts) -> zpt::json {
	std::string _expression("SELECT * FROM ");
	std::string _count_expression("SELECT COUNT(1) FROM ");
	_expression += _collection;
	_count_expression += _collection;
	if (_pattern->ok() && _pattern->type() == zpt::JSObject) {
		std::string _where;
		zpt::mariadb::get_query(_pattern, _where);
		_expression += std::string(" WHERE ") + _where;
		_count_expression += std::string(" WHERE ") + _where;
	}
	zpt::mariadb::get_query(_opts, _expression);
	size_t _size = size_t(this->query(_collection, _count_expression, _opts)[0]["count"]);
	zpt::json _return = {
		"size", _size, 
		"elements", this->query(_collection, _expression, _opts)
	};
	return _return;
	
}

auto zpt::mariadb::Client::all(std::string _collection, zpt::json _opts) -> zpt::json {
	std::string _expression("SELECT * FROM ");
	std::string _count_expression("SELECT COUNT(1) FROM ");
	_expression += _collection;
	_count_expression += _collection;
	zpt::mariadb::get_query(_opts, _expression);
	size_t _size = size_t(this->query(_collection, _count_expression, _opts)[0]["count"]);
	zpt::json _return = {
		"size", _size, 
		"elements", this->query(_collection, _expression, _opts)
	};
	return _return;
}