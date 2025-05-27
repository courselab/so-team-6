/*
 *    SPDX-FileCopyrightText: 2025 João Sena <joaopedromsena@gmail.com>
 *   
 *    SPDX-License-Identifier: GPL-3.0-or-later
 *
 *  This file is a derivative of SYSeg (https://gitlab.com/monaco/syseg)
 *  and includes modifications made by the following author(s):
 *  João Sena <joaopedromsena@gmail.com>
 */

#ifndef E8_H
#define E8_H

void __attribute__((fastcall, naked)) printf(const char *);

const unsigned int __attribute__((fastcall, naked)) wtii();

#endif	
