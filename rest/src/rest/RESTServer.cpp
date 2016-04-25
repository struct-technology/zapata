/*
                                           __i|vIvvvls=_,                                           
                                         _nnnvvvvvvvvv}"'                                           
                                         =nvnnvnnvnnvn;                                             
                                         -"""*{voonnonoSon.                                         
                       ... .. --~ --` `-`.::~. ._.-""*2ooos                                         
                  .=saaawwmmmmWWBWWWmmmmwwaa,;. ._..,. "{X2;                                        
             _awwWWmmmmmmm##mmmmmmmmmBmm####BWmwa. -_; . )SS,                                       
          .amBm####mmmmmmmmmmmmmmmmmmmmmmm#m##m###a   "`  {Xo                                       
        _w#########################################(  -,  )XZc                                      
      _Jm##Z#Z#Z#Z#Z#UUUUUZ#Z#Z#Z#Z#Z#Z#Z#Z#Z#Z#Z#ZL  .,   {ZU,                                     
     _mZ#ZZZZZZUZZUZZZZZZZZUZZUZZZZZZZZZZZZZZZZZZZZm,  .=   )#h.                                    
    _mXXZXZZZZZZZZZXZZZZZZZZZZXZZZZZZZZZZZZZZZZZZZZXX,   -   -4L                                    
   .dXXXXZXXZXXXXXXXZXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX,   -`   "`                                   
   )SSXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXs,   -.                                      
   oS2S2SS2SS2XSSS2X2SS2SS2S2S2S2S2SS2S2S2S2S2S2S2S2S2SSSs,   -.,                                   
  :X2S2222222222o222So22222222SoSoSo2222222222222222o22o2S2o,.   `.                       _s_r      
  )n2oo222222222222o2o222o222o2o2oo22o2o2o22o2o2oo2o2o2o2oo2X2s,.   -       __s__s_s_aaoXXXX7       
  )voooooooooooooooooooooooooooooooooooooooooooooooooooooooonoo2os,.         -"!Y1XSXX2Y*!"`        
  :nnonoonnnnnnnnnnnnnnnnnnnnnnnogmpnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnoos,.                             
   1nnvnnvnnnvnnvnnvnnvnnvnnvnnvdQQQpvnvnnvnvnvnvnvnvnvvvvvvnvvvnvnvnnni=_%________,=___,.          
   =vvvvvvvvvvvvvvvvvvvvvvvvvvvvmQQQWguvvvvvnvnwwwggggmQQQQQmmgwvvvvvvvvnnvvnnnvnnnnnv"~            
    <vvvvvvvvvvvvvvvvvvvvvvvvvomQQQQQQQQQQQQQQQQWQQQQQQQWWWWQQQQQgr~^^^^^^^^^^^^^--                 
     +vlIIlIlllIllIowwywymmQWQQQQQQ$QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQw                                
      -<IlllIlIlqmQQQQQWQQQQQQQQQQQ-QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQm,                              
        -+vllllqWWQQQQQQQQQQQQQQQQE )WQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQm,                             
           -^+imWQQQQQQQQQQQQQQQQQ'  "$QQQQQQQQQQQQQQQQQQQQQQQQQQQQWQQm                             
               QQQQQQQQQQQQQQQQWD'     ??WWWWWWWWWWW#BBUHVTTTVHQWQQQQQQL                            
               QQQP???????!!""~                                 -9WQQQQQ,                           
               QQ[                                                -$QQQQL                           
               WW.                                                 -QWQQQ/                          
                                                                    -^^""~                          

 s_a_s_s_a_s_%   _ssaaas,,     _s_a_a __aaaaa,,    __aaaas,,      ,s_dQs__s_,    __aaaaa,,.         
:QP?????9mQWT!  QQT!"!!??Qg  . "???QQdT??????9WQc 3QD?!"!??$Q,  . ???$QT????'   ]Q@?!"!??9Qc  .     
-"'_aawV?"` _a,_wwYYT??TYQW;-Qr   .QQ.        _QW ww2T?T?T?$Wf $k    jQ(     <a swZY?T?T?9QE ]Q     
_yQQQwwawwawyQ(]WmwaaawwY4QgyW'   .QW9AmywwwwmB?'-WWwaaawwZTWQwmf    "WQwwawyW! 9QgaaawwpTWQwmP     
-~------------   -^^^^-    ~-  __saWQs__%=--       -~^^^-    --        -~^^~-     ~^"^~    -~`     

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

#include <dlfcn.h>
#include <zapata/exceptions/ClosedException.h>
#include <zapata/exceptions/InterruptedException.h>
#include <zapata/json.h>
#include <zapata/log/log.h>
#include <sys/sem.h>
#include <zapata/text/convert.h>
#include <zapata/file/manip.h>
#include <zapata/rest/RESTServer.h>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <zapata/rest/codes_rest.h>

zapata::RESTServer::RESTServer(zapata::JSONObj& _options) : __pool( new zapata::RESTPool(_options)), __options(_options) {
	this->__next = -1;
	this->__initialized = false;

	if (!!this->options()["max_jobs"] && ((int) this->options()["max_jobs"]) != -1) {
		this->max((size_t) this->options()["max_jobs"]);
	}
	else {
		this->max(1);
	}

	if (!this->options()["max_connections_per_job"]->ok() || ((int) this->options()["max_connections_per_job"]) <= 0) {
		((zapata::JSONObj&) this->options()) << "max_connections_per_job" << 1024;
	}

	if (!!this->options()["log"]["level"]) {
		zapata::log_lvl = (int) this->options()["log"]["level"];
	}
	if (!!this->options()["log"]["file"]) {
		zapata::log_fd = new ofstream();
		((ofstream*) zapata::log_fd)->open(((string) this->options()["log"]["file"]).data());
	}

	if (this->__options["modules"]->ok()) {
		for (auto _i : this->__options["modules"]->obj()) {
			string _key = _i.first;
			JSONElement _value = _i.second;

			string _lib_file("lib");
			_lib_file.append((string) _value["lib"]);
			_lib_file.append(".so");

			if (_lib_file.length() > 6) {
				void *hndl = dlopen(_lib_file.data(), RTLD_NOW);
				if (hndl == nullptr) {
					zlog(string(dlerror()), zapata::error);
				}
				else {
					void (*_populate)(zapata::RESTPoolPtr&);
					_populate = (void (*)(zapata::RESTPoolPtr&)) dlsym(hndl, "restify");
					_populate(this->__pool);
				}
			}
		}
	}
	if (!!this->options()["rest"]["uploads"]["upload_controller"]) {
		/*
		 *  definition of handlers for the file upload controller
		 *  registered as a Controller
		 */
		this->__pool->on(zapata::HTTPPost, "^/api/1.0/file/upload$", [] (zapata::HTTPReq& _req, zapata::HTTPRep& _rep, zapata::JSONPtr _config, zapata::RESTPoolPtr& _pool) -> void {
			string _body = _req->body();
			assertz(_body.length() != 0, "Body entity must be provided.", zapata::HTTP412, zapata::ERRBodyEntityMustBeProvided);

			assertz(_req->header("Content-Type").find("application/json") != string::npos, "Body entity must be 'application/json'", zapata::HTTP406, zapata::ERRBodyEntityWrongContentType);

			zapata::JSONObj& _params = zapata::fromstr(_body);

			assertz(!!_params["uploaded_file"], "The 'uploaded_file' parameter must be provided.", zapata::HTTP412, zapata::ERRRequiredField);

			string _from((string) _params["uploaded_file"]);
			string _to((string) _config["rest"]["uploads"]["upload_path"]);
			zapata::normalize_path(_to, true);

			string _originalname(_req->header("X-Original-Filename"));
			string _path;
			string _name;
			string _mime;
			do {
				string _dir(_to);
				_path.assign("");
				zapata::generate_hash(_path);
				_dir.insert(_dir.length(), _path);
				zapata::mkdir_recursive(_dir, 0777);
				_path.insert(_path.length(), "/");

				if (_originalname.length() != 0) {
					_path.insert(_path.length(), _originalname);

					_mime.assign(_req->header("X-Original-Mimetype"));
					if (_mime.length() != 0) {
						_mime.assign(_mime.substr(0, _mime.find(";")));
					}
				}
				else {
					zapata::MIMEType _m = zapata::get_mime((string) _params["uploaded_file"]);
					_path.insert(_path.length(), "_uploaded");
					_path.insert(_path.length(), zapata::mimetype_extensions[_m]);

					_mime.assign(zapata::mimetype_names[_m]);
				}

				_name.assign(_path);
				_path.insert(0, _to);
			}
			while (zapata::path_exists(_path));

			string _encoding(_req->header("X-Content-Transfer-Encoding"));
			transform(_encoding.begin(), _encoding.end(), _encoding.begin(), ::toupper);
			if (_encoding == "BASE64") {
				fstream _ifs;
				_ifs.open(_from.data());
				fstream _ofs;
				_ofs.open(_path.data());

				zapata::base64::decode(_ifs, _ofs);

				_ifs.close();
				_ofs.flush();
				_ofs.close();
			}
			else {
				assertz(zapata::copy_path((string ) _params["uploaded_file"], _path), "There was an error copying the temporary file to the 'upload_path' directory.", zapata::HTTP500, zapata::ERRFilePermissions);
			}

			string _location((string) _config["rest"]["uploads"]["upload_url"]);
			zapata::normalize_path(_location, true);
			_location.insert(_location.length(), _name);

			_rep->status(zapata::HTTP201);
			_rep->header("X-File-Mimetype", _mime);
			_rep->header("Location", _location);
		});

		/*
		 *  definition of handlers for the file upload removal controller
		 *  registered as a Controller
		 */
		this->__pool->on(zapata::HTTPPost, "^/api/1.0/file/remove$", [] (zapata::HTTPReq& _req, zapata::HTTPRep& _rep, zapata::JSONPtr _config, zapata::RESTPoolPtr& _pool) -> void {
			string _body = _req->body();
			assertz(_body.length() != 0, "Body entity must be provided.", zapata::HTTP412, zapata::ERRBodyEntityMustBeProvided);

			assertz(_req->header("Content-Type").find("application/json") != string::npos, "Body entity must be 'application/json'", zapata::HTTP406, zapata::ERRBodyEntityWrongContentType);

			zapata::JSONObj& _params = zapata::fromstr(_body);

			assertz(!!_params["file_url"], "The 'file_url' parameter must be provided.", zapata::HTTP412, zapata::ERRRequiredField);

			string _url((string) _params["file_url"]);
			string _url_root(_config["rest"]["uploads"]["upload_url"]);
			string _path_root(_config["rest"]["uploads"]["upload_path"]);

			zapata::replace(_url, _url_root, _path_root);

			assertz(zapata::path_exists(_url), "Couldn't find the designated file", zapata::HTTP404, zapata::ERRFileNotFound);

			_url.assign(_url.substr(0, _url.rfind("/")));

			string _cmd("rm -rf ");
			_cmd.insert(_cmd.length(), _url);
			_cmd.insert(_cmd.length(), " > /dev/null");
			assertz(system(_cmd.data()) == 0, "There was an error removing the uploaded file.", zapata::HTTP500, zapata::ERRFilePermissions);

			_rep->status(zapata::HTTP200);
		});
	}

	assertz(this->options()["rest"]["port"]->ok(), "an HTTP listening port must be provided in the configuration file", 500, 0);

	unsigned int _port =  (unsigned int) this->options()["rest"]["port"];
	string _text("starting RESTful server on port ");
	zapata::tostr(_text, _port);
	zlog(_text, zapata::notice);
	this->__ss.bind(_port);

	if (this->options()["rest"]["websocket"]["port"]->ok()) {
		zapata::Job _ws([ & ] (zapata::Job& _self) -> void {
			unsigned int _ws_port = (unsigned int) this->options()["rest"]["websocket"]["port"];
			zapata::websocketserverstream _sws;
			string _text("starting RESTful web-socket listener on port ");
			zapata::tostr(_text, _ws_port);
			zlog(_text, zapata::notice);
			_sws.bind(_ws_port);

			for (; true; ) {
				int _fd_ws = -1;
				_sws.accept(& _fd_ws);
				
				zapata::websocketstream _cws(_fd_ws);
				_cws.handshake();
				_cws.unassign();
				
				size_t _next = this->next();
				(* this->__jobs.at(_next)).assign(_fd_ws);
			}
		});
		_ws->start();
	}
}

