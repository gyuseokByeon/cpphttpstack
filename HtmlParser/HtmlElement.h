/* 
 * Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#ifndef _HTML_ELEMENT_H_
#define _HTML_ELEMENT_H_

#include "SipPlatformDefine.h"
#include <string>
#include <list>
#include <map>

class CHtmlElement;

/**
 * @ingroup HtmlParser
 * @brief XML attribute map. key 는 attribute 이름이고 value 는 attribute 값이다.
 */
typedef std::map< std::string, std::string > HTML_ATTRIBUTE_MAP;

/**
 * @ingroup HtmlParser
 * @brief CXmlElement 객체 리스트
 */
typedef std::list< CHtmlElement > HTML_ELEMENT_LIST;

class CHtmlElement
{
public:
	CHtmlElement();
	~CHtmlElement();

	int Parse( const char * pszText, int iTextLen );
	int Parse( std::string & strText );

	int ToString( char * pszText, int iTextSize, bool bUseTab = false, int iDepth = 0 );
	void ToString( std::string & strText, bool bUseTab = false, int iDepth = 0 );

	void Clear( );

	const char * SelectAttribute( const char * pszName );
	const char * SelectAttributeTrim( const char * pszName );
	bool SelectAttribute( const char * pszName, std::string & strValue );
	bool SelectAttributeTrim( const char * pszName, std::string & strValue );
	bool SelectAttribute( const char * pszName, int & iValue );

	CHtmlElement * SelectElement( const char * pszName, const int iIndex = 0 );
	CHtmlElement * SelectElement( const int iIndex );
	bool SelectElementList( const char * pszName, HTML_ELEMENT_LIST & clsList );

	const char * GetElementData( const char * pszName, const int iIndex = 0 );
	const char * GetElementDataTrim( const char * pszName, const int iIndex = 0 );
	bool SelectElementData( const char * pszName, std::string & strData, const int iIndex = 0 );
	bool SelectElementTrimData( const char * pszName, std::string & strData, const int iIndex = 0 );

	const char * GetName();
	const char * GetData();

	bool IsDataEmpty();

protected:
	void SetName( const char * pszText, int iNameLen );

	std::string	m_strName;
	std::string m_strData;

	HTML_ATTRIBUTE_MAP	m_clsAttributeMap;
	HTML_ELEMENT_LIST		m_clsElementList;

	bool m_bNotParseUntilNameEnd;
};

#endif
