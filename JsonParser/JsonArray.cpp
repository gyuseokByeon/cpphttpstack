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

#include "SipPlatformDefine.h"
#include "JsonObject.h"
#include "Log.h"
#include "MemoryDebug.h"

CJsonArray::CJsonArray()
{
	m_cType = JSON_TYPE_ARRAY;
}

CJsonArray::~CJsonArray()
{
	Clear();
}

/**
 * @ingroup JsonParser
 * @brief JSON �迭 ���ڿ� �Ľ��Ͽ��� �ڷᱸ���� �����Ѵ�.
 * @param pszText		JSON �迭 ���ڿ�
 * @param iTextLen	JSON �迭 ���ڿ� ����
 * @returns JSON �迭 ���ڿ� �Ľ̿� �����ϸ� �Ľ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CJsonArray::Parse( const char * pszText, int iTextLen )
{
	int iPos = -1, iParseLen;
	uint8_t	cType = 0;
	std::string	strName;

	Clear();

	for( int i = 0; i < iTextLen; ++i )
	{
		if( pszText[i] == '[' )
		{
			iPos = i + 1;
			break;
		}
	}

	if( iPos == -1 ) 
	{
		CLog::Print( LOG_ERROR, "%s [ is not found", __FUNCTION__ );
		return -1;
	}

	for( int i = iPos; i < iTextLen; ++i )
	{
		if( cType == 0 )
		{
			CJsonType * pclsType = CJsonObject::GetJsonType( pszText, iTextLen, i );

			if( pclsType )
			{
				iParseLen = pclsType->Parse( pszText + i, iTextLen - i );
				if( iParseLen == -1 )
				{
					CLog::Print( LOG_ERROR, "%s json parse error", __FUNCTION__ );
					delete pclsType;
					return -1;
				}

				m_clsList.push_back( pclsType );
				cType = 0;
				i += iParseLen;
			}
		}
		else if( pszText[i] == ']' )
		{
			if( cType == 0 )
			{
				iPos = i + 1;
				break;
			}
		}
	}

	return iPos;
}

/**
 * @ingroup JsonParser
 * @brief �ڷᱸ���� JSON �迭 ���ڿ��� ��ȯ�Ѵ�.
 * @param strText JSON �迭 ���ڿ� ���� ����
 * @returns JSON �迭 ���ڿ� ���̸� �����Ѵ�.
 */
int CJsonArray::ToString( std::string & strText )
{
	JSON_LIST::iterator itJL;
	std::string strBuf;

	strBuf.append( "[" );

	for( itJL = m_clsList.begin(); itJL != m_clsList.end(); ++itJL )
	{
		if( itJL == m_clsList.begin() )
		{
			strBuf.append( " " );
		}
		else
		{
			strBuf.append( ", " );
		}

		CJsonObject::JsonToString( *itJL, strBuf );
	}

	strBuf.append( " ]" );
	strText.append( strBuf );

	return strBuf.length();
}

/**
 * @ingroup JsonParser
 * @brief �ڽ��� ������ ��ü�� �����Ѵ�.
 * @returns �����ϸ� �ڽ��� ������ ��ü�� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
CJsonType * CJsonArray::Copy( )
{
	CJsonArray * pclsArray = new CJsonArray();
	if( pclsArray == NULL ) return NULL;

	JSON_LIST::iterator itJL;

	for( itJL = m_clsList.begin(); itJL != m_clsList.end(); ++itJL )
	{
		CJsonType * pclsType = (*itJL)->Copy();
		if( pclsType == NULL )
		{
			delete pclsArray;
			return NULL;
		}

		pclsArray->m_clsList.push_back( pclsType );
	}

	return pclsArray;
}

/**
 * @ingroup JsonParser
 * @brief �ڷᱸ���� �ʱ�ȭ��Ų��.
 */
void CJsonArray::Clear()
{
	JSON_LIST::iterator itJL;

	for( itJL = m_clsList.begin(); itJL != m_clsList.end(); ++itJL )
	{
		delete *itJL;
	}

	m_clsList.clear();
}