zapata::RESTServer::~RESTServer(){
}

void zapata::RESTServer::wait() {
	int _cs_fd = -1;
	this->__ss.accept(&_cs_fd);
	size_t _next = this->next();
	(* this->__jobs.at(_next)).assign(_cs_fd);
}

void zapata::RESTServer::start() {
	try {
		for (; true; ) {
			this->wait();
		}
	}
	catch (zapata::InterruptedException& e) {
		return;
	}
}

zapata::RESTPoolPtr zapata::RESTServer::pool() {
	return this->__pool;
}

zapata::JSONObj& zapata::RESTServer::options() {
	return this->__options;
}

void zapata::RESTServer::notify() {

}

size_t zapata::RESTServer::max() {
	return this->__max_idx;
}

size_t zapata::RESTServer::next() {
	if (this->__next == this->max() - 1) {
		this->__initialized = true;
		this->__next = -1;
	}
	if (!this->__initialized) {
		zapata::RESTJob * _job = new RESTJob(this->__options);
		this->__jobs.push_back(_job);
		(* _job).pool(this->__pool);
		(* _job)->start();
	}
	return ++this->__next;
}

void zapata::RESTServer::max(size_t _max) {
	this->__max_idx = _max;
}

void zapata::dirs(std::string _dir, zapata::JSONPtr& _options) {
	std::vector<std::string> _files;
	zapata::ls(_dir, _files, false);
	for (auto _file : _files) {
		try {
			zapata::JSONPtr _conf;
			std::ifstream _ifs;
			_ifs.open(_file.data());
			_ifs >> _conf;
			_ifs.close();

			for (auto _new_field : _conf->obj()) {
				if (!_options[_new_field.first]->ok()) {
					_options << _new_field.first << _new_field.second;
				}
				else {
					zapata::JSONPtr _merged = _options[_new_field.first] + _new_field.second;
					_options >> _new_field.first
					_options << _new_field.first << _merged;
				}
			}
		}
		catch(zapata::SyntaxErrorException& _e) {}
	}
}

void zapata::env(zapata::JSONObj& _options) {
	zapata::JSONPtr _ptr = zapata::make_ptr(_options);
	zapata::env(_ptr);
}

void zapata::env(zapata::JSONPtr& _options) {
	_options->inspect(zapata::make_ptr(JSON( "$regexp" << "\\$\\{([^}]+)\\}" )), [ & ] (zapata::JSONElementT * _element, std::string _key, zapata::JSONElementT * _parent, zapata::JSONPtr _pattern) -> void {
		string _var = _key.substr(2, _key.length() - 3);
		char * _valuec = std::getenv(_var.data());
		string _value;
		if (_valuec != nullptr) {
			_value.assign(_valuec);
		}
		assertz(_value.length() != 0, "no environment variable with that name", 0, 0);
		(* _parent) << _key << _value;
	});
}