# 1 "<built-in>"
# 1 "driver.c"
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* stdio.h expanded by -frewrite-includes */
#include <stdio.h>
#else /* stdio.h expanded by -frewrite-includes */
# 1 "driver.c"
# 1 "/usr/include/stdio.h" 1 3 4
/* Define ISO C stdio on top of C++ iostreams.
   Copyright (C) 1991-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

/*
 *	ISO C99 Standard: 7.19 Input/output	<stdio.h>
 */

#ifndef _STDIO_H
#define _STDIO_H	1

#define __GLIBC_INTERNAL_STARTING_HEADER_IMPLEMENTATION
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* libc-header-start.h expanded by -frewrite-includes */
#include <bits/libc-header-start.h>
#else /* libc-header-start.h expanded by -frewrite-includes */
# 27 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 1 3 4
/* Handle feature test macros at the start of a header.
   Copyright (C) 2016-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

/* This header is internal to glibc and should not be included outside
   of glibc headers.  Headers including it must define
   __GLIBC_INTERNAL_STARTING_HEADER_IMPLEMENTATION first.  This header
   cannot have multiple include guards because ISO C feature test
   macros depend on the definition of the macro when an affected
   header is included, not when the first system header is
   included.  */

#ifndef __GLIBC_INTERNAL_STARTING_HEADER_IMPLEMENTATION
# error "Never include <bits/libc-header-start.h> directly."
#endif
# 30 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4

#undef __GLIBC_INTERNAL_STARTING_HEADER_IMPLEMENTATION

#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* features.h expanded by -frewrite-includes */
#include <features.h>
#else /* features.h expanded by -frewrite-includes */
# 33 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# 1 "/usr/include/features.h" 1 3 4
/* Copyright (C) 1991-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#ifndef	_FEATURES_H
#define	_FEATURES_H	1

/* These are defined by the user (or the compiler)
   to specify the desired environment:

   __STRICT_ANSI__	ISO Standard C.
   _ISOC99_SOURCE	Extensions to ISO C89 from ISO C99.
   _ISOC11_SOURCE	Extensions to ISO C99 from ISO C11.
   _ISOC2X_SOURCE	Extensions to ISO C99 from ISO C2X.
   __STDC_WANT_LIB_EXT2__
			Extensions to ISO C99 from TR 27431-2:2010.
   __STDC_WANT_IEC_60559_BFP_EXT__
			Extensions to ISO C11 from TS 18661-1:2014.
   __STDC_WANT_IEC_60559_FUNCS_EXT__
			Extensions to ISO C11 from TS 18661-4:2015.
   __STDC_WANT_IEC_60559_TYPES_EXT__
			Extensions to ISO C11 from TS 18661-3:2015.

   _POSIX_SOURCE	IEEE Std 1003.1.
   _POSIX_C_SOURCE	If ==1, like _POSIX_SOURCE; if >=2 add IEEE Std 1003.2;
			if >=199309L, add IEEE Std 1003.1b-1993;
			if >=199506L, add IEEE Std 1003.1c-1995;
			if >=200112L, all of IEEE 1003.1-2004
			if >=200809L, all of IEEE 1003.1-2008
   _XOPEN_SOURCE	Includes POSIX and XPG things.  Set to 500 if
			Single Unix conformance is wanted, to 600 for the
			sixth revision, to 700 for the seventh revision.
   _XOPEN_SOURCE_EXTENDED XPG things and X/Open Unix extensions.
   _LARGEFILE_SOURCE	Some more functions for correct standard I/O.
   _LARGEFILE64_SOURCE	Additional functionality from LFS for large files.
   _FILE_OFFSET_BITS=N	Select default filesystem interface.
   _ATFILE_SOURCE	Additional *at interfaces.
   _GNU_SOURCE		All of the above, plus GNU extensions.
   _DEFAULT_SOURCE	The default set of features (taking precedence over
			__STRICT_ANSI__).

   _FORTIFY_SOURCE	Add security hardening to many library functions.
			Set to 1 or 2; 2 performs stricter checks than 1.

   _REENTRANT, _THREAD_SAFE
			Obsolete; equivalent to _POSIX_C_SOURCE=199506L.

   The `-ansi' switch to the GNU C compiler, and standards conformance
   options such as `-std=c99', define __STRICT_ANSI__.  If none of
   these are defined, or if _DEFAULT_SOURCE is defined, the default is
   to have _POSIX_SOURCE set to one and _POSIX_C_SOURCE set to
   200809L, as well as enabling miscellaneous functions from BSD and
   SVID.  If more than one of these are defined, they accumulate.  For
   example __STRICT_ANSI__, _POSIX_SOURCE and _POSIX_C_SOURCE together
   give you ISO C, 1003.1, and 1003.2, but nothing else.

   These are defined by this file and are used by the
   header files to decide what to declare or define:

   __GLIBC_USE (F)	Define things from feature set F.  This is defined
			to 1 or 0; the subsequent macros are either defined
			or undefined, and those tests should be moved to
			__GLIBC_USE.
   __USE_ISOC11		Define ISO C11 things.
   __USE_ISOC99		Define ISO C99 things.
   __USE_ISOC95		Define ISO C90 AMD1 (C95) things.
   __USE_ISOCXX11	Define ISO C++11 things.
   __USE_POSIX		Define IEEE Std 1003.1 things.
   __USE_POSIX2		Define IEEE Std 1003.2 things.
   __USE_POSIX199309	Define IEEE Std 1003.1, and .1b things.
   __USE_POSIX199506	Define IEEE Std 1003.1, .1b, .1c and .1i things.
   __USE_XOPEN		Define XPG things.
   __USE_XOPEN_EXTENDED	Define X/Open Unix things.
   __USE_UNIX98		Define Single Unix V2 things.
   __USE_XOPEN2K        Define XPG6 things.
   __USE_XOPEN2KXSI     Define XPG6 XSI things.
   __USE_XOPEN2K8       Define XPG7 things.
   __USE_XOPEN2K8XSI    Define XPG7 XSI things.
   __USE_LARGEFILE	Define correct standard I/O things.
   __USE_LARGEFILE64	Define LFS things with separate names.
   __USE_FILE_OFFSET64	Define 64bit interface as default.
   __USE_MISC		Define things from 4.3BSD or System V Unix.
   __USE_ATFILE		Define *at interfaces and AT_* constants for them.
   __USE_GNU		Define GNU extensions.
   __USE_FORTIFY_LEVEL	Additional security measures used, according to level.

   The macros `__GNU_LIBRARY__', `__GLIBC__', and `__GLIBC_MINOR__' are
   defined by this file unconditionally.  `__GNU_LIBRARY__' is provided
   only for compatibility.  All new code should use the other symbols
   to test for features.

   All macros listed above as possibly being defined by this file are
   explicitly undefined if they are not explicitly defined.
   Feature-test macros that are not defined by the user or compiler
   but are implied by the other feature-test macros defined (or by the
   lack of any definitions) are defined by the file.

   ISO C feature test macros depend on the definition of the macro
   when an affected header is included, not when the first system
   header is included, and so they are handled in
   <bits/libc-header-start.h>, which does not have a multiple include
   guard.  Feature test macros that can be handled from the first
   system header included are handled here.  */


/* Undefine everything, so we get a clean slate.  */
#undef	__USE_ISOC11
#undef	__USE_ISOC99
#undef	__USE_ISOC95
#undef	__USE_ISOCXX11
#undef	__USE_POSIX
#undef	__USE_POSIX2
#undef	__USE_POSIX199309
#undef	__USE_POSIX199506
#undef	__USE_XOPEN
#undef	__USE_XOPEN_EXTENDED
#undef	__USE_UNIX98
#undef	__USE_XOPEN2K
#undef	__USE_XOPEN2KXSI
#undef	__USE_XOPEN2K8
#undef	__USE_XOPEN2K8XSI
#undef	__USE_LARGEFILE
#undef	__USE_LARGEFILE64
#undef	__USE_FILE_OFFSET64
#undef	__USE_MISC
#undef	__USE_ATFILE
#undef	__USE_GNU
#undef	__USE_FORTIFY_LEVEL
#undef	__KERNEL_STRICT_NAMES
#undef	__GLIBC_USE_ISOC2X
#undef	__GLIBC_USE_DEPRECATED_GETS
#undef	__GLIBC_USE_DEPRECATED_SCANF

/* Suppress kernel-name space pollution unless user expressedly asks
   for it.  */
#ifndef _LOOSE_KERNEL_NAMES
# define __KERNEL_STRICT_NAMES
#endif
# 152 "/usr/include/features.h" 3 4

/* Convenience macro to test the version of gcc.
   Use like this:
   #if __GNUC_PREREQ (2,8)
   ... code requiring gcc 2.8 or later ...
   #endif
   Note: only works for GCC 2.0 and later, because __GNUC_MINOR__ was
   added in 2.0.  */
#if 0 /* disabled by -frewrite-includes */
#if defined __GNUC__ && defined __GNUC_MINOR__
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 161 "/usr/include/features.h" 3 4
# define __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#else
# 164 "/usr/include/features.h" 3 4
# define __GNUC_PREREQ(maj, min) 0
#endif
# 166 "/usr/include/features.h" 3 4

/* Similarly for clang.  Features added to GCC after version 4.2 may
   or may not also be available in clang, and clang's definitions of
   __GNUC(_MINOR)__ are fixed at 4 and 2 respectively.  Not all such
   features can be queried via __has_extension/__has_feature.  */
#if 0 /* disabled by -frewrite-includes */
#if defined __clang_major__ && defined __clang_minor__
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 172 "/usr/include/features.h" 3 4
# define __glibc_clang_prereq(maj, min) \
  ((__clang_major__ << 16) + __clang_minor__ >= ((maj) << 16) + (min))
#else
# 175 "/usr/include/features.h" 3 4
# define __glibc_clang_prereq(maj, min) 0
#endif
# 177 "/usr/include/features.h" 3 4

/* Whether to use feature set F.  */
#define __GLIBC_USE(F)	__GLIBC_USE_ ## F

/* _BSD_SOURCE and _SVID_SOURCE are deprecated aliases for
   _DEFAULT_SOURCE.  If _DEFAULT_SOURCE is present we do not
   issue a warning; the expectation is that the source is being
   transitioned to use the new macro.  */
#if 0 /* disabled by -frewrite-includes */
#if (defined _BSD_SOURCE || defined _SVID_SOURCE) \
    && !defined _DEFAULT_SOURCE
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 187 "/usr/include/features.h" 3 4
# warning "_BSD_SOURCE and _SVID_SOURCE are deprecated, use _DEFAULT_SOURCE"
# undef  _DEFAULT_SOURCE
# define _DEFAULT_SOURCE	1
#endif
# 191 "/usr/include/features.h" 3 4

/* If _GNU_SOURCE was defined by the user, turn on all the other features.  */
#ifdef _GNU_SOURCE
# undef  _ISOC95_SOURCE
# define _ISOC95_SOURCE	1
# undef  _ISOC99_SOURCE
# define _ISOC99_SOURCE	1
# undef  _ISOC11_SOURCE
# define _ISOC11_SOURCE	1
# undef  _ISOC2X_SOURCE
# define _ISOC2X_SOURCE	1
# undef  _POSIX_SOURCE
# define _POSIX_SOURCE	1
# undef  _POSIX_C_SOURCE
# define _POSIX_C_SOURCE	200809L
# undef  _XOPEN_SOURCE
# define _XOPEN_SOURCE	700
# undef  _XOPEN_SOURCE_EXTENDED
# define _XOPEN_SOURCE_EXTENDED	1
# undef	 _LARGEFILE64_SOURCE
# define _LARGEFILE64_SOURCE	1
# undef  _DEFAULT_SOURCE
# define _DEFAULT_SOURCE	1
# undef  _ATFILE_SOURCE
# define _ATFILE_SOURCE	1
#endif
# 217 "/usr/include/features.h" 3 4

/* If nothing (other than _GNU_SOURCE and _DEFAULT_SOURCE) is defined,
   define _DEFAULT_SOURCE.  */
#if 0 /* disabled by -frewrite-includes */
#if (defined _DEFAULT_SOURCE					\
     || (!defined __STRICT_ANSI__				\
	 && !defined _ISOC99_SOURCE && !defined _ISOC11_SOURCE	\
	 && !defined _ISOC2X_SOURCE				\
	 && !defined _POSIX_SOURCE && !defined _POSIX_C_SOURCE	\
	 && !defined _XOPEN_SOURCE))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 226 "/usr/include/features.h" 3 4
# undef  _DEFAULT_SOURCE
# define _DEFAULT_SOURCE	1
#endif
# 229 "/usr/include/features.h" 3 4

/* This is to enable the ISO C2X extension.  */
#if 0 /* disabled by -frewrite-includes */
#if (defined _ISOC2X_SOURCE \
     || (defined __STDC_VERSION__ && __STDC_VERSION__ > 201710L))
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 233 "/usr/include/features.h" 3 4
# define __GLIBC_USE_ISOC2X	1
#else
# 235 "/usr/include/features.h" 3 4
# define __GLIBC_USE_ISOC2X	0
#endif
# 237 "/usr/include/features.h" 3 4

/* This is to enable the ISO C11 extension.  */
#if 0 /* disabled by -frewrite-includes */
#if (defined _ISOC11_SOURCE || defined _ISOC2X_SOURCE \
     || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 201112L))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 241 "/usr/include/features.h" 3 4
# define __USE_ISOC11	1
#endif
# 243 "/usr/include/features.h" 3 4

/* This is to enable the ISO C99 extension.  */
#if 0 /* disabled by -frewrite-includes */
#if (defined _ISOC99_SOURCE || defined _ISOC11_SOURCE			\
     || defined _ISOC2X_SOURCE						\
     || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 248 "/usr/include/features.h" 3 4
# define __USE_ISOC99	1
#endif
# 250 "/usr/include/features.h" 3 4

/* This is to enable the ISO C90 Amendment 1:1995 extension.  */
#if 0 /* disabled by -frewrite-includes */
#if (defined _ISOC99_SOURCE || defined _ISOC11_SOURCE			\
     || defined _ISOC2X_SOURCE						\
     || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 199409L))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 255 "/usr/include/features.h" 3 4
# define __USE_ISOC95	1
#endif
# 257 "/usr/include/features.h" 3 4

#ifdef __cplusplus
/* This is to enable compatibility for ISO C++17.  */
#if 0 /* disabled by -frewrite-includes */
# if __cplusplus >= 201703L
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 261 "/usr/include/features.h" 3 4
#  define __USE_ISOC11	1
# endif
# 263 "/usr/include/features.h" 3 4
/* This is to enable compatibility for ISO C++11.
   Check the temporary macro for now, too.  */
#if 0 /* disabled by -frewrite-includes */
# if __cplusplus >= 201103L || defined __GXX_EXPERIMENTAL_CXX0X__
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 266 "/usr/include/features.h" 3 4
#  define __USE_ISOCXX11	1
#  define __USE_ISOC99	1
# endif
# 269 "/usr/include/features.h" 3 4
#endif
# 270 "/usr/include/features.h" 3 4

/* If none of the ANSI/POSIX macros are defined, or if _DEFAULT_SOURCE
   is defined, use POSIX.1-2008 (or another version depending on
   _XOPEN_SOURCE).  */
#ifdef _DEFAULT_SOURCE
#if 0 /* disabled by -frewrite-includes */
# if !defined _POSIX_SOURCE && !defined _POSIX_C_SOURCE
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 276 "/usr/include/features.h" 3 4
#  define __USE_POSIX_IMPLICITLY	1
# endif
# 278 "/usr/include/features.h" 3 4
# undef  _POSIX_SOURCE
# define _POSIX_SOURCE	1
# undef  _POSIX_C_SOURCE
# define _POSIX_C_SOURCE	200809L
#endif
# 283 "/usr/include/features.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if ((!defined __STRICT_ANSI__					\
      || (defined _XOPEN_SOURCE && (_XOPEN_SOURCE - 0) >= 500))	\
     && !defined _POSIX_SOURCE && !defined _POSIX_C_SOURCE)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 287 "/usr/include/features.h" 3 4
# define _POSIX_SOURCE	1
#if 0 /* disabled by -frewrite-includes */
# if defined _XOPEN_SOURCE && (_XOPEN_SOURCE - 0) < 500
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 289 "/usr/include/features.h" 3 4
#  define _POSIX_C_SOURCE	2
#if 0 /* disabled by -frewrite-includes */
#if 0
# elif defined _XOPEN_SOURCE && (_XOPEN_SOURCE - 0) < 600
#endif
#endif /* disabled by -frewrite-includes */
#elif 0 /* evaluated by -frewrite-includes */
# 291 "/usr/include/features.h" 3 4
#  define _POSIX_C_SOURCE	199506L
#if 0 /* disabled by -frewrite-includes */
#if 0
# elif defined _XOPEN_SOURCE && (_XOPEN_SOURCE - 0) < 700
#endif
#endif /* disabled by -frewrite-includes */
#elif 0 /* evaluated by -frewrite-includes */
# 293 "/usr/include/features.h" 3 4
#  define _POSIX_C_SOURCE	200112L
# else
# 295 "/usr/include/features.h" 3 4
#  define _POSIX_C_SOURCE	200809L
# endif
# 297 "/usr/include/features.h" 3 4
# define __USE_POSIX_IMPLICITLY	1
#endif
# 299 "/usr/include/features.h" 3 4

/* Some C libraries once required _REENTRANT and/or _THREAD_SAFE to be
   defined in all multithreaded code.  GNU libc has not required this
   for many years.  We now treat them as compatibility synonyms for
   _POSIX_C_SOURCE=199506L, which is the earliest level of POSIX with
   comprehensive support for multithreaded code.  Using them never
   lowers the selected level of POSIX conformance, only raises it.  */
