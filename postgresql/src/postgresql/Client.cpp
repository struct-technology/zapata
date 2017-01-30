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

#include <zapata/postgresql/Client.h>

zpt::pgsql::ClientPtr::ClientPtr(zpt::pgsql::Client * _target) : std::shared_ptr<zpt::pgsql::Client>(_target) {
}

zpt::pgsql::ClientPtr::ClientPtr(zpt::json _options, std::string _conf_path) : std::shared_ptr<zpt::pgsql::Client>(new zpt::pgsql::Client(_options, _conf_path)) {
}

zpt::pgsql::ClientPtr::~ClientPtr() {
}

zpt::pgsql::Client::Client(zpt::json _options, std::string _conf_path) : __options( _options), __pgsql_conf(_options->getPath(_conf_path)), __conf_path(_conf_path), __conn(__pgsql_conf["bind"]->str() + std::string(";Database=") + __pgsql_conf["db"]->str() + (__pgsql_conf["user"]->ok() ? std::string(";User=") + __pgsql_conf["user"]->str() + std::string(";Password=") + __pgsql_conf["passwd"]->str() : "") + std::string(";")) {
}

zpt::pgsql::Client::~Client() {
}

auto zpt::pgsql::Client::name() -> std::string {
	return std::string("pgsql://") + ((std::string) this->__pgsql_conf["bind"]) + std::string("/") + ((std::string) this->__pgsql_conf["db"]);
}

auto zpt::pgsql::Client::options() -> zpt::json {
	return this->__options;
}

auto zpt::pgsql::Client::events(zpt::ev::emitter _emitter) -> void {
	this->__events = _emitter;
}

auto zpt::pgsql::Client::events() -> zpt::ev::emitter {
	return this->__events;
}

auto zpt::pgsql::Client::mutations(zpt::mutation::emitter _emitter) -> void {
}

auto zpt::pgsql::Client::mutations() -> zpt::mutation::emitter {
	return this->__events->mutations();
}

auto zpt::pgsql::Client::connect(zpt::json _opts) -> void {
}

auto zpt::pgsql::Client::reconnect() -> void {
}

auto zpt::pgsql::Client::insert(std::string _collection, std::string _href_prefix, zpt::json _document, zpt::json _opts) -> std::string {	
	assertz(_document->ok() && _document->type() == zpt::JSObject, "'_document' must be of type JSObject", 412, 0);
	std::lock_guard< std::mutex > _lock(this->__mtx);
	pqxx::work _stmt(this->__conn);

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
		_columns += zpt::pgsql::escape(_c.first);
		if (_values.length() != 0) {
			_values += std::string(",");
		}
		std::string _val = zpt::pgsql::escape(std::string(_c.second));
		_values += _val;
	}

	_expression += _columns + std::string(") VALUES (") + _values + (")");
	try {
		_stmt.exec(_expression);
		_stmt.commit();
	}
	catch(std::exception& _e) {
		assertz(false, _e.what(), 412, 0);
	}

	zpt::Connector::insert(_collection, _href_prefix, _document, _opts);
	return _document["id"]->str();
}

auto zpt::pgsql::Client::save(std::string _collection, std::string _href, zpt::json _document, zpt::json _opts) -> int {	
	assertz(_document->ok() && _document->type() == zpt::JSObject, "'_document' must be of type JSObject", 412, 0);
	std::lock_guard< std::mutex > _lock(this->__mtx);
	pqxx::work _stmt(this->__conn);

	std::string _expression("UPDATE ");
	_expression += _collection;
	_expression += string(" SET ");
	std::string _sets;
	for (auto _c : _document->obj()){
		if (_sets.length() != 0) {
			_sets += string(",");
		}
		std::string _val = zpt::pgsql::escape(std::string(_c.second));
		_sets += zpt::pgsql::escape(_c.first) + string("=") + _val;
	}
	_expression += _sets;

	zpt::json _splited = zpt::split(_href, "/");
	_expression += string(" WHERE id=") + zpt::pgsql::escape(_splited->arr()->back()->str());

	try {
		_stmt.exec(_expression);
		_stmt.commit();
	}
	catch(std::exception& _e) {}

	zpt::Connector::save(_collection, _href, _document, _opts);
	return 1;
}

