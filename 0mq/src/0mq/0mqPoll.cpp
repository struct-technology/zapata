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

#include <zapata/0mq/0mqPoll.h>

zapata::ZMQPoll::ZMQPoll(zapata::JSONObj& _options, zapata::EventEmitterPtr _emiter) : __options( _options), __context(1), __id(0), __poll(nullptr), __emitter(_emiter) {
	this->__internal = new zmq::socket_t * [2];

	this->__internal[0] = new zmq::socket_t(this->__context, ZMQ_REP);
	this->__internal[1] = new zmq::socket_t(this->__context, ZMQ_REQ);

	std::string _bind("inproc://notify");
	this->__internal[0]->bind(_bind.data());
	this->__internal[1]->connect(_bind.data());

	this->__poll_size = this->__sockets.size() + 1;
	this->__poll = (zmq::pollitem_t *) realloc(this->__poll, (this->__sockets.size() + 1) * sizeof(zmq::pollitem_t));
	this->__poll[0] = { * this->__internal[0], 0, ZMQ_POLLIN, 0 };

	this->__mtx = new pthread_mutex_t();
	this->__attr = new pthread_mutexattr_t();
	pthread_mutexattr_init(this->__attr);
	pthread_mutex_init(this->__mtx, this->__attr);	
}

zapata::ZMQPoll::~ZMQPoll() {
	this->__internal[0]->close();
	this->__internal[1]->close();
	delete this->__internal[0];
	delete this->__internal[1];
 	delete [] this->__internal;
 	free(this->__poll);

	pthread_mutexattr_destroy(this->__attr);
	pthread_mutex_destroy(this->__mtx);
	delete this->__mtx;
	delete this->__attr;

	zlog(string("0mq poll clean up"), zapata::notice);
}

zapata::JSONObj& zapata::ZMQPoll::options() {
	return this->__options;
}

zapata::EventEmitterPtr zapata::ZMQPoll::emitter() {
	return this->__emitter;
}

void zapata::ZMQPoll::repoll() {
	zlog(string("scanning poll"), zapata::notice);
	this->__poll = (zmq::pollitem_t *) realloc(this->__poll, (this->__sockets.size() + 1) * sizeof(zmq::pollitem_t));

	size_t _i = 1;
	for (auto _socket : this->__sockets) {
		if (_i < this->__poll_size) {
			_i++;
			continue;
		}
		this->__poll[_i] = { _socket->socket(), 0, ZMQ_POLLIN, 0 };
		_i++;
	}
	this->__poll_size = this->__sockets.size() + 1;
}

void zapata::ZMQPoll::loop() {
	this->__id = pthread_self();

	for(; true; ) {
		try {			
			zmq::poll(this->__poll, this->__poll_size, -1);
		}
		catch(zmq::error_t& _e) {
			zlog(string("while polling sockets: ") + _e.what(), zapata::error);
		}

		if (this->__poll[0].revents & ZMQ_POLLIN) {
			this->repoll();

			zmq::message_t _reply;
			try {
				this->__internal[0]->recv(& _reply);
			}
			catch(zmq::error_t& _e) {
			}
			zmq::message_t _signal(2);
			memcpy ((void *) _signal.data(), "ok", 2);
			this->__internal[0]->send(_signal);
		}
		else {
			for (size_t _k = 1; _k != this->__sockets.size() + 1; _k++) {
				if (this->__poll[_k].revents & ZMQ_POLLIN) {
					zapata::JSONPtr _envelope = this->__sockets[_k - 1]->recv();
					std::string _pstr(_envelope["performative"]->str());
					zapata::ev::Performative _performative;
					zapata::fromstr(_pstr, & _performative);
					zapata::JSONPtr _result = this->__emitter->trigger(_performative, _envelope["resource"]->str(), _envelope["payload"]);
					if (_result != zapata::undefined) {
						this->__sockets[_k - 1]->send(_result);
					}
				}
			}
		}
	}
}


extern "C" int zapata_0mq() {
	return 1;
}
