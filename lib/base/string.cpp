/******************************************************************************
 * Icinga 2                                                                   *
 * Copyright (C) 2012-2017 Icinga Development Team (https://www.icinga.com/)  *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software Foundation     *
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ******************************************************************************/

#include "base/string.hpp"
#include "base/value.hpp"
#include "base/primitivetype.hpp"
#include "base/tlsutility.hpp"
#include "base/dictionary.hpp"
#include <ostream>

using namespace icinga;

REGISTER_BUILTIN_TYPE(String, String::GetPrototype());

const String::SizeType String::NPos = std::string::npos;

#ifndef _MSC_VER
String::String(Value&& other)
{
	*this = std::move(other);
}
#endif /* _MSC_VER */

String& String::operator=(Value&& other)
{
	if (other.IsString())
		m_Data = std::move(other.Get<String>());
	else
		*this = static_cast<String>(other);

	return *this;
}

String& String::operator+=(const Value& rhs)
{
	m_Data += static_cast<String>(rhs);
	return *this;
}

bool String::TimeConstantCompare(const String& other) const
{
	const String hashed1 = SHA256(*this);
	const String hashed2 = SHA256(other);

	const char *p1 = hashed1.CStr();
	const char *p2 = hashed2.CStr();

	volatile char c = 0;
	for (size_t i=0; i<64; ++i)
		c |= p1[i] ^ p2[i];
	return (c == 0);
}