auto zpt::pgsql::Client::set(std::string _collection, std::string _href, zpt::json _document, zpt::json _opts) -> int {
	assertz(_document->ok() && _document->type() == zpt::JSObject, "'_document' must be of type JSObject", 412, 0);
	std::lock_guard< std::mutex > _lock(this->__mtx);
	pqxx::work _stmt(this->__conn);

	std::string _expression("UPDATE ");
	_expression += _collection;
	_expression += string(" SET ");
	std::string _sets;
	for (auto _c : _document->obj()){
		if (_sets.length() != 0) {
			_sets += string(",");
		}
		std::string _val = zpt::pgsql::escape(std::string(_c.second));
		_sets += zpt::pgsql::escape(_c.first) + string("=") + _val;
	}
	_expression += _sets;

	zpt::json _splited = zpt::split(_href, "/");
	_expression += string(" WHERE id=") + zpt::pgsql::escape(_splited->arr()->back()->str());

	try {
		_stmt.exec(_expression);
		_stmt.commit();
	}
	catch(std::exception& _e) {}

	zpt::Connector::save(_collection, _href, _document, _opts);
	return 1;
}

auto zpt::pgsql::Client::set(std::string _collection, zpt::json _pattern, zpt::json _document, zpt::json _opts) -> int {
	assertz(_document->ok() && _document->type() == zpt::JSObject, "'_document' must be of type JSObject", 412, 0);
	std::lock_guard< std::mutex > _lock(this->__mtx);
	pqxx::work _stmt(this->__conn);

	std::string _expression("UPDATE ");
	_expression += _collection;
	_expression += string(" SET ");
	std::string _sets;
	for (auto _c : _document->obj()){
		if (_sets.length() != 0) {
			_sets += string(",");
		}
		std::string _val = zpt::pgsql::escape(std::string(_c.second));
		_sets += zpt::pgsql::escape(_c.first) + string("=") + _val;
	}
	_expression += _sets;

	if (_pattern->ok() && _pattern->type() == zpt::JSObject) {
		std::string _where;
		zpt::pgsql::get_query(_pattern, _where);
		_expression += std::string(" WHERE ") + _where;
	}
	zpt::pgsql::get_query(_opts, _expression);
	
	int _size = 0;
	try {
		_size = _stmt.exec(_expression)[0][0].as<int>();
		_stmt.commit();
	}
	catch(std::exception& _e) {}

	zpt::Connector::set(_collection, _pattern, _document, _opts);
	return _size;
}

auto zpt::pgsql::Client::unset(std::string _collection, std::string _href, zpt::json _document, zpt::json _opts) -> int {
	assertz(_document->ok() && _document->type() == zpt::JSObject, "'_document' must be of type JSObject", 412, 0);
	std::lock_guard< std::mutex > _lock(this->__mtx);
	pqxx::work _stmt(this->__conn);

	std::string _expression("UPDATE ");
	_expression += _collection;
	_expression += string(" SET ");
	std::string _sets;
	for (auto _c : _document->obj()){
		if (_sets.length() != 0) {
			_sets += string(",");
		}
		_sets += zpt::pgsql::escape(_c.first) + string("=NULL");
	}
	_expression += _sets;

	zpt::json _splited = zpt::split(_href, "/");
	_expression += string(" WHERE id=") + zpt::pgsql::escape(_splited->arr()->back()->str());

	try {
		_stmt.exec(_expression);
		_stmt.commit();
	}
	catch(std::exception& _e) {}

	zpt::Connector::unset(_collection, _href, _document, _opts);
	return 1;
}

