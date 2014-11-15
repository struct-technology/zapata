/*
The MIT License (MIT)

Copyright (c) 2014 Pedro (n@zgul) Figueiredo <pedro.figueiredo@gmail.com>

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

#pragma once

#define DEBUG_JSON

#include <ostream>
#include <vector>
#include <zapata/core.h>

using namespace std;
#if !defined __APPLE__
using namespace __gnu_cxx;
#endif

#ifndef CRLF
#define CRLF "\r\n"
#endif

namespace zapata {

	extern string nil_header;
	extern const char* method_names[];
	extern const char* status_names[];

	enum HTTPType {
		HTTPRequest,
		HTTPReply
	};

	enum HTTPMethod {
		HTTPGet = 0,
		HTTPPut = 1,
		HTTPPost = 2,
		HTTPDelete = 3,
		HTTPHead = 4,
		HTTPTrace = 5,
		HTTPOptions = 6,
		HTTPPatch = 7,
		HTTPConnect = 8
	};

	enum HTTPStatus {
		HTTP100 = 100,
		HTTP101 = 101,
		HTTP102 = 102,
		HTTP200 = 200,
		HTTP201 = 201,
		HTTP202 = 202,
		HTTP203 = 203,
		HTTP204 = 204,
		HTTP205 = 205,
		HTTP206 = 206,
		HTTP207 = 207,
		HTTP208 = 208,
		HTTP226 = 226,
		HTTP300 = 300,
		HTTP301 = 301,
		HTTP302 = 302,
		HTTP303 = 303,
		HTTP304 = 304,
		HTTP305 = 305,
		HTTP306 = 306,
		HTTP307 = 307,
		HTTP308 = 308,
		HTTP400 = 400,
		HTTP401 = 401,
		HTTP402 = 402,
		HTTP403 = 403,
		HTTP404 = 404,
		HTTP405 = 405,
		HTTP406 = 406,
		HTTP407 = 407,
		HTTP408 = 408,
		HTTP409 = 409,
		HTTP410 = 410,
		HTTP411 = 411,
		HTTP412 = 412,
		HTTP413 = 413,
		HTTP414 = 414,
		HTTP415 = 415,
		HTTP416 = 416,
		HTTP417 = 417,
		HTTP422 = 422,
		HTTP423 = 423,
		HTTP424 = 424,
		HTTP425 = 425,
		HTTP426 = 426,
		HTTP427 = 427,
		HTTP428 = 428,
		HTTP429 = 429,
		HTTP430 = 430,
		HTTP431 = 431,
		HTTP500 = 500,
		HTTP501 = 501,
		HTTP502 = 502,
		HTTP503 = 503,
		HTTP504 = 504,
		HTTP505 = 505,
		HTTP506 = 506,
		HTTP507 = 507,
		HTTP508 = 508,
		HTTP509 = 509,
		HTTP510 = 510,
		HTTP511 = 511
	};

	typedef JSONObj HTTPObj;
	typedef JSONPtr HTTPPtr;

	void fromstr(string& _in, HTTPMethod* _out);

	class HTTPReq: public HTTPObj {
	public:
		HTTPReq();
		virtual ~HTTPReq();

		HTTPMethod method();
		void method(HTTPMethod);
		string& url();
		void url(string);
		string& body();
		void body(string);
		string& query();
		void query(string);
		string header(const char* _name);
		void header(const char* _name, const char* _value);
		void header(const char* _name, string _value);
		string param(const char* _name);
		void param(const char* _name, const char* _value);
		void param(const char* _name, string _value);

		virtual void stringify(string& _out);
		virtual void stringify(ostream& _out);

	private:
		string __url;
		string __query;
		string __body;
		HTTPMethod __method;

	};

	class HTTPRep: public HTTPObj {
	public:
		HTTPRep();
		virtual ~HTTPRep();

		HTTPStatus status();
		void status(HTTPStatus);
		string& body();
		void body(string);
		string header(const char* _idx);
		void header(const char* _name, const char* _value);
		void header(const char* _name, string _value);

		virtual void stringify(string& _out);
		virtual void stringify(ostream& _out);

	private:
		string __body;
		HTTPStatus __status;
	};

	void init(HTTPReq& _out);
	void init(HTTPRep& _out);
}