#if 0 /* disabled by -frewrite-includes */
#if ((!defined _POSIX_C_SOURCE || (_POSIX_C_SOURCE - 0) < 199506L) \
     && (defined _REENTRANT || defined _THREAD_SAFE))
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 308 "/usr/include/features.h" 3 4
# define _POSIX_SOURCE   1
# undef  _POSIX_C_SOURCE
# define _POSIX_C_SOURCE 199506L
#endif
# 312 "/usr/include/features.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if (defined _POSIX_SOURCE					\
     || (defined _POSIX_C_SOURCE && _POSIX_C_SOURCE >= 1)	\
     || defined _XOPEN_SOURCE)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 316 "/usr/include/features.h" 3 4
# define __USE_POSIX	1
#endif
# 318 "/usr/include/features.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined _POSIX_C_SOURCE && _POSIX_C_SOURCE >= 2 || defined _XOPEN_SOURCE
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 320 "/usr/include/features.h" 3 4
# define __USE_POSIX2	1
#endif
# 322 "/usr/include/features.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined _POSIX_C_SOURCE && (_POSIX_C_SOURCE - 0) >= 199309L
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 324 "/usr/include/features.h" 3 4
# define __USE_POSIX199309	1
#endif
# 326 "/usr/include/features.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined _POSIX_C_SOURCE && (_POSIX_C_SOURCE - 0) >= 199506L
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 328 "/usr/include/features.h" 3 4
# define __USE_POSIX199506	1
#endif
# 330 "/usr/include/features.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined _POSIX_C_SOURCE && (_POSIX_C_SOURCE - 0) >= 200112L
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 332 "/usr/include/features.h" 3 4
# define __USE_XOPEN2K		1
# undef __USE_ISOC95
# define __USE_ISOC95		1
# undef __USE_ISOC99
# define __USE_ISOC99		1
#endif
# 338 "/usr/include/features.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined _POSIX_C_SOURCE && (_POSIX_C_SOURCE - 0) >= 200809L
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 340 "/usr/include/features.h" 3 4
# define __USE_XOPEN2K8		1
# undef  _ATFILE_SOURCE
# define _ATFILE_SOURCE	1
#endif
# 344 "/usr/include/features.h" 3 4

#ifdef	_XOPEN_SOURCE
# define __USE_XOPEN	1
#if 0 /* disabled by -frewrite-includes */
# if (_XOPEN_SOURCE - 0) >= 500
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 348 "/usr/include/features.h" 3 4
#  define __USE_XOPEN_EXTENDED	1
#  define __USE_UNIX98	1
#  undef _LARGEFILE_SOURCE
#  define _LARGEFILE_SOURCE	1
#if 0 /* disabled by -frewrite-includes */
#  if (_XOPEN_SOURCE - 0) >= 600
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 353 "/usr/include/features.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#   if (_XOPEN_SOURCE - 0) >= 700
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 354 "/usr/include/features.h" 3 4
#    define __USE_XOPEN2K8	1
#    define __USE_XOPEN2K8XSI	1
#   endif
# 357 "/usr/include/features.h" 3 4
#   define __USE_XOPEN2K	1
#   define __USE_XOPEN2KXSI	1
#   undef __USE_ISOC95
#   define __USE_ISOC95		1
#   undef __USE_ISOC99
#   define __USE_ISOC99		1
#  endif
# 364 "/usr/include/features.h" 3 4
# else
# 365 "/usr/include/features.h" 3 4
#  ifdef _XOPEN_SOURCE_EXTENDED
#   define __USE_XOPEN_EXTENDED	1
#  endif
# 368 "/usr/include/features.h" 3 4
# endif
# 369 "/usr/include/features.h" 3 4
#endif
# 370 "/usr/include/features.h" 3 4

#ifdef _LARGEFILE_SOURCE
# define __USE_LARGEFILE	1
#endif
# 374 "/usr/include/features.h" 3 4

#ifdef _LARGEFILE64_SOURCE
# define __USE_LARGEFILE64	1
#endif
# 378 "/usr/include/features.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined _FILE_OFFSET_BITS && _FILE_OFFSET_BITS == 64
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 380 "/usr/include/features.h" 3 4
# define __USE_FILE_OFFSET64	1
#endif
# 382 "/usr/include/features.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined _DEFAULT_SOURCE
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 384 "/usr/include/features.h" 3 4
# define __USE_MISC	1
#endif
# 386 "/usr/include/features.h" 3 4

#ifdef	_ATFILE_SOURCE
# define __USE_ATFILE	1
#endif
# 390 "/usr/include/features.h" 3 4

#ifdef	_GNU_SOURCE
# define __USE_GNU	1
#endif
# 394 "/usr/include/features.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined _FORTIFY_SOURCE && _FORTIFY_SOURCE > 0 \
    && __GNUC_PREREQ (4, 1) && defined __OPTIMIZE__ && __OPTIMIZE__ > 0
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 397 "/usr/include/features.h" 3 4
#if 0 /* disabled by -frewrite-includes */
# if _FORTIFY_SOURCE > 1
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 398 "/usr/include/features.h" 3 4
#  define __USE_FORTIFY_LEVEL 2
# else
# 400 "/usr/include/features.h" 3 4
#  define __USE_FORTIFY_LEVEL 1
# endif
# 402 "/usr/include/features.h" 3 4
#else
# 403 "/usr/include/features.h" 3 4
# define __USE_FORTIFY_LEVEL 0
#endif
# 405 "/usr/include/features.h" 3 4

/* The function 'gets' existed in C89, but is impossible to use
   safely.  It has been removed from ISO C11 and ISO C++14.  Note: for
   compatibility with various implementations of <cstdio>, this test
   must consider only the value of __cplusplus when compiling C++.  */
#if 0 /* disabled by -frewrite-includes */
#if defined __cplusplus ? __cplusplus >= 201402L : defined __USE_ISOC11
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 411 "/usr/include/features.h" 3 4
# define __GLIBC_USE_DEPRECATED_GETS 0
#else
# 413 "/usr/include/features.h" 3 4
# define __GLIBC_USE_DEPRECATED_GETS 1
#endif
# 415 "/usr/include/features.h" 3 4

/* GNU formerly extended the scanf functions with modified format
   specifiers %as, %aS, and %a[...] that allocate a buffer for the
   input using malloc.  This extension conflicts with ISO C99, which
   defines %a as a standalone format specifier that reads a floating-
   point number; moreover, POSIX.1-2008 provides the same feature
   using the modifier letter 'm' instead (%ms, %mS, %m[...]).

   We now follow C99 unless GNU extensions are active and the compiler
   is specifically in C89 or C++98 mode (strict or not).  For
   instance, with GCC, -std=gnu11 will have C99-compliant scanf with
   or without -D_GNU_SOURCE, but -std=c89 -D_GNU_SOURCE will have the
   old extension.  */
#if 0 /* disabled by -frewrite-includes */
#if (defined __USE_GNU							\
     && (defined __cplusplus						\
	 ? (__cplusplus < 201103L && !defined __GXX_EXPERIMENTAL_CXX0X__) \
	 : (!defined __STDC_VERSION__ || __STDC_VERSION__ < 199901L)))
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 432 "/usr/include/features.h" 3 4
# define __GLIBC_USE_DEPRECATED_SCANF 1
#else
# 434 "/usr/include/features.h" 3 4
# define __GLIBC_USE_DEPRECATED_SCANF 0
#endif
# 436 "/usr/include/features.h" 3 4

/* Get definitions of __STDC_* predefined macros, if the compiler has
   not preincluded this header automatically.  */
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* stdc-predef.h expanded by -frewrite-includes */
#include <stdc-predef.h>
#else /* stdc-predef.h expanded by -frewrite-includes */
# 439 "/usr/include/features.h" 3 4
# 1 "/usr/include/stdc-predef.h" 1 3 4
/* Copyright (C) 1991-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#ifndef	_STDC_PREDEF_H
#define	_STDC_PREDEF_H	1

/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */

/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */

#ifdef __GCC_IEC_559
#if 0 /* disabled by -frewrite-includes */
# if __GCC_IEC_559 > 0
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 38 "/usr/include/stdc-predef.h" 3 4
#  define __STDC_IEC_559__		1
# endif
# 40 "/usr/include/stdc-predef.h" 3 4
#else
# 41 "/usr/include/stdc-predef.h" 3 4
# define __STDC_IEC_559__		1
#endif
# 43 "/usr/include/stdc-predef.h" 3 4

#ifdef __GCC_IEC_559_COMPLEX
#if 0 /* disabled by -frewrite-includes */
# if __GCC_IEC_559_COMPLEX > 0
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 46 "/usr/include/stdc-predef.h" 3 4
#  define __STDC_IEC_559_COMPLEX__	1
# endif
# 48 "/usr/include/stdc-predef.h" 3 4
#else
# 49 "/usr/include/stdc-predef.h" 3 4
# define __STDC_IEC_559_COMPLEX__	1
#endif
# 51 "/usr/include/stdc-predef.h" 3 4

/* wchar_t uses Unicode 10.0.0.  Version 10.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2017, fifth edition, plus
   the following additions from Amendment 1 to the fifth edition:
   - 56 emoji characters
   - 285 hentaigana
   - 3 additional Zanabazar Square characters */
#define __STDC_ISO_10646__		201706L

#endif
# 61 "/usr/include/stdc-predef.h" 3 4
#endif /* stdc-predef.h expanded by -frewrite-includes */
# 440 "/usr/include/features.h" 2 3 4

/* This macro indicates that the installed library is the GNU C Library.
   For historic reasons the value now is 6 and this will stay from now
   on.  The use of this variable is deprecated.  Use __GLIBC__ and
   __GLIBC_MINOR__ now (see below) when you want to test for a specific
   GNU C library version and use the values in <gnu/lib-names.h> to get
   the sonames of the shared libraries.  */
#undef  __GNU_LIBRARY__
#define __GNU_LIBRARY__ 6

/* Major and minor version number of the GNU C library package.  Use
   these macros to test for features in specific releases.  */
#define	__GLIBC__	2
#define	__GLIBC_MINOR__	31

#define __GLIBC_PREREQ(maj, min) \
	((__GLIBC__ << 16) + __GLIBC_MINOR__ >= ((maj) << 16) + (min))

/* This is here only because every header file already includes this one.  */
#ifndef __ASSEMBLER__
# ifndef _SYS_CDEFS_H
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* cdefs.h expanded by -frewrite-includes */
#  include <sys/cdefs.h>
#else /* cdefs.h expanded by -frewrite-includes */
# 461 "/usr/include/features.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 1 3 4
/* Copyright (C) 1992-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#ifndef	_SYS_CDEFS_H
#define	_SYS_CDEFS_H	1

/* We are almost always included from features.h. */
#ifndef _FEATURES_H
#if 0 /* expanded by -frewrite-includes */
# include <features.h>
#endif /* expanded by -frewrite-includes */
# 23 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# 24 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#endif
# 25 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* The GNU libc does not support any K&R compilers or the traditional mode
   of ISO C compilers anymore.  Check for some of the combinations not
   anymore supported.  */
#if 0 /* disabled by -frewrite-includes */
#if defined __GNUC__ && !defined __STDC__
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 30 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# error "You need a ISO C conforming compiler to use the glibc headers"
#endif
# 32 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* Some user header file might have defined this before.  */
#undef	__P
#undef	__PMT

#ifdef __GNUC__

/* All functions, except those with callbacks or those that
   synchronize memory, are leaf functions.  */
#if 0 /* disabled by -frewrite-includes */
# if __GNUC_PREREQ (4, 6) && !defined _LIBC
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 42 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __LEAF , __leaf__
#  define __LEAF_ATTR __attribute__ ((__leaf__))
# else
# 45 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __LEAF
#  define __LEAF_ATTR
# endif
# 48 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* GCC can always grok prototypes.  For C++ programs we add throw()
   to help it optimize the function calls.  But this works only with
   gcc 2.8.x and egcs.  For gcc 3.2 and up we even mark C functions
   as non-throwing using a function attribute since programs can use
   the -fexceptions options for C code as well.  */
#if 0 /* disabled by -frewrite-includes */
# if !defined __cplusplus && __GNUC_PREREQ (3, 3)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 55 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __THROW	__attribute__ ((__nothrow__ __LEAF))
#  define __THROWNL	__attribute__ ((__nothrow__))
#  define __NTH(fct)	__attribute__ ((__nothrow__ __LEAF)) fct
#  define __NTHNL(fct)  __attribute__ ((__nothrow__)) fct
# else
# 60 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#  if defined __cplusplus && __GNUC_PREREQ (2,8)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 61 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#   define __THROW	throw ()
#   define __THROWNL	throw ()
#   define __NTH(fct)	__LEAF_ATTR fct throw ()
#   define __NTHNL(fct) fct throw ()
#  else
# 66 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#   define __THROW
#   define __THROWNL
#   define __NTH(fct)	fct
#   define __NTHNL(fct) fct
#  endif
# 71 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# endif
# 72 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#else	/* Not GCC.  */
# 74 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if (defined __cplusplus						\
      || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L))
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 77 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __inline	inline
# else
# 79 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __inline		/* No inline functions.  */
# endif
# 81 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

# define __THROW
# define __THROWNL
# define __NTH(fct)	fct

#endif	/* GCC.  */
# 87 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* Compilers that are not clang may object to
       #if defined __clang__ && __has_extension(...)
   even though they do not need to evaluate the right-hand side of the &&.  */
#if 0 /* disabled by -frewrite-includes */
#if defined __clang__ && defined __has_extension
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 92 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __glibc_clang_has_extension(ext) __has_extension (ext)
#else
# 94 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __glibc_clang_has_extension(ext) 0
#endif
# 96 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* These two macros are not used in glibc anymore.  They are kept here
   only because some other projects expect the macros to be defined.  */
#define __P(args)	args
#define __PMT(args)	args

/* For these things, GCC behaves the ANSI way normally,
   and the non-ANSI way under -traditional.  */

#define __CONCAT(x,y)	x ## y
#define __STRING(x)	#x

/* This is not a typedef so `const __ptr_t' does the right thing.  */
#define __ptr_t void *


/* C++ needs to know that types and declarations are C, not C++.  */
#ifdef	__cplusplus
# define __BEGIN_DECLS	extern "C" {
# define __END_DECLS	}
#else
# 117 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __BEGIN_DECLS
# define __END_DECLS
#endif
# 120 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4


/* Fortify support.  */
#define __bos(ptr) __builtin_object_size (ptr, __USE_FORTIFY_LEVEL > 1)
#define __bos0(ptr) __builtin_object_size (ptr, 0)

#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (4,3)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 127 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __warndecl(name, msg) \
  extern void name (void) __attribute__((__warning__ (msg)))
# define __warnattr(msg) __attribute__((__warning__ (msg)))
# define __errordecl(name, msg) \
  extern void name (void) __attribute__((__error__ (msg)))
