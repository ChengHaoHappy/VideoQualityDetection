/******************************************************************************
*
*  Copyright (C), 2001-2005, Huawei Tech. Co., Ltd.
*
*******************************************************************************
*  File Name     : url_code.h
*  Version       : Initial Draft
*  Author        : sst
*  Created       : 2021/9/6
*  Last Modified :
*  Description   : url_code.cpp header file
*  Function List :
*
*
*  History:
*
*       1.  Date         : 2021/9/6
*           Author       : sst
*           Modification : Created file
*
******************************************************************************/
#ifndef __URL_CODE_H__
#define __URL_CODE_H__


/*==============================================*
 *      include header files                    *
 *----------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <string>
using namespace std;
/*==============================================*
 *      constants or macros define              *
 *----------------------------------------------*/


 /*==============================================*
  *      project-wide global variables           *
  *----------------------------------------------*/



  /*==============================================*
   *      routines' or functions' implementations *
   *----------------------------------------------*/
extern string UrlDecode(const string& str, string& dst);
extern string UrlEncode(const string& str, string& dst);

#endif /* __URL_CODE_H__ */