/**
 * @ingroup JsonParser
 * @brief �迭 �ڷᱸ������ Element �ε����� �ش��ϴ� ���ڿ� ���� �˻��Ѵ�.
 * @param iIndex		Element �ε���
 * @param strValue	Element ��
 * @returns �˻��� �����ϰ� �ش� ���� ���ڿ� Ÿ���� ��� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::SelectData( int iIndex, std::string & strValue )
{
	CJsonType * pclsType;

	if( SelectData( iIndex, &pclsType ) == false ) return false;
	if( pclsType->m_cType != JSON_TYPE_STRING )
	{
		CLog::Print( LOG_ERROR, "%s type(%d) is not string (%s)", __FUNCTION__, iIndex, pclsType->GetTypeString() );
		return false;
	}

	strValue = ((CJsonString *)pclsType)->m_strValue;

	return true;
}

/**
 * @ingroup JsonParser
 * @brief �迭 �ڷᱸ������ Element �ε����� �ش��ϴ� ���� ���� �˻��Ѵ�.
 * @param iIndex Element �ε���
 * @param iValue Element ��
 * @returns �˻��� �����ϰ� �ش� ���� ���� Ÿ���� ��� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::SelectData( int iIndex, int64_t & iValue )
{
	CJsonType * pclsType;

	if( SelectData( iIndex, &pclsType ) == false ) return false;
	if( pclsType->m_cType != JSON_TYPE_INT )
	{
		CLog::Print( LOG_ERROR, "%s type(%d) is not int (%s)", __FUNCTION__, iIndex, pclsType->GetTypeString() );
		return false;
	}

	iValue = ((CJsonInt *)pclsType)->m_iValue;

	return true;
}

/**
 * @ingroup JsonParser
 * @brief �迭 �ڷᱸ������ Element �ε����� �ش��ϴ� boolean ���� �˻��Ѵ�.
 * @param iIndex Element �ε���
 * @param bValue Element ��
 * @returns �˻��� �����ϰ� �ش� ���� boolean Ÿ���� ��� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::SelectData( int iIndex, bool & bValue )
{
	CJsonType * pclsType;

	if( SelectData( iIndex, &pclsType ) == false ) return false;
	if( pclsType->m_cType != JSON_TYPE_BOOL )
	{
		CLog::Print( LOG_ERROR, "%s type(%d) is not bool (%s)", __FUNCTION__, iIndex, pclsType->GetTypeString() );
		return false;
	}

	bValue = ((CJsonBool *)pclsType)->m_bValue;

	return true;
}

/**
 * @ingroup JsonParser
 * @brief �迭 �ڷᱸ������ Element �ε����� �ش��ϴ� object ���� �˻��Ѵ�.
 * @param iIndex			Element �ε���
 * @param ppclsObject Element ��
 * @returns �˻��� �����ϰ� �ش� ���� object Ÿ���� ��� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::SelectData( int iIndex, CJsonObject ** ppclsObject )
{
	CJsonType * pclsType;

	if( SelectData( iIndex, &pclsType ) == false ) return false;
	if( pclsType->m_cType != JSON_TYPE_OBJECT )
	{
		CLog::Print( LOG_ERROR, "%s type(%d) is not object (%s)", __FUNCTION__, iIndex, pclsType->GetTypeString() );
		return false;
	}

	*ppclsObject = (CJsonObject *)pclsType;

	return true;
}

/**
 * @ingroup JsonParser
 * @brief �迭 �ڷᱸ������ Element �ε����� �ش��ϴ� �迭 ���� �˻��Ѵ�.
 * @param iIndex			Element �ε���
 * @param ppclsArray	Element ��
 * @returns �˻��� �����ϰ� �ش� ���� �迭 Ÿ���� ��� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::SelectData( int iIndex, CJsonArray ** ppclsArray )
{
	CJsonType * pclsType;

	if( SelectData( iIndex, &pclsType ) == false ) return false;
	if( pclsType->m_cType != JSON_TYPE_ARRAY )
	{
		CLog::Print( LOG_ERROR, "%s type(%d) is not array (%s)", __FUNCTION__, iIndex, pclsType->GetTypeString() );
		return false;
	}

	*ppclsArray = (CJsonArray *)pclsType;

	return true;
}

/**
 * @ingroup JsonParser
 * @brief �迭 �ڷᱸ������ Element �ε����� �ش��ϴ� ���� �˻��Ѵ�.
 * @param iIndex		Element �ε���
 * @param ppclsType Element ��
 * @returns �˻��� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::SelectData( int iIndex, CJsonType ** ppclsType )
{
	if( iIndex >= (int)m_clsList.size() ) 
	{
		CLog::Print( LOG_ERROR, "%s iIndex(%d) >= m_clsList.size(%d)", __FUNCTION__, iIndex, m_clsList.size() );
		return false;
	}

	JSON_LIST::iterator itJL;
	int iCount = 0;

	for( itJL = m_clsList.begin(); itJL != m_clsList.end(); ++itJL )
	{
		if( iIndex == iCount )
		{
			*ppclsType = *itJL;
			return true;
		}

		++iCount;
	}

	return false;
}

/**
 * @ingroup JsonParser
 * @brief JSON �迭�� ���ڿ� Element ���� �߰��Ѵ�.
 * @param pszValue Element ��
 * @returns JSON �迭�� Element ���� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::InsertData( const char * pszValue )
{
	CJsonString * pclsNew = new CJsonString();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	pclsNew->m_strValue = pszValue;
	m_clsList.push_back( pclsNew );

	return true;
}

/**
 * @ingroup JsonParser
 * @brief JSON �迭�� ���� Element ���� �߰��Ѵ�.
 * @param iValue Element ��
 * @returns JSON �迭�� Element ���� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::InsertData( int32_t iValue )
{
	return InsertData( (int64_t)iValue );
}

/**
 * @ingroup JsonParser
 * @brief JSON �迭�� ���� Element ���� �߰��Ѵ�.
 * @param iValue Element ��
 * @returns JSON �迭�� Element ���� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::InsertData( int64_t iValue )
{
	CJsonInt * pclsNew = new CJsonInt();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	pclsNew->m_iValue = iValue;
	m_clsList.push_back( pclsNew );

	return true;
}

/**
 * @ingroup JsonParser
 * @brief JSON �迭�� boolean Element ���� �߰��Ѵ�.
 * @param bValue Element ��
 * @returns JSON �迭�� Element ���� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::InsertData( bool bValue )
{
	CJsonBool * pclsNew = new CJsonBool();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	pclsNew->m_bValue = bValue;
	m_clsList.push_back( pclsNew );

	return true;
}

/**
 * @ingroup JsonParser
 * @brief JSON �迭�� Element ���� �߰��Ѵ�.
 * @param pclsType Element ��
 * @returns JSON �迭�� Element ���� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CJsonArray::InsertData( CJsonType * pclsType )
{
	CJsonType * pclsNew = pclsType->Copy();
	if( pclsNew == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	m_clsList.push_back( pclsNew );

	return true;
}