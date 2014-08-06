/*
    This file is part of Zapata.

    Zapata is free software: you can redistribute it and/or modify
    it under the terms of the Lesser GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Zapata is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <api/codes_rest.h>
#include <api/codes_users.h>
#include <base/assert.h>
#include <base/smart_ptr.h>
#include <db/convert_mongo.h>
#include <exceptions/AssertionException.h>
#include <http/HTTPObj.h>
#include <http/params.h>
#include <json/JSONObj.h>
#include <mongo/bson/bson-inl.h>
#include <mongo/bson/bsonmisc.h>
#include <mongo/bson/bsonobj.h>
#include <mongo/bson/bsonobjbuilder.h>
#include <mongo/client/connpool.h>
#include <mongo/client/dbclientcursor.h>
#include <mongo/client/dbclientinterface.h>
#include <parsers/json.h>
#include <resource/RESTResource.h>
#include <resource/UsersCollection.h>
#include <stddef.h>
#include <text/convert.h>
#include <memory>
#include <string>

zapata::UsersCollection::UsersCollection() :
	zapata::RESTCollection("^/users$") {
}

zapata::UsersCollection::~UsersCollection() {
}

void zapata::UsersCollection::get(HTTPReq& _req, HTTPRep& _rep) {
	mongo::ScopedDbConnection* _conn = mongo::ScopedDbConnection::getScopedDbConnection((string) this->configuration()["zapata"]["mongodb"]["address"]);
	string _collection((string) this->configuration()["zapata"]["mongodb"]["db"]);
	_collection.insert(_collection.length(), "." + ((string) this->configuration()["zapata_users"]["mongodb"]["collection"]));

	zapata::JSONObj _params;
	zapata::fromparams(_req, _params, zapata::RESTfulCollection, true);

	zapata::JSONObj _out;
	try {
		zapata::torestcollection(_conn, _collection, _params, _out);
	}
	catch (mongo::exception& _e) {
		_conn->done();
		delete _conn;
		assertz(false, _e.what(), zapata::HTTP500, zapata::ERRGeneric);
	}
	_conn->done();
	delete _conn;

	assertz(((size_t) _out["size"]) != 0, "The requested resource is empty", zapata::HTTP204, zapata::ERRResourceIsEmpty);

	string _text;
	zapata::tostr(_text, _out);
	_rep->status(zapata::HTTP200);
	_rep->body(_text);
	_rep << "Cache-Control" << "no-store" << "Pragma" << "no-cache" << "Content-Type" << "application/json" << "Content-Length" << _text.length();
}

void zapata::UsersCollection::post(HTTPReq& _req, HTTPRep& _rep) {
	string _body = _req->body();
	assertz(_body.length() != 0, "Body entity must be provided.", zapata::HTTP412, zapata::ERRBodyEntityMustBeProvided);

	string _content_type = _req->header("Content-Type");
	assertz(_content_type.find("application/json") != string::npos, "Body entity must be 'application/json'", zapata::HTTP406, zapata::ERRBodyEntityWrongContentType);

	zapata::JSONObj _record;
	zapata::fromstr(_body, _record);
	if (!_record["id"] && !!_record["email"]) {
		_record << "id" << (string) _record["email"];
	}
	if (!_record["email"] && !!_record["id"]) {
		string _email((string) _record["id"]);
		_email.insert(_email.length(), "@");
		_email.insert(_email.length(), (string) this->configuration()["zapata"] ["rest"]["bind_address"]);
		_record << "email" << _email;
	}

	assertz(!!_record["fullname"], "The 'name' field is mandatory", zapata::HTTP412, zapata::ERRNameMandatory);
	assertz(!!_record["id"], "The 'id' field is mandatory", zapata::HTTP412, zapata::ERRIDMandatory);
	assertz(!!_record["email"], "The 'email' field is mandatory", zapata::HTTP412, zapata::ERREmailMandatory);
	assertz(!!_record["password"], "The 'password' field is mandatory", zapata::HTTP412, zapata::ERRPasswordMandatory);
	assertz(!!_record["confirmation_password"], "The 'confirmation_password' field is mandatory", zapata::HTTP412, zapata::ERRConfirmationMandatory);
	assertz(((string ) _record["confirmation_password"]) == ((string ) _record["password"]), "The 'password' and 'confirmation_password' fields don't match", zapata::HTTP412, zapata::ERRPasswordConfirmationDontMatch);

	mongo::ScopedDbConnection* _conn = mongo::ScopedDbConnection::getScopedDbConnection((string) this->configuration()["zapata"]["mongodb"]["address"]);
	string _collection((string) this->configuration()["zapata"]["mongodb"]["db"]);
	_collection.insert(_collection.length(), "." + ((string) this->configuration()["zapata_users"]["mongodb"]["collection"]));

	string __id;
	bool _exists = true;
	try {

		unique_ptr<mongo::DBClientCursor> _ptr = (*_conn)->query(_collection, QUERY("id" << (string) _record["id"]));
		_exists = _ptr->more();
		_ptr->decouple();
		 (*_conn)->killCursor(_ptr->getCursorId());
		 delete _ptr.release();

		 if (!_exists) {
			bool _error = true;
			do {
				__id.assign("/users/");
				zapata::generate_key(__id);
				_record << "_id" << __id;

				mongo::BSONObjBuilder _bo;
				zapata::tomongo(_record, _bo);
				(*_conn)->insert(_collection, _bo.obj());

				_error = (*_conn)->getLastError().length() != 0;
			}
			while(_error);
		 }
	}
	catch (mongo::exception& _e) {
		_conn->done();
		delete _conn;
		assertz(false, _e.what(), zapata::HTTP500, zapata::ERRGeneric);
	}

	_conn->done();
	delete _conn;

	 assertz(!_exists, "Already exists a user identified by that ID", zapata::HTTP412, zapata::ERRUserAlreadyExists);

	zapata::JSONObj _rep_body;
	_rep_body << "href" << __id;
	string _text;
	zapata::tostr(_text, _rep_body);
	_rep->status(zapata::HTTP201);
	_rep << "Location" << __id << "Content-Length" << (long) _text.length();
	_rep->body(_text);

}

void zapata::UsersCollection::head(HTTPReq& _req, HTTPRep& _rep) {
	mongo::ScopedDbConnection* _conn = mongo::ScopedDbConnection::getScopedDbConnection((string) this->configuration()["zapata"]["mongodb"]["address"]);
	string _collection((string) this->configuration()["zapata"]["mongodb"]["db"]);
	_collection.insert(_collection.length(), "." + ((string) this->configuration()["zapata_users"]["mongodb"]["collection"]));

	zapata::JSONObj _params;
	zapata::fromparams(_req, _params, zapata::RESTfulCollection);

	zapata::JSONObj _out;
	try {
		zapata::torestcollection(_conn, _collection, _params, _out);
	}
	catch (mongo::exception& _e) {
		_conn->done();
		delete _conn;
		assertz(false, _e.what(), zapata::HTTP500, zapata::ERRGeneric);
	}
	_conn->done();
	delete _conn;

	assertz(((size_t) _out["size"]) != 0, "The requested resource is empty", zapata::HTTP204, zapata::ERRResourceIsEmpty);

	string _text;
	zapata::tostr(_text, _out);
	_rep->status(zapata::HTTP200);
	_rep << "Cache-Control" << "no-store" << "Pragma" << "no-cache" << "Content-Type" << "application/json" << "Content-Length" << _text.length();
}
