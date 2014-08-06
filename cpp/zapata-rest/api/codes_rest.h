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

#pragma once

namespace zapata {
	// start at 0
	enum ErrorCodeRest {
		ERRGeneric = 0,
		ERRBodyEntityMustBeProvided = 1,
		ERRBodyEntityWrongContentType = 2,
		ERRRequiredField = 3,
		ERRFileNotFound = 4,
		ERRFilePermissions = 5,
		ERRResourceNotFound = 6,
		ERRResourceIsEmpty = 7,
		ERRResourceDuplicate = 8,
		ERRConfigParameterNotFound = 9
	};
}
