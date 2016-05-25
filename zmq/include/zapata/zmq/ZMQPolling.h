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

#pragma once

#include <zapata/base.h>
#include <zapata/json.h>
#include <zapata/events.h>
#include <regex.h>
#include <string>
#include <map>
#include <memory>
#include <zmq.hpp>

using namespace std;
#if !defined __APPLE__
using namespace __gnu_cxx;
#endif

#define ZMQ_REQ_REP -1
#define ZMQ_PUB_SUB -2
#define ZMQ_XPUB_XSUB -3
#define ZMQ_PUSH_PULL -4

namespace zpt {

	short str2type(std::string _type);
	
	class ZMQPoll;
	class ZMQ;

	typedef std::shared_ptr<zpt::ZMQ> ZMQPtr;

	class ZMQPollPtr : public std::shared_ptr<zpt::ZMQPoll> {
	public:
		ZMQPollPtr(zpt::JSONPtr _options, zpt::EventEmitterPtr _emiter);
		ZMQPollPtr(zpt::JSONPtr _options);
		ZMQPollPtr(zpt::ZMQPoll * _ptr);
		virtual ~ZMQPollPtr();
	};
	
	class ZMQPoll {
	public:
		ZMQPoll(zpt::JSONPtr _options, zpt::EventEmitterPtr _emiter);
		ZMQPoll(zpt::JSONPtr _options);
		virtual ~ZMQPoll();
		
		virtual zpt::JSONPtr options();
		virtual zpt::EventEmitterPtr emitter();
		virtual zpt::ZMQPollPtr self();
		
		virtual void poll(zpt::ZMQPtr _socket);
		virtual void loop();
		virtual zpt::ZMQPtr bind(short _type, std::string _connection);
		
	protected:
		virtual void lock();
		virtual void unlock();
		
	private:
		zpt::JSONPtr __options;
		std::map< std::string, zpt::ZMQPtr > __by_name;
		std::vector< zpt::ZMQPtr > __sockets;
		zmq::context_t __context;
		zmq::socket_t ** __internal;
		::pthread_t __id;
		zmq::pollitem_t * __poll;
		size_t __poll_size;
		pthread_mutex_t* __pt_mtx;
		pthread_mutexattr_t* __pt_attr;
		zpt::EventEmitterPtr __emitter;
		zpt::ZMQPollPtr __self;
		
		virtual void repoll() final;
	};

	class ZMQ {
	public:
		ZMQ(zpt::JSONPtr _options, zpt::EventEmitterPtr _emitter);
		ZMQ(std::string _obj_path, zpt::JSONPtr _options, zpt::EventEmitterPtr _emitter);
		ZMQ(std::string _connection, zpt::EventEmitterPtr _emitter);
		virtual ~ZMQ();
		
		virtual zpt::JSONPtr options();
		virtual zmq::context_t& context();
		virtual std::string& connection();
		virtual zpt::ZMQPtr self();
		virtual zpt::EventEmitterPtr emitter();
		
		virtual zpt::JSONPtr recv();
		virtual zpt::JSONPtr send(zpt::ev::Performative _performative, std::string _resource, zpt::JSONPtr _payload);
		virtual zpt::JSONPtr send(zpt::JSONPtr _envelope);
		
		virtual zmq::socket_t& socket() = 0;
		virtual zmq::socket_t& in() = 0;
		virtual zmq::socket_t& out() = 0;
		virtual short int type() = 0;
		virtual void listen(zpt::ZMQPollPtr _poll) = 0;
		
		virtual void disconnect();
		
	protected:
		virtual void lock();
		virtual void unlock();
		
	private:
		zpt::JSONPtr __options;
		zmq::context_t __context;
		std::string __connection;
		zpt::ZMQPtr __self;
		zpt::EventEmitterPtr __emitter;
		pthread_mutex_t* __pt_mtx;
		pthread_mutexattr_t* __pt_attr;
	};

	class ZMQReq : public zpt::ZMQ {
	public:
		ZMQReq(zpt::JSONPtr _options, zpt::EventEmitterPtr _emitter);
		ZMQReq(std::string _obj_path, zpt::JSONPtr _options, zpt::EventEmitterPtr _emitter);
		ZMQReq(std::string _connection, zpt::EventEmitterPtr _emitter);
		virtual ~ZMQReq();
		
		virtual zpt::JSONPtr send(zpt::JSONPtr _envelope);
		
		virtual zmq::socket_t& socket();
		virtual zmq::socket_t& in();
		virtual zmq::socket_t& out();
		virtual short int type();
		virtual void listen(zpt::ZMQPollPtr _poll);
		
	private:
		zmq::socket_t * __socket;
	};
	
	class ZMQRep : public zpt::ZMQ {
	public:
		ZMQRep(zpt::JSONPtr _options, zpt::EventEmitterPtr _emitter);
		ZMQRep(std::string _obj_path, zpt::JSONPtr _options, zpt::EventEmitterPtr _emitter);
		ZMQRep(std::string _connection, zpt::EventEmitterPtr _emitter);
		virtual ~ZMQRep();
		
		virtual zmq::socket_t& socket();
		virtual zmq::socket_t& in();
		virtual zmq::socket_t& out();
		virtual short int type();
		virtual void listen(zpt::ZMQPollPtr _poll);
		
	private:
		zmq::socket_t * __socket;
	};
	
	class ZMQXPubXSub : public zpt::ZMQ {
	public:
		ZMQXPubXSub(zpt::JSONPtr _options, zpt::EventEmitterPtr _emitter);
		ZMQXPubXSub(std::string _obj_path, zpt::JSONPtr _options, zpt::EventEmitterPtr _emitter);
		ZMQXPubXSub(std::string _connection, zpt::EventEmitterPtr _emitter);
		virtual ~ZMQXPubXSub();
		
		virtual zmq::socket_t& socket();
		virtual zmq::socket_t& in();
		virtual zmq::socket_t& out();
		virtual short int type();
		virtual void listen(zpt::ZMQPollPtr _poll);
		virtual void loop();
		
	private:
		zmq::socket_t * __socket_sub;
			zmq::socket_t * __socket_pub;
	};
	
	class ZMQPubSub : public zpt::ZMQ {
	public:
		ZMQPubSub(zpt::JSONPtr _options, zpt::EventEmitterPtr _emitter);
		ZMQPubSub(std::string _obj_path, zpt::JSONPtr _options, zpt::EventEmitterPtr _emitter);
		ZMQPubSub(std::string _connection, zpt::EventEmitterPtr _emitter);
		virtual ~ZMQPubSub();
		
		virtual zpt::JSONPtr recv();
		virtual zpt::JSONPtr send(zpt::JSONPtr _envelope);
		
		virtual zmq::socket_t& socket();
		virtual zmq::socket_t& in();
		virtual zmq::socket_t& out();
		virtual short int type();
		virtual void listen(zpt::ZMQPollPtr _poll);
		
	private:
		zmq::socket_t * __socket_sub;
		zmq::socket_t * __socket_pub;
	};
	
}

