/*
The MIT License (MIT)

Copyright (c) 2017 n@zgul <n@zgul.me>

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

#include <zapata/zmq/ZMQPolling.h>
#include <zapata/zmq.h>
#include <chrono>
#include <ossp/uuid++.hh>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iomanip>

#define HTTP_REP 0
#define HTTP_REQ 1

#define within(num) (int)((float)(num)*random() / (RAND_MAX + 1.0))

namespace zpt {
zmq::context_t __context(1);
}

zpt::ZMQReq::ZMQReq(std::string _connection, zpt::json _options)
    : zpt::Channel(_connection, _options), __socket(nullptr) {
	this->__socket = zmq::socket_ptr(new zmq::socket_t(zpt::__context, ZMQ_REQ));
	// this->__socket->setsockopt(ZMQ_SNDTIMEO, 10000);
	this->uri(_connection);
	if (this->uri()["scheme"] == zpt::json::string("tcp") && this->uri()["type"] == zpt::json::string("@")) {
		int _available = 1025;
		if (this->uri()["port"] != zpt::json::string("*")) {
			_available = int(this->uri()["port"]);
		}
		do {
			this->connection(std::string("@tcp://") + std::string(this->uri()["domain"]) +
					 std::string(":") + std::to_string(_available));
			this->uri() << "port" << std::to_string(_available);
			try {
				this->__socket->bind(std::string("tcp://") + std::string(this->uri()["domain"]) +
						     std::string(":") + std::to_string(_available));
				break;
			} catch (zmq::error_t& _e) {
			}
			_available++;
		} while (_available < 60999);
		assertz(_available < 60999, std::string("could not attach socket to ") + _connection, 500, 0);
	} else {
		this->connection(std::string(this->uri()["type"]) + std::string(this->uri()["scheme"]) +
				 std::string("://") + std::string(this->uri()["authority"]));
		try {
			if (this->uri()["type"] == zpt::json::string("@")) {
				this->__socket->bind(std::string(this->uri()["scheme"]) + std::string("://") +
						     std::string(this->uri()["authority"]));
			} else {
				this->__socket->connect(std::string(this->uri()["scheme"]) + std::string("://") +
							std::string(this->uri()["authority"]));
			}
		} catch (zmq::error_t& _e) {
			assertz(false, std::string("could not attach socket to ") + this->connection(), 500, 0);
		}
	}

	zverbose(std::string("attaching ") + zpt::type2str(this->type()) + std::string(" socket to ") +
		 this->connection());
}

zpt::ZMQReq::~ZMQReq() {
	if (this->__socket.get() != nullptr) {
		zverbose(std::string("detaching ") + zpt::type2str(this->type()) + std::string(" from ") +
			 this->connection());
		this->__socket->close();
		this->__socket.reset();
	}
}

auto zpt::ZMQReq::socket() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQReq::in() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQReq::out() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQReq::fd() -> int {
	int _return = 0;
	size_t _size = sizeof(_return);
	this->__socket->getsockopt(ZMQ_FD, &_return, &_size);
	return _return;
}

auto zpt::ZMQReq::in_mtx() -> std::mutex& { return this->__mtx; }

auto zpt::ZMQReq::out_mtx() -> std::mutex& { return this->__mtx; }

auto zpt::ZMQReq::type() -> short int { return ZMQ_REQ; }

auto zpt::ZMQReq::protocol() -> std::string { return "ZMQ-REQ/4.1"; }

zpt::ZMQRep::ZMQRep(std::string _connection, zpt::json _options)
    : zpt::Channel(_connection, _options), __socket(nullptr) {
	this->__socket = zmq::socket_ptr(new zmq::socket_t(zpt::__context, ZMQ_REP));
	// this->__socket->setsockopt(ZMQ_RCVTIMEO, 10000);
	this->uri(_connection);
	if (this->uri()["scheme"] == zpt::json::string("tcp") && this->uri()["type"] == zpt::json::string("@")) {
		int _available = 1025;
		if (this->uri()["port"] != zpt::json::string("*")) {
			_available = int(this->uri()["port"]);
		}
		do {
			this->connection(std::string("@tcp://") + std::string(this->uri()["domain"]) +
					 std::string(":") + std::to_string(_available));
			this->uri() << "port" << std::to_string(_available);
			try {
				this->__socket->bind(std::string("tcp://") + std::string(this->uri()["domain"]) +
						     std::string(":") + std::to_string(_available));
				break;
			} catch (zmq::error_t& _e) {
			}
			_available++;
		} while (_available < 60999);
		assertz(_available < 60999, std::string("could not attach socket to ") + _connection, 500, 0);
	} else {
		this->connection(std::string(this->uri()["type"]) + std::string(this->uri()["scheme"]) +
				 std::string("://") + std::string(this->uri()["authority"]));
		try {
			if (this->uri()["type"] == zpt::json::string("@")) {
				this->__socket->bind(std::string(this->uri()["scheme"]) + std::string("://") +
						     std::string(this->uri()["authority"]));
			} else {
				this->__socket->connect(std::string(this->uri()["scheme"]) + std::string("://") +
							std::string(this->uri()["authority"]));
			}
		} catch (zmq::error_t& _e) {
			assertz(false, std::string("could not attach socket to ") + this->connection(), 500, 0);
		}
	}

	zverbose(std::string("attaching ") + zpt::type2str(this->type()) + std::string(" socket to ") +
		 this->connection());
}

zpt::ZMQRep::~ZMQRep() {
	if (this->__socket.get() != nullptr) {
		zverbose(std::string("detaching ") + zpt::type2str(this->type()) + std::string(" from ") +
			 this->connection());
		this->__socket->close();
		this->__socket.reset();
	}
}

auto zpt::ZMQRep::socket() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQRep::in() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQRep::out() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQRep::fd() -> int {
	int _return = 0;
	size_t _size = sizeof(_return);
	this->__socket->getsockopt(ZMQ_FD, &_return, &_size);
	return _return;
}

auto zpt::ZMQRep::in_mtx() -> std::mutex& { return this->__mtx; }

auto zpt::ZMQRep::out_mtx() -> std::mutex& { return this->__mtx; }

auto zpt::ZMQRep::type() -> short int { return ZMQ_REP; }

auto zpt::ZMQRep::protocol() -> std::string { return "ZMQ-REP/4.1"; }

zpt::ZMQXPubXSub::ZMQXPubXSub(std::string _connection, zpt::json _options)
    : zpt::Channel(_connection, _options), __socket_sub(nullptr), __socket_pub(nullptr) {
	this->__socket_sub = zmq::socket_ptr(new zmq::socket_t(zpt::__context, ZMQ_XSUB));
	this->__socket_pub = zmq::socket_ptr(new zmq::socket_t(zpt::__context, ZMQ_XPUB));
	this->uri(_connection);

	std::string _connection_xpub;
	if (this->uri(1)["scheme"] == zpt::json::string("tcp")) {
		int _available = 1025;
		if (this->uri(1)["port"] != zpt::json::string("*")) {
			_available = int(this->uri(1)["port"]);
		}
		do {
			this->uri(1) << "port" << std::to_string(_available);
			try {
				_connection_xpub.assign(std::string("@tcp://") + std::string(this->uri(1)["domain"]) +
							std::string(":") + std::to_string(_available));
				this->__socket_pub->bind(std::string("tcp://") + std::string(this->uri(1)["domain"]) +
							 std::string(":") + std::to_string(_available));
				break;
			} catch (zmq::error_t& _e) {
			}
			_available++;
		} while (_available < 60999);
		assertz(_available < 60999, std::string("could not attach socket to ") + _connection, 500, 0);
	} else {
		try {
			_connection_xpub.assign(std::string("@") + std::string(this->uri(1)["scheme"]) +
						std::string("://") + std::string(this->uri(1)["authority"]));
			this->__socket_pub->bind(std::string(this->uri(1)["scheme"]) + std::string("://") +
						 std::string(this->uri(1)["authority"]));
		} catch (zmq::error_t& _e) {
			assertz(false, std::string("could not attach socket to ") + _connection, 500, 0);
		}
	}

	std::string _connection_xsub;
	if (this->uri(0)["scheme"] == zpt::json::string("tcp")) {
		int _available = 1025;
		if (this->uri(0)["port"] != zpt::json::string("*")) {
			_available = int(this->uri(0)["port"]);
		}
		do {
			this->uri(0) << "port" << std::to_string(_available);
			try {
				_connection_xsub.assign(std::string("@tcp://") + std::string(this->uri(0)["domain"]) +
							std::string(":") + std::to_string(_available));
				this->__socket_sub->bind(std::string("tcp://") + std::string(this->uri(0)["domain"]) +
							 std::string(":") + std::to_string(_available));
				break;
			} catch (zmq::error_t& _e) {
			}
			_available++;
		} while (_available < 60999);
		assertz(_available < 60999, std::string("could not attach socket to ") + _connection, 500, 0);
	} else {
		try {
			_connection_xsub.assign(std::string("@") + std::string(this->uri(0)["scheme"]) +
						std::string("://") + std::string(this->uri(0)["authority"]));
			this->__socket_sub->connect(std::string(this->uri(0)["scheme"]) + std::string("://") +
						    std::string(this->uri(0)["authority"]));
		} catch (zmq::error_t& _e) {
			assertz(false, std::string("could not attach socket to ") + _connection, 500, 0);
		}
	}

	std::thread _proxy(
	    [](zmq::socket_ptr _frontend, zmq::socket_ptr _backend) -> void {
		    zmq::proxy((void*)(*_frontend), (void*)(*_backend), nullptr);
	    },
	    this->__socket_pub,
	    this->__socket_sub);
	_proxy.detach();

	this->connection(_connection_xpub + std::string(",") + _connection_xsub);
	zverbose(std::string("attaching ") + zpt::type2str(this->type()) + std::string(" socket to ") +
		 this->connection());
}

zpt::ZMQXPubXSub::~ZMQXPubXSub() {
	zverbose(std::string("detaching ") + zpt::type2str(this->type()) + std::string(" from ") + this->connection());
	if (this->__socket_sub.get() != nullptr) {
		this->__socket_sub->close();
		this->__socket_sub.reset();
	}
	if (this->__socket_pub.get() != nullptr) {
		this->__socket_pub->close();
		this->__socket_pub.reset();
	}
}

auto zpt::ZMQXPubXSub::socket() -> zmq::socket_ptr { return this->__socket_sub; }

auto zpt::ZMQXPubXSub::in() -> zmq::socket_ptr { return this->__socket_sub; }

auto zpt::ZMQXPubXSub::out() -> zmq::socket_ptr { return this->__socket_pub; }

auto zpt::ZMQXPubXSub::fd() -> int {
	int _return = 0;
	size_t _size = sizeof(_return);
	this->__socket_sub->getsockopt(ZMQ_FD, &_return, &_size);
	return _return;
}

auto zpt::ZMQXPubXSub::in_mtx() -> std::mutex& { return this->__mtx; }

auto zpt::ZMQXPubXSub::out_mtx() -> std::mutex& { return this->__mtx; }

auto zpt::ZMQXPubXSub::type() -> short int { return ZMQ_XPUB_XSUB; }

auto zpt::ZMQXPubXSub::protocol() -> std::string { return "ZMQ-XPUB-XSUB/4.1"; }

zpt::ZMQPubSub::ZMQPubSub(std::string _connection, zpt::json _options)
    : zpt::Channel(_connection, _options), __socket_sub(nullptr), __socket_pub(nullptr) {
	this->__socket_sub = zmq::socket_ptr(new zmq::socket_t(zpt::__context, ZMQ_SUB));
	this->__socket_pub = zmq::socket_ptr(new zmq::socket_t(zpt::__context, ZMQ_PUB));
	this->uri(_connection);

	std::string _connection_pub;
	if (this->uri(0)["scheme"] == zpt::json::string("tcp") && this->uri(0)["type"] == zpt::json::string("@")) {
		int _available = 1025;
		if (this->uri(0)["port"] != zpt::json::string("*")) {
			_available = int(this->uri(0)["port"]);
		}
		do {
			this->uri(0) << "port" << std::to_string(_available);
			try {
				_connection_pub.assign(std::string("@tcp://") + std::string(this->uri(0)["domain"]) +
						       std::string(":") + std::to_string(_available));
				this->__socket_pub->bind(std::string("tcp://") + std::string(this->uri(0)["domain"]) +
							 std::string(":") + std::to_string(_available));
				break;
			} catch (zmq::error_t& _e) {
			}
			_available++;
		} while (_available < 60999);
		assertz(_available < 60999, std::string("could not attach socket to ") + _connection, 500, 0);
	} else {
		_connection_pub.assign(std::string(this->uri(0)["type"]) + std::string(this->uri(0)["scheme"]) +
				       std::string("://") + std::string(this->uri(0)["authority"]));
		try {
			if (this->uri(0)["type"] == zpt::json::string("@")) {
				this->__socket_pub->bind(std::string(this->uri(0)["scheme"]) + std::string("://") +
							 std::string(this->uri(0)["authority"]));
			} else {
				this->__socket_pub->connect(std::string(this->uri(0)["scheme"]) + std::string("://") +
							    std::string(this->uri(0)["authority"]));
			}
		} catch (zmq::error_t& _e) {
			assertz(false, std::string("could not attach socket to ") + _connection, 500, 0);
		}
	}

	std::string _connection_sub;
	if (this->uri(1)["scheme"] == zpt::json::string("tcp") && this->uri(1)["type"] == zpt::json::string("@")) {
		int _available = 1025;
		if (this->uri(1)["port"] != zpt::json::string("*")) {
			_available = int(this->uri(1)["port"]);
		}
		do {
			this->uri(1) << "port" << std::to_string(_available);
			try {
				_connection_sub.assign(std::string("@tcp://") + std::string(this->uri(1)["domain"]) +
						       std::string(":") + std::to_string(_available));
				this->__socket_sub->bind(std::string("tcp://") + std::string(this->uri(1)["domain"]) +
							 std::string(":") + std::to_string(_available));
				break;
			} catch (zmq::error_t& _e) {
			}
			_available++;
		} while (_available < 60999);
		assertz(_available < 60999, std::string("could not attach socket to ") + _connection, 500, 0);
	} else {
		_connection_sub.assign(std::string(this->uri(1)["type"]) + std::string(this->uri(1)["scheme"]) +
				       std::string("://") + std::string(this->uri(1)["authority"]));
		try {
			if (this->uri(1)["type"] == zpt::json::string("@")) {
				this->__socket_sub->bind(std::string(this->uri(1)["scheme"]) + std::string("://") +
							 std::string(this->uri(1)["authority"]));
			} else {
				this->__socket_sub->connect(std::string(this->uri(1)["scheme"]) + std::string("://") +
							    std::string(this->uri(1)["authority"]));
			}
		} catch (zmq::error_t& _e) {
			assertz(false, std::string("could not attach socket to ") + _connection, 500, 0);
		}
	}

	this->connection(_connection_pub + std::string(",") + _connection_sub);
	zverbose(std::string("attaching ") + zpt::type2str(this->type()) + std::string(" socket to ") +
		 this->connection());
}

zpt::ZMQPubSub::~ZMQPubSub() {
	zverbose(std::string("detaching ") + zpt::type2str(this->type()) + std::string(" from ") + this->connection());
	if (this->__socket_sub.get() != nullptr) {
		this->__socket_sub->close();
		this->__socket_sub.reset();
	}
	if (this->__socket_pub.get() != nullptr) {
		this->__socket_pub->close();
		this->__socket_pub.reset();
	}
}

auto zpt::ZMQPubSub::socket() -> zmq::socket_ptr { return this->__socket_sub; }

auto zpt::ZMQPubSub::in() -> zmq::socket_ptr { return this->__socket_sub; }

auto zpt::ZMQPubSub::out() -> zmq::socket_ptr { return this->__socket_pub; }

auto zpt::ZMQPubSub::fd() -> int {
	int _return = 0;
	size_t _size = sizeof(_return);
	this->__socket_sub->getsockopt(ZMQ_FD, &_return, &_size);
	return _return;
}

auto zpt::ZMQPubSub::in_mtx() -> std::mutex& { return this->__mtx; }

auto zpt::ZMQPubSub::out_mtx() -> std::mutex& { return this->__out_mtx; }

auto zpt::ZMQPubSub::type() -> short int { return ZMQ_PUB_SUB; }

auto zpt::ZMQPubSub::protocol() -> std::string { return "ZMQ-PUB-SUB/4.1"; }

void zpt::ZMQPubSub::subscribe(std::string _prefix) {
	std::lock_guard<std::mutex> _lock(this->in_mtx());
	for (size_t _p = 0; _p != 8; _p++) {
		this->__socket_sub->setsockopt(
		    ZMQ_SUBSCRIBE, (zpt::ev::to_str((zpt::ev::performative)_p) + std::string(" ") + _prefix).data(), 0);
	}
}

zpt::ZMQPub::ZMQPub(std::string _connection, zpt::json _options)
    : zpt::Channel(_connection, _options), __socket(nullptr) {
	this->__socket = zmq::socket_ptr(new zmq::socket_t(zpt::__context, ZMQ_PUB));
	this->uri(_connection);
	if (this->uri()["scheme"] == zpt::json::string("tcp") && this->uri()["type"] == zpt::json::string("@")) {
		int _available = 1025;
		if (this->uri()["port"] != zpt::json::string("*")) {
			_available = int(this->uri()["port"]);
		}
		do {
			this->connection(std::string("@tcp://") + std::string(this->uri()["domain"]) +
					 std::string(":") + std::to_string(_available));
			this->uri() << "port" << std::to_string(_available);
			try {
				this->__socket->bind(std::string("tcp://") + std::string(this->uri()["domain"]) +
						     std::string(":") + std::to_string(_available));
				break;
			} catch (zmq::error_t& _e) {
			}
			_available++;
		} while (_available < 60999);
		assertz(_available < 60999, std::string("could not attach socket to ") + _connection, 500, 0);
	} else {
		this->connection(std::string(this->uri()["type"]) + std::string(this->uri()["scheme"]) +
				 std::string("://") + std::string(this->uri()["authority"]));
		try {
			if (this->uri()["type"] == zpt::json::string("@")) {
				this->__socket->bind(std::string(this->uri()["scheme"]) + std::string("://") +
						     std::string(this->uri()["authority"]));
			} else {
				this->__socket->connect(std::string(this->uri()["scheme"]) + std::string("://") +
							std::string(this->uri()["authority"]));
			}
		} catch (zmq::error_t& _e) {
			assertz(false, std::string("could not attach socket to ") + this->connection(), 500, 0);
		}
	}

	zverbose(std::string("attaching ") + zpt::type2str(this->type()) + std::string(" socket to ") +
		 this->connection());
}

zpt::ZMQPub::~ZMQPub() {
	if (this->__socket.get() != nullptr) {
		zverbose(std::string("detaching ") + zpt::type2str(this->type()) + std::string(" from ") +
			 this->connection());
		this->__socket->close();
		this->__socket.reset();
	}
}

auto zpt::ZMQPub::socket() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQPub::in() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQPub::out() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQPub::fd() -> int {
	int _return = 0;
	size_t _size = sizeof(_return);
	this->__socket->getsockopt(ZMQ_FD, &_return, &_size);
	return _return;
}

auto zpt::ZMQPub::in_mtx() -> std::mutex& { return this->__mtx; }

auto zpt::ZMQPub::out_mtx() -> std::mutex& { return this->__mtx; }

auto zpt::ZMQPub::type() -> short int { return ZMQ_PUB; }

auto zpt::ZMQPub::protocol() -> std::string { return "ZMQ-PUB/4.1"; }

auto zpt::ZMQPub::recv() -> zpt::json { return zpt::undefined; }

zpt::ZMQSub::ZMQSub(std::string _connection, zpt::json _options)
    : zpt::Channel(_connection, _options), __socket(nullptr) {
	this->__socket = zmq::socket_ptr(new zmq::socket_t(zpt::__context, ZMQ_SUB));
	this->uri(_connection);
	if (this->uri()["scheme"] == zpt::json::string("tcp") && this->uri()["type"] == zpt::json::string("@")) {
		int _available = 1025;
		if (this->uri()["port"] != zpt::json::string("*")) {
			_available = int(this->uri()["port"]);
		}
		do {
			this->connection(std::string("@tcp://") + std::string(this->uri()["domain"]) +
					 std::string(":") + std::to_string(_available));
			this->uri() << "port" << std::to_string(_available);
			try {
				this->__socket->bind(std::string("tcp://") + std::string(this->uri()["domain"]) +
						     std::string(":") + std::to_string(_available));
				break;
			} catch (zmq::error_t& _e) {
			}
			_available++;
		} while (_available < 60999);
		assertz(_available < 60999, std::string("could not attach socket to ") + _connection, 500, 0);
	} else {
		this->connection(std::string(this->uri()["type"]) + std::string(this->uri()["scheme"]) +
				 std::string("://") + std::string(this->uri()["authority"]));
		try {
			if (this->uri()["type"] == zpt::json::string("@")) {
				this->__socket->bind(std::string(this->uri()["scheme"]) + std::string("://") +
						     std::string(this->uri()["authority"]));
			} else {
				this->__socket->connect(std::string(this->uri()["scheme"]) + std::string("://") +
							std::string(this->uri()["authority"]));
			}
		} catch (zmq::error_t& _e) {
			assertz(false, std::string("could not attach socket to ") + this->connection(), 500, 0);
		}
	}

	zverbose(std::string("attaching ") + zpt::type2str(this->type()) + std::string(" socket to ") +
		 this->connection());
}

zpt::ZMQSub::~ZMQSub() {
	if (this->__socket.get() != nullptr) {
		zverbose(std::string("detaching ") + zpt::type2str(this->type()) + std::string(" from ") +
			 this->connection());
		this->__socket->close();
		this->__socket.reset();
	}
}

auto zpt::ZMQSub::socket() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQSub::in() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQSub::out() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQSub::fd() -> int {
	int _return = 0;
	size_t _size = sizeof(_return);
	this->__socket->getsockopt(ZMQ_FD, &_return, &_size);
	return _return;
}

auto zpt::ZMQSub::in_mtx() -> std::mutex& { return this->__mtx; }

auto zpt::ZMQSub::out_mtx() -> std::mutex& { return this->__mtx; }

auto zpt::ZMQSub::type() -> short int { return ZMQ_SUB; }

auto zpt::ZMQSub::protocol() -> std::string { return "ZMQ-SUB/4.1"; }

auto zpt::ZMQSub::send(zpt::json _envelope) -> zpt::json { return zpt::undefined; }

void zpt::ZMQSub::subscribe(std::string _prefix) {
	std::lock_guard<std::mutex> _lock(this->in_mtx());
	for (size_t _p = 0; _p != 8; _p++) {
		this->__socket->setsockopt(
		    ZMQ_SUBSCRIBE, (zpt::ev::to_str((zpt::ev::performative)_p) + std::string(" ") + _prefix).data(), 0);
	}
}

zpt::ZMQPush::ZMQPush(std::string _connection, zpt::json _options)
    : zpt::Channel(_connection, _options), __socket(nullptr) {
	this->__socket = zmq::socket_ptr(new zmq::socket_t(zpt::__context, ZMQ_PUSH));
	this->__socket->setsockopt(ZMQ_SNDHWM, 100000);
	this->uri(_connection);
	if (this->uri()["scheme"] == zpt::json::string("tcp") && this->uri()["type"] == zpt::json::string("@")) {
		int _available = 1025;
		if (this->uri()["port"] != zpt::json::string("*")) {
			_available = int(this->uri()["port"]);
		}
		do {
			this->connection(std::string("@tcp://") + std::string(this->uri()["domain"]) +
					 std::string(":") + std::to_string(_available));
			this->uri() << "port" << std::to_string(_available);
			try {
				this->__socket->bind(std::string("tcp://") + std::string(this->uri()["domain"]) +
						     std::string(":") + std::to_string(_available));
				break;
			} catch (zmq::error_t& _e) {
			}
			_available++;
		} while (_available < 60999);
		assertz(_available < 60999, std::string("could not attach socket to ") + _connection, 500, 0);
	} else {
		this->connection(std::string(this->uri()["type"]) + std::string(this->uri()["scheme"]) +
				 std::string("://") + std::string(this->uri()["authority"]));
		try {
			if (this->uri()["type"] == zpt::json::string("@")) {
				this->__socket->bind(std::string(this->uri()["scheme"]) + std::string("://") +
						     std::string(this->uri()["authority"]));
			} else {
				this->__socket->connect(std::string(this->uri()["scheme"]) + std::string("://") +
							std::string(this->uri()["authority"]));
			}
		} catch (zmq::error_t& _e) {
			assertz(false, std::string("could not attach socket to ") + this->connection(), 500, 0);
		}
	}

	zverbose(std::string("attaching ") + zpt::type2str(this->type()) + std::string(" socket to ") +
		 this->connection());
}

zpt::ZMQPush::~ZMQPush() {
	if (this->__socket.get() != nullptr) {
		zverbose(std::string("detaching ") + zpt::type2str(this->type()) + std::string(" from ") +
			 this->connection());
		this->__socket->close();
		this->__socket.reset();
	}
}

auto zpt::ZMQPush::socket() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQPush::in() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQPush::out() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQPush::fd() -> int {
	int _return = 0;
	size_t _size = sizeof(_return);
	this->__socket->getsockopt(ZMQ_FD, &_return, &_size);
	return _return;
}

auto zpt::ZMQPush::in_mtx() -> std::mutex& { return this->__mtx; }

auto zpt::ZMQPush::out_mtx() -> std::mutex& { return this->__mtx; }

auto zpt::ZMQPush::type() -> short int { return ZMQ_PUSH; }

auto zpt::ZMQPush::protocol() -> std::string { return "ZMQ-PUSH/4.1"; }

auto zpt::ZMQPush::recv() -> zpt::json { return zpt::undefined; }

zpt::ZMQPull::ZMQPull(std::string _connection, zpt::json _options)
    : zpt::Channel(_connection, _options), __socket(nullptr) {
	this->__socket = zmq::socket_ptr(new zmq::socket_t(zpt::__context, ZMQ_PULL));
	this->uri(_connection);
	if (this->uri()["scheme"] == zpt::json::string("tcp") && this->uri()["type"] == zpt::json::string("@")) {
		int _available = 1025;
		if (this->uri()["port"] != zpt::json::string("*")) {
			_available = int(this->uri()["port"]);
		}
		do {
			this->connection(std::string("@tcp://") + std::string(this->uri()["domain"]) +
					 std::string(":") + std::to_string(_available));
			this->uri() << "port" << std::to_string(_available);
			try {
				this->__socket->bind(std::string("tcp://") + std::string(this->uri()["domain"]) +
						     std::string(":") + std::to_string(_available));
				break;
			} catch (zmq::error_t& _e) {
			}
			_available++;
		} while (_available < 60999);
		assertz(_available < 60999, std::string("could not attach socket to ") + _connection, 500, 0);
	} else {
		this->connection(std::string(this->uri()["type"]) + std::string(this->uri()["scheme"]) +
				 std::string("://") + std::string(this->uri()["authority"]));
		try {
			if (this->uri()["type"] == zpt::json::string("@")) {
				this->__socket->bind(std::string(this->uri()["scheme"]) + std::string("://") +
						     std::string(this->uri()["authority"]));
			} else {
				this->__socket->connect(std::string(this->uri()["scheme"]) + std::string("://") +
							std::string(this->uri()["authority"]));
			}
		} catch (zmq::error_t& _e) {
			assertz(false, std::string("could not attach socket to ") + this->connection(), 500, 0);
		}
	}

	zverbose(std::string("attaching ") + zpt::type2str(this->type()) + std::string(" socket to ") +
		 this->connection());
}

zpt::ZMQPull::~ZMQPull() {
	if (this->__socket.get() != nullptr) {
		zverbose(std::string("detaching ") + zpt::type2str(this->type()) + std::string(" from ") +
			 this->connection());
		this->__socket->close();
		this->__socket.reset();
	}
}

auto zpt::ZMQPull::socket() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQPull::in() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQPull::out() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQPull::fd() -> int {
	int _return = 0;
	size_t _size = sizeof(_return);
	this->__socket->getsockopt(ZMQ_FD, &_return, &_size);
	return _return;
}

auto zpt::ZMQPull::in_mtx() -> std::mutex& { return this->__mtx; }

auto zpt::ZMQPull::out_mtx() -> std::mutex& { return this->__mtx; }

auto zpt::ZMQPull::type() -> short int { return ZMQ_PULL; }

auto zpt::ZMQPull::protocol() -> std::string { return "ZMQ-PULL/4.1"; }

auto zpt::ZMQPull::send(zpt::json _envelope) -> zpt::json { return zpt::undefined; }

zpt::ZMQRouterDealer::ZMQRouterDealer(std::string _connection, zpt::json _options)
    : zpt::Channel(_connection, _options), __socket_router(nullptr), __socket_dealer(nullptr) {
	this->__socket_router = zmq::socket_ptr(new zmq::socket_t(zpt::__context, ZMQ_ROUTER));
	this->__socket_dealer = zmq::socket_ptr(new zmq::socket_t(zpt::__context, ZMQ_DEALER));
	this->uri(_connection);

	std::string _connection_router;
	if (this->uri(0)["scheme"] == zpt::json::string("tcp") && this->uri(0)["type"] == zpt::json::string("@")) {
		int _available = 1025;
		if (this->uri(0)["port"] != zpt::json::string("*")) {
			_available = int(this->uri(0)["port"]);
		}
		do {
			this->uri(0) << "port" << std::to_string(_available);
			try {
				_connection_router.assign(std::string("@tcp://") + std::string(this->uri(0)["domain"]) +
							  std::string(":") + std::to_string(_available));
				this->__socket_router->bind(std::string("tcp://") +
							    std::string(this->uri(0)["domain"]) + std::string(":") +
							    std::to_string(_available));
				break;
			} catch (zmq::error_t& _e) {
			}
			_available++;
		} while (_available < 60999);
		assertz(_available < 60999, std::string("could not attach socket to ") + _connection, 500, 0);
	} else {
		_connection_router.assign(std::string(this->uri(0)["scheme"]) + std::string("://") +
					  std::string(this->uri(0)["authority"]));
		try {
			if (this->uri(0)["type"] == zpt::json::string("@")) {
				this->__socket_router->bind(std::string(this->uri(0)["scheme"]) + std::string("://") +
							    std::string(this->uri(0)["authority"]));
			} else {
				this->__socket_router->connect(std::string(this->uri(0)["scheme"]) +
							       std::string("://") +
							       std::string(this->uri(0)["authority"]));
			}
		} catch (zmq::error_t& _e) {
			assertz(false, std::string("could not attach socket to ") + this->connection(), 500, 0);
		}
	}

	std::string _connection_dealer;
	if (this->uri(1)["scheme"] == zpt::json::string("tcp") && this->uri(1)["type"] == zpt::json::string("@")) {
		int _available = 1025;
		if (this->uri(1)["port"] != zpt::json::string("*")) {
			_available = int(this->uri(1)["port"]);
		}
		do {
			this->uri(1) << "port" << std::to_string(_available);
			try {
				_connection_dealer.assign(std::string("@tcp://") + std::string(this->uri(1)["domain"]) +
							  std::string(":") + std::to_string(_available));
				this->__socket_dealer->bind(std::string("tcp://") +
							    std::string(this->uri(1)["domain"]) + std::string(":") +
							    std::to_string(_available));
				break;
			} catch (zmq::error_t& _e) {
			}
			_available++;
		} while (_available < 60999);
		assertz(_available < 60999, std::string("could not attach socket to ") + _connection, 500, 0);
	} else {
		_connection_dealer.assign(std::string(this->uri(1)["scheme"]) + std::string("://") +
					  std::string(this->uri(1)["authority"]));
		try {
			if (this->uri(1)["type"] == zpt::json::string("@")) {
				this->__socket_dealer->bind(std::string(this->uri(1)["scheme"]) + std::string("://") +
							    std::string(this->uri(1)["authority"]));
			} else {
				this->__socket_dealer->connect(std::string(this->uri(1)["scheme"]) +
							       std::string("://") +
							       std::string(this->uri(1)["authority"]));
			}
		} catch (zmq::error_t& _e) {
			assertz(false, std::string("could not attach socket to ") + this->connection(), 500, 0);
		}
	}

	std::thread _proxy(
	    [](zmq::socket_ptr _frontend, zmq::socket_ptr _backend) -> void {
		    zmq::proxy((void*)(*_frontend), (void*)(*_backend), nullptr);
	    },
	    this->__socket_router,
	    this->__socket_dealer);
	_proxy.detach();

	this->connection(_connection_router + std::string(",") + _connection_dealer);
	zverbose(std::string("attaching ") + zpt::type2str(this->type()) + std::string(" socket to ") +
		 this->connection());
}

zpt::ZMQRouterDealer::~ZMQRouterDealer() {
	zverbose(std::string("detaching ") + zpt::type2str(this->type()) + std::string(" from ") + this->connection());
	if (this->__socket_router.get() != nullptr) {
		this->__socket_router->close();
		this->__socket_router.reset();
	}
	if (this->__socket_dealer.get() != nullptr) {
		this->__socket_dealer->close();
		this->__socket_dealer.reset();
	}
}

auto zpt::ZMQRouterDealer::socket() -> zmq::socket_ptr { return this->__socket_router; }

auto zpt::ZMQRouterDealer::in() -> zmq::socket_ptr { return this->__socket_router; }

auto zpt::ZMQRouterDealer::out() -> zmq::socket_ptr { return this->__socket_dealer; }

auto zpt::ZMQRouterDealer::fd() -> int {
	int _return = 0;
	size_t _size = sizeof(_return);
	this->__socket_router->getsockopt(ZMQ_FD, &_return, &_size);
	return _return;
}

auto zpt::ZMQRouterDealer::in_mtx() -> std::mutex& { return this->__mtx; }

auto zpt::ZMQRouterDealer::out_mtx() -> std::mutex& { return this->__mtx; }

auto zpt::ZMQRouterDealer::type() -> short int { return ZMQ_ROUTER_DEALER; }

auto zpt::ZMQRouterDealer::protocol() -> std::string { return "ZMQ-ROUTER-DEALER/4.1"; }

zpt::ZMQRouter::ZMQRouter(std::string _connection, zpt::json _options)
    : zpt::Channel(_connection, _options), __socket(nullptr) {
	this->__socket = zmq::socket_ptr(new zmq::socket_t(zpt::__context, ZMQ_ROUTER));
	this->uri(_connection);
	if (this->uri()["scheme"] == zpt::json::string("tcp") && this->uri()["type"] == zpt::json::string("@")) {
		int _available = 1025;
		if (this->uri()["port"] != zpt::json::string("*")) {
			_available = int(this->uri()["port"]);
		}
		do {
			this->uri() << "port" << std::to_string(_available);
			try {
				this->connection(std::string("@tcp://") + std::string(this->uri()["domain"]) +
						 std::string(":") + std::to_string(_available));
				this->__socket->bind(std::string("tcp://") + std::string(this->uri()["domain"]) +
						     std::string(":") + std::to_string(_available));
				break;
			} catch (zmq::error_t& _e) {
			}
			_available++;
		} while (_available < 60999);
		assertz(_available < 60999, std::string("could not attach socket to ") + _connection, 500, 0);
	} else {
		this->connection(std::string(this->uri()["type"]) + std::string(this->uri()["scheme"]) +
				 std::string("://") + std::string(this->uri()["authority"]));
		try {
			if (this->uri()["type"] == zpt::json::string("@")) {
				this->__socket->bind(std::string(this->uri()["scheme"]) + std::string("://") +
						     std::string(this->uri()["authority"]));
			} else {
				this->__socket->connect(std::string(this->uri()["scheme"]) + std::string("://") +
							std::string(this->uri()["authority"]));
			}
		} catch (zmq::error_t& _e) {
			assertz(false, std::string("could not attach socket to ") + this->connection(), 500, 0);
		}
	}

	zverbose(std::string("attaching ") + zpt::type2str(this->type()) + std::string(" socket to ") +
		 this->connection());
}

zpt::ZMQRouter::~ZMQRouter() {
	if (this->__socket.get() != nullptr) {
		zverbose(std::string("detaching ") + zpt::type2str(this->type()) + std::string(" from ") +
			 this->connection());
		this->__socket->close();
		this->__socket.reset();
	}
}

auto zpt::ZMQRouter::socket() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQRouter::in() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQRouter::out() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQRouter::fd() -> int {
	int _return = 0;
	size_t _size = sizeof(_return);
	this->__socket->getsockopt(ZMQ_FD, &_return, &_size);
	return _return;
}

auto zpt::ZMQRouter::in_mtx() -> std::mutex& { return this->__mtx; }

auto zpt::ZMQRouter::out_mtx() -> std::mutex& { return this->__mtx; }

auto zpt::ZMQRouter::type() -> short int { return ZMQ_ROUTER; }

auto zpt::ZMQRouter::protocol() -> std::string { return "ZMQ-ROUTER/4.1"; }

auto zpt::ZMQRouter::send(zpt::json _envelope) -> zpt::json {
	std::string _uuid;
	zmq::message_t* _frame1 = nullptr;
	{
		std::lock_guard<std::mutex> _lock(this->__sock_mtx);
		_uuid.assign(std::string(_envelope["channel"]));
		auto _found = this->__sock_id.find(_uuid);
		if (_found == this->__sock_id.end()) {
			zlog(std::string("couldn't find socket associated with '") + _uuid + std::string("':") +
				 zpt::ev::pretty(_envelope),
			     zpt::critical);
			return zpt::undefined;
		}
		_frame1 = _found->second;
		this->__sock_id.erase(_found);
	}

	assertz(_envelope["performative"]->ok() && _envelope["resource"]->ok(),
		"'performative' and 'resource' attributes are required",
		412,
		0);
	assertz(!_envelope["headers"]->ok() || _envelope["headers"]->type() == zpt::JSObject,
		"'headers' must be of type JSON object",
		412,
		0);

	zpt::json _uri = zpt::uri::parse(_envelope["resource"]);
	_envelope << "resource" << _uri["path"] << "protocol" << this->protocol() << "params"
		  << ((_envelope["params"]->is_object() ? _envelope["params"] : zpt::undefined) + _uri["query"]);

	zpt::ev::performative _performative = (zpt::ev::performative)((int)_envelope["performative"]);
	if (_performative == zpt::ev::Reply) {
		assertz(_envelope["status"]->ok(), "'status' attribute is required", 412, 0);
		_envelope["headers"] << "X-Status" << _envelope["status"];
	}
	if (!_envelope["payload"]->ok()) {
		_envelope << "payload" << zpt::json::object();
	}
	if (_envelope["payload"]["assertion_failed"]->ok() && _envelope["payload"]["code"]->ok()) {
		_envelope["headers"] << "X-Error" << _envelope["payload"]["code"];
	}
	int _status = (int)_envelope["headers"]["X-Status"];

	std::string _directive(zpt::ev::to_str(_performative) + std::string(" ") + _envelope["resource"]->str() +
			       (_performative == zpt::ev::Reply
				    ? std::string(" ") + std::to_string(_status)
				    : std::string(""))); // + std::string(" ") + zpt::generate::r_uuid());
	std::string _buffer(_envelope);

	zmq::message_t _frame2(0);
	zmq::message_t _frame3(_directive.length());
	zmq::message_t _frame4(_buffer.length());

	memcpy(_frame3.data(), _directive.data(), _directive.length());
	memcpy(_frame4.data(), _buffer.data(), _buffer.length());

	std::lock_guard<std::mutex> _lock(this->out_mtx());
	assertz(this->out()->send(*_frame1, ZMQ_SNDMORE), std::string("unable to send message"), 500, 0);
	delete _frame1;
	assertz(this->out()->send(_frame2, ZMQ_SNDMORE), std::string("unable to send message"), 500, 0);
	assertz(this->out()->send(_frame3, ZMQ_SNDMORE), std::string("unable to send message"), 500, 0);
	assertz(this->out()->send(_frame4), std::string("unable to send message"), 500, 0);
	ztrace(std::string("> ") + _directive);
	zverbose(zpt::ev::pretty(_envelope));

	return zpt::undefined;
}

auto zpt::ZMQRouter::recv() -> zpt::json {
	zmq::message_t* _frame1 = new zmq::message_t();
	zmq::message_t _frame2;
	zmq::message_t _frame3;
	zmq::message_t _frame4;

	try {
		int64_t _more = 0;
		size_t _more_size = sizeof _more;
		std::string _uuid;

		{
			std::lock_guard<std::mutex> _lock(this->in_mtx());
			this->in()->recv(_frame1);
			this->in()->getsockopt(ZMQ_RCVMORE, &_more, &_more_size);
			if (_more != 0) {
				this->in()->recv(&_frame2);
			}
			_more = 0;
			this->in()->getsockopt(ZMQ_RCVMORE, &_more, &_more_size);
			if (_more != 0) {
				this->in()->recv(&_frame3);
			}
			_more = 0;
			this->in()->getsockopt(ZMQ_RCVMORE, &_more, &_more_size);
			if (_more != 0) {
				this->in()->recv(&_frame4);
			}
		}

		std::string _id(static_cast<char*>(_frame1->data()), _frame1->size());
		std::string _empty(static_cast<char*>(_frame2.data()), _frame2.size());
		std::string _directive(static_cast<char*>(_frame3.data()), _frame3.size());
		std::string _raw(static_cast<char*>(_frame4.data()), _frame4.size());

		if (_raw.length() == 0 && _empty.length() != 0) {
			_raw.assign(_directive);
			_directive.assign(_empty);
		}

		try {
			zpt::json _envelope(_raw);
			{
				std::lock_guard<std::mutex> _lock(this->__sock_mtx);
				if (!_envelope["channel"]->ok() ||
				    !zpt::test::uuid(std::string(_envelope["channel"]))) {
					_uuid.assign(zpt::generate::r_uuid());
					_envelope << "channel" << _uuid;
				} else {
					_uuid.assign(std::string(_envelope["channel"]));
				}
				this->__sock_id.insert(std::make_pair(_uuid, _frame1));
			}

			_envelope << "protocol" << this->protocol();
			ztrace(std::string("< ") + _directive);
			zverbose(zpt::ev::pretty(_envelope));

			return _envelope;
		} catch (zpt::SyntaxErrorException& _e) {
			delete _frame1;
			return {"protocol",
				this->protocol(),
				"error",
				true,
				"status",
				400,
				"payload",
				{"text", _e.what(), "assertion_failed", _e.what(), "code", 1060}};
		}
	} catch (zmq::error_t& _e) {
		delete _frame1;
		zlog(_e.what(), zpt::error);
		throw;
	}
	delete _frame1;
	return {"protocol",
		this->protocol(),
		"error",
		true,
		"status",
		503,
		"payload",
		{"text", "upstream container not reachable", "assertion_failed", "sock->is_open()", "code", 1061}};
}

zpt::ZMQDealer::ZMQDealer(std::string _connection, zpt::json _options)
    : zpt::Channel(_connection, _options), __socket(nullptr) {
	this->__socket = zmq::socket_ptr(new zmq::socket_t(zpt::__context, ZMQ_DEALER));
	this->__socket->setsockopt(ZMQ_RCVTIMEO, 0);
	std::stringstream _ss;
	_ss << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << within(0x10000) << "-" << std::setw(4)
	    << std::setfill('0') << within(0x10000);
	this->__socket->setsockopt(ZMQ_IDENTITY, _ss.str().c_str(), _ss.str().length());

	this->uri(_connection);
	if (this->uri()["scheme"] == zpt::json::string("tcp") && this->uri()["type"] == zpt::json::string("@")) {
		int _available = 1025;
		if (this->uri()["port"] != zpt::json::string("*")) {
			_available = int(this->uri()["port"]);
		}
		do {
			this->uri() << "port" << std::to_string(_available);
			try {
				this->connection(std::string("@tcp://") + std::string(this->uri()["domain"]) +
						 std::string(":") + std::to_string(_available));
				this->__socket->bind(std::string("tcp://") + std::string(this->uri()["domain"]) +
						     std::string(":") + std::to_string(_available));
				break;
			} catch (zmq::error_t& _e) {
			}
			_available++;
		} while (_available < 60999);
		assertz(_available < 60999, std::string("could not attach socket to ") + _connection, 500, 0);
	} else {
		this->connection(std::string(this->uri()["type"]) + std::string(this->uri()["scheme"]) +
				 std::string("://") + std::string(this->uri()["authority"]));
		try {
			if (this->uri()["type"] == zpt::json::string("@")) {
				this->__socket->bind(std::string(this->uri()["scheme"]) + std::string("://") +
						     std::string(this->uri()["authority"]));
			} else {
				this->__socket->connect(std::string(this->uri()["scheme"]) + std::string("://") +
							std::string(this->uri()["authority"]));
			}
		} catch (zmq::error_t& _e) {
			assertz(false, std::string("could not attach socket to ") + this->connection(), 500, 0);
		}
	}

	zverbose(std::string("attaching ") + zpt::type2str(this->type()) + std::string(" socket to ") +
		 this->connection());
}

zpt::ZMQDealer::~ZMQDealer() {
	if (this->__socket.get() != nullptr) {
		zverbose(std::string("detaching ") + zpt::type2str(this->type()) + std::string(" from ") +
			 this->connection());
		this->__socket->close();
		this->__socket.reset();
	}
}

auto zpt::ZMQDealer::socket() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQDealer::in() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQDealer::out() -> zmq::socket_ptr { return this->__socket; }

auto zpt::ZMQDealer::fd() -> int {
	int _return = 0;
	size_t _size = sizeof(_return);
	this->__socket->getsockopt(ZMQ_FD, &_return, &_size);
	return _return;
}

auto zpt::ZMQDealer::in_mtx() -> std::mutex& { return this->__mtx; }

auto zpt::ZMQDealer::out_mtx() -> std::mutex& { return this->__mtx; }

auto zpt::ZMQDealer::type() -> short int { return ZMQ_DEALER; }

auto zpt::ZMQDealer::protocol() -> std::string { return "ZMQ-DEALER/4.1"; }

auto zpt::ZMQDealer::send(zpt::json _envelope) -> zpt::json { return zpt::Channel::send(_envelope); }

auto zpt::ZMQDealer::recv() -> zpt::json {
	zmq::message_t _frame1;
	zmq::message_t _frame2;
	zmq::message_t _frame3;

	try {
		int64_t _more = 0;
		size_t _more_size = sizeof _more;

		{
			std::lock_guard<std::mutex> _lock(this->in_mtx());
			this->in()->recv(&_frame1);
			this->in()->getsockopt(ZMQ_RCVMORE, &_more, &_more_size);
			if (_more != 0) {
				this->in()->recv(&_frame2);
			}
			_more = 0;
			this->in()->getsockopt(ZMQ_RCVMORE, &_more, &_more_size);
			if (_more != 0) {
				this->in()->recv(&_frame3);
			}
		}

		std::string _empty(static_cast<char*>(_frame1.data()), _frame1.size());
		std::string _directive(static_cast<char*>(_frame2.data()), _frame2.size());
		std::string _raw(static_cast<char*>(_frame3.data()), _frame3.size());

		if (_raw.length() == 0 && _empty.length() != 0) {
			_raw.assign(_directive);
			_directive.assign(_empty);
		}

		if (_raw.length() == 0 || _directive.length() == 0) {
			return zpt::undefined;
		}

		try {
			zpt::json _envelope(_raw);
			if (_envelope->ok() &&
			    (!_envelope["channel"]->ok() || !zpt::test::uuid(std::string(_envelope["channel"])))) {
				_envelope << "channel" << zpt::generate::r_uuid();
			}
			_envelope << "protocol" << this->protocol();
			ztrace(std::string("< ") + _directive);
			zverbose(zpt::ev::pretty(_envelope));

			return _envelope;
		} catch (zpt::SyntaxErrorException& _e) {
			return {"protocol",
				this->protocol(),
				"error",
				true,
				"status",
				400,
				"payload",
				{"text", _e.what(), "assertion_failed", _e.what(), "code", 1060}};
		}
	} catch (zmq::error_t& _e) {
		zlog(_e.what(), zpt::error);
		throw;
	}
	return {"protocol",
		this->protocol(),
		"error",
		true,
		"status",
		503,
		"payload",
		{"text", "upstream container not reachable", "assertion_failed", "sock->is_open()", "code", 1061}};
}

zpt::ZMQHttp::ZMQHttp(zpt::socketstream_ptr _underlying, zpt::json _options)
    : zpt::Channel(std::string(_underlying->ssl() ? "https://" : "http://") +
		       std::string(_underlying->host() != "" ? _underlying->host() : "*") + std::string(":") +
		       std::to_string(_underlying->port()),
		   _options),
      __underlying(_underlying), __state(-1) {}

zpt::ZMQHttp::~ZMQHttp() {}

auto zpt::ZMQHttp::underlying() -> zpt::socketstream_ptr {
	return this->__underlying;
	;
}

auto zpt::ZMQHttp::socket() -> zmq::socket_ptr { return zmq::socket_ptr(nullptr); }

auto zpt::ZMQHttp::in() -> zmq::socket_ptr { return zmq::socket_ptr(nullptr); }

auto zpt::ZMQHttp::out() -> zmq::socket_ptr { return zmq::socket_ptr(nullptr); }

auto zpt::ZMQHttp::fd() -> int { return this->__underlying->buffer().get_socket(); }

auto zpt::ZMQHttp::close() -> void { this->__underlying->close(); }

auto zpt::ZMQHttp::available() -> bool {
	char _c;
	int _read = ::recv(this->fd(), &_c, 1, MSG_PEEK);
	return _read > 0;
}

auto zpt::ZMQHttp::in_mtx() -> std::mutex& { return this->__mtx; }

auto zpt::ZMQHttp::out_mtx() -> std::mutex& { return this->__mtx; }

auto zpt::ZMQHttp::type() -> short int { return ZMQ_HTTP_RAW; }

auto zpt::ZMQHttp::protocol() -> std::string { return "HTTP/1.1"; }

auto zpt::ZMQHttp::send(zpt::json _envelope) -> zpt::json {
	zpt::ev::performative _performative = (zpt::ev::performative)((int)_envelope["performative"]);

	assertz((_performative == zpt::ev::Reply && this->__state == HTTP_REQ) ||
		    (_performative != zpt::ev::Reply && this->__state != HTTP_REQ),
		"HTTP socket state doesn't allow you to send a reply without having "
		"received a request",
		400,
		1201);
	assertz(_envelope["resource"]->ok(), "'resource' attribute is required", 412, 0);

	zpt::json _uri = zpt::uri::parse(_envelope["resource"]);
	_envelope << "resource" << _uri["path"] << "protocol" << this->protocol() << "params"
		  << ((_envelope["params"]->is_object() ? _envelope["params"] : zpt::undefined) + _uri["query"]);

	if (_performative == zpt::ev::Reply) {
		assertz(_envelope["status"]->ok(), "'status' attribute is required", 412, 0);
		_envelope["headers"] << "X-Status" << _envelope["status"];
	}
	if (_envelope["payload"]["assertion_failed"]->ok() && _envelope["payload"]["code"]->ok()) {
		_envelope["headers"] << "X-Error" << _envelope["payload"]["code"];
	}
	try {
		{
			std::lock_guard<std::mutex> _lock(this->out_mtx());
			std::string _message;
			if (_performative == zpt::ev::Reply) {
				_message.assign(std::string(zpt::rest::zmq2http_rep(_envelope)));
				this->__state = HTTP_REP;
			} else {
				_message.assign(std::string(zpt::rest::zmq2http_req(
				    _envelope,
				    this->__underlying->host() +
					((this->__underlying->ssl() && this->__underlying->port() == 443) ||
						 (!this->__underlying->ssl() && this->__underlying->port() == 80)
					     ? std::string("")
					     : std::string(":") + std::to_string(this->__underlying->port())))));
				this->__state = HTTP_REQ;
				this->__cid = std::string(_envelope["channel"]);
				this->__resource = std::string(_envelope["resource"]);
			}
			(*this->__underlying) << _message << std::flush;
			assertz(!this->__underlying->is_error(), this->__underlying->error_string(), 503, 0);
		}

		ztrace(std::string("> ") + zpt::ev::to_str(_performative) + std::string(" ") +
		       _envelope["resource"]->str() +
		       (_performative == zpt::ev::Reply ? std::string(" ") + std::string(_envelope["status"])
							: std::string("")));
		zverbose(zpt::ev::pretty(_envelope));
	} catch (std::ios_base::failure& _e) {
	}
	return zpt::undefined;
}

auto zpt::ZMQHttp::recv() -> zpt::json {
	zpt::json _in;
	try {
		{
			std::lock_guard<std::mutex> _lock(this->in_mtx());
			if (this->__state != HTTP_REQ) {
				zpt::http::req _request;
				(*this->__underlying) >> _request;
				assertz(!this->__underlying->is_error(), this->__underlying->error_string(), 503, 0);
				this->__state = HTTP_REQ;
				try {
					_in = zpt::rest::http2zmq(_request);
				} catch (zpt::SyntaxErrorException& _e) {
					zlog(std::string("error while parsing HTTP message body: syntax "
							 "error exception"),
					     zpt::error);
					return {"protocol",
						this->protocol(),
						"status",
						400,
						"payload",
						{"text",
						 _e.what(),
						 "assertion_failed",
						 _e.what(),
						 "cod"
						 "e",
						 1062}};
				}
				this->__cid = std::string(_in["channel"]);
			} else {
				zpt::http::rep _reply;
				(*this->__underlying) >> _reply;
				assertz(!this->__underlying->is_error(), this->__underlying->error_string(), 503, 0);
				this->__state = HTTP_REP;
				if (_reply->header("X-Cid") == "") {
					_reply->header("X-Cid", this->__cid);
				}
				if (_reply->header("X-Resource") == "") {
					_reply->header("X-Resource", this->__resource);
				}
				try {
					_in = zpt::rest::http2zmq(_reply);
				} catch (zpt::SyntaxErrorException& _e) {
					zlog(std::string("error while parsing HTTP message body: syntax "
							 "error exception"),
					     zpt::error);
					return {"protocol",
						this->protocol(),
						"status",
						400,
						"payload",
						{"text",
						 _e.what(),
						 "assertion_failed",
						 _e.what(),
						 "cod"
						 "e",
						 1062}};
				}
			}
		}
	} catch (zpt::SyntaxErrorException& _e) {
		zlog(std::string("error while parsing HTTP request: syntax error exception"), zpt::error);
		return {"protocol",
			this->protocol(),
			"error",
			true,
			"status",
			400,
			"payload",
			{"text", _e.what(), "assertion_failed", _e.what(), "code", 1062}};
	}
	_in << "protocol" << this->protocol();
	ztrace(std::string("< ") + zpt::ev::to_str(zpt::ev::performative(int(_in["performative"]))) + std::string(" ") +
	       _in["resource"]->str() + (zpt::ev::performative(int(_in["performative"])) == zpt::ev::Reply
					     ? std::string(" ") + std::string(_in["status"])
					     : std::string("")));
	zverbose(zpt::ev::pretty(_in));
	return _in;
}

auto zpt::str2type(std::string _type) -> short {
	std::transform(_type.begin(), _type.end(), _type.begin(), ::toupper);
	if (_type == "ROUTER/DEALER") {
		return ZMQ_ROUTER_DEALER;
	}
	if (_type == "ROUTER") {
		return ZMQ_ROUTER;
	}
	if (_type == "DEALER") {
		return ZMQ_DEALER;
	}
	if (_type == "REQ") {
		return ZMQ_REQ;
	}
	if (_type == "REP") {
		return ZMQ_REP;
	}
	if (_type == "PUB/SUB") {
		return ZMQ_PUB_SUB;
	}
	if (_type == "XPUB/XSUB") {
		return ZMQ_XPUB_XSUB;
	}
	if (_type == "PUB") {
		return ZMQ_PUB;
	}
	if (_type == "SUB") {
		return ZMQ_SUB;
	}
	if (_type == "PUSH") {
		return ZMQ_PUSH;
	}
	if (_type == "PULL") {
		return ZMQ_PULL;
	}
	if (_type == "HTTP" || _type == "HTTPS") {
		return ZMQ_HTTP_RAW;
	}
	if (_type == "UPNP") {
		return ZMQ_UPNP_RAW;
	}
	if (_type == "MQTT" || _type == "MQTTS") {
		return ZMQ_MQTT_RAW;
	}
	return -20;
}

auto zpt::type2str(short _type) -> std::string {
	switch (_type) {
	case ZMQ_ROUTER_DEALER: {
		return "ROUTER/DEALER";
	}
	case ZMQ_ROUTER: {
		return "ROUTER";
	}
	case ZMQ_DEALER: {
		return "DEALER";
	}
	case ZMQ_REQ: {
		return "REQ";
	}
	case ZMQ_REP: {
		return "REP";
	}
	case ZMQ_PUB_SUB: {
		return "PUB/SUB";
	}
	case ZMQ_XPUB_XSUB: {
		return "XPUB/XSUB";
	}
	case ZMQ_PUB: {
		return "PUB";
	}
	case ZMQ_SUB: {
		return "SUB";
	}
	case ZMQ_PUSH: {
		return "PUSH";
	}
	case ZMQ_PULL: {
		return "PULL";
	}
	case ZMQ_HTTP_RAW: {
		return "HTTP";
	}
	case ZMQ_UPNP_RAW: {
		return "HTTP";
	}
	case ZMQ_MQTT_RAW: {
		return "MQTT";
	}
	}
	return "UNKNOWN_SOCKET_TYPE";
}

auto zpt::net::getip(std::string _if) -> std::string {
	std::string _out;
	struct ifaddrs *_if_addr = nullptr, *_ifa = nullptr;
	void* _tmp_add_ptr = nullptr;

	getifaddrs(&_if_addr);
	for (_ifa = _if_addr; _ifa != nullptr; _ifa = _ifa->ifa_next) {
		if (_ifa->ifa_addr->sa_family == AF_INET) {
			char _mask[INET_ADDRSTRLEN];
			void* _mask_ptr = &((struct sockaddr_in*)_ifa->ifa_netmask)->sin_addr;
			inet_ntop(AF_INET, _mask_ptr, _mask, INET_ADDRSTRLEN);
			if (strcmp(_mask, "255.0.0.0") != 0 &&
			    (_if.length() == 0 || std::string(_ifa->ifa_name) == _if)) {
				_tmp_add_ptr = &((struct sockaddr_in*)_ifa->ifa_addr)->sin_addr;
				char _address_buf[INET_ADDRSTRLEN];
				bzero(_address_buf, INET_ADDRSTRLEN);
				inet_ntop(AF_INET, _tmp_add_ptr, _address_buf, INET_ADDRSTRLEN);
				_out.assign(_address_buf);
				if (_if_addr != nullptr)
					freeifaddrs(_if_addr);
				if (_out.length() == 0 || _out == "::") {
					return "127.0.0.1";
				}
				return _out;
			}
		} else if (_ifa->ifa_addr->sa_family == AF_INET6) {
			char _mask[INET6_ADDRSTRLEN];
			void* _mask_ptr = &((struct sockaddr_in*)_ifa->ifa_netmask)->sin_addr;
			inet_ntop(AF_INET6, _mask_ptr, _mask, INET6_ADDRSTRLEN);
			if (strcmp(_mask, "255.0.0.0") != 0 &&
			    (_if.length() == 0 || std::string(_ifa->ifa_name) == _if)) {
				_tmp_add_ptr = &((struct sockaddr_in*)_ifa->ifa_addr)->sin_addr;
				char _address_buf[INET6_ADDRSTRLEN];
				bzero(_address_buf, INET6_ADDRSTRLEN);
				inet_ntop(AF_INET6, _tmp_add_ptr, _address_buf, INET6_ADDRSTRLEN);
				_out.assign(_address_buf);
				if (_if_addr != nullptr)
					freeifaddrs(_if_addr);
				if (_out.length() == 0 || _out == "::") {
					return "127.0.0.1";
				}
				return _out;
			}
		}
	}
	if (_if_addr != nullptr)
		freeifaddrs(_if_addr);
	if (_out.length() == 0 || _out == "::") {
		return "127.0.0.1";
	}
	return _out;
}

auto zpt::rest::http2zmq(zpt::http::req _request) -> zpt::json {
	zpt::json _return = zpt::json::object();
	_return << "channel"
		<< (_request->header("X-Cid").length() != 0 ? _request->header("X-Cid") : zpt::generate::r_uuid())
		<< "performative" << _request->method() << "resource" << _request->url();

	zpt::json _payload;
	if (_request->body() != "") {
		if (_request->header("Content-Type").find("application/x-www-form-urlencoded") != std::string::npos) {
			_payload = zpt::rest::http::deserialize(_request->body());
		} else if (_request->header("Content-Type").find("application/json") != std::string::npos) {
			_payload = zpt::json(_request->body());
		} else {
			_payload = {"text", _request->body()};
		}
	}
	_return << "payload" << _payload;

	if (_request->params().size() != 0) {
		zpt::json _params = zpt::json::object();
		for (auto _param : _request->params()) {
			_params << _param.first << zpt::url::r_decode(_param.second);
		}
		_return << "params" << _params;
	}

	zpt::json _headers = zpt::json::object();
	for (auto _header : _request->headers()) {
		_headers << _header.first << _header.second;
	}
	if (_headers->obj()->size() != 0) {
		_return << "headers" << _headers;
	}

	return _return;
}

auto zpt::rest::http2zmq(zpt::http::rep _reply) -> zpt::json {
	zpt::json _return = zpt::json::object();
	_return << "status" << (int)_reply->status() << "channel"
		<< (_reply->header("X-Cid").length() != 0 ? _reply->header("X-Cid") : zpt::undefined) << "performative"
		<< zpt::ev::Reply << "resource" << _reply->header("X-Resource");

	std::string _body = _reply->body();
	zpt::json _payload;
	zpt::trim(_body);
	if (_body != "") {
		if (_reply->header("Content-Type").find("application/x-www-form-urlencoded") != std::string::npos) {
			_payload = zpt::rest::http::deserialize(_reply->body());
		} else if (_reply->header("Content-Type").find("application/json") != std::string::npos) {
			_payload = zpt::json(_reply->body());
		} else {
			_payload = {"text", _reply->body()};
		}
	}
	_return << "payload" << _payload;

	zpt::json _headers = zpt::json::object();
	for (auto _header : _reply->headers()) {
		_headers << _header.first << _header.second;
	}
	if (_headers->obj()->size() != 0) {
		_return << "headers" << _headers;
	}

	return _return;
}

auto zpt::rest::zmq2http_rep(zpt::json _out) -> zpt::http::rep {
	zpt::http::rep _return;
	_return->status((zpt::HTTPStatus)((int)_out["status"]));

	if (_out["headers"]->is_object()) {
		;
		for (auto _header : _out["headers"]->obj()) {
			_return->header(_header.first, ((std::string)_header.second));
		}
	}
	if (_out["channel"]->ok()) {
		_return->header("X-Cid", std::string(_out["channel"]));
	}
	_return->header("X-Resource", _out["resource"]);

	if (_return->status() != zpt::HTTP204 && _return->status() != zpt::HTTP304 &&
	    _return->status() >= zpt::HTTP200) {
		if (((!_out["payload"]->is_object() && !_out["payload"]->is_array()) ||
		     (_out["payload"]->is_object() && _out["payload"]->obj()->size() != 0) ||
		     (_out["payload"]->is_array() && _out["payload"]->arr()->size() != 0))) {
			if (_out["headers"]["Content-Type"]->ok() &&
			    std::string(_out["headers"]["Content-Type"]) != "application/json") {
				if (std::string(_out["headers"]["Content-Type"]) ==
					"application/x-www-form-urlencoded" ||
				    std::string(_out["headers"]["Content-Type"]) == "multipart/form-data") {
					std::string _body;
					if (_out["payload"]->is_object()) {
						for (auto _param : _out["payload"]->obj()) {
							if (_body.length() != 0) {
								_body += std::string("&");
							}
							_body += _param.first + std::string("=") +
								 zpt::url::r_encode(std::string(_param.second));
						}
					} else {
						_body = std::string("payload=") +
							zpt::url::r_encode(std::string(_out["payload"]));
					}
					zpt::trim(_body);
					_return->header("Content-Length", std::to_string(_body.length()));
					_return->body(_body);
				} else {
					std::string _body = std::string(_out["payload"]);
					zpt::trim(_body);
					_return->header("Content-Type", "text/plain");
					_return->header("Content-Length", std::to_string(_body.length()));
					_return->body(_body);
				}
			} else {
				std::string _body = std::string(_out["payload"]);
				zpt::trim(_body);
				if (_out["payload"]->is_object() || _out["payload"]->is_array()) {
					_return->header("Content-Type", "application/json");
				} else {
					_return->header("Content-Type", "text/plain");
				}
				_return->body(_body);
				_return->header("Content-Length", std::to_string(_body.length()));
			}
		} else {
			_return->header("Content-Length", "0");
		}
	}
	return _return;
}

auto zpt::rest::zmq2http_req(zpt::json _out, std::string _host) -> zpt::http::req {
	zpt::http::req _return;
	_return->method(zpt::ev::performative(int(_out["performative"])));
	_return->url(std::string(_out["resource"]));

	if (!_out["headers"]->is_object()) {
		_out << "headers" << zpt::json::object();
	}
	_out["headers"] << "Host" << _host;
	for (auto _header : _out["headers"]->obj()) {
		_return->header(_header.first, ((std::string)_header.second));
	}

	if (_out["channel"]->ok()) {
		_return->header("X-Cid", std::string(_out["channel"]));
	}

	if (_out["params"]->is_object()) {
		for (auto _param : _out["params"]->obj()) {
			_return->param(_param.first, ((std::string)_param.second));
		}
	}

	if (((!_out["payload"]->is_object() && !_out["payload"]->is_array()) ||
	     (_out["payload"]->is_object() && _out["payload"]->obj()->size() != 0) ||
	     (_out["payload"]->is_array() && _out["payload"]->arr()->size() != 0))) {
		if (_out["headers"]["Content-Type"]->ok() &&
		    std::string(_out["headers"]["Content-Type"]) != "application/json") {
			if (std::string(_out["headers"]["Content-Type"]) == "application/x-www-form-urlencoded" ||
			    std::string(_out["headers"]["Content-Type"]) == "multipart/form-data") {
				std::string _body;
				if (_out["payload"]->is_object()) {
					for (auto _param : _out["payload"]->obj()) {
						if (_body.length() != 0) {
							_body += std::string("&");
						}
						_body += _param.first + std::string("=") +
							 zpt::url::r_encode(std::string(_param.second));
					}
				} else {
					_body =
					    std::string("payload=") + zpt::url::r_encode(std::string(_out["payload"]));
				}
				zpt::trim(_body);
				_return->header("Content-Length", std::to_string(_body.length()));
				_return->body(_body);
			} else {
				std::string _body = std::string(_out["payload"]);
				zpt::trim(_body);
				_return->header("Content-Type", "text/plain");
				_return->header("Content-Length", std::to_string(_body.length()));
				_return->body(_body);
			}
		} else {
			std::string _body = std::string(_out["payload"]);
			zpt::trim(_body);
			if (_body.length() != 0) {
				if (_out["payload"]->is_object() || _out["payload"]->is_array()) {
					_return->header("Content-Type", "application/json");
				} else {
					_return->header("Content-Type", "text/plain");
				}
				_return->header("Content-Length", std::to_string(_body.length()));
				_return->body(_body);
			} else {
				_return->header("Content-Length", "0");
			}
		}
	}
	return _return;
}

zpt::json zpt::rest::http::deserialize(std::string _body) {
	zpt::json _return = zpt::json::object();
	std::string _name;
	std::string _collected;
	for (const auto& _c : _body) {
		switch (_c) {
		case '=': {
			_name.assign(_collected.data());
			_collected.assign("");
			break;
		}
		case '&': {
			zpt::url::decode(_collected);
			_return << _name << _collected;
			_name.assign("");
			_collected.assign("");
			break;
		}
		default: { _collected.push_back(_c); }
		}
	}
	zpt::url::decode(_collected);
	_return << _name << _collected;
	return _return;
}

extern "C" auto zpt_zmq() -> int { return 1; }
