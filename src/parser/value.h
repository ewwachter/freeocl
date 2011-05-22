/*
	FreeOCL - a free OpenCL implementation for CPU
	Copyright (C) 2011  Roland Brochard

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#ifndef __FREEOCL_PARSER_VALUE_H__
#define __FREEOCL_PARSER_VALUE_H__

#include "node.h"
#include "native_type.h"
#include "pointer_type.h"
#include <cstdint>

namespace FreeOCL
{
	template<class T>
	class Value : public Node
	{
	public:
		Value(const T &v) : v(v)	{}
		virtual ~Value()	{}

		const T &getValue() const	{	return v;	}

		virtual smartptr<Type> getType() const;

		virtual void write(std::ostream& out) const
		{
			out << v << ' ';
		}
	private:
		const T v;
	};
}

#endif
