/* -*-c++-*- IfcQuery www.ifcquery.com
*
MIT License

Copyright (c) 2017 Fabian Gerold

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>
#include "ifcpp/model/GlobalDefines.h"
#include "ifcpp/model/BuildingObject.h"
#include "WriterUtil.h"

std::string encodeStepString( const std::wstring& str )
{
	std::stringstream result;
	constexpr auto beginUnicodeTag = "\\X2\\";
	constexpr auto endUnicodeTag = "\\X0\\";
	auto hasOpenedUnicodeTag = false;

	result << std::hex << std::setfill('0') << std::uppercase;
	for (auto const append_char : str)
	{
		//chars 32 to 126 (inclusive) may appear as raw characters
		//everything else has to be encoded
		if (32 > append_char)
		{
			if( hasOpenedUnicodeTag )
				result << std::setw(4) << int{append_char};
			//use short encoding form, instead of long when not between unicode tag
			result << "\\X\\" << std::setw(2) << int{append_char};
		}
		else if( 127 > append_char )
		{
			if( hasOpenedUnicodeTag )
			{
				result << endUnicodeTag;
				hasOpenedUnicodeTag = false;
			}
			result << static_cast<char>(append_char);
		}
		else
		{
			if( !hasOpenedUnicodeTag )
			{
				result << beginUnicodeTag;
				hasOpenedUnicodeTag = true;
			}
			result << std::setw(4) << int{append_char};
		}
	}

	if( hasOpenedUnicodeTag )
	{
		result << endUnicodeTag;
		hasOpenedUnicodeTag = false;
	}

	return result.str();
}
