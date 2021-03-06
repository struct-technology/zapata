#pragma once

#include <string>
#include <zapata/rest.h>
#include <zapata/postgresql.h>
#include <zapata/mysql.h>
#include <zapata/mongodb.h>
#include <zapata/redis.h>
#include <zapata/applications/datums/ResourceOwners.h>
#include <zapata/applications/datums/Applications.h>
#include <zapata/applications/datums/MyApplications.h>
#include <zapata/applications/datums/MyUsers.h>

using namespace std;
#if !defined __APPLE__
using namespace __gnu_cxx;
#endif

namespace zpt {
namespace apps {

namespace datums {
namespace MyUsers {
auto get(std::string _topic, zpt::ev::emitter _emitter, zpt::json _identity, zpt::json _envelope) -> zpt::json;
auto query(std::string _topic, zpt::json _filter, zpt::ev::emitter _emitter, zpt::json _identity, zpt::json _envelope)
    -> zpt::json;
auto insert(std::string _topic,
	    zpt::json _document,
	    zpt::ev::emitter _emitter,
	    zpt::json _identity,
	    zpt::json _envelope) -> zpt::json;
auto save(std::string _topic, zpt::json _document, zpt::ev::emitter _emitter, zpt::json _identity, zpt::json _envelope)
    -> zpt::json;
auto set(std::string _topic, zpt::json _document, zpt::ev::emitter _emitter, zpt::json _identity, zpt::json _envelope)
    -> zpt::json;
auto set(std::string _topic,
	 zpt::json _document,
	 zpt::json _filter,
	 zpt::ev::emitter _emitter,
	 zpt::json _identity,
	 zpt::json _envelope) -> zpt::json;
auto remove(std::string _topic, zpt::ev::emitter _emitter, zpt::json _identity, zpt::json _envelope) -> zpt::json;
auto remove(std::string _topic, zpt::json _filter, zpt::ev::emitter _emitter, zpt::json _identity, zpt::json _envelope)
    -> zpt::json;
}
}
}
}