auto zpt::pgsql::Client::unset(std::string _collection, zpt::json _pattern, zpt::json _document, zpt::json _opts) -> int {
	assertz(_document->ok() && _document->type() == zpt::JSObject, "'_document' must be of type JSObject", 412, 0);
	std::lock_guard< std::mutex > _lock(this->__mtx);
	pqxx::work _stmt(this->__conn);

	std::string _expression("UPDATE ");
	_expression += _collection;
	_expression += string(" SET ");
	std::string _sets;
	for (auto _c : _document->obj()){
		if (_sets.length() != 0) {
			_sets += string(",");
		}
		_sets += zpt::pgsql::escape(_c.first) + string("=NULL");
	}
	_expression += _sets;

	if (_pattern->ok() && _pattern->type() == zpt::JSObject) {
		std::string _where;
		zpt::pgsql::get_query(_pattern, _where);
		_expression += std::string(" WHERE ") + _where;
	}
	zpt::pgsql::get_query(_opts, _expression);
	
	int _size = 0;
	try {
		_size = _stmt.exec(_expression)[0][0].as<int>();
		_stmt.commit();
	}
	catch(std::exception& _e) {}

	zpt::Connector::unset(_collection, _pattern, _document, _opts);
	return _size;
}

auto zpt::pgsql::Client::remove(std::string _collection, std::string _href, zpt::json _opts) -> int {
	std::lock_guard< std::mutex > _lock(this->__mtx);
	pqxx::work _stmt(this->__conn);

	std::string _expression("DELETE FROM ");
	_expression += _collection;

	zpt::json _splited = zpt::split(_href, "/");
	_expression += string(" WHERE id=") + zpt::pgsql::escape(_splited->arr()->back()->str());

	try {
		_stmt.exec(_expression);
		_stmt.commit();
	}
	catch(std::exception& _e) {}

	zpt::Connector::remove(_collection, _href, _opts);
	return 1;
}

auto zpt::pgsql::Client::remove(std::string _collection, zpt::json _pattern, zpt::json _opts) -> int {
	std::lock_guard< std::mutex > _lock(this->__mtx);
	pqxx::work _stmt(this->__conn);

	std::string _expression("DELETE FROM ");
	_expression += _collection;

	if (_pattern->ok() && _pattern->type() == zpt::JSObject) {
		std::string _where;
		zpt::pgsql::get_query(_pattern, _where);
		_expression += std::string(" WHERE ") + _where;
	}
	zpt::pgsql::get_query(_opts, _expression);
	
	int _size = 0;
	try {
		_size = _stmt.exec(_expression)[0][0].as<int>();
		_stmt.commit();
	}
	catch(std::exception& _e) {}

	zpt::Connector::remove(_collection, _pattern, _opts);
	return _size;
}

auto zpt::pgsql::Client::query(std::string _collection, std::string _pattern, zpt::json _opts) -> zpt::json {
	std::lock_guard< std::mutex > _lock(this->__mtx);
	zpt::json _elements = zpt::json::array();
	pqxx::work _stmt(this->__conn);

	pqxx::result _result = _stmt.exec(_pattern);
	for (auto _r : _result) {
		_elements << zpt::pgsql::fromsql_r(_r);
	}
	return _elements;
}

auto zpt::pgsql::Client::query(std::string _collection, zpt::json _pattern, zpt::json _opts) -> zpt::json {
	std::string _expression("SELECT * FROM ");
	std::string _count_expression("SELECT COUNT(1) FROM ");
	_expression += _collection;
	_count_expression += _collection;
	if (_pattern->ok() && _pattern->type() == zpt::JSObject) {
		std::string _where;
		zpt::pgsql::get_query(_pattern, _where);
		_expression += std::string(" WHERE ") + _where;
		_count_expression += std::string(" WHERE ") + _where;
	}
	zpt::pgsql::get_query(_opts, _expression);
	size_t _size = size_t(this->query(_collection, _count_expression, _opts)[0]["count"]);
	zpt::json _return = {
		"size", _size, 
		"elements", this->query(_collection, _expression, _opts)
	};
	return _return;
}

auto zpt::pgsql::Client::all(std::string _collection, zpt::json _opts) -> zpt::json {
	std::string _expression("SELECT * FROM ");
	std::string _count_expression("SELECT COUNT(1) FROM ");
	_expression += _collection;
	_count_expression += _collection;
	zpt::pgsql::get_query(_opts, _expression);
	size_t _size = size_t(this->query(_collection, _count_expression, _opts)[0]["count"]);
	zpt::json _return = {
		"size", _size, 
		"elements", this->query(_collection, _expression, _opts)
	};
	return _return;
}