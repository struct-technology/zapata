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

#pragma once

#include <zapata/json/JSONObj.h>
#include <zapata/json/JSONLexer.h>

namespace zpt {

class JSONTokenizerLexer : public JSONLexer {
      public:
	JSONTokenizerLexer(std::istream& _in = std::cin, std::ostream& _out = std::cout);
	virtual ~JSONTokenizerLexer();

	void switchRoots(zpt::JSONPtr& _root);

	void result(zpt::JSONType _in);
	void finish(zpt::JSONType _in);

	void init(zpt::JSONType _in_type, const string _in_str);
	void init(zpt::JSONType _in_type);
	void init(bool _in);
	void init(long long _in);
	void init(double _in);
	void init(std::string _in);
	void init(zpt::lambda _in);
	void init();

	void add();

	zpt::JSONElementT* __root;
	zpt::JSONType __root_type;
	zpt::JSONElementT* __parent;
};
}
