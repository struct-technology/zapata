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

#include <zapata/log/log.h>

#include <zapata/text/convert.h>
#include <sys/time.h>
#include <unistd.h>
#include <strings.h>

namespace zpt {
	short int log_lvl = 0;
	ostream* log_fd = nullptr;
	long log_pid = 0;
	string* log_pname = nullptr;
	char* log_hname = nullptr;

	const char* log_lvl_names[] = {
		"\033[1;31m\033[4;31memergency\033[0m | ",
		"\033[0;31m\033[4;31malert\033[0m     | ",
		"\033[0;33m\033[4;33mcritical\033[0m  | ",
		"\033[0;33m\033[4;33merror\033[0m     | ",
		"\033[0;35m\033[4;35mwarning\033[0m   | ",
		"\033[0;36m\033[4;36mnotice\033[0m    | ",
		"\033[0;34m\033[4;34minfo\033[0m      | ",
		"\033[0;37m\033[4;37mdebug\033[0m     | "
	};
}

int zpt::log(string _text, zpt::LogLevel _level, string _host, int _line, string _file) {
	if (zpt::log_fd == nullptr) {
		return - 1;
	}
	struct timeval _tp;
	gettimeofday(& _tp, nullptr);

	zpt::replace(_text, "\n", "\\n");
	zpt::replace(_text, "\"", "\\\"");

	string _log("{\"version\":\"1.1\",\"host\":\"");
	_log.insert(_log.length(), _host);
	_log.insert(_log.length(), "\",\"source\":\"");
	_log.insert(_log.length(), _host);
	_log.insert(_log.length(), "\",\"short_message\":\"");
	_log.insert(_log.length(), _text);
	_log.insert(_log.length(), "\",\"full_message\":\"");
	_log.insert(_log.length(), _file);
	_log.insert(_log.length(), ":");
	zpt::tostr(_log, _line);
	_log.insert(_log.length(), " | ");
	_log.insert(_log.length(), _text);
	_log.insert(_log.length(), "\",\"timestamp\":");
	zpt::tostr(_log, _tp.tv_sec);
	_log.insert(_log.length(), ".");
	zpt::tostr(_log, (int) (_tp.tv_usec / 1000));
	_log.insert(_log.length(), ",\"level\":");
	zpt::tostr(_log, (int) _level);
	_log.insert(_log.length(), ",\"pid\":");
	zpt::tostr(_log, zpt::log_pid);
	_log.insert(_log.length(), ",\"exec\":\"");
	_log.insert(_log.length(), * zpt::log_pname);
	_log.insert(_log.length(), "\",\"file\":\"");
	_log.insert(_log.length(), _file);
	_log.insert(_log.length(), "\",\"line\":");
	zpt::tostr(_log, _line);
	_log.insert(_log.length(), "}");

	(* zpt::log_fd) << _log << endl << flush;
	return 0;
}

char* zpt::log_hostname() {
	if (zpt::log_hname == nullptr) {
		zpt::log_hname = new char[65];
		bzero(zpt::log_hname, 65);
		gethostname(zpt::log_hname, 64);
	}
	return zpt::log_hname;
}
