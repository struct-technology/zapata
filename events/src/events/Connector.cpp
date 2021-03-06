/*
The MIT License (MIT)

Copyright (c) 2017 n@zgul <naazgull@dfz.pt>

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

#include <zapata/events/EventEmitter.h>

zpt::Connector::Connector() {}

zpt::Connector::~Connector() {}

auto zpt::Connector::connection() -> zpt::json { return this->__connection; }

auto zpt::Connector::connection(zpt::json _conn_conf) -> void { this->__connection = _conn_conf; }

auto zpt::Connector::connect() -> void {}

auto zpt::Connector::reconnect() -> void {}

auto zpt::Connector::insert(std::string _collection, std::string _href_prefix, zpt::json _record, zpt::json _opts)
    -> std::string {
	if (bool(_opts["mutated-event"]))
		return "";
	assertz(_record["href"]->ok(), "required fields: 'href'", 412, 0);
	this->events()->route(
	    zpt::ev::Reply,
	    zpt::path::join({zpt::array,
			     this->events()->version(),
			     "mutations",
			     "insert",
			     zpt::r_replace(std::string(_record["href"]),
					    std::string("/") + this->events()->version() + std::string("/"),
					    "")}),
	    {"headers",
	     (_opts["headers"] + zpt::json{"X-Connector", this->name()}),
	     "performative",
	     "insert",
	     "href",
	     _record["href"],
	     "new",
	     _record},
	    {"mqtt", true});
	return std::string(_record["href"]);
}

auto zpt::Connector::upsert(std::string _collection, std::string _href_prefix, zpt::json _record, zpt::json _opts)
    -> std::string {
	return std::string(_record["href"]);
}

auto zpt::Connector::save(std::string _collection, std::string _href, zpt::json _record, zpt::json _opts) -> int {
	if (bool(_opts["mutated-event"]))
		return 0;
	this->events()->route(
	    zpt::ev::Reply,
	    zpt::path::join(
		{zpt::array,
		 this->events()->version(),
		 "mutations",
		 "replace",
		 zpt::r_replace(_href, std::string("/") + this->events()->version() + std::string("/"), "")}),
	    {"headers",
	     (_opts["headers"] + zpt::json{"X-Connector", this->name()}),
	     "performative",
	     "save",
	     "href",
	     _href,
	     "new",
	     _record},
	    {"mqtt", true});
	return 0;
}

auto zpt::Connector::set(std::string _collection, std::string _href, zpt::json _record, zpt::json _opts) -> int {
	if (bool(_opts["mutated-event"]))
		return 0;
	this->events()->route(
	    zpt::ev::Reply,
	    zpt::path::join(
		{zpt::array,
		 this->events()->version(),
		 "mutations",
		 "update",
		 zpt::r_replace(_href, std::string("/") + this->events()->version() + std::string("/"), "")}),
	    {"headers",
	     (_opts["headers"] + zpt::json{"X-Connector", this->name()}),
	     "performative",
	     "set",
	     "href",
	     _href,
	     "changes",
	     _record},
	    {"mqtt", true});
	return 0;
}

auto zpt::Connector::set(std::string _collection, zpt::json _pattern, zpt::json _record, zpt::json _opts) -> int {
	if (bool(_opts["mutated-event"]))
		return 0;
	std::string _href = std::string(_opts["href"]);
	this->events()->route(
	    zpt::ev::Reply,
	    zpt::path::join(
		{zpt::array,
		 this->events()->version(),
		 "mutations",
		 "update",
		 zpt::r_replace(_href, std::string("/") + this->events()->version() + std::string("/"), "")}),
	    {"headers",
	     (_opts["headers"] + zpt::json{"X-Connector", this->name()}),
	     "performative",
	     "set",
	     "href",
	     _href,
	     "changes",
	     _record,
	     "filter",
	     _pattern},
	    {"mqtt", true});
	return 0;
}

auto zpt::Connector::unset(std::string _collection, std::string _href, zpt::json _record, zpt::json _opts) -> int {
	if (bool(_opts["mutated-event"]))
		return 0;
	this->events()->route(
	    zpt::ev::Reply,
	    zpt::path::join(
		{zpt::array,
		 this->events()->version(),
		 "mutations",
		 "update",
		 zpt::r_replace(_href, std::string("/") + this->events()->version() + std::string("/"), "")}),
	    {"headers",
	     (_opts["headers"] + zpt::json{"X-Connector", this->name()}),
	     "performative",
	     "unset",
	     "href",
	     _href,
	     "changes",
	     _record},
	    {"mqtt", true});
	return 0;
}

auto zpt::Connector::unset(std::string _collection, zpt::json _pattern, zpt::json _record, zpt::json _opts) -> int {
	if (bool(_opts["mutated-event"]))
		return 0;
	std::string _href = std::string(_opts["href"]);
	this->events()->route(
	    zpt::ev::Reply,
	    zpt::path::join(
		{zpt::array,
		 this->events()->version(),
		 "mutations",
		 "update",
		 zpt::r_replace(_href, std::string("/") + this->events()->version() + std::string("/"), "")}),
	    {"headers",
	     (_opts["headers"] + zpt::json{"X-Connector", this->name()}),
	     "performative",
	     "unset",
	     "href",
	     _href,
	     "changes",
	     _record,
	     "filter",
	     _pattern},
	    {"mqtt", true});
	return 0;
}

auto zpt::Connector::remove(std::string _collection, std::string _href, zpt::json _opts) -> int {
	if (bool(_opts["mutated-event"]))
		return 0;
	this->events()->route(
	    zpt::ev::Reply,
	    zpt::path::join(
		{zpt::array,
		 this->events()->version(),
		 "mutations",
		 "remove",
		 zpt::r_replace(_href, std::string("/") + this->events()->version() + std::string("/"), "")}),
	    {"headers",
	     (_opts["headers"] + zpt::json{"X-Connector", this->name()}),
	     "performative",
	     "remove",
	     "href",
	     _href,
	     "removed",
	     _opts["removed"]},
	    {"mqtt", true});
	return 0;
}

auto zpt::Connector::remove(std::string _collection, zpt::json _pattern, zpt::json _opts) -> int {
	if (bool(_opts["mutated-event"]))
		return 0;
	std::string _href = std::string(_opts["href"]);
	this->events()->route(
	    zpt::ev::Reply,
	    zpt::path::join(
		{zpt::array,
		 this->events()->version(),
		 "mutations",
		 "remove",
		 zpt::r_replace(_href, std::string("/") + this->events()->version() + std::string("/"), "")}),
	    {"headers",
	     (_opts["headers"] + zpt::json{"X-Connector", this->name()}),
	     "performative",
	     "remove",
	     "href",
	     _href,
	     "filter",
	     _pattern,
	     "removed",
	     _opts["removed"]},
	    {"mqtt", true});
	return 0;
}

auto zpt::Connector::get(std::string _collection, std::string _href, zpt::json _opts) -> zpt::json {
	return zpt::undefined;
}

auto zpt::Connector::query(std::string _collection, std::string _query, zpt::json _opts) -> zpt::json {
	return zpt::undefined;
}

auto zpt::Connector::query(std::string _collection, zpt::json _query, zpt::json _opts) -> zpt::json {
	return zpt::undefined;
}

auto zpt::Connector::all(std::string _collection, zpt::json _opts) -> zpt::json { return zpt::undefined; }

auto zpt::is_sql(std::string _name) -> bool {
	return _name.find("pgsql") != std::string::npos || _name.find("postgresql") != std::string::npos ||
	       _name.find("mysql") != std::string::npos || _name.find("mysql") != std::string::npos;
}
