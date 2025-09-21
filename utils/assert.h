#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2016 v71 
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

inline void _better_assert(const char *condition, const char *message, const char *fileline)
{
	std::cerr << "File      : " << fileline << "\n" << "Assertion : \"" << condition << "\" ( " << message << " )\nLine      : " << std::to_string(__LINE__) << std::endl;
	std::abort();
}

inline void _better_assert(const char *condition, const std::string &message, const char *fileline)
{
	_better_assert(condition, message.c_str(), fileline);
}

#ifdef NDEBUG
#define better_assert(condition, message) static_cast<void>(0)
#else
#define better_assert(condition, message) \
    static_cast<bool>(condition)          \
        ? static_cast<void>(0)            \
        : _better_assert(#condition, message, __FILE__ ":" _STRINGIZE(__LINE__))
#endif