#else
# 133 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __warndecl(name, msg) extern void name (void)
# define __warnattr(msg)
# define __errordecl(name, msg) extern void name (void)
#endif
# 137 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* Support for flexible arrays.
   Headers that should use flexible arrays only if they're "real"
   (e.g. only if they won't affect sizeof()) should test
   #if __glibc_c99_flexarr_available.  */
#if 0 /* disabled by -frewrite-includes */
#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 143 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __flexarr	[]
# define __glibc_c99_flexarr_available 1
#if 0 /* disabled by -frewrite-includes */
#if 0
#elif __GNUC_PREREQ (2,97)
#endif
#endif /* disabled by -frewrite-includes */
#elif 0 /* evaluated by -frewrite-includes */
# 146 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
/* GCC 2.97 supports C99 flexible array members as an extension,
   even when in C89 mode or compiling C++ (any version).  */
# define __flexarr	[]
# define __glibc_c99_flexarr_available 1
#if 0 /* disabled by -frewrite-includes */
#if 0
#elif defined __GNUC__
#endif
#endif /* disabled by -frewrite-includes */
#elif 0 /* evaluated by -frewrite-includes */
# 151 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
/* Pre-2.97 GCC did not support C99 flexible arrays but did have
   an equivalent extension with slightly different notation.  */
# define __flexarr	[0]
# define __glibc_c99_flexarr_available 1
#else
# 156 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
/* Some other non-C99 compiler.  Approximate with [1].  */
# define __flexarr	[1]
# define __glibc_c99_flexarr_available 0
#endif
# 160 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4


/* __asm__ ("xyz") is used throughout the headers to rename functions
   at the assembly language level.  This is wrapped by the __REDIRECT
   macro, in order to support compilers that can do this some other
   way.  When compilers don't support asm-names at all, we have to do
   preprocessor tricks instead (which don't have exactly the right
   semantics, but it's the best we can do).

   Example:
   int __REDIRECT(setpgrp, (__pid_t pid, __pid_t pgrp), setpgid); */

#if 0 /* disabled by -frewrite-includes */
#if defined __GNUC__ && __GNUC__ >= 2
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 173 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

# define __REDIRECT(name, proto, alias) name proto __asm__ (__ASMNAME (#alias))
# ifdef __cplusplus
#  define __REDIRECT_NTH(name, proto, alias) \
     name proto __THROW __asm__ (__ASMNAME (#alias))
#  define __REDIRECT_NTHNL(name, proto, alias) \
     name proto __THROWNL __asm__ (__ASMNAME (#alias))
# else
# 181 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __REDIRECT_NTH(name, proto, alias) \
     name proto __asm__ (__ASMNAME (#alias)) __THROW
#  define __REDIRECT_NTHNL(name, proto, alias) \
     name proto __asm__ (__ASMNAME (#alias)) __THROWNL
# endif
# 186 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __ASMNAME(cname)  __ASMNAME2 (__USER_LABEL_PREFIX__, cname)
# define __ASMNAME2(prefix, cname) __STRING (prefix) cname

/*
#elif __SOME_OTHER_COMPILER__

# define __REDIRECT(name, proto, alias) name proto; \
	_Pragma("let " #name " = " #alias)
*/
#endif
# 196 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* GCC has various useful declarations that can be made with the
   `__attribute__' syntax.  All of the ways we use this do fine if
   they are omitted for compilers that don't understand it. */
#if 0 /* disabled by -frewrite-includes */
#if !defined __GNUC__ || __GNUC__ < 2
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 201 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute__(xyz)	/* Ignore */
#endif
# 203 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* At some point during the gcc 2.96 development the `malloc' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (2,96)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 208 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_malloc__ __attribute__ ((__malloc__))
#else
# 210 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_malloc__ /* Ignore */
#endif
# 212 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* Tell the compiler which arguments to an allocation function
   indicate the size of the allocation.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (4, 3)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 216 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_alloc_size__(params) \
  __attribute__ ((__alloc_size__ params))
#else
# 219 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_alloc_size__(params) /* Ignore.  */
#endif
# 221 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* At some point during the gcc 2.96 development the `pure' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (2,96)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 226 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_pure__ __attribute__ ((__pure__))
#else
# 228 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_pure__ /* Ignore */
#endif
# 230 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* This declaration tells the compiler that the value is constant.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (2,5)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 233 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_const__ __attribute__ ((__const__))
#else
# 235 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_const__ /* Ignore */
#endif
# 237 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* At some point during the gcc 3.1 development the `used' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (3,1)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 242 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_used__ __attribute__ ((__used__))
# define __attribute_noinline__ __attribute__ ((__noinline__))
#else
# 245 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_used__ __attribute__ ((__unused__))
# define __attribute_noinline__ /* Ignore */
#endif
# 248 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* Since version 3.2, gcc allows marking deprecated functions.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (3,2)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 251 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_deprecated__ __attribute__ ((__deprecated__))
#else
# 253 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_deprecated__ /* Ignore */
#endif
# 255 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* Since version 4.5, gcc also allows one to specify the message printed
   when a deprecated function is used.  clang claims to be gcc 4.2, but
   may also support this feature.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (4,5) \
    || __glibc_clang_has_extension (__attribute_deprecated_with_message__)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 261 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_deprecated_msg__(msg) \
	 __attribute__ ((__deprecated__ (msg)))
#else
# 264 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_deprecated_msg__(msg) __attribute_deprecated__
#endif
# 266 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* At some point during the gcc 2.8 development the `format_arg' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.
   If several `format_arg' attributes are given for the same function, in
   gcc-3.0 and older, all but the last one are ignored.  In newer gccs,
   all designated arguments are considered.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (2,8)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 274 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_format_arg__(x) __attribute__ ((__format_arg__ (x)))
#else
# 276 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_format_arg__(x) /* Ignore */
#endif
# 278 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* At some point during the gcc 2.97 development the `strfmon' format
   attribute for functions was introduced.  We don't want to use it
   unconditionally (although this would be possible) since it
   generates warnings.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (2,97)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 284 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_format_strfmon__(a,b) \
  __attribute__ ((__format__ (__strfmon__, a, b)))
#else
# 287 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_format_strfmon__(a,b) /* Ignore */
#endif
# 289 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* The nonull function attribute allows to mark pointer parameters which
   must not be NULL.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (3,3)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 293 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __nonnull(params) __attribute__ ((__nonnull__ params))
#else
# 295 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __nonnull(params)
#endif
# 297 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* If fortification mode, we warn about unused results of certain
   function calls which can lead to problems.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (3,4)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 301 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_warn_unused_result__ \
   __attribute__ ((__warn_unused_result__))
#if 0 /* disabled by -frewrite-includes */
# if __USE_FORTIFY_LEVEL > 0
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 304 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __wur __attribute_warn_unused_result__
# endif
# 306 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#else
# 307 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_warn_unused_result__ /* empty */
#endif
# 309 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#ifndef __wur
# define __wur /* Ignore */
#endif
# 312 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* Forces a function to be always inlined.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (3,2)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 315 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
/* The Linux kernel defines __always_inline in stddef.h (283d7573), and
   it conflicts with this definition.  Therefore undefine it first to
   allow either header to be included first.  */
# undef __always_inline
# define __always_inline __inline __attribute__ ((__always_inline__))
#else
# 321 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# undef __always_inline
# define __always_inline __inline
#endif
# 324 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* Associate error messages with the source location of the call site rather
   than with the source location inside the function.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (4,3)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 328 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_artificial__ __attribute__ ((__artificial__))
#else
# 330 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_artificial__ /* Ignore */
#endif
# 332 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* GCC 4.3 and above with -std=c99 or -std=gnu99 implements ISO C99
   inline semantics, unless -fgnu89-inline is used.  Using __GNUC_STDC_INLINE__
   or __GNUC_GNU_INLINE is not a good enough check for gcc because gcc versions
   older than 4.3 may define these macros and still not guarantee GNU inlining
   semantics.

   clang++ identifies itself as gcc-4.2, but has support for GNU inlining
   semantics, that can be checked for by using the __GNUC_STDC_INLINE_ and
   __GNUC_GNU_INLINE__ macro definitions.  */
#if 0 /* disabled by -frewrite-includes */
#if (!defined __cplusplus || __GNUC_PREREQ (4,3) \
     || (defined __clang__ && (defined __GNUC_STDC_INLINE__ \
			       || defined __GNUC_GNU_INLINE__)))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 345 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#if 0 /* disabled by -frewrite-includes */
# if defined __GNUC_STDC_INLINE__ || defined __cplusplus
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 346 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __extern_inline extern __inline __attribute__ ((__gnu_inline__))
#  define __extern_always_inline \
  extern __always_inline __attribute__ ((__gnu_inline__))
# else
# 350 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __extern_inline extern __inline
#  define __extern_always_inline extern __always_inline
# endif
# 353 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#endif
# 354 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#ifdef __extern_always_inline
# define __fortify_function __extern_always_inline __attribute_artificial__
#endif
# 358 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* GCC 4.3 and above allow passing all anonymous arguments of an
   __extern_always_inline function to some other vararg function.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (4,3)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 362 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __va_arg_pack() __builtin_va_arg_pack ()
# define __va_arg_pack_len() __builtin_va_arg_pack_len ()
#endif
# 365 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* It is possible to compile containing GCC extensions even if GCC is
   run in pedantic mode if the uses are carefully marked using the
   `__extension__' keyword.  But this is not generally available before
   version 2.8.  */
#if 0 /* disabled by -frewrite-includes */
#if !__GNUC_PREREQ (2,8)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 371 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __extension__		/* Ignore */
#endif
# 373 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* __restrict is known in EGCS 1.2 and above. */
#if 0 /* disabled by -frewrite-includes */
#if !__GNUC_PREREQ (2,92)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 376 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#if 0 /* disabled by -frewrite-includes */
# if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 377 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __restrict	restrict
# else
# 379 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __restrict	/* Ignore */
# endif
# 381 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#endif
# 382 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* ISO C99 also allows to declare arrays as non-overlapping.  The syntax is
     array_name[restrict]
   GCC 3.1 supports this.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (3,1) && !defined __GNUG__
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 387 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __restrict_arr	__restrict
#else
# 389 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# ifdef __GNUC__
#  define __restrict_arr	/* Not supported in old GCC.  */
# else
# 392 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#  if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 393 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#   define __restrict_arr	restrict
#  else
# 395 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
/* Some other non-C99 compiler.  */
#   define __restrict_arr	/* Not supported.  */
#  endif
# 398 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# endif
# 399 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#endif
# 400 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __GNUC__ >= 3
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 402 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __glibc_unlikely(cond)	__builtin_expect ((cond), 0)
# define __glibc_likely(cond)	__builtin_expect ((cond), 1)
#else
# 405 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __glibc_unlikely(cond)	(cond)
# define __glibc_likely(cond)	(cond)
#endif
# 408 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#ifdef __has_attribute
# define __glibc_has_attribute(attr)	__has_attribute (attr)
#else
# 412 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __glibc_has_attribute(attr)	0
#endif
# 414 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if (!defined _Noreturn \
     && (defined __STDC_VERSION__ ? __STDC_VERSION__ : 0) < 201112 \
     &&  !__GNUC_PREREQ (4,7))
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 418 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#if 0 /* disabled by -frewrite-includes */
# if __GNUC_PREREQ (2,8)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 419 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define _Noreturn __attribute__ ((__noreturn__))
# else
# 421 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define _Noreturn
# endif
# 423 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#endif
# 424 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (8, 0)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 426 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
/* Describes a char array whose address can safely be passed as the first
   argument to strncpy and strncat, as the char array is not necessarily
   a NUL-terminated string.  */
# define __attribute_nonstring__ __attribute__ ((__nonstring__))
#else
# 431 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_nonstring__
#endif
# 433 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* Undefine (also defined in libc-symbols.h).  */
#undef __attribute_copy__
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (9, 0)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 437 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
/* Copies attributes from the declaration or type referenced by
   the argument.  */
# define __attribute_copy__(arg) __attribute__ ((__copy__ (arg)))
#else
# 441 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_copy__(arg)
#endif
# 443 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if (!defined _Static_assert && !defined __cplusplus \
     && (defined __STDC_VERSION__ ? __STDC_VERSION__ : 0) < 201112 \
     && (!__GNUC_PREREQ (4, 6) || defined __STRICT_ANSI__))
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 447 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define _Static_assert(expr, diagnostic) \
    extern int (*__Static_assert_function (void)) \
      [!!sizeof (struct { int __error_if_negative: (expr) ? 2 : -1; })]
#endif
# 451 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* wordsize.h expanded by -frewrite-includes */
#include <bits/wordsize.h>
#else /* wordsize.h expanded by -frewrite-includes */
# 452 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
/* Determine the wordsize from the preprocessor defines.  */

#if 0 /* disabled by -frewrite-includes */
#if defined __x86_64__ && !defined __ILP32__
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 4 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 3 4
# define __WORDSIZE	64
#else
# 6 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 3 4
# define __WORDSIZE	32
#define __WORDSIZE32_SIZE_ULONG		0
#define __WORDSIZE32_PTRDIFF_LONG	0
#endif
# 10 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 3 4

#ifdef __x86_64__
# define __WORDSIZE_TIME64_COMPAT32	1
/* Both x86-64 and x32 use the 64-bit system call interface.  */
# define __SYSCALL_WORDSIZE		64
#else
# 16 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 3 4
# define __WORDSIZE_TIME64_COMPAT32	0
#endif
# 18 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 3 4
#endif /* wordsize.h expanded by -frewrite-includes */
# 453 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 2 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* long-double.h expanded by -frewrite-includes */
#include <bits/long-double.h>
#else /* long-double.h expanded by -frewrite-includes */
# 453 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/long-double.h" 1 3 4
/* Properties of long double type.  ldbl-96 version.
   Copyright (C) 2016-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License  published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

/* long double is distinct from double, so there is nothing to
   define here.  */
#define __LONG_DOUBLE_USES_FLOAT128 0
#endif /* long-double.h expanded by -frewrite-includes */
# 454 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 2 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined __LONG_DOUBLE_MATH_OPTIONAL && defined __NO_LONG_DOUBLE_MATH
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 456 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __LDBL_COMPAT 1
# ifdef __REDIRECT
#  define __LDBL_REDIR1(name, proto, alias) __REDIRECT (name, proto, alias)
#  define __LDBL_REDIR(name, proto) \
  __LDBL_REDIR1 (name, proto, __nldbl_##name)
#  define __LDBL_REDIR1_NTH(name, proto, alias) __REDIRECT_NTH (name, proto, alias)
#  define __LDBL_REDIR_NTH(name, proto) \
  __LDBL_REDIR1_NTH (name, proto, __nldbl_##name)
#  define __LDBL_REDIR1_DECL(name, alias) \
  extern __typeof (name) name __asm (__ASMNAME (#alias));
#  define __LDBL_REDIR_DECL(name) \
  extern __typeof (name) name __asm (__ASMNAME ("__nldbl_" #name));
#  define __REDIRECT_LDBL(name, proto, alias) \
  __LDBL_REDIR1 (name, proto, __nldbl_##alias)
#  define __REDIRECT_NTH_LDBL(name, proto, alias) \
  __LDBL_REDIR1_NTH (name, proto, __nldbl_##alias)
# endif
# 473 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#endif
# 474 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#if !defined __LDBL_COMPAT || !defined __REDIRECT
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 475 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __LDBL_REDIR1(name, proto, alias) name proto
# define __LDBL_REDIR(name, proto) name proto
# define __LDBL_REDIR1_NTH(name, proto, alias) name proto __THROW
# define __LDBL_REDIR_NTH(name, proto) name proto __THROW
# define __LDBL_REDIR_DECL(name)
# ifdef __REDIRECT
#  define __REDIRECT_LDBL(name, proto, alias) __REDIRECT (name, proto, alias)
#  define __REDIRECT_NTH_LDBL(name, proto, alias) \
  __REDIRECT_NTH (name, proto, alias)
# endif
# 485 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#endif
# 486 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* __glibc_macro_warning (MESSAGE) issues warning MESSAGE.  This is
   intended for use in preprocessor macros.

   Note: MESSAGE must be a _single_ string; concatenation of string
   literals is not supported.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (4,8) || __glibc_clang_prereq (3,5)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 493 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __glibc_macro_warning1(message) _Pragma (#message)
# define __glibc_macro_warning(message) \
  __glibc_macro_warning1 (GCC warning message)
#else
# 497 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __glibc_macro_warning(msg)
#endif
# 499 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* Generic selection (ISO C11) is a C-only feature, available in GCC
   since version 4.9.  Previous versions do not provide generic
   selection, even though they might set __STDC_VERSION__ to 201112L,
   when in -std=c11 mode.  Thus, we must check for !defined __GNUC__
   when testing __STDC_VERSION__ for generic selection support.
   On the other hand, Clang also defines __GNUC__, so a clang-specific
   check is required to enable the use of generic selection.  */
#if 0 /* disabled by -frewrite-includes */
#if !defined __cplusplus \
    && (__GNUC_PREREQ (4, 9) \
	|| __glibc_clang_has_extension (c_generic_selections) \
	|| (!defined __GNUC__ && defined __STDC_VERSION__ \
	    && __STDC_VERSION__ >= 201112L))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 512 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __HAVE_GENERIC_SELECTION 1
#else
# 514 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __HAVE_GENERIC_SELECTION 0
#endif
# 516 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#endif	 /* sys/cdefs.h */
# 518 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#endif /* cdefs.h expanded by -frewrite-includes */
# 462 "/usr/include/features.h" 2 3 4
# endif
# 463 "/usr/include/features.h" 3 4

/* If we don't have __REDIRECT, prototypes will be missing if
   __USE_FILE_OFFSET64 but not __USE_LARGEFILE[64]. */
#if 0 /* disabled by -frewrite-includes */
# if defined __USE_FILE_OFFSET64 && !defined __REDIRECT
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 467 "/usr/include/features.h" 3 4
#  define __USE_LARGEFILE	1
#  define __USE_LARGEFILE64	1
# endif
# 470 "/usr/include/features.h" 3 4

#endif	/* !ASSEMBLER */
# 472 "/usr/include/features.h" 3 4

/* Decide whether we can define 'extern inline' functions in headers.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (2, 7) && defined __OPTIMIZE__ \
    && !defined __OPTIMIZE_SIZE__ && !defined __NO_INLINE__ \
    && defined __extern_inline
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 477 "/usr/include/features.h" 3 4
# define __USE_EXTERN_INLINES	1
#endif
# 479 "/usr/include/features.h" 3 4


/* This is here only because every header file already includes this one.
   Get the definitions of all the appropriate `__stub_FUNCTION' symbols.
   <gnu/stubs.h> contains `#define __stub_FUNCTION' when FUNCTION is a stub
   that will always return failure (and set errno to ENOSYS).  */
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* stubs.h expanded by -frewrite-includes */
#include <gnu/stubs.h>
#else /* stubs.h expanded by -frewrite-includes */
# 485 "/usr/include/features.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 1 3 4
/* This file is automatically generated.
   This file selects the right generated file of `__stub_FUNCTION' macros
   based on the architecture being compiled for.  */


#if 0 /* disabled by -frewrite-includes */
#if !defined __x86_64__
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 7 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 3 4
#if 0 /* expanded by -frewrite-includes */
# include <gnu/stubs-32.h>
#endif /* expanded by -frewrite-includes */
# 7 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 3 4
# 8 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 3 4
#endif
# 9 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#if defined __x86_64__ && defined __LP64__
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 10 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* stubs-64.h expanded by -frewrite-includes */
# include <gnu/stubs-64.h>
#else /* stubs-64.h expanded by -frewrite-includes */
# 10 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/gnu/stubs-64.h" 1 3 4
/* This file is automatically generated.
   It defines a symbol `__stub_FUNCTION' for each function
   in the C library which is a stub, meaning it will fail
   every time called, usually setting errno to ENOSYS.  */

#ifdef _LIBC
# error Applications may not define the macro _LIBC
#endif
# 9 "/usr/include/x86_64-linux-gnu/gnu/stubs-64.h" 3 4

#define __stub___compat_bdflush
#define __stub_chflags
#define __stub_fchflags
#define __stub_gtty
#define __stub_lchmod
#define __stub_revoke
#define __stub_setlogin
#define __stub_sigreturn
#define __stub_sstk
#define __stub_stty
#endif /* stubs-64.h expanded by -frewrite-includes */
# 11 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 2 3 4
#endif
# 12 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#if defined __x86_64__ && defined __ILP32__
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 13 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 3 4
#if 0 /* expanded by -frewrite-includes */
# include <gnu/stubs-x32.h>
#endif /* expanded by -frewrite-includes */
# 13 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 3 4
# 14 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 3 4
#endif
# 15 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 3 4
#endif /* stubs.h expanded by -frewrite-includes */
# 486 "/usr/include/features.h" 2 3 4


#endif	/* features.h  */
# 489 "/usr/include/features.h" 3 4
#endif /* features.h expanded by -frewrite-includes */
# 34 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 2 3 4

/* ISO/IEC TR 24731-2:2010 defines the __STDC_WANT_LIB_EXT2__
   macro.  */
#undef __GLIBC_USE_LIB_EXT2
#if 0 /* disabled by -frewrite-includes */
#if (defined __USE_GNU							\
     || (defined __STDC_WANT_LIB_EXT2__ && __STDC_WANT_LIB_EXT2__ > 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 40 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_LIB_EXT2 1
#else
# 42 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_LIB_EXT2 0
#endif
# 44 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4

/* ISO/IEC TS 18661-1:2014 defines the __STDC_WANT_IEC_60559_BFP_EXT__
   macro.  Most but not all symbols enabled by that macro in TS
   18661-1 are enabled unconditionally in C2X; the symbols in Annex F
   still require that macro in C2X.  */
#undef __GLIBC_USE_IEC_60559_BFP_EXT
#if 0 /* disabled by -frewrite-includes */
#if defined __USE_GNU || defined __STDC_WANT_IEC_60559_BFP_EXT__
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 51 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_IEC_60559_BFP_EXT 1
#else
# 53 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_IEC_60559_BFP_EXT 0
#endif
# 55 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
#undef __GLIBC_USE_IEC_60559_BFP_EXT_C2X
#if 0 /* disabled by -frewrite-includes */
#if __GLIBC_USE (IEC_60559_BFP_EXT) || __GLIBC_USE (ISOC2X)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 57 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_IEC_60559_BFP_EXT_C2X 1
#else
# 59 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_IEC_60559_BFP_EXT_C2X 0
#endif
# 61 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4

/* ISO/IEC TS 18661-4:2015 defines the
   __STDC_WANT_IEC_60559_FUNCS_EXT__ macro.  Other than the reduction
   functions, the symbols from this TS are enabled unconditionally in
   C2X.  */
#undef __GLIBC_USE_IEC_60559_FUNCS_EXT
#if 0 /* disabled by -frewrite-includes */
#if defined __USE_GNU || defined __STDC_WANT_IEC_60559_FUNCS_EXT__
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 68 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_IEC_60559_FUNCS_EXT 1
#else
# 70 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_IEC_60559_FUNCS_EXT 0
#endif
# 72 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
#undef __GLIBC_USE_IEC_60559_FUNCS_EXT_C2X
#if 0 /* disabled by -frewrite-includes */
#if __GLIBC_USE (IEC_60559_FUNCS_EXT) || __GLIBC_USE (ISOC2X)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 74 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_IEC_60559_FUNCS_EXT_C2X 1
#else
# 76 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_IEC_60559_FUNCS_EXT_C2X 0
#endif
# 78 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4

/* ISO/IEC TS 18661-3:2015 defines the
   __STDC_WANT_IEC_60559_TYPES_EXT__ macro.  */
#undef __GLIBC_USE_IEC_60559_TYPES_EXT
#if 0 /* disabled by -frewrite-includes */
#if defined __USE_GNU || defined __STDC_WANT_IEC_60559_TYPES_EXT__
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 83 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_IEC_60559_TYPES_EXT 1
#else
# 85 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_IEC_60559_TYPES_EXT 0
#endif
# 87 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
#endif /* libc-header-start.h expanded by -frewrite-includes */
# 28 "/usr/include/stdio.h" 2 3 4

__BEGIN_DECLS

#define __need_size_t
#define __need_NULL
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* stddef.h expanded by -frewrite-includes */
#include <stddef.h>
#else /* stddef.h expanded by -frewrite-includes */
# 33 "/usr/include/stdio.h" 3 4
# 1 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 1 3 4
/*===---- stddef.h - Basic type definitions --------------------------------===
 *
 * Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
 * See https://llvm.org/LICENSE.txt for license information.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 *
 *===-----------------------------------------------------------------------===
 */

/*
 * This header is designed to be included multiple times. If any of the __need_
 * macros are defined, then only that subset of interfaces are provided. This
 * can be useful for POSIX headers that need to not expose all of stddef.h, but
 * need to use some of its interfaces. Otherwise this header provides all of
 * the expected interfaces.
 *
 * When clang modules are enabled, this header is a textual header to support
 * the multiple include behavior. As such, it doesn't directly declare anything
 * so that it doesn't add duplicate declarations to all of its includers'
 * modules.
 */
#if 0 /* disabled by -frewrite-includes */
#if defined(__MVS__) && __has_include_next(<stddef.h>)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 23 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#undef __need_ptrdiff_t
#undef __need_size_t
#undef __need_rsize_t
#undef __need_wchar_t
#undef __need_NULL
#undef __need_nullptr_t
#undef __need_unreachable
#undef __need_max_align_t
#undef __need_offsetof
#undef __need_wint_t
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_header_macro.h>
#endif /* expanded by -frewrite-includes */
# 33 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
# 34 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include_next <stddef.h>
#endif /* expanded by -frewrite-includes */
# 34 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
# 35 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4

#else
# 37 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if !defined(__need_ptrdiff_t) && !defined(__need_size_t) &&                   \
    !defined(__need_rsize_t) && !defined(__need_wchar_t) &&                    \
    !defined(__need_NULL) && !defined(__need_nullptr_t) &&                     \
    !defined(__need_unreachable) && !defined(__need_max_align_t) &&            \
    !defined(__need_offsetof) && !defined(__need_wint_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 43 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#define __need_ptrdiff_t
#define __need_size_t
/* ISO9899:2011 7.20 (C11 Annex K): Define rsize_t if __STDC_WANT_LIB_EXT1__ is
 * enabled. */
#if 0 /* disabled by -frewrite-includes */
#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ >= 1
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 48 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#define __need_rsize_t
#endif
# 50 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#define __need_wchar_t
#if 0 /* disabled by -frewrite-includes */
#if !defined(__STDDEF_H) || __has_feature(modules)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 52 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
/*
 * __stddef_null.h is special when building without modules: if __need_NULL is
 * set, then it will unconditionally redefine NULL. To avoid stepping on client
 * definitions of NULL, __need_NULL should only be set the first time this
 * header is included, that is when __STDDEF_H is not defined. However, when
 * building with modules, this header is a textual header and needs to
 * unconditionally include __stdef_null.h to support multiple submodules
 * exporting _Builtin_stddef.null. Take module SM with submodules A and B, whose
 * headers both include stddef.h When SM.A builds, __STDDEF_H will be defined.
 * When SM.B builds, the definition from SM.A will leak when building without
 * local submodule visibility. stddef.h wouldn't include __stddef_null.h, and
 * SM.B wouldn't import _Builtin_stddef.null, and SM.B's `export *` wouldn't
 * export NULL as expected. When building with modules, always include
 * __stddef_null.h so that everything works as expected.
 */
#define __need_NULL
#endif
# 69 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L) ||              \
    defined(__cplusplus)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 71 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#define __need_nullptr_t
#endif
# 73 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 74 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#define __need_unreachable
#endif
# 76 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L) ||              \
    (defined(__cplusplus) && __cplusplus >= 201103L)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 78 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#define __need_max_align_t
#endif
# 80 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#define __need_offsetof
/* wint_t is provided by <wchar.h> and not <stddef.h>. It's here
 * for compatibility, but must be explicitly requested. Therefore
 * __need_wint_t is intentionally not defined here. */
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_header_macro.h>
#endif /* expanded by -frewrite-includes */
# 84 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
# 85 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#endif
# 86 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_ptrdiff_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 88 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_ptrdiff_t.h>
#endif /* expanded by -frewrite-includes */
# 88 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
# 89 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#undef __need_ptrdiff_t
#endif /* defined(__need_ptrdiff_t) */
# 91 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_size_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 93 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* __stddef_size_t.h expanded by -frewrite-includes */
#include <__stddef_size_t.h>
#else /* __stddef_size_t.h expanded by -frewrite-includes */
# 93 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
# 1 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/__stddef_size_t.h" 1 3 4
/*===---- __stddef_size_t.h - Definition of size_t -------------------------===
 *
 * Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
 * See https://llvm.org/LICENSE.txt for license information.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 *
 *===-----------------------------------------------------------------------===
 */

/*
 * When -fbuiltin-headers-in-system-modules is set this is a non-modular header
 * and needs to behave as if it was textual.
 */
#if 0 /* disabled by -frewrite-includes */
#if !defined(_SIZE_T) ||                                                       \
    (__has_feature(modules) && !__building_module(_Builtin_stddef))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 16 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/__stddef_size_t.h" 3 4
#define _SIZE_T

typedef __SIZE_TYPE__ size_t;

#endif
# 21 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/__stddef_size_t.h" 3 4
#endif /* __stddef_size_t.h expanded by -frewrite-includes */
# 94 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 2 3 4
#undef __need_size_t
#endif /*defined(__need_size_t) */
# 96 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_rsize_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 98 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_rsize_t.h>
#endif /* expanded by -frewrite-includes */
# 98 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
# 99 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#undef __need_rsize_t
#endif /* defined(__need_rsize_t) */
# 101 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_wchar_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 103 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_wchar_t.h>
#endif /* expanded by -frewrite-includes */
# 103 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
# 104 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#undef __need_wchar_t
#endif /* defined(__need_wchar_t) */
# 106 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_NULL)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 108 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* __stddef_null.h expanded by -frewrite-includes */
#include <__stddef_null.h>
#else /* __stddef_null.h expanded by -frewrite-includes */
# 108 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
# 1 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/__stddef_null.h" 1 3 4
/*===---- __stddef_null.h - Definition of NULL -----------------------------===
 *
 * Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
 * See https://llvm.org/LICENSE.txt for license information.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 *
 *===-----------------------------------------------------------------------===
 */

#if 0 /* disabled by -frewrite-includes */
#if !defined(NULL) || !__building_module(_Builtin_stddef)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 11 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/__stddef_null.h" 3 4

/* linux/stddef.h will define NULL to 0. glibc (and other) headers then define
 * __need_NULL and rely on stddef.h to redefine NULL to the correct value again.
 * Modules don't support redefining macros like that, but support that pattern
 * in the non-modules case.
 */
#undef NULL

#ifdef __cplusplus
#if 0 /* disabled by -frewrite-includes */
#if !defined(__MINGW32__) && !defined(_MSC_VER)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 21 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/__stddef_null.h" 3 4
#define NULL __null
#else
# 23 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/__stddef_null.h" 3 4
#define NULL 0
#endif
# 25 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/__stddef_null.h" 3 4
#else
# 26 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/__stddef_null.h" 3 4
#define NULL ((void*)0)
#endif
# 28 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/__stddef_null.h" 3 4

#endif
# 30 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/__stddef_null.h" 3 4
#endif /* __stddef_null.h expanded by -frewrite-includes */
# 109 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 2 3 4
#undef __need_NULL
#endif /* defined(__need_NULL) */
# 111 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_nullptr_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 113 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_nullptr_t.h>
#endif /* expanded by -frewrite-includes */
# 113 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
# 114 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#undef __need_nullptr_t
#endif /* defined(__need_nullptr_t) */
# 116 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_unreachable)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 118 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_unreachable.h>
#endif /* expanded by -frewrite-includes */
# 118 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
# 119 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#undef __need_unreachable
#endif /* defined(__need_unreachable) */
# 121 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_max_align_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 123 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_max_align_t.h>
#endif /* expanded by -frewrite-includes */
# 123 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
# 124 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#undef __need_max_align_t
#endif /* defined(__need_max_align_t) */
# 126 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_offsetof)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 128 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_offsetof.h>
#endif /* expanded by -frewrite-includes */
# 128 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
# 129 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#undef __need_offsetof
#endif /* defined(__need_offsetof) */
# 131 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4

/* Some C libraries expect to see a wint_t here. Others (notably MinGW) will use
__WINT_TYPE__ directly; accommodate both by requiring __need_wint_t */
#if 0 /* disabled by -frewrite-includes */
#if defined(__need_wint_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 135 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_wint_t.h>
#endif /* expanded by -frewrite-includes */
# 135 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
# 136 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#undef __need_wint_t
#endif /* __need_wint_t */
# 138 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4

#endif /* __MVS__ */
# 140 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stddef.h" 3 4
#endif /* stddef.h expanded by -frewrite-includes */
# 34 "/usr/include/stdio.h" 2 3 4

#define __need___va_list
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* stdarg.h expanded by -frewrite-includes */
#include <stdarg.h>
#else /* stdarg.h expanded by -frewrite-includes */
# 36 "/usr/include/stdio.h" 3 4
# 1 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 1 3 4
/*===---- stdarg.h - Variable argument handling ----------------------------===
 *
 * Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
 * See https://llvm.org/LICENSE.txt for license information.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 *
 *===-----------------------------------------------------------------------===
 */

/*
 * This header is designed to be included multiple times. If any of the __need_
 * macros are defined, then only that subset of interfaces are provided. This
 * can be useful for POSIX headers that need to not expose all of stdarg.h, but
 * need to use some of its interfaces. Otherwise this header provides all of
 * the expected interfaces.
 *
 * When clang modules are enabled, this header is a textual header to support
 * the multiple include behavior. As such, it doesn't directly declare anything
 * so that it doesn't add duplicate declarations to all of its includers'
 * modules.
 */
#if 0 /* disabled by -frewrite-includes */
#if defined(__MVS__) && __has_include_next(<stdarg.h>)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 23 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4
#undef __need___va_list
#undef __need_va_list
#undef __need_va_arg
#undef __need___va_copy
#undef __need_va_copy
#if 0 /* expanded by -frewrite-includes */
#include <__stdarg_header_macro.h>
#endif /* expanded by -frewrite-includes */
# 28 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4
# 29 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include_next <stdarg.h>
#endif /* expanded by -frewrite-includes */
# 29 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4
# 30 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4

#else
# 32 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#if !defined(__need___va_list) && !defined(__need_va_list) &&                  \
    !defined(__need_va_arg) && !defined(__need___va_copy) &&                   \
    !defined(__need_va_copy)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 35 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4
#define __need___va_list
#define __need_va_list
#define __need_va_arg
#define __need___va_copy
/* GCC always defines __va_copy, but does not define va_copy unless in c99 mode
 * or -ansi is not specified, since it was not part of C90.
 */
#if 0 /* disabled by -frewrite-includes */
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) ||              \
    (defined(__cplusplus) && __cplusplus >= 201103L) ||                        \
    !defined(__STRICT_ANSI__)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 45 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4
#define __need_va_copy
#endif
# 47 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stdarg_header_macro.h>
#endif /* expanded by -frewrite-includes */
# 47 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4
# 48 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4
#endif
# 49 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4

#ifdef __need___va_list
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* __stdarg___gnuc_va_list.h expanded by -frewrite-includes */
#include <__stdarg___gnuc_va_list.h>
#else /* __stdarg___gnuc_va_list.h expanded by -frewrite-includes */
# 51 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4
# 1 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/__stdarg___gnuc_va_list.h" 1 3 4
/*===---- __stdarg___gnuc_va_list.h - Definition of __gnuc_va_list ---------===
 *
 * Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
 * See https://llvm.org/LICENSE.txt for license information.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 *
 *===-----------------------------------------------------------------------===
 */

#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST
typedef __builtin_va_list __gnuc_va_list;
#endif
# 14 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/__stdarg___gnuc_va_list.h" 3 4
#endif /* __stdarg___gnuc_va_list.h expanded by -frewrite-includes */
# 52 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 2 3 4
#undef __need___va_list
#endif /* defined(__need___va_list) */
# 54 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4

#ifdef __need_va_list
#if 0 /* expanded by -frewrite-includes */
#include <__stdarg_va_list.h>
#endif /* expanded by -frewrite-includes */
# 56 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4
# 57 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4
#undef __need_va_list
#endif /* defined(__need_va_list) */
# 59 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4

#ifdef __need_va_arg
#if 0 /* expanded by -frewrite-includes */
#include <__stdarg_va_arg.h>
#endif /* expanded by -frewrite-includes */
# 61 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4
# 62 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4
#undef __need_va_arg
#endif /* defined(__need_va_arg) */
# 64 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4

#ifdef __need___va_copy
#if 0 /* expanded by -frewrite-includes */
#include <__stdarg___va_copy.h>
#endif /* expanded by -frewrite-includes */
# 66 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4
# 67 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4
#undef __need___va_copy
#endif /* defined(__need___va_copy) */
# 69 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4

#ifdef __need_va_copy
#if 0 /* expanded by -frewrite-includes */
#include <__stdarg_va_copy.h>
#endif /* expanded by -frewrite-includes */
# 71 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4
# 72 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4
#undef __need_va_copy
#endif /* defined(__need_va_copy) */
# 74 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4

#endif /* __MVS__ */
# 76 "/home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include/stdarg.h" 3 4
#endif /* stdarg.h expanded by -frewrite-includes */
# 37 "/usr/include/stdio.h" 2 3 4

#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* types.h expanded by -frewrite-includes */
#include <bits/types.h>
#else /* types.h expanded by -frewrite-includes */
# 38 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types.h" 1 3 4
/* bits/types.h -- definitions of __*_t types underlying *_t types.
   Copyright (C) 2002-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

/*
 * Never include this file directly; use <sys/types.h> instead.
 */

#ifndef	_BITS_TYPES_H
#define	_BITS_TYPES_H	1

#if 0 /* expanded by -frewrite-includes */
#include <features.h>
#endif /* expanded by -frewrite-includes */
# 26 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
# 27 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* wordsize.h expanded by -frewrite-includes */
#include <bits/wordsize.h>
#else /* wordsize.h expanded by -frewrite-includes */
# 27 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
/* Determine the wordsize from the preprocessor defines.  */

#if 0 /* disabled by -frewrite-includes */
#if defined __x86_64__ && !defined __ILP32__
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 4 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 3 4
# define __WORDSIZE	64
#else
# 6 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 3 4
# define __WORDSIZE	32
#define __WORDSIZE32_SIZE_ULONG		0
#define __WORDSIZE32_PTRDIFF_LONG	0
#endif
# 10 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 3 4

#ifdef __x86_64__
# define __WORDSIZE_TIME64_COMPAT32	1
/* Both x86-64 and x32 use the 64-bit system call interface.  */
# define __SYSCALL_WORDSIZE		64
#else
# 16 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 3 4
# define __WORDSIZE_TIME64_COMPAT32	0
#endif
# 18 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 3 4
#endif /* wordsize.h expanded by -frewrite-includes */
# 28 "/usr/include/x86_64-linux-gnu/bits/types.h" 2 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* timesize.h expanded by -frewrite-includes */
#include <bits/timesize.h>
#else /* timesize.h expanded by -frewrite-includes */
# 28 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 1 3 4
/* Bit size of the time_t type at glibc build time, x86-64 and x32 case.
   Copyright (C) 2018-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#if 0 /* disabled by -frewrite-includes */
#if defined __x86_64__ && defined __ILP32__
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 20 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 3 4
/* For x32, time is 64-bit even though word size is 32-bit.  */
# define __TIMESIZE	64
#else
# 23 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 3 4
/* For others, time size is word size.  */
# define __TIMESIZE	__WORDSIZE
#endif
# 26 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 3 4
#endif /* timesize.h expanded by -frewrite-includes */
# 29 "/usr/include/x86_64-linux-gnu/bits/types.h" 2 3 4

/* Convenience types.  */
typedef unsigned char __u_char;
typedef unsigned short int __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;

/* Fixed-size types, underlying types depend on word size and compiler.  */
typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;
#if 0 /* disabled by -frewrite-includes */
#if __WORDSIZE == 64
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 44 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
typedef signed long int __int64_t;
typedef unsigned long int __uint64_t;
#else
# 47 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
__extension__ typedef signed long long int __int64_t;
__extension__ typedef unsigned long long int __uint64_t;
#endif
# 50 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4

/* Smallest types with at least a given width.  */
typedef __int8_t __int_least8_t;
typedef __uint8_t __uint_least8_t;
typedef __int16_t __int_least16_t;
typedef __uint16_t __uint_least16_t;
typedef __int32_t __int_least32_t;
typedef __uint32_t __uint_least32_t;
typedef __int64_t __int_least64_t;
typedef __uint64_t __uint_least64_t;

/* quad_t is also 64 bits.  */
#if 0 /* disabled by -frewrite-includes */
#if __WORDSIZE == 64
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 63 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
typedef long int __quad_t;
typedef unsigned long int __u_quad_t;
#else
# 66 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
__extension__ typedef long long int __quad_t;
__extension__ typedef unsigned long long int __u_quad_t;
#endif
# 69 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4

/* Largest integral types.  */
#if 0 /* disabled by -frewrite-includes */
#if __WORDSIZE == 64
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 72 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
typedef long int __intmax_t;
typedef unsigned long int __uintmax_t;
#else
# 75 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
__extension__ typedef long long int __intmax_t;
__extension__ typedef unsigned long long int __uintmax_t;
#endif
# 78 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4


/* The machine-dependent file <bits/typesizes.h> defines __*_T_TYPE
   macros for each of the OS types we define below.  The definitions
   of those macros must use the following macros for underlying types.
   We define __S<SIZE>_TYPE and __U<SIZE>_TYPE for the signed and unsigned
   variants of each of the following integer types on this machine.

	16		-- "natural" 16-bit type (always short)
	32		-- "natural" 32-bit type (always int)
	64		-- "natural" 64-bit type (long or long long)
	LONG32		-- 32-bit type, traditionally long
	QUAD		-- 64-bit type, traditionally long long
	WORD		-- natural type of __WORDSIZE bits (int or long)
	LONGWORD	-- type of __WORDSIZE bits, traditionally long

   We distinguish WORD/LONGWORD, 32/LONG32, and 64/QUAD so that the
   conventional uses of `long' or `long long' type modifiers match the
   types we define, even when a less-adorned type would be the same size.
   This matters for (somewhat) portably writing printf/scanf formats for
   these types, where using the appropriate l or ll format modifiers can
   make the typedefs and the formats match up across all GNU platforms.  If
   we used `long' when it's 64 bits where `long long' is expected, then the
   compiler would warn about the formats not matching the argument types,
   and the programmer changing them to shut up the compiler would break the
   program's portability.

   Here we assume what is presently the case in all the GCC configurations
   we support: long long is always 64 bits, long is always word/address size,
   and int is always 32 bits.  */

#define	__S16_TYPE		short int
#define __U16_TYPE		unsigned short int
#define	__S32_TYPE		int
#define __U32_TYPE		unsigned int
#define __SLONGWORD_TYPE	long int
#define __ULONGWORD_TYPE	unsigned long int
#if 0 /* disabled by -frewrite-includes */
#if __WORDSIZE == 32
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 116 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
# define __SQUAD_TYPE		__int64_t
# define __UQUAD_TYPE		__uint64_t
# define __SWORD_TYPE		int
# define __UWORD_TYPE		unsigned int
# define __SLONG32_TYPE		long int
# define __ULONG32_TYPE		unsigned long int
# define __S64_TYPE		__int64_t
# define __U64_TYPE		__uint64_t
/* We want __extension__ before typedef's that use nonstandard base types
   such as `long long' in C89 mode.  */
# define __STD_TYPE		__extension__ typedef
#if 0 /* disabled by -frewrite-includes */
#if 0
#elif __WORDSIZE == 64
#endif
#endif /* disabled by -frewrite-includes */
#elif 1 /* evaluated by -frewrite-includes */
# 128 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
# define __SQUAD_TYPE		long int
# define __UQUAD_TYPE		unsigned long int
# define __SWORD_TYPE		long int
# define __UWORD_TYPE		unsigned long int
# define __SLONG32_TYPE		int
# define __ULONG32_TYPE		unsigned int
# define __S64_TYPE		long int
# define __U64_TYPE		unsigned long int
/* No need to mark the typedef with __extension__.   */
# define __STD_TYPE		typedef
#else
# 139 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
# error
#endif
# 141 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* typesizes.h expanded by -frewrite-includes */
#include <bits/typesizes.h>	/* Defines __*_T_TYPE macros.  */
#else /* typesizes.h expanded by -frewrite-includes */
# 141 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/typesizes.h" 1 3 4
/* bits/typesizes.h -- underlying types for *_t.  Linux/x86-64 version.
   Copyright (C) 2012-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#ifndef _BITS_TYPES_H
# error "Never include <bits/typesizes.h> directly; use <sys/types.h> instead."
#endif
# 22 "/usr/include/x86_64-linux-gnu/bits/typesizes.h" 3 4

#ifndef	_BITS_TYPESIZES_H
#define	_BITS_TYPESIZES_H	1

/* See <bits/types.h> for the meaning of these macros.  This file exists so
   that <bits/types.h> need not vary across different GNU platforms.  */

/* X32 kernel interface is 64-bit.  */
#if 0 /* disabled by -frewrite-includes */
#if defined __x86_64__ && defined __ILP32__
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 31 "/usr/include/x86_64-linux-gnu/bits/typesizes.h" 3 4
# define __SYSCALL_SLONG_TYPE	__SQUAD_TYPE
# define __SYSCALL_ULONG_TYPE	__UQUAD_TYPE
#else
# 34 "/usr/include/x86_64-linux-gnu/bits/typesizes.h" 3 4
# define __SYSCALL_SLONG_TYPE	__SLONGWORD_TYPE
# define __SYSCALL_ULONG_TYPE	__ULONGWORD_TYPE
#endif
# 37 "/usr/include/x86_64-linux-gnu/bits/typesizes.h" 3 4

#define __DEV_T_TYPE		__UQUAD_TYPE
#define __UID_T_TYPE		__U32_TYPE
#define __GID_T_TYPE		__U32_TYPE
#define __INO_T_TYPE		__SYSCALL_ULONG_TYPE
#define __INO64_T_TYPE		__UQUAD_TYPE
#define __MODE_T_TYPE		__U32_TYPE
#ifdef __x86_64__
# define __NLINK_T_TYPE		__SYSCALL_ULONG_TYPE
# define __FSWORD_T_TYPE	__SYSCALL_SLONG_TYPE
#else
# 48 "/usr/include/x86_64-linux-gnu/bits/typesizes.h" 3 4
# define __NLINK_T_TYPE		__UWORD_TYPE
# define __FSWORD_T_TYPE	__SWORD_TYPE
#endif
# 51 "/usr/include/x86_64-linux-gnu/bits/typesizes.h" 3 4
#define __OFF_T_TYPE		__SYSCALL_SLONG_TYPE
#define __OFF64_T_TYPE		__SQUAD_TYPE
#define __PID_T_TYPE		__S32_TYPE
#define __RLIM_T_TYPE		__SYSCALL_ULONG_TYPE
#define __RLIM64_T_TYPE		__UQUAD_TYPE
#define __BLKCNT_T_TYPE		__SYSCALL_SLONG_TYPE
#define __BLKCNT64_T_TYPE	__SQUAD_TYPE
#define __FSBLKCNT_T_TYPE	__SYSCALL_ULONG_TYPE
#define __FSBLKCNT64_T_TYPE	__UQUAD_TYPE
#define __FSFILCNT_T_TYPE	__SYSCALL_ULONG_TYPE
#define __FSFILCNT64_T_TYPE	__UQUAD_TYPE
#define __ID_T_TYPE		__U32_TYPE
#define __CLOCK_T_TYPE		__SYSCALL_SLONG_TYPE
#define __TIME_T_TYPE		__SYSCALL_SLONG_TYPE
#define __USECONDS_T_TYPE	__U32_TYPE
#define __SUSECONDS_T_TYPE	__SYSCALL_SLONG_TYPE
#define __DADDR_T_TYPE		__S32_TYPE
#define __KEY_T_TYPE		__S32_TYPE
#define __CLOCKID_T_TYPE	__S32_TYPE
#define __TIMER_T_TYPE		void *
#define __BLKSIZE_T_TYPE	__SYSCALL_SLONG_TYPE
#define __FSID_T_TYPE		struct { int __val[2]; }
#define __SSIZE_T_TYPE		__SWORD_TYPE
#define __CPU_MASK_TYPE 	__SYSCALL_ULONG_TYPE

#ifdef __x86_64__
/* Tell the libc code that off_t and off64_t are actually the same type
   for all ABI purposes, even if possibly expressed as different base types
   for C type-checking purposes.  */
# define __OFF_T_MATCHES_OFF64_T	1

/* Same for ino_t and ino64_t.  */
# define __INO_T_MATCHES_INO64_T	1

/* And for __rlim_t and __rlim64_t.  */
# define __RLIM_T_MATCHES_RLIM64_T	1

/* And for fsblkcnt_t, fsblkcnt64_t, fsfilcnt_t and fsfilcnt64_t.  */
# define __STATFS_MATCHES_STATFS64  1
#else
# 91 "/usr/include/x86_64-linux-gnu/bits/typesizes.h" 3 4
# define __RLIM_T_MATCHES_RLIM64_T	0

# define __STATFS_MATCHES_STATFS64  0
#endif
# 95 "/usr/include/x86_64-linux-gnu/bits/typesizes.h" 3 4

/* Number of descriptors that can fit in an `fd_set'.  */
#define __FD_SETSIZE		1024


#endif /* bits/typesizes.h */
# 101 "/usr/include/x86_64-linux-gnu/bits/typesizes.h" 3 4
#endif /* typesizes.h expanded by -frewrite-includes */
# 142 "/usr/include/x86_64-linux-gnu/bits/types.h" 2 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* time64.h expanded by -frewrite-includes */
#include <bits/time64.h>	/* Defines __TIME*_T_TYPE macros.  */
#else /* time64.h expanded by -frewrite-includes */
# 142 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/time64.h" 1 3 4
/* bits/time64.h -- underlying types for __time64_t.  Generic version.
   Copyright (C) 2018-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#ifndef _BITS_TYPES_H
# error "Never include <bits/time64.h> directly; use <sys/types.h> instead."
#endif
# 22 "/usr/include/x86_64-linux-gnu/bits/time64.h" 3 4

#ifndef	_BITS_TIME64_H
#define	_BITS_TIME64_H	1

/* Define __TIME64_T_TYPE so that it is always a 64-bit type.  */

#if 0 /* disabled by -frewrite-includes */
#if __TIMESIZE == 64
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 29 "/usr/include/x86_64-linux-gnu/bits/time64.h" 3 4
/* If we already have 64-bit time type then use it.  */
# define __TIME64_T_TYPE		__TIME_T_TYPE
#else
# 32 "/usr/include/x86_64-linux-gnu/bits/time64.h" 3 4
/* Define a 64-bit time type alongsize the 32-bit one.  */
# define __TIME64_T_TYPE		__SQUAD_TYPE
#endif
# 35 "/usr/include/x86_64-linux-gnu/bits/time64.h" 3 4

#endif /* bits/time64.h */
# 37 "/usr/include/x86_64-linux-gnu/bits/time64.h" 3 4
#endif /* time64.h expanded by -frewrite-includes */
# 143 "/usr/include/x86_64-linux-gnu/bits/types.h" 2 3 4


__STD_TYPE __DEV_T_TYPE __dev_t;	/* Type of device numbers.  */
__STD_TYPE __UID_T_TYPE __uid_t;	/* Type of user identifications.  */
__STD_TYPE __GID_T_TYPE __gid_t;	/* Type of group identifications.  */
__STD_TYPE __INO_T_TYPE __ino_t;	/* Type of file serial numbers.  */
__STD_TYPE __INO64_T_TYPE __ino64_t;	/* Type of file serial numbers (LFS).*/
__STD_TYPE __MODE_T_TYPE __mode_t;	/* Type of file attribute bitmasks.  */
__STD_TYPE __NLINK_T_TYPE __nlink_t;	/* Type of file link counts.  */
__STD_TYPE __OFF_T_TYPE __off_t;	/* Type of file sizes and offsets.  */
__STD_TYPE __OFF64_T_TYPE __off64_t;	/* Type of file sizes and offsets (LFS).  */
__STD_TYPE __PID_T_TYPE __pid_t;	/* Type of process identifications.  */
__STD_TYPE __FSID_T_TYPE __fsid_t;	/* Type of file system IDs.  */
__STD_TYPE __CLOCK_T_TYPE __clock_t;	/* Type of CPU usage counts.  */
__STD_TYPE __RLIM_T_TYPE __rlim_t;	/* Type for resource measurement.  */
__STD_TYPE __RLIM64_T_TYPE __rlim64_t;	/* Type for resource measurement (LFS).  */
__STD_TYPE __ID_T_TYPE __id_t;		/* General type for IDs.  */
__STD_TYPE __TIME_T_TYPE __time_t;	/* Seconds since the Epoch.  */
__STD_TYPE __USECONDS_T_TYPE __useconds_t; /* Count of microseconds.  */
__STD_TYPE __SUSECONDS_T_TYPE __suseconds_t; /* Signed count of microseconds.  */

__STD_TYPE __DADDR_T_TYPE __daddr_t;	/* The type of a disk address.  */
__STD_TYPE __KEY_T_TYPE __key_t;	/* Type of an IPC key.  */

/* Clock ID used in clock and timer functions.  */
__STD_TYPE __CLOCKID_T_TYPE __clockid_t;

/* Timer ID returned by `timer_create'.  */
__STD_TYPE __TIMER_T_TYPE __timer_t;

/* Type to represent block size.  */
__STD_TYPE __BLKSIZE_T_TYPE __blksize_t;

/* Types from the Large File Support interface.  */

/* Type to count number of disk blocks.  */
__STD_TYPE __BLKCNT_T_TYPE __blkcnt_t;
__STD_TYPE __BLKCNT64_T_TYPE __blkcnt64_t;

/* Type to count file system blocks.  */
__STD_TYPE __FSBLKCNT_T_TYPE __fsblkcnt_t;
__STD_TYPE __FSBLKCNT64_T_TYPE __fsblkcnt64_t;

/* Type to count file system nodes.  */
__STD_TYPE __FSFILCNT_T_TYPE __fsfilcnt_t;
__STD_TYPE __FSFILCNT64_T_TYPE __fsfilcnt64_t;

/* Type of miscellaneous file system fields.  */
__STD_TYPE __FSWORD_T_TYPE __fsword_t;

__STD_TYPE __SSIZE_T_TYPE __ssize_t; /* Type of a byte count, or error.  */

/* Signed long type used in system calls.  */
__STD_TYPE __SYSCALL_SLONG_TYPE __syscall_slong_t;
/* Unsigned long type used in system calls.  */
__STD_TYPE __SYSCALL_ULONG_TYPE __syscall_ulong_t;

/* These few don't really vary by system, they always correspond
   to one of the other defined types.  */
typedef __off64_t __loff_t;	/* Type of file sizes and offsets (LFS).  */
typedef char *__caddr_t;

/* Duplicates info from stdint.h but this is used in unistd.h.  */
__STD_TYPE __SWORD_TYPE __intptr_t;

/* Duplicate info from sys/socket.h.  */
__STD_TYPE __U32_TYPE __socklen_t;

/* C99: An integer type that can be accessed as an atomic entity,
   even in the presence of asynchronous interrupts.
   It is not currently necessary for this to be machine-specific.  */
typedef int __sig_atomic_t;

/* Seconds since the Epoch, visible to user code when time_t is too
   narrow only for consistency with the old way of widening too-narrow
   types.  User code should never use __time64_t.  */
#if 0 /* disabled by -frewrite-includes */
#if __TIMESIZE == 64 && defined __LIBC
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 220 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
# define __time64_t __time_t
#if 0 /* disabled by -frewrite-includes */
#if 0
#elif __TIMESIZE != 64
#endif
#endif /* disabled by -frewrite-includes */
#elif 0 /* evaluated by -frewrite-includes */
# 222 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
__STD_TYPE __TIME64_T_TYPE __time64_t;
#endif
# 224 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4

#undef __STD_TYPE

#endif /* bits/types.h */
# 228 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
#endif /* types.h expanded by -frewrite-includes */
# 39 "/usr/include/stdio.h" 2 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* __fpos_t.h expanded by -frewrite-includes */
#include <bits/types/__fpos_t.h>
#else /* __fpos_t.h expanded by -frewrite-includes */
# 39 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h" 1 3 4
#ifndef _____fpos_t_defined
#define _____fpos_t_defined 1

#if 0 /* expanded by -frewrite-includes */
#include <bits/types.h>
#endif /* expanded by -frewrite-includes */
# 4 "/usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h" 3 4
# 5 "/usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h" 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* __mbstate_t.h expanded by -frewrite-includes */
#include <bits/types/__mbstate_t.h>
#else /* __mbstate_t.h expanded by -frewrite-includes */
# 5 "/usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h" 1 3 4
#ifndef ____mbstate_t_defined
#define ____mbstate_t_defined 1

/* Integral type unchanged by default argument promotions that can
   hold any value corresponding to members of the extended character
   set, as well as at least one value that does not correspond to any
   member of the extended character set.  */
#ifndef __WINT_TYPE__
# define __WINT_TYPE__ unsigned int
#endif
# 11 "/usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h" 3 4

/* Conversion state information.  */
typedef struct
{
  int __count;
  union
  {
    __WINT_TYPE__ __wch;
    char __wchb[4];
  } __value;		/* Value so far.  */
} __mbstate_t;

#endif
# 24 "/usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h" 3 4
#endif /* __mbstate_t.h expanded by -frewrite-includes */
# 6 "/usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h" 2 3 4

/* The tag name of this struct is _G_fpos_t to preserve historic
   C++ mangled names for functions taking fpos_t arguments.
   That name should not be used in new code.  */
typedef struct _G_fpos_t
{
  __off_t __pos;
  __mbstate_t __state;
} __fpos_t;

#endif
# 17 "/usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h" 3 4
#endif /* __fpos_t.h expanded by -frewrite-includes */
# 40 "/usr/include/stdio.h" 2 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* __fpos64_t.h expanded by -frewrite-includes */
#include <bits/types/__fpos64_t.h>
#else /* __fpos64_t.h expanded by -frewrite-includes */
# 40 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h" 1 3 4
#ifndef _____fpos64_t_defined
#define _____fpos64_t_defined 1

#if 0 /* expanded by -frewrite-includes */
#include <bits/types.h>
#endif /* expanded by -frewrite-includes */
# 4 "/usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h" 3 4
# 5 "/usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <bits/types/__mbstate_t.h>
#endif /* expanded by -frewrite-includes */
# 5 "/usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h" 3 4
# 6 "/usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h" 3 4

/* The tag name of this struct is _G_fpos64_t to preserve historic
   C++ mangled names for functions taking fpos_t and/or fpos64_t
   arguments.  That name should not be used in new code.  */
typedef struct _G_fpos64_t
{
  __off64_t __pos;
  __mbstate_t __state;
} __fpos64_t;

#endif
# 17 "/usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h" 3 4
#endif /* __fpos64_t.h expanded by -frewrite-includes */
# 41 "/usr/include/stdio.h" 2 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* __FILE.h expanded by -frewrite-includes */
#include <bits/types/__FILE.h>
#else /* __FILE.h expanded by -frewrite-includes */
# 41 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/__FILE.h" 1 3 4
#ifndef ____FILE_defined
#define ____FILE_defined 1

struct _IO_FILE;
typedef struct _IO_FILE __FILE;

#endif
# 8 "/usr/include/x86_64-linux-gnu/bits/types/__FILE.h" 3 4
#endif /* __FILE.h expanded by -frewrite-includes */
# 42 "/usr/include/stdio.h" 2 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* FILE.h expanded by -frewrite-includes */
#include <bits/types/FILE.h>
#else /* FILE.h expanded by -frewrite-includes */
# 42 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/FILE.h" 1 3 4
#ifndef __FILE_defined
#define __FILE_defined 1

struct _IO_FILE;

/* The opaque type of streams.  This is the definition used elsewhere.  */
typedef struct _IO_FILE FILE;

#endif
# 10 "/usr/include/x86_64-linux-gnu/bits/types/FILE.h" 3 4
#endif /* FILE.h expanded by -frewrite-includes */
# 43 "/usr/include/stdio.h" 2 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* struct_FILE.h expanded by -frewrite-includes */
#include <bits/types/struct_FILE.h>
#else /* struct_FILE.h expanded by -frewrite-includes */
# 43 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h" 1 3 4
/* Copyright (C) 1991-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#ifndef __struct_FILE_defined
#define __struct_FILE_defined 1

/* Caution: The contents of this file are not part of the official
   stdio.h API.  However, much of it is part of the official *binary*
   interface, and therefore cannot be changed.  */

#if 0 /* disabled by -frewrite-includes */
#if defined _IO_USE_OLD_IO_FILE && !defined _LIBC
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 26 "/usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h" 3 4
# error "_IO_USE_OLD_IO_FILE should only be defined when building libc itself"
#endif
# 28 "/usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined _IO_lock_t_defined && !defined _LIBC
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 30 "/usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h" 3 4
# error "_IO_lock_t_defined should only be defined when building libc itself"
#endif
# 32 "/usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h" 3 4

#if 0 /* expanded by -frewrite-includes */
#include <bits/types.h>
#endif /* expanded by -frewrite-includes */
# 33 "/usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h" 3 4
# 34 "/usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h" 3 4

struct _IO_FILE;
struct _IO_marker;
struct _IO_codecvt;
struct _IO_wide_data;

/* During the build of glibc itself, _IO_lock_t will already have been
   defined by internal headers.  */
#ifndef _IO_lock_t_defined
typedef void _IO_lock_t;
#endif
# 45 "/usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h" 3 4

/* The tag name of this struct is _IO_FILE to preserve historic
   C++ mangled names for functions taking FILE* arguments.
   That name should not be used in new code.  */
struct _IO_FILE
{
  int _flags;		/* High-order word is _IO_MAGIC; rest is flags. */

  /* The following pointers correspond to the C++ streambuf protocol. */
  char *_IO_read_ptr;	/* Current read pointer */
  char *_IO_read_end;	/* End of get area. */
  char *_IO_read_base;	/* Start of putback+get area. */
  char *_IO_write_base;	/* Start of put area. */
  char *_IO_write_ptr;	/* Current put pointer. */
  char *_IO_write_end;	/* End of put area. */
  char *_IO_buf_base;	/* Start of reserve area. */
  char *_IO_buf_end;	/* End of reserve area. */

  /* The following fields are used to support backing up and undo. */
  char *_IO_save_base; /* Pointer to start of non-current get area. */
  char *_IO_backup_base;  /* Pointer to first valid character of backup area */
  char *_IO_save_end; /* Pointer to end of non-current get area. */

  struct _IO_marker *_markers;

  struct _IO_FILE *_chain;

  int _fileno;
  int _flags2;
  __off_t _old_offset; /* This used to be _offset but it's too small.  */

  /* 1+column number of pbase(); 0 is unknown. */
  unsigned short _cur_column;
  signed char _vtable_offset;
  char _shortbuf[1];

  _IO_lock_t *_lock;
#ifdef _IO_USE_OLD_IO_FILE
};

struct _IO_FILE_complete
{
  struct _IO_FILE _file;
#endif
# 89 "/usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h" 3 4
  __off64_t _offset;
  /* Wide character stream stuff.  */
  struct _IO_codecvt *_codecvt;
  struct _IO_wide_data *_wide_data;
  struct _IO_FILE *_freeres_list;
  void *_freeres_buf;
  size_t __pad5;
  int _mode;
  /* Make sure we don't get into trouble again.  */
  char _unused2[15 * sizeof (int) - 4 * sizeof (void *) - sizeof (size_t)];
};

/* These macros are used by bits/stdio.h and internal headers.  */
#define __getc_unlocked_body(_fp)					\
  (__glibc_unlikely ((_fp)->_IO_read_ptr >= (_fp)->_IO_read_end)	\
   ? __uflow (_fp) : *(unsigned char *) (_fp)->_IO_read_ptr++)

#define __putc_unlocked_body(_ch, _fp)					\
  (__glibc_unlikely ((_fp)->_IO_write_ptr >= (_fp)->_IO_write_end)	\
   ? __overflow (_fp, (unsigned char) (_ch))				\
   : (unsigned char) (*(_fp)->_IO_write_ptr++ = (_ch)))

#define _IO_EOF_SEEN 0x0010
#define __feof_unlocked_body(_fp) (((_fp)->_flags & _IO_EOF_SEEN) != 0)

#define _IO_ERR_SEEN 0x0020
#define __ferror_unlocked_body(_fp) (((_fp)->_flags & _IO_ERR_SEEN) != 0)

#define _IO_USER_LOCK 0x8000
/* Many more flag bits are defined internally.  */

#endif
# 121 "/usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h" 3 4
#endif /* struct_FILE.h expanded by -frewrite-includes */
# 44 "/usr/include/stdio.h" 2 3 4

#ifdef __USE_GNU
#if 0 /* expanded by -frewrite-includes */
# include <bits/types/cookie_io_functions_t.h>
#endif /* expanded by -frewrite-includes */
# 46 "/usr/include/stdio.h" 3 4
# 47 "/usr/include/stdio.h" 3 4
#endif
# 48 "/usr/include/stdio.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined __USE_XOPEN || defined __USE_XOPEN2K8
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 50 "/usr/include/stdio.h" 3 4
# ifdef __GNUC__
#  ifndef _VA_LIST_DEFINED
typedef __gnuc_va_list va_list;
#   define _VA_LIST_DEFINED
#  endif
# 55 "/usr/include/stdio.h" 3 4
# else
# 56 "/usr/include/stdio.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#  include <stdarg.h>
#endif /* expanded by -frewrite-includes */
# 56 "/usr/include/stdio.h" 3 4
# 57 "/usr/include/stdio.h" 3 4
# endif
# 58 "/usr/include/stdio.h" 3 4
#endif
# 59 "/usr/include/stdio.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined __USE_UNIX98 || defined __USE_XOPEN2K
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 61 "/usr/include/stdio.h" 3 4
# ifndef __off_t_defined
# ifndef __USE_FILE_OFFSET64
typedef __off_t off_t;
# else
# 65 "/usr/include/stdio.h" 3 4
typedef __off64_t off_t;
# endif
# 67 "/usr/include/stdio.h" 3 4
# define __off_t_defined
# endif
# 69 "/usr/include/stdio.h" 3 4
#if 0 /* disabled by -frewrite-includes */
# if defined __USE_LARGEFILE64 && !defined __off64_t_defined
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 70 "/usr/include/stdio.h" 3 4
typedef __off64_t off64_t;
# define __off64_t_defined
# endif
# 73 "/usr/include/stdio.h" 3 4
#endif
# 74 "/usr/include/stdio.h" 3 4

#ifdef __USE_XOPEN2K8
# ifndef __ssize_t_defined
typedef __ssize_t ssize_t;
# define __ssize_t_defined
# endif
# 80 "/usr/include/stdio.h" 3 4
#endif
# 81 "/usr/include/stdio.h" 3 4

/* The type of the second argument to `fgetpos' and `fsetpos'.  */
#ifndef __USE_FILE_OFFSET64
typedef __fpos_t fpos_t;
#else
# 86 "/usr/include/stdio.h" 3 4
typedef __fpos64_t fpos_t;
#endif
# 88 "/usr/include/stdio.h" 3 4
#ifdef __USE_LARGEFILE64
typedef __fpos64_t fpos64_t;
#endif
# 91 "/usr/include/stdio.h" 3 4

/* The possibilities for the third argument to `setvbuf'.  */
#define _IOFBF 0		/* Fully buffered.  */
#define _IOLBF 1		/* Line buffered.  */
#define _IONBF 2		/* No buffering.  */


/* Default buffer size.  */
#define BUFSIZ 8192


/* The value returned by fgetc and similar functions to indicate the
   end of the file.  */
#define EOF (-1)


/* The possibilities for the third argument to `fseek'.
   These values should not be changed.  */
#define SEEK_SET	0	/* Seek from beginning of file.  */
#define SEEK_CUR	1	/* Seek from current position.  */
#define SEEK_END	2	/* Seek from end of file.  */
#ifdef __USE_GNU
# define SEEK_DATA	3	/* Seek to next data.  */
# define SEEK_HOLE	4	/* Seek to next hole.  */
#endif
# 116 "/usr/include/stdio.h" 3 4


#if 0 /* disabled by -frewrite-includes */
#if defined __USE_MISC || defined __USE_XOPEN
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 119 "/usr/include/stdio.h" 3 4
/* Default path prefix for `tempnam' and `tmpnam'.  */
# define P_tmpdir	"/tmp"
#endif
# 122 "/usr/include/stdio.h" 3 4


/* Get the values:
   L_tmpnam	How long an array of chars must be to be passed to `tmpnam'.
   TMP_MAX	The minimum number of unique filenames generated by tmpnam
		(and tempnam when it uses tmpnam's name space),
		or tempnam (the two are separate).
   L_ctermid	How long an array to pass to `ctermid'.
   L_cuserid	How long an array to pass to `cuserid'.
   FOPEN_MAX	Minimum number of files that can be open at once.
   FILENAME_MAX	Maximum length of a filename.  */
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* stdio_lim.h expanded by -frewrite-includes */
#include <bits/stdio_lim.h>
#else /* stdio_lim.h expanded by -frewrite-includes */
# 133 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/stdio_lim.h" 1 3 4
/* Copyright (C) 1994-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#ifndef _BITS_STDIO_LIM_H
#define _BITS_STDIO_LIM_H 1

#ifndef _STDIO_H
# error "Never include <bits/stdio_lim.h> directly; use <stdio.h> instead."
#endif
# 24 "/usr/include/x86_64-linux-gnu/bits/stdio_lim.h" 3 4

#define L_tmpnam 20
#define TMP_MAX 238328
#define FILENAME_MAX 4096

#ifdef __USE_POSIX
# define L_ctermid 9
#if 0 /* disabled by -frewrite-includes */
# if !defined __USE_XOPEN2K || defined __USE_GNU
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 32 "/usr/include/x86_64-linux-gnu/bits/stdio_lim.h" 3 4
#  define L_cuserid 9
# endif
# 34 "/usr/include/x86_64-linux-gnu/bits/stdio_lim.h" 3 4
#endif
# 35 "/usr/include/x86_64-linux-gnu/bits/stdio_lim.h" 3 4

#undef  FOPEN_MAX
#define FOPEN_MAX 16

#endif /* bits/stdio_lim.h */
# 40 "/usr/include/x86_64-linux-gnu/bits/stdio_lim.h" 3 4
#endif /* stdio_lim.h expanded by -frewrite-includes */
# 134 "/usr/include/stdio.h" 2 3 4


/* Standard streams.  */
extern FILE *stdin;		/* Standard input stream.  */
extern FILE *stdout;		/* Standard output stream.  */
extern FILE *stderr;		/* Standard error output stream.  */
/* C89/C99 say they're macros.  Make them happy.  */
#define stdin stdin
#define stdout stdout
#define stderr stderr

/* Remove file FILENAME.  */
extern int remove (const char *__filename) __THROW;
/* Rename file OLD to NEW.  */
extern int rename (const char *__old, const char *__new) __THROW;

#ifdef __USE_ATFILE
/* Rename file OLD relative to OLDFD to NEW relative to NEWFD.  */
extern int renameat (int __oldfd, const char *__old, int __newfd,
		     const char *__new) __THROW;
#endif
# 155 "/usr/include/stdio.h" 3 4

#ifdef __USE_GNU
/* Flags for renameat2.  */
# define RENAME_NOREPLACE (1 << 0)
# define RENAME_EXCHANGE (1 << 1)
# define RENAME_WHITEOUT (1 << 2)

/* Rename file OLD relative to OLDFD to NEW relative to NEWFD, with
   additional flags.  */
extern int renameat2 (int __oldfd, const char *__old, int __newfd,
		      const char *__new, unsigned int __flags) __THROW;
#endif
# 167 "/usr/include/stdio.h" 3 4

/* Create a temporary file and open it read/write.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
#ifndef __USE_FILE_OFFSET64
extern FILE *tmpfile (void) __wur;
#else
# 175 "/usr/include/stdio.h" 3 4
# ifdef __REDIRECT
extern FILE *__REDIRECT (tmpfile, (void), tmpfile64) __wur;
# else
# 178 "/usr/include/stdio.h" 3 4
#  define tmpfile tmpfile64
# endif
# 180 "/usr/include/stdio.h" 3 4
#endif
# 181 "/usr/include/stdio.h" 3 4

#ifdef __USE_LARGEFILE64
extern FILE *tmpfile64 (void) __wur;
#endif
# 185 "/usr/include/stdio.h" 3 4

/* Generate a temporary filename.  */
extern char *tmpnam (char *__s) __THROW __wur;

#ifdef __USE_MISC
/* This is the reentrant variant of `tmpnam'.  The only difference is
   that it does not allow S to be NULL.  */
extern char *tmpnam_r (char *__s) __THROW __wur;
#endif
# 194 "/usr/include/stdio.h" 3 4


#if 0 /* disabled by -frewrite-includes */
#if defined __USE_MISC || defined __USE_XOPEN
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 197 "/usr/include/stdio.h" 3 4
/* Generate a unique temporary filename using up to five characters of PFX
   if it is not NULL.  The directory to put this file in is searched for
   as follows: First the environment variable "TMPDIR" is checked.
   If it contains the name of a writable directory, that directory is used.
   If not and if DIR is not NULL, that value is checked.  If that fails,
   P_tmpdir is tried and finally "/tmp".  The storage for the filename
   is allocated by `malloc'.  */
extern char *tempnam (const char *__dir, const char *__pfx)
     __THROW __attribute_malloc__ __wur;
#endif
# 207 "/usr/include/stdio.h" 3 4


/* Close STREAM.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int fclose (FILE *__stream);
/* Flush STREAM, or all streams if STREAM is NULL.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int fflush (FILE *__stream);

#ifdef __USE_MISC
/* Faster versions when locking is not required.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern int fflush_unlocked (FILE *__stream);
#endif
# 229 "/usr/include/stdio.h" 3 4

#ifdef __USE_GNU
/* Close all streams.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern int fcloseall (void);
#endif
# 239 "/usr/include/stdio.h" 3 4


#ifndef __USE_FILE_OFFSET64
/* Open a file and create a new stream for it.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern FILE *fopen (const char *__restrict __filename,
		    const char *__restrict __modes) __wur;
/* Open a file, replacing an existing stream with it.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern FILE *freopen (const char *__restrict __filename,
		      const char *__restrict __modes,
		      FILE *__restrict __stream) __wur;
#else
# 256 "/usr/include/stdio.h" 3 4
# ifdef __REDIRECT
extern FILE *__REDIRECT (fopen, (const char *__restrict __filename,
				 const char *__restrict __modes), fopen64)
  __wur;
extern FILE *__REDIRECT (freopen, (const char *__restrict __filename,
				   const char *__restrict __modes,
				   FILE *__restrict __stream), freopen64)
  __wur;
# else
# 265 "/usr/include/stdio.h" 3 4
#  define fopen fopen64
#  define freopen freopen64
# endif
# 268 "/usr/include/stdio.h" 3 4
#endif
# 269 "/usr/include/stdio.h" 3 4
#ifdef __USE_LARGEFILE64
extern FILE *fopen64 (const char *__restrict __filename,
		      const char *__restrict __modes) __wur;
extern FILE *freopen64 (const char *__restrict __filename,
			const char *__restrict __modes,
			FILE *__restrict __stream) __wur;
#endif
# 276 "/usr/include/stdio.h" 3 4

#ifdef	__USE_POSIX
/* Create a new stream that refers to an existing system file descriptor.  */
extern FILE *fdopen (int __fd, const char *__modes) __THROW __wur;
#endif
# 281 "/usr/include/stdio.h" 3 4

#ifdef	__USE_GNU
/* Create a new stream that refers to the given magic cookie,
   and uses the given functions for input and output.  */
extern FILE *fopencookie (void *__restrict __magic_cookie,
			  const char *__restrict __modes,
			  cookie_io_functions_t __io_funcs) __THROW __wur;
#endif
# 289 "/usr/include/stdio.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined __USE_XOPEN2K8 || __GLIBC_USE (LIB_EXT2)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 291 "/usr/include/stdio.h" 3 4
/* Create a new stream that refers to a memory buffer.  */
extern FILE *fmemopen (void *__s, size_t __len, const char *__modes)
  __THROW __wur;

/* Open a stream that writes into a malloc'd buffer that is expanded as
   necessary.  *BUFLOC and *SIZELOC are updated with the buffer's location
   and the number of characters written on fflush or fclose.  */
extern FILE *open_memstream (char **__bufloc, size_t *__sizeloc) __THROW __wur;
#endif
# 300 "/usr/include/stdio.h" 3 4


/* If BUF is NULL, make STREAM unbuffered.
   Else make it use buffer BUF, of size BUFSIZ.  */
extern void setbuf (FILE *__restrict __stream, char *__restrict __buf) __THROW;
/* Make STREAM use buffering mode MODE.
   If BUF is not NULL, use N bytes of it for buffering;
   else allocate an internal buffer N bytes long.  */
extern int setvbuf (FILE *__restrict __stream, char *__restrict __buf,
		    int __modes, size_t __n) __THROW;

#ifdef	__USE_MISC
/* If BUF is NULL, make STREAM unbuffered.
   Else make it use SIZE bytes of BUF for buffering.  */
extern void setbuffer (FILE *__restrict __stream, char *__restrict __buf,
		       size_t __size) __THROW;

/* Make STREAM line-buffered.  */
extern void setlinebuf (FILE *__stream) __THROW;
#endif
# 320 "/usr/include/stdio.h" 3 4


/* Write formatted output to STREAM.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int fprintf (FILE *__restrict __stream,
		    const char *__restrict __format, ...);
/* Write formatted output to stdout.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int printf (const char *__restrict __format, ...);
/* Write formatted output to S.  */
extern int sprintf (char *__restrict __s,
		    const char *__restrict __format, ...) __THROWNL;

/* Write formatted output to S from argument list ARG.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int vfprintf (FILE *__restrict __s, const char *__restrict __format,
		     __gnuc_va_list __arg);
/* Write formatted output to stdout from argument list ARG.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int vprintf (const char *__restrict __format, __gnuc_va_list __arg);
/* Write formatted output to S from argument list ARG.  */
extern int vsprintf (char *__restrict __s, const char *__restrict __format,
		     __gnuc_va_list __arg) __THROWNL;

#if 0 /* disabled by -frewrite-includes */
#if defined __USE_ISOC99 || defined __USE_UNIX98
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 353 "/usr/include/stdio.h" 3 4
/* Maximum chars of output to write in MAXLEN.  */
extern int snprintf (char *__restrict __s, size_t __maxlen,
		     const char *__restrict __format, ...)
     __THROWNL __attribute__ ((__format__ (__printf__, 3, 4)));

extern int vsnprintf (char *__restrict __s, size_t __maxlen,
		      const char *__restrict __format, __gnuc_va_list __arg)
     __THROWNL __attribute__ ((__format__ (__printf__, 3, 0)));
#endif
# 362 "/usr/include/stdio.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __GLIBC_USE (LIB_EXT2)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 364 "/usr/include/stdio.h" 3 4
/* Write formatted output to a string dynamically allocated with `malloc'.
   Store the address of the string in *PTR.  */
extern int vasprintf (char **__restrict __ptr, const char *__restrict __f,
		      __gnuc_va_list __arg)
     __THROWNL __attribute__ ((__format__ (__printf__, 2, 0))) __wur;
extern int __asprintf (char **__restrict __ptr,
		       const char *__restrict __fmt, ...)
     __THROWNL __attribute__ ((__format__ (__printf__, 2, 3))) __wur;
extern int asprintf (char **__restrict __ptr,
		     const char *__restrict __fmt, ...)
     __THROWNL __attribute__ ((__format__ (__printf__, 2, 3))) __wur;
#endif
# 376 "/usr/include/stdio.h" 3 4

#ifdef __USE_XOPEN2K8
/* Write formatted output to a file descriptor.  */
extern int vdprintf (int __fd, const char *__restrict __fmt,
		     __gnuc_va_list __arg)
     __attribute__ ((__format__ (__printf__, 2, 0)));
extern int dprintf (int __fd, const char *__restrict __fmt, ...)
     __attribute__ ((__format__ (__printf__, 2, 3)));
#endif
# 385 "/usr/include/stdio.h" 3 4


/* Read formatted input from STREAM.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int fscanf (FILE *__restrict __stream,
		   const char *__restrict __format, ...) __wur;
/* Read formatted input from stdin.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int scanf (const char *__restrict __format, ...) __wur;
/* Read formatted input from S.  */
extern int sscanf (const char *__restrict __s,
		   const char *__restrict __format, ...) __THROW;

/* For historical reasons, the C99-compliant versions of the scanf
   functions are at alternative names.  When __LDBL_COMPAT is in
   effect, this is handled in bits/stdio-ldbl.h.  */
#if 0 /* disabled by -frewrite-includes */
#if !__GLIBC_USE (DEPRECATED_SCANF) && !defined __LDBL_COMPAT
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 406 "/usr/include/stdio.h" 3 4
# ifdef __REDIRECT
extern int __REDIRECT (fscanf, (FILE *__restrict __stream,
				const char *__restrict __format, ...),
		       __isoc99_fscanf) __wur;
extern int __REDIRECT (scanf, (const char *__restrict __format, ...),
		       __isoc99_scanf) __wur;
extern int __REDIRECT_NTH (sscanf, (const char *__restrict __s,
				    const char *__restrict __format, ...),
			   __isoc99_sscanf);
# else
# 416 "/usr/include/stdio.h" 3 4
extern int __isoc99_fscanf (FILE *__restrict __stream,
			    const char *__restrict __format, ...) __wur;
extern int __isoc99_scanf (const char *__restrict __format, ...) __wur;
extern int __isoc99_sscanf (const char *__restrict __s,
			    const char *__restrict __format, ...) __THROW;
#  define fscanf __isoc99_fscanf
#  define scanf __isoc99_scanf
#  define sscanf __isoc99_sscanf
# endif
# 425 "/usr/include/stdio.h" 3 4
#endif
# 426 "/usr/include/stdio.h" 3 4

#ifdef	__USE_ISOC99
/* Read formatted input from S into argument list ARG.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int vfscanf (FILE *__restrict __s, const char *__restrict __format,
		    __gnuc_va_list __arg)
     __attribute__ ((__format__ (__scanf__, 2, 0))) __wur;

/* Read formatted input from stdin into argument list ARG.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int vscanf (const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__format__ (__scanf__, 1, 0))) __wur;

/* Read formatted input from S into argument list ARG.  */
extern int vsscanf (const char *__restrict __s,
		    const char *__restrict __format, __gnuc_va_list __arg)
     __THROW __attribute__ ((__format__ (__scanf__, 2, 0)));

/* Same redirection as above for the v*scanf family.  */
#if 0 /* disabled by -frewrite-includes */
# if !__GLIBC_USE (DEPRECATED_SCANF)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 450 "/usr/include/stdio.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#  if defined __REDIRECT && !defined __LDBL_COMPAT
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 451 "/usr/include/stdio.h" 3 4
extern int __REDIRECT (vfscanf,
		       (FILE *__restrict __s,
			const char *__restrict __format, __gnuc_va_list __arg),
		       __isoc99_vfscanf)
     __attribute__ ((__format__ (__scanf__, 2, 0))) __wur;
extern int __REDIRECT (vscanf, (const char *__restrict __format,
				__gnuc_va_list __arg), __isoc99_vscanf)
     __attribute__ ((__format__ (__scanf__, 1, 0))) __wur;
extern int __REDIRECT_NTH (vsscanf,
			   (const char *__restrict __s,
			    const char *__restrict __format,
			    __gnuc_va_list __arg), __isoc99_vsscanf)
     __attribute__ ((__format__ (__scanf__, 2, 0)));
#if 0 /* disabled by -frewrite-includes */
#if 0
#  elif !defined __REDIRECT
#endif
#endif /* disabled by -frewrite-includes */
#elif 0 /* evaluated by -frewrite-includes */
# 465 "/usr/include/stdio.h" 3 4
extern int __isoc99_vfscanf (FILE *__restrict __s,
			     const char *__restrict __format,
			     __gnuc_va_list __arg) __wur;
extern int __isoc99_vscanf (const char *__restrict __format,
			    __gnuc_va_list __arg) __wur;
extern int __isoc99_vsscanf (const char *__restrict __s,
			     const char *__restrict __format,
			     __gnuc_va_list __arg) __THROW;
#   define vfscanf __isoc99_vfscanf
#   define vscanf __isoc99_vscanf
#   define vsscanf __isoc99_vsscanf
#  endif
# 477 "/usr/include/stdio.h" 3 4
# endif
# 478 "/usr/include/stdio.h" 3 4
#endif /* Use ISO C9x.  */
# 479 "/usr/include/stdio.h" 3 4


/* Read a character from STREAM.

   These functions are possible cancellation points and therefore not
   marked with __THROW.  */
extern int fgetc (FILE *__stream);
extern int getc (FILE *__stream);

/* Read a character from stdin.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int getchar (void);

#ifdef __USE_POSIX199506
/* These are defined in POSIX.1:1996.

   These functions are possible cancellation points and therefore not
   marked with __THROW.  */
extern int getc_unlocked (FILE *__stream);
extern int getchar_unlocked (void);
#endif /* Use POSIX.  */
# 502 "/usr/include/stdio.h" 3 4

#ifdef __USE_MISC
/* Faster version when locking is not necessary.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern int fgetc_unlocked (FILE *__stream);
#endif /* Use MISC.  */
# 512 "/usr/include/stdio.h" 3 4


/* Write a character to STREAM.

   These functions are possible cancellation points and therefore not
   marked with __THROW.

   These functions is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int fputc (int __c, FILE *__stream);
extern int putc (int __c, FILE *__stream);

/* Write a character to stdout.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int putchar (int __c);

#ifdef __USE_MISC
/* Faster version when locking is not necessary.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern int fputc_unlocked (int __c, FILE *__stream);
#endif /* Use MISC.  */
# 539 "/usr/include/stdio.h" 3 4

#ifdef __USE_POSIX199506
/* These are defined in POSIX.1:1996.

   These functions are possible cancellation points and therefore not
   marked with __THROW.  */
extern int putc_unlocked (int __c, FILE *__stream);
extern int putchar_unlocked (int __c);
#endif /* Use POSIX.  */
# 548 "/usr/include/stdio.h" 3 4


#if 0 /* disabled by -frewrite-includes */
#if defined __USE_MISC \
    || (defined __USE_XOPEN && !defined __USE_XOPEN2K)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 552 "/usr/include/stdio.h" 3 4
/* Get a word (int) from STREAM.  */
extern int getw (FILE *__stream);

/* Write a word (int) to STREAM.  */
extern int putw (int __w, FILE *__stream);
#endif
# 558 "/usr/include/stdio.h" 3 4


/* Get a newline-terminated string of finite length from STREAM.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern char *fgets (char *__restrict __s, int __n, FILE *__restrict __stream)
     __wur;

#if 0 /* disabled by -frewrite-includes */
#if __GLIBC_USE (DEPRECATED_GETS)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 568 "/usr/include/stdio.h" 3 4
/* Get a newline-terminated string from stdin, removing the newline.

   This function is impossible to use safely.  It has been officially
   removed from ISO C11 and ISO C++14, and we have also removed it
   from the _GNU_SOURCE feature list.  It remains available when
   explicitly using an old ISO C, Unix, or POSIX standard.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern char *gets (char *__s) __wur __attribute_deprecated__;
#endif
# 579 "/usr/include/stdio.h" 3 4

#ifdef __USE_GNU
/* This function does the same as `fgets' but does not lock the stream.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern char *fgets_unlocked (char *__restrict __s, int __n,
			     FILE *__restrict __stream) __wur;
#endif
# 590 "/usr/include/stdio.h" 3 4


#if 0 /* disabled by -frewrite-includes */
#if defined __USE_XOPEN2K8 || __GLIBC_USE (LIB_EXT2)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 593 "/usr/include/stdio.h" 3 4
/* Read up to (and including) a DELIMITER from STREAM into *LINEPTR
   (and null-terminate it). *LINEPTR is a pointer returned from malloc (or
   NULL), pointing to *N characters of space.  It is realloc'd as
   necessary.  Returns the number of characters read (not including the
   null terminator), or -1 on error or EOF.

   These functions are not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation they are cancellation points and
   therefore not marked with __THROW.  */
extern __ssize_t __getdelim (char **__restrict __lineptr,
                             size_t *__restrict __n, int __delimiter,
                             FILE *__restrict __stream) __wur;
extern __ssize_t getdelim (char **__restrict __lineptr,
                           size_t *__restrict __n, int __delimiter,
                           FILE *__restrict __stream) __wur;

/* Like `getdelim', but reads up to a newline.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern __ssize_t getline (char **__restrict __lineptr,
                          size_t *__restrict __n,
                          FILE *__restrict __stream) __wur;
#endif
# 620 "/usr/include/stdio.h" 3 4


/* Write a string to STREAM.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int fputs (const char *__restrict __s, FILE *__restrict __stream);

/* Write a string, followed by a newline, to stdout.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int puts (const char *__s);


/* Push a character back onto the input buffer of STREAM.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int ungetc (int __c, FILE *__stream);


/* Read chunks of generic data from STREAM.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern size_t fread (void *__restrict __ptr, size_t __size,
		     size_t __n, FILE *__restrict __stream) __wur;
/* Write chunks of generic data to STREAM.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern size_t fwrite (const void *__restrict __ptr, size_t __size,
		      size_t __n, FILE *__restrict __s);

#ifdef __USE_GNU
/* This function does the same as `fputs' but does not lock the stream.

   This function is not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation it is a cancellation point and
   therefore not marked with __THROW.  */
extern int fputs_unlocked (const char *__restrict __s,
			   FILE *__restrict __stream);
#endif
# 665 "/usr/include/stdio.h" 3 4

#ifdef __USE_MISC
/* Faster versions when locking is not necessary.

   These functions are not part of POSIX and therefore no official
   cancellation point.  But due to similarity with an POSIX interface
   or due to the implementation they are cancellation points and
   therefore not marked with __THROW.  */
extern size_t fread_unlocked (void *__restrict __ptr, size_t __size,
			      size_t __n, FILE *__restrict __stream) __wur;
extern size_t fwrite_unlocked (const void *__restrict __ptr, size_t __size,
			       size_t __n, FILE *__restrict __stream);
#endif
# 678 "/usr/include/stdio.h" 3 4


/* Seek to a certain position on STREAM.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int fseek (FILE *__stream, long int __off, int __whence);
/* Return the current position of STREAM.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern long int ftell (FILE *__stream) __wur;
/* Rewind to the beginning of STREAM.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern void rewind (FILE *__stream);

/* The Single Unix Specification, Version 2, specifies an alternative,
   more adequate interface for the two functions above which deal with
   file offset.  `long int' is not the right type.  These definitions
   are originally defined in the Large File Support API.  */

#if 0 /* disabled by -frewrite-includes */
#if defined __USE_LARGEFILE || defined __USE_XOPEN2K
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 702 "/usr/include/stdio.h" 3 4
# ifndef __USE_FILE_OFFSET64
/* Seek to a certain position on STREAM.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int fseeko (FILE *__stream, __off_t __off, int __whence);
/* Return the current position of STREAM.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern __off_t ftello (FILE *__stream) __wur;
# else
# 714 "/usr/include/stdio.h" 3 4
#  ifdef __REDIRECT
extern int __REDIRECT (fseeko,
		       (FILE *__stream, __off64_t __off, int __whence),
		       fseeko64);
extern __off64_t __REDIRECT (ftello, (FILE *__stream), ftello64);
#  else
# 720 "/usr/include/stdio.h" 3 4
#   define fseeko fseeko64
#   define ftello ftello64
#  endif
# 723 "/usr/include/stdio.h" 3 4
# endif
# 724 "/usr/include/stdio.h" 3 4
#endif
# 725 "/usr/include/stdio.h" 3 4

#ifndef __USE_FILE_OFFSET64
/* Get STREAM's position.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int fgetpos (FILE *__restrict __stream, fpos_t *__restrict __pos);
/* Set STREAM's position.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int fsetpos (FILE *__stream, const fpos_t *__pos);
#else
# 738 "/usr/include/stdio.h" 3 4
# ifdef __REDIRECT
extern int __REDIRECT (fgetpos, (FILE *__restrict __stream,
				 fpos_t *__restrict __pos), fgetpos64);
extern int __REDIRECT (fsetpos,
		       (FILE *__stream, const fpos_t *__pos), fsetpos64);
# else
# 744 "/usr/include/stdio.h" 3 4
#  define fgetpos fgetpos64
#  define fsetpos fsetpos64
# endif
# 747 "/usr/include/stdio.h" 3 4
#endif
# 748 "/usr/include/stdio.h" 3 4

#ifdef __USE_LARGEFILE64
extern int fseeko64 (FILE *__stream, __off64_t __off, int __whence);
extern __off64_t ftello64 (FILE *__stream) __wur;
extern int fgetpos64 (FILE *__restrict __stream, fpos64_t *__restrict __pos);
extern int fsetpos64 (FILE *__stream, const fpos64_t *__pos);
#endif
# 755 "/usr/include/stdio.h" 3 4

/* Clear the error and EOF indicators for STREAM.  */
extern void clearerr (FILE *__stream) __THROW;
/* Return the EOF indicator for STREAM.  */
extern int feof (FILE *__stream) __THROW __wur;
/* Return the error indicator for STREAM.  */
extern int ferror (FILE *__stream) __THROW __wur;

#ifdef __USE_MISC
/* Faster versions when locking is not required.  */
extern void clearerr_unlocked (FILE *__stream) __THROW;
extern int feof_unlocked (FILE *__stream) __THROW __wur;
extern int ferror_unlocked (FILE *__stream) __THROW __wur;
#endif
# 769 "/usr/include/stdio.h" 3 4


/* Print a message describing the meaning of the value of errno.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern void perror (const char *__s);

/* Provide the declarations for `sys_errlist' and `sys_nerr' if they
   are available on this system.  Even if available, these variables
   should not be used directly.  The `strerror' function provides
   all the necessary functionality.  */
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* sys_errlist.h expanded by -frewrite-includes */
#include <bits/sys_errlist.h>
#else /* sys_errlist.h expanded by -frewrite-includes */
# 781 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/sys_errlist.h" 1 3 4
/* Declare sys_errlist and sys_nerr, or don't.  Compatibility (do) version.
   Copyright (C) 2002-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#ifndef _STDIO_H
# error "Never include <bits/sys_errlist.h> directly; use <stdio.h> instead."
#endif
# 22 "/usr/include/x86_64-linux-gnu/bits/sys_errlist.h" 3 4

/* sys_errlist and sys_nerr are deprecated.  Use strerror instead.  */

#ifdef  __USE_MISC
extern int sys_nerr;
extern const char *const sys_errlist[];
#endif
# 29 "/usr/include/x86_64-linux-gnu/bits/sys_errlist.h" 3 4
#ifdef  __USE_GNU
extern int _sys_nerr;
extern const char *const _sys_errlist[];
#endif
# 33 "/usr/include/x86_64-linux-gnu/bits/sys_errlist.h" 3 4
#endif /* sys_errlist.h expanded by -frewrite-includes */
# 782 "/usr/include/stdio.h" 2 3 4


#ifdef	__USE_POSIX
/* Return the system file descriptor for STREAM.  */
extern int fileno (FILE *__stream) __THROW __wur;
#endif /* Use POSIX.  */
# 788 "/usr/include/stdio.h" 3 4

#ifdef __USE_MISC
/* Faster version when locking is not required.  */
extern int fileno_unlocked (FILE *__stream) __THROW __wur;
#endif
# 793 "/usr/include/stdio.h" 3 4


#ifdef __USE_POSIX2
/* Create a new stream connected to a pipe running the given command.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern FILE *popen (const char *__command, const char *__modes) __wur;

/* Close a stream opened by popen and return the status of its child.

   This function is a possible cancellation point and therefore not
   marked with __THROW.  */
extern int pclose (FILE *__stream);
#endif
# 808 "/usr/include/stdio.h" 3 4


#ifdef	__USE_POSIX
/* Return the name of the controlling terminal.  */
extern char *ctermid (char *__s) __THROW;
#endif /* Use POSIX.  */
# 814 "/usr/include/stdio.h" 3 4


#if 0 /* disabled by -frewrite-includes */
#if (defined __USE_XOPEN && !defined __USE_XOPEN2K) || defined __USE_GNU
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 817 "/usr/include/stdio.h" 3 4
/* Return the name of the current user.  */
extern char *cuserid (char *__s);
#endif /* Use X/Open, but not issue 6.  */
# 820 "/usr/include/stdio.h" 3 4


#ifdef	__USE_GNU
struct obstack;			/* See <obstack.h>.  */

/* Write formatted output to an obstack.  */
extern int obstack_printf (struct obstack *__restrict __obstack,
			   const char *__restrict __format, ...)
     __THROWNL __attribute__ ((__format__ (__printf__, 2, 3)));
extern int obstack_vprintf (struct obstack *__restrict __obstack,
			    const char *__restrict __format,
			    __gnuc_va_list __args)
     __THROWNL __attribute__ ((__format__ (__printf__, 2, 0)));
#endif /* Use GNU.  */
# 834 "/usr/include/stdio.h" 3 4


#ifdef __USE_POSIX199506
/* These are defined in POSIX.1:1996.  */

/* Acquire ownership of STREAM.  */
extern void flockfile (FILE *__stream) __THROW;

/* Try to acquire ownership of STREAM but do not block if it is not
   possible.  */
extern int ftrylockfile (FILE *__stream) __THROW __wur;

/* Relinquish the ownership granted for STREAM.  */
extern void funlockfile (FILE *__stream) __THROW;
#endif /* POSIX */
# 849 "/usr/include/stdio.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined __USE_XOPEN && !defined __USE_XOPEN2K && !defined __USE_GNU
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 851 "/usr/include/stdio.h" 3 4
/*  X/Open Issues 1-5 required getopt to be declared in this
   header.  It was removed in Issue 6.  GNU follows Issue 6.  */
#if 0 /* expanded by -frewrite-includes */
# include <bits/getopt_posix.h>
#endif /* expanded by -frewrite-includes */
# 853 "/usr/include/stdio.h" 3 4
# 854 "/usr/include/stdio.h" 3 4
#endif
# 855 "/usr/include/stdio.h" 3 4

/* Slow-path routines used by the optimized inline functions in
   bits/stdio.h.  */
extern int __uflow (FILE *);
extern int __overflow (FILE *, int);

/* If we are compiling with optimizing read this file.  It contains
   several optimizing inline functions and macros.  */
#ifdef __USE_EXTERN_INLINES
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* stdio.h expanded by -frewrite-includes */
# include <bits/stdio.h>
#else /* stdio.h expanded by -frewrite-includes */
# 864 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/stdio.h" 1 3 4
/* Optimizing macros and inline functions for stdio functions.
   Copyright (C) 1998-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#ifndef _BITS_STDIO_H
#define _BITS_STDIO_H 1

#ifndef _STDIO_H
# error "Never include <bits/stdio.h> directly; use <stdio.h> instead."
#endif
# 25 "/usr/include/x86_64-linux-gnu/bits/stdio.h" 3 4

#ifndef __extern_inline
# define __STDIO_INLINE inline
#else
# 29 "/usr/include/x86_64-linux-gnu/bits/stdio.h" 3 4
# define __STDIO_INLINE __extern_inline
#endif
# 31 "/usr/include/x86_64-linux-gnu/bits/stdio.h" 3 4


#ifdef __USE_EXTERN_INLINES
/* For -D_FORTIFY_SOURCE{,=2} bits/stdio2.h will define a different
   inline.  */
#if 0 /* disabled by -frewrite-includes */
# if !(__USE_FORTIFY_LEVEL > 0 && defined __fortify_function)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 37 "/usr/include/x86_64-linux-gnu/bits/stdio.h" 3 4
/* Write formatted output to stdout from argument list ARG.  */
__STDIO_INLINE int
vprintf (const char *__restrict __fmt, __gnuc_va_list __arg)
{
  return vfprintf (stdout, __fmt, __arg);
}
# endif
# 44 "/usr/include/x86_64-linux-gnu/bits/stdio.h" 3 4

/* Read a character from stdin.  */
__STDIO_INLINE int
getchar (void)
{
  return getc (stdin);
}


# ifdef __USE_MISC
/* Faster version when locking is not necessary.  */
__STDIO_INLINE int
fgetc_unlocked (FILE *__fp)
{
  return __getc_unlocked_body (__fp);
}
# endif /* misc */
# 61 "/usr/include/x86_64-linux-gnu/bits/stdio.h" 3 4


# ifdef __USE_POSIX
/* This is defined in POSIX.1:1996.  */
__STDIO_INLINE int
getc_unlocked (FILE *__fp)
{
  return __getc_unlocked_body (__fp);
}

/* This is defined in POSIX.1:1996.  */
__STDIO_INLINE int
getchar_unlocked (void)
{
  return __getc_unlocked_body (stdin);
}
# endif	/* POSIX */
# 78 "/usr/include/x86_64-linux-gnu/bits/stdio.h" 3 4


/* Write a character to stdout.  */
__STDIO_INLINE int
putchar (int __c)
{
  return putc (__c, stdout);
}


# ifdef __USE_MISC
/* Faster version when locking is not necessary.  */
__STDIO_INLINE int
fputc_unlocked (int __c, FILE *__stream)
{
  return __putc_unlocked_body (__c, __stream);
}
# endif /* misc */
# 96 "/usr/include/x86_64-linux-gnu/bits/stdio.h" 3 4


# ifdef __USE_POSIX
/* This is defined in POSIX.1:1996.  */
__STDIO_INLINE int
putc_unlocked (int __c, FILE *__stream)
{
  return __putc_unlocked_body (__c, __stream);
}

/* This is defined in POSIX.1:1996.  */
__STDIO_INLINE int
putchar_unlocked (int __c)
{
  return __putc_unlocked_body (__c, stdout);
}
# endif	/* POSIX */
# 113 "/usr/include/x86_64-linux-gnu/bits/stdio.h" 3 4


# ifdef	__USE_GNU
/* Like `getdelim', but reads up to a newline.  */
__STDIO_INLINE __ssize_t
getline (char **__lineptr, size_t *__n, FILE *__stream)
{
  return __getdelim (__lineptr, __n, '\n', __stream);
}
# endif /* GNU */
# 123 "/usr/include/x86_64-linux-gnu/bits/stdio.h" 3 4


# ifdef __USE_MISC
/* Faster versions when locking is not required.  */
__STDIO_INLINE int
__NTH (feof_unlocked (FILE *__stream))
{
  return __feof_unlocked_body (__stream);
}

/* Faster versions when locking is not required.  */
__STDIO_INLINE int
__NTH (ferror_unlocked (FILE *__stream))
{
  return __ferror_unlocked_body (__stream);
}
# endif /* misc */
# 140 "/usr/include/x86_64-linux-gnu/bits/stdio.h" 3 4

#endif /* Use extern inlines.  */
# 142 "/usr/include/x86_64-linux-gnu/bits/stdio.h" 3 4


#if 0 /* disabled by -frewrite-includes */
#if defined __USE_MISC && defined __GNUC__ && defined __OPTIMIZE__ \
    && !defined __cplusplus
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 146 "/usr/include/x86_64-linux-gnu/bits/stdio.h" 3 4
/* Perform some simple optimizations.  */
# define fread_unlocked(ptr, size, n, stream) \
  (__extension__ ((__builtin_constant_p (size) && __builtin_constant_p (n)    \
		   && (size_t) (size) * (size_t) (n) <= 8		      \
		   && (size_t) (size) != 0)				      \
		  ? ({ char *__ptr = (char *) (ptr);			      \
		       FILE *__stream = (stream);			      \
		       size_t __cnt;					      \
		       for (__cnt = (size_t) (size) * (size_t) (n);	      \
			    __cnt > 0; --__cnt)				      \
			 {						      \
			   int __c = getc_unlocked (__stream);		      \
			   if (__c == EOF)				      \
			     break;					      \
			   *__ptr++ = __c;				      \
			 }						      \
		       ((size_t) (size) * (size_t) (n) - __cnt)		      \
			/ (size_t) (size); })				      \
		  : (((__builtin_constant_p (size) && (size_t) (size) == 0)   \
		      || (__builtin_constant_p (n) && (size_t) (n) == 0))     \
			/* Evaluate all parameters once.  */		      \
		     ? ((void) (ptr), (void) (stream), (void) (size),	      \
			(void) (n), (size_t) 0)				      \
		     : fread_unlocked (ptr, size, n, stream))))

# define fwrite_unlocked(ptr, size, n, stream) \
  (__extension__ ((__builtin_constant_p (size) && __builtin_constant_p (n)    \
		   && (size_t) (size) * (size_t) (n) <= 8		      \
		   && (size_t) (size) != 0)				      \
		  ? ({ const char *__ptr = (const char *) (ptr);	      \
		       FILE *__stream = (stream);			      \
		       size_t __cnt;					      \
		       for (__cnt = (size_t) (size) * (size_t) (n);	      \
			    __cnt > 0; --__cnt)				      \
			 if (putc_unlocked (*__ptr++, __stream) == EOF)	      \
			   break;					      \
		       ((size_t) (size) * (size_t) (n) - __cnt)		      \
			/ (size_t) (size); })				      \
		  : (((__builtin_constant_p (size) && (size_t) (size) == 0)   \
		      || (__builtin_constant_p (n) && (size_t) (n) == 0))     \
			/* Evaluate all parameters once.  */		      \
		     ? ((void) (ptr), (void) (stream), (void) (size),	      \
			(void) (n), (size_t) 0)				      \
		     : fwrite_unlocked (ptr, size, n, stream))))
#endif
# 191 "/usr/include/x86_64-linux-gnu/bits/stdio.h" 3 4

/* Define helper macro.  */
#undef __STDIO_INLINE

#endif /* bits/stdio.h.  */
# 196 "/usr/include/x86_64-linux-gnu/bits/stdio.h" 3 4
#endif /* stdio.h expanded by -frewrite-includes */
# 865 "/usr/include/stdio.h" 2 3 4
#endif
# 866 "/usr/include/stdio.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#if __USE_FORTIFY_LEVEL > 0 && defined __fortify_function
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 867 "/usr/include/stdio.h" 3 4
#if 0 /* expanded by -frewrite-includes */
# include <bits/stdio2.h>
#endif /* expanded by -frewrite-includes */
# 867 "/usr/include/stdio.h" 3 4
# 868 "/usr/include/stdio.h" 3 4
#endif
# 869 "/usr/include/stdio.h" 3 4
#ifdef __LDBL_COMPAT
#if 0 /* expanded by -frewrite-includes */
# include <bits/stdio-ldbl.h>
#endif /* expanded by -frewrite-includes */
# 870 "/usr/include/stdio.h" 3 4
# 871 "/usr/include/stdio.h" 3 4
#endif
# 872 "/usr/include/stdio.h" 3 4

__END_DECLS

#endif /* <stdio.h> included.  */
# 876 "/usr/include/stdio.h" 3 4
#endif /* stdio.h expanded by -frewrite-includes */
# 2 "driver.c" 2
int a[][1][15], c[][1][15], e[][1][15], f[][1][15], g[][1][15], m[][1][15],
    n[][1][15], p[][1][15], q[][1][15];
short b[];
int d[];
short w[][1][15], i[][1][15], j[][1][15], k[][1][15], l[][1][15], r[][1][15];
int x[][15][15], y[][15][15];
_Bool o[][1][1];
_Bool s[][1];
void z() {
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        a[t][u][v] = 6;
  for (size_t t = 0; t < 5; ++t)
    b[t] = 8;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        c[t][u][v] = 70;
  for (size_t t = 0; t < 5; ++t)
    d[t] = 400431475742631619;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        e[t][u][v] = 30;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        f[t][u][v] = 80048846733619458;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        g[t][u][v] = 3;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        w[t][u][v] = 2;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        i[t][u][v] = 9;
  for (size_t t = 0; t < 5;)
    for (size_t u = 0; u < 5; ++u)
      for (t = 0; t < 15; ++t)
        for (size_t v = 0; v < 15; ++v)
          x[t][u][v] = 9;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        j[t][u][v] = 5;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        k[t][u][v] = 7;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        l[t][u][v] = 5;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        m[t][u][v] = 901674895436748;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        n[t][u][v] = 6018762;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 5; ++u)
      for (size_t v = 0; v < 5; ++v)
        o[t][u][v] = 0;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        p[t][u][v] = 801932439917686;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        q[t][u][v] = 6;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        r[t][u][v] = 9;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 5; ++u)
      s[t][u] = 1;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        y[t][u][v] = 2;
}
