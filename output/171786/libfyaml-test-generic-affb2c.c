# 1 "<built-in>"
# 1 "/home/panto/work/libfyaml/test/libfyaml-test-generic.c"
/*
 * libfyaml-test-generic.c - libfyaml generics tests
 *
 * Copyright (c) 2023 Pantelis Antoniou <pantelis.antoniou@konsulko.com>
 *
 * SPDX-License-Identifier: MIT
 */

#ifdef HAVE_CONFIG_H
#if 0 /* expanded by -frewrite-includes */
#include "config.h"
#endif /* expanded by -frewrite-includes */
# 10 "/home/panto/work/libfyaml/test/libfyaml-test-generic.c"
# 11 "/home/panto/work/libfyaml/test/libfyaml-test-generic.c"
#endif
# 12 "/home/panto/work/libfyaml/test/libfyaml-test-generic.c"

#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* stdlib.h expanded by -frewrite-includes */
#include <stdlib.h>
#else /* stdlib.h expanded by -frewrite-includes */
# 13 "/home/panto/work/libfyaml/test/libfyaml-test-generic.c"
# 1 "/usr/include/stdlib.h" 1 3 4
/* Copyright (C) 1991-2024 Free Software Foundation, Inc.
   Copyright The GNU Toolchain Authors.
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
 *	ISO C99 Standard: 7.20 General utilities	<stdlib.h>
 */

#ifndef	_STDLIB_H

#define __GLIBC_INTERNAL_STARTING_HEADER_IMPLEMENTATION
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* libc-header-start.h expanded by -frewrite-includes */
#include <bits/libc-header-start.h>
#else /* libc-header-start.h expanded by -frewrite-includes */
# 26 "/usr/include/stdlib.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 1 3 4
/* Handle feature test macros at the start of a header.
   Copyright (C) 2016-2024 Free Software Foundation, Inc.
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
/* Copyright (C) 1991-2024 Free Software Foundation, Inc.
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
   __STDC_WANT_IEC_60559_EXT__
			ISO C2X interfaces defined only in Annex F.

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
   _DYNAMIC_STACK_SIZE_SOURCE Select correct (but non compile-time constant)
			MINSIGSTKSZ, SIGSTKSZ and PTHREAD_STACK_MIN.
   _GNU_SOURCE		All of the above, plus GNU extensions.
   _DEFAULT_SOURCE	The default set of features (taking precedence over
			__STRICT_ANSI__).

   _FORTIFY_SOURCE	Add security hardening to many library functions.
			Set to 1, 2 or 3; 3 performs stricter checks than 2, which
			performs stricter checks than 1.

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
   __USE_DYNAMIC_STACK_SIZE Define correct (but non compile-time constant)
			MINSIGSTKSZ, SIGSTKSZ and PTHREAD_STACK_MIN.
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
#undef	__USE_DYNAMIC_STACK_SIZE
#undef	__USE_GNU
#undef	__USE_FORTIFY_LEVEL
#undef	__KERNEL_STRICT_NAMES
#undef	__GLIBC_USE_ISOC2X
#undef	__GLIBC_USE_DEPRECATED_GETS
#undef	__GLIBC_USE_DEPRECATED_SCANF
#undef	__GLIBC_USE_C2X_STRTOL

/* Suppress kernel-name space pollution unless user expressedly asks
   for it.  */
#ifndef _LOOSE_KERNEL_NAMES
# define __KERNEL_STRICT_NAMES
#endif
# 161 "/usr/include/features.h" 3 4

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
# 170 "/usr/include/features.h" 3 4
# define __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#else
# 173 "/usr/include/features.h" 3 4
# define __GNUC_PREREQ(maj, min) 0
#endif
# 175 "/usr/include/features.h" 3 4

/* Similarly for clang.  Features added to GCC after version 4.2 may
   or may not also be available in clang, and clang's definitions of
   __GNUC(_MINOR)__ are fixed at 4 and 2 respectively.  Not all such
   features can be queried via __has_extension/__has_feature.  */
#if 0 /* disabled by -frewrite-includes */
#if defined __clang_major__ && defined __clang_minor__
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 181 "/usr/include/features.h" 3 4
# define __glibc_clang_prereq(maj, min) \
  ((__clang_major__ << 16) + __clang_minor__ >= ((maj) << 16) + (min))
#else
# 184 "/usr/include/features.h" 3 4
# define __glibc_clang_prereq(maj, min) 0
#endif
# 186 "/usr/include/features.h" 3 4

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
# 196 "/usr/include/features.h" 3 4
# warning "_BSD_SOURCE and _SVID_SOURCE are deprecated, use _DEFAULT_SOURCE"
# undef  _DEFAULT_SOURCE
# define _DEFAULT_SOURCE	1
#endif
# 200 "/usr/include/features.h" 3 4

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
# undef  _DYNAMIC_STACK_SIZE_SOURCE
# define _DYNAMIC_STACK_SIZE_SOURCE 1
#endif
# 228 "/usr/include/features.h" 3 4

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
# 237 "/usr/include/features.h" 3 4
# undef  _DEFAULT_SOURCE
# define _DEFAULT_SOURCE	1
#endif
# 240 "/usr/include/features.h" 3 4

/* This is to enable the ISO C2X extension.  */
#if 0 /* disabled by -frewrite-includes */
#if (defined _ISOC2X_SOURCE \
     || (defined __STDC_VERSION__ && __STDC_VERSION__ > 201710L))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 244 "/usr/include/features.h" 3 4
# define __GLIBC_USE_ISOC2X	1
#else
# 246 "/usr/include/features.h" 3 4
# define __GLIBC_USE_ISOC2X	0
#endif
# 248 "/usr/include/features.h" 3 4

/* This is to enable the ISO C11 extension.  */
#if 0 /* disabled by -frewrite-includes */
#if (defined _ISOC11_SOURCE || defined _ISOC2X_SOURCE \
     || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 201112L))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 252 "/usr/include/features.h" 3 4
# define __USE_ISOC11	1
#endif
# 254 "/usr/include/features.h" 3 4

/* This is to enable the ISO C99 extension.  */
#if 0 /* disabled by -frewrite-includes */
#if (defined _ISOC99_SOURCE || defined _ISOC11_SOURCE			\
     || defined _ISOC2X_SOURCE						\
     || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 259 "/usr/include/features.h" 3 4
# define __USE_ISOC99	1
#endif
# 261 "/usr/include/features.h" 3 4

/* This is to enable the ISO C90 Amendment 1:1995 extension.  */
#if 0 /* disabled by -frewrite-includes */
#if (defined _ISOC99_SOURCE || defined _ISOC11_SOURCE			\
     || defined _ISOC2X_SOURCE						\
     || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 199409L))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 266 "/usr/include/features.h" 3 4
# define __USE_ISOC95	1
#endif
# 268 "/usr/include/features.h" 3 4

#ifdef __cplusplus
/* This is to enable compatibility for ISO C++17.  */
#if 0 /* disabled by -frewrite-includes */
# if __cplusplus >= 201703L
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 272 "/usr/include/features.h" 3 4
#  define __USE_ISOC11	1
# endif
# 274 "/usr/include/features.h" 3 4
/* This is to enable compatibility for ISO C++11.
   Check the temporary macro for now, too.  */
#if 0 /* disabled by -frewrite-includes */
# if __cplusplus >= 201103L || defined __GXX_EXPERIMENTAL_CXX0X__
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 277 "/usr/include/features.h" 3 4
#  define __USE_ISOCXX11	1
#  define __USE_ISOC99	1
# endif
# 280 "/usr/include/features.h" 3 4
#endif
# 281 "/usr/include/features.h" 3 4

/* If none of the ANSI/POSIX macros are defined, or if _DEFAULT_SOURCE
   is defined, use POSIX.1-2008 (or another version depending on
   _XOPEN_SOURCE).  */
#ifdef _DEFAULT_SOURCE
#if 0 /* disabled by -frewrite-includes */
# if !defined _POSIX_SOURCE && !defined _POSIX_C_SOURCE
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 287 "/usr/include/features.h" 3 4
#  define __USE_POSIX_IMPLICITLY	1
# endif
# 289 "/usr/include/features.h" 3 4
# undef  _POSIX_SOURCE
# define _POSIX_SOURCE	1
# undef  _POSIX_C_SOURCE
# define _POSIX_C_SOURCE	200809L
#endif
# 294 "/usr/include/features.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if ((!defined __STRICT_ANSI__					\
      || (defined _XOPEN_SOURCE && (_XOPEN_SOURCE - 0) >= 500))	\
     && !defined _POSIX_SOURCE && !defined _POSIX_C_SOURCE)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 298 "/usr/include/features.h" 3 4
# define _POSIX_SOURCE	1
#if 0 /* disabled by -frewrite-includes */
# if defined _XOPEN_SOURCE && (_XOPEN_SOURCE - 0) < 500
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 300 "/usr/include/features.h" 3 4
#  define _POSIX_C_SOURCE	2
#if 0 /* disabled by -frewrite-includes */
#if 0
# elif defined _XOPEN_SOURCE && (_XOPEN_SOURCE - 0) < 600
#endif
#endif /* disabled by -frewrite-includes */
#elif 0 /* evaluated by -frewrite-includes */
# 302 "/usr/include/features.h" 3 4
#  define _POSIX_C_SOURCE	199506L
#if 0 /* disabled by -frewrite-includes */
#if 0
# elif defined _XOPEN_SOURCE && (_XOPEN_SOURCE - 0) < 700
#endif
#endif /* disabled by -frewrite-includes */
#elif 0 /* evaluated by -frewrite-includes */
# 304 "/usr/include/features.h" 3 4
#  define _POSIX_C_SOURCE	200112L
# else
# 306 "/usr/include/features.h" 3 4
#  define _POSIX_C_SOURCE	200809L
# endif
# 308 "/usr/include/features.h" 3 4
# define __USE_POSIX_IMPLICITLY	1
#endif
# 310 "/usr/include/features.h" 3 4

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
# 319 "/usr/include/features.h" 3 4
# define _POSIX_SOURCE   1
# undef  _POSIX_C_SOURCE
# define _POSIX_C_SOURCE 199506L
#endif
# 323 "/usr/include/features.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if (defined _POSIX_SOURCE					\
     || (defined _POSIX_C_SOURCE && _POSIX_C_SOURCE >= 1)	\
     || defined _XOPEN_SOURCE)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 327 "/usr/include/features.h" 3 4
# define __USE_POSIX	1
#endif
# 329 "/usr/include/features.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined _POSIX_C_SOURCE && _POSIX_C_SOURCE >= 2 || defined _XOPEN_SOURCE
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 331 "/usr/include/features.h" 3 4
# define __USE_POSIX2	1
#endif
# 333 "/usr/include/features.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined _POSIX_C_SOURCE && (_POSIX_C_SOURCE - 0) >= 199309L
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 335 "/usr/include/features.h" 3 4
# define __USE_POSIX199309	1
#endif
# 337 "/usr/include/features.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined _POSIX_C_SOURCE && (_POSIX_C_SOURCE - 0) >= 199506L
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 339 "/usr/include/features.h" 3 4
# define __USE_POSIX199506	1
#endif
# 341 "/usr/include/features.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined _POSIX_C_SOURCE && (_POSIX_C_SOURCE - 0) >= 200112L
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 343 "/usr/include/features.h" 3 4
# define __USE_XOPEN2K		1
# undef __USE_ISOC95
# define __USE_ISOC95		1
# undef __USE_ISOC99
# define __USE_ISOC99		1
#endif
# 349 "/usr/include/features.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined _POSIX_C_SOURCE && (_POSIX_C_SOURCE - 0) >= 200809L
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 351 "/usr/include/features.h" 3 4
# define __USE_XOPEN2K8		1
# undef  _ATFILE_SOURCE
# define _ATFILE_SOURCE	1
#endif
# 355 "/usr/include/features.h" 3 4

#ifdef	_XOPEN_SOURCE
# define __USE_XOPEN	1
#if 0 /* disabled by -frewrite-includes */
# if (_XOPEN_SOURCE - 0) >= 500
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 359 "/usr/include/features.h" 3 4
#  define __USE_XOPEN_EXTENDED	1
#  define __USE_UNIX98	1
#  undef _LARGEFILE_SOURCE
#  define _LARGEFILE_SOURCE	1
#if 0 /* disabled by -frewrite-includes */
#  if (_XOPEN_SOURCE - 0) >= 600
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 364 "/usr/include/features.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#   if (_XOPEN_SOURCE - 0) >= 700
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 365 "/usr/include/features.h" 3 4
#    define __USE_XOPEN2K8	1
#    define __USE_XOPEN2K8XSI	1
#   endif
# 368 "/usr/include/features.h" 3 4
#   define __USE_XOPEN2K	1
#   define __USE_XOPEN2KXSI	1
#   undef __USE_ISOC95
#   define __USE_ISOC95		1
#   undef __USE_ISOC99
#   define __USE_ISOC99		1
#  endif
# 375 "/usr/include/features.h" 3 4
# else
# 376 "/usr/include/features.h" 3 4
#  ifdef _XOPEN_SOURCE_EXTENDED
#   define __USE_XOPEN_EXTENDED	1
#  endif
# 379 "/usr/include/features.h" 3 4
# endif
# 380 "/usr/include/features.h" 3 4
#endif
# 381 "/usr/include/features.h" 3 4

#ifdef _LARGEFILE_SOURCE
# define __USE_LARGEFILE	1
#endif
# 385 "/usr/include/features.h" 3 4

#ifdef _LARGEFILE64_SOURCE
# define __USE_LARGEFILE64	1
#endif
# 389 "/usr/include/features.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined _FILE_OFFSET_BITS && _FILE_OFFSET_BITS == 64
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 391 "/usr/include/features.h" 3 4
# define __USE_FILE_OFFSET64	1
#endif
# 393 "/usr/include/features.h" 3 4

#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* features-time64.h expanded by -frewrite-includes */
#include <features-time64.h>
#else /* features-time64.h expanded by -frewrite-includes */
# 394 "/usr/include/features.h" 3 4
# 1 "/usr/include/features-time64.h" 1 3 4
/* Features part to handle 64-bit time_t support.
   Copyright (C) 2021-2024 Free Software Foundation, Inc.
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

/* We need to know the word size in order to check the time size.  */
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* wordsize.h expanded by -frewrite-includes */
#include <bits/wordsize.h>
#else /* wordsize.h expanded by -frewrite-includes */
# 20 "/usr/include/features-time64.h" 3 4
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
# 21 "/usr/include/features-time64.h" 2 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* timesize.h expanded by -frewrite-includes */
#include <bits/timesize.h>
#else /* timesize.h expanded by -frewrite-includes */
# 21 "/usr/include/features-time64.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 1 3 4
/* Bit size of the time_t type at glibc build time, x86-64 and x32 case.
   Copyright (C) 2018-2024 Free Software Foundation, Inc.
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

#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* wordsize.h expanded by -frewrite-includes */
#include <bits/wordsize.h>
#else /* wordsize.h expanded by -frewrite-includes */
# 19 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 3 4
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
# 20 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 2 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined __x86_64__ && defined __ILP32__
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 22 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 3 4
/* For x32, time is 64-bit even though word size is 32-bit.  */
# define __TIMESIZE	64
#else
# 25 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 3 4
/* For others, time size is word size.  */
# define __TIMESIZE	__WORDSIZE
#endif
# 28 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 3 4
#endif /* timesize.h expanded by -frewrite-includes */
# 22 "/usr/include/features-time64.h" 2 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined _TIME_BITS
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 24 "/usr/include/features-time64.h" 3 4
#if 0 /* disabled by -frewrite-includes */
# if _TIME_BITS == 64
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 25 "/usr/include/features-time64.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#  if ! defined (_FILE_OFFSET_BITS) || _FILE_OFFSET_BITS != 64
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 26 "/usr/include/features-time64.h" 3 4
#   error "_TIME_BITS=64 is allowed only with _FILE_OFFSET_BITS=64"
#if 0 /* disabled by -frewrite-includes */
#if 0
#  elif __TIMESIZE == 32
#endif
#endif /* disabled by -frewrite-includes */
#elif 0 /* evaluated by -frewrite-includes */
# 28 "/usr/include/features-time64.h" 3 4
#   define __USE_TIME_BITS64	1
#  endif
# 30 "/usr/include/features-time64.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#if 0
# elif _TIME_BITS == 32
#endif
#endif /* disabled by -frewrite-includes */
#elif 0 /* evaluated by -frewrite-includes */
# 31 "/usr/include/features-time64.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#  if __TIMESIZE > 32
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 32 "/usr/include/features-time64.h" 3 4
#   error "_TIME_BITS=32 is not compatible with __TIMESIZE > 32"
#  endif
# 34 "/usr/include/features-time64.h" 3 4
# else
# 35 "/usr/include/features-time64.h" 3 4
#  error Invalid _TIME_BITS value (can only be 32 or 64-bit)
# endif
# 37 "/usr/include/features-time64.h" 3 4
#endif
# 38 "/usr/include/features-time64.h" 3 4
#endif /* features-time64.h expanded by -frewrite-includes */
# 395 "/usr/include/features.h" 2 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined _DEFAULT_SOURCE
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 397 "/usr/include/features.h" 3 4
# define __USE_MISC	1
#endif
# 399 "/usr/include/features.h" 3 4

#ifdef	_ATFILE_SOURCE
# define __USE_ATFILE	1
#endif
# 403 "/usr/include/features.h" 3 4

#ifdef	_DYNAMIC_STACK_SIZE_SOURCE
# define __USE_DYNAMIC_STACK_SIZE	1
#endif
# 407 "/usr/include/features.h" 3 4

#ifdef	_GNU_SOURCE
# define __USE_GNU	1
#endif
# 411 "/usr/include/features.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined _FORTIFY_SOURCE && _FORTIFY_SOURCE > 0 \
    && defined __OPTIMIZE__ && __OPTIMIZE__ > 0
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 414 "/usr/include/features.h" 3 4
#if 0 /* disabled by -frewrite-includes */
# if !__GNUC_PREREQ (4, 1)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 415 "/usr/include/features.h" 3 4
#  warning _FORTIFY_SOURCE requires GCC 4.1 or later
#if 0 /* disabled by -frewrite-includes */
#if 0
# elif _FORTIFY_SOURCE > 2 && (__glibc_clang_prereq (9, 0)		      \
			       || __GNUC_PREREQ (12, 0))
#endif
#endif /* disabled by -frewrite-includes */
#elif 0 /* evaluated by -frewrite-includes */
# 418 "/usr/include/features.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#  if _FORTIFY_SOURCE > 3
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 420 "/usr/include/features.h" 3 4
#   warning _FORTIFY_SOURCE > 3 is treated like 3 on this platform
#  endif
# 422 "/usr/include/features.h" 3 4
#  define __USE_FORTIFY_LEVEL 3
#if 0 /* disabled by -frewrite-includes */
#if 0
# elif _FORTIFY_SOURCE > 1
#endif
#endif /* disabled by -frewrite-includes */
#elif 0 /* evaluated by -frewrite-includes */
# 424 "/usr/include/features.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#  if _FORTIFY_SOURCE > 2
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 425 "/usr/include/features.h" 3 4
#   warning _FORTIFY_SOURCE > 2 is treated like 2 on this platform
#  endif
# 427 "/usr/include/features.h" 3 4
#  define __USE_FORTIFY_LEVEL 2
# else
# 429 "/usr/include/features.h" 3 4
#  define __USE_FORTIFY_LEVEL 1
# endif
# 431 "/usr/include/features.h" 3 4
#endif
# 432 "/usr/include/features.h" 3 4
#ifndef __USE_FORTIFY_LEVEL
# define __USE_FORTIFY_LEVEL 0
#endif
# 435 "/usr/include/features.h" 3 4

/* The function 'gets' existed in C89, but is impossible to use
   safely.  It has been removed from ISO C11 and ISO C++14.  Note: for
   compatibility with various implementations of <cstdio>, this test
   must consider only the value of __cplusplus when compiling C++.  */
#if 0 /* disabled by -frewrite-includes */
#if defined __cplusplus ? __cplusplus >= 201402L : defined __USE_ISOC11
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 441 "/usr/include/features.h" 3 4
# define __GLIBC_USE_DEPRECATED_GETS 0
#else
# 443 "/usr/include/features.h" 3 4
# define __GLIBC_USE_DEPRECATED_GETS 1
#endif
# 445 "/usr/include/features.h" 3 4

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
# 462 "/usr/include/features.h" 3 4
# define __GLIBC_USE_DEPRECATED_SCANF 1
#else
# 464 "/usr/include/features.h" 3 4
# define __GLIBC_USE_DEPRECATED_SCANF 0
#endif
# 466 "/usr/include/features.h" 3 4

/* ISO C2X added support for a 0b or 0B prefix on binary constants as
   inputs to strtol-family functions (base 0 or 2).  This macro is
   used to condition redirection in headers to allow that redirection
   to be disabled when building those functions, despite _GNU_SOURCE
   being defined.  */
#if 0 /* disabled by -frewrite-includes */
#if __GLIBC_USE (ISOC2X)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 473 "/usr/include/features.h" 3 4
# define __GLIBC_USE_C2X_STRTOL 1
#else
# 475 "/usr/include/features.h" 3 4
# define __GLIBC_USE_C2X_STRTOL 0
#endif
# 477 "/usr/include/features.h" 3 4

/* Get definitions of __STDC_* predefined macros, if the compiler has
   not preincluded this header automatically.  */
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* stdc-predef.h expanded by -frewrite-includes */
#include <stdc-predef.h>
#else /* stdc-predef.h expanded by -frewrite-includes */
# 480 "/usr/include/features.h" 3 4
# 1 "/usr/include/stdc-predef.h" 1 3 4
/* Copyright (C) 1991-2024 Free Software Foundation, Inc.
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
#  define __STDC_IEC_60559_BFP__ 	201404L
# endif
# 41 "/usr/include/stdc-predef.h" 3 4
#else
# 42 "/usr/include/stdc-predef.h" 3 4
# define __STDC_IEC_559__		1
# define __STDC_IEC_60559_BFP__ 	201404L
#endif
# 45 "/usr/include/stdc-predef.h" 3 4

#ifdef __GCC_IEC_559_COMPLEX
#if 0 /* disabled by -frewrite-includes */
# if __GCC_IEC_559_COMPLEX > 0
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 48 "/usr/include/stdc-predef.h" 3 4
#  define __STDC_IEC_559_COMPLEX__	1
#  define __STDC_IEC_60559_COMPLEX__	201404L
# endif
# 51 "/usr/include/stdc-predef.h" 3 4
#else
# 52 "/usr/include/stdc-predef.h" 3 4
# define __STDC_IEC_559_COMPLEX__	1
# define __STDC_IEC_60559_COMPLEX__	201404L
#endif
# 55 "/usr/include/stdc-predef.h" 3 4

/* wchar_t uses Unicode 10.0.0.  Version 10.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2017, fifth edition, plus
   the following additions from Amendment 1 to the fifth edition:
   - 56 emoji characters
   - 285 hentaigana
   - 3 additional Zanabazar Square characters */
#define __STDC_ISO_10646__		201706L

#endif
# 65 "/usr/include/stdc-predef.h" 3 4
#endif /* stdc-predef.h expanded by -frewrite-includes */
# 481 "/usr/include/features.h" 2 3 4

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
#define	__GLIBC_MINOR__	39

#define __GLIBC_PREREQ(maj, min) \
	((__GLIBC__ << 16) + __GLIBC_MINOR__ >= ((maj) << 16) + (min))

/* This is here only because every header file already includes this one.  */
#ifndef __ASSEMBLER__
# ifndef _SYS_CDEFS_H
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* cdefs.h expanded by -frewrite-includes */
#  include <sys/cdefs.h>
#else /* cdefs.h expanded by -frewrite-includes */
# 502 "/usr/include/features.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 1 3 4
/* Copyright (C) 1992-2024 Free Software Foundation, Inc.
   Copyright The GNU Toolchain Authors.
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
# 24 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# 25 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#endif
# 26 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* The GNU libc does not support any K&R compilers or the traditional mode
   of ISO C compilers anymore.  Check for some of the combinations not
   supported anymore.  */
#if 0 /* disabled by -frewrite-includes */
#if defined __GNUC__ && !defined __STDC__ && !defined __cplusplus
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 31 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# error "You need a ISO C or C++ conforming compiler to use the glibc headers"
#endif
# 33 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* Some user header file might have defined this before.  */
#undef	__P
#undef	__PMT

/* Compilers that lack __has_attribute may object to
       #if defined __has_attribute && __has_attribute (...)
   even though they do not need to evaluate the right-hand side of the &&.
   Similarly for __has_builtin, etc.  */
#if 0 /* disabled by -frewrite-includes */
#if (defined __has_attribute \
     && (!defined __clang_minor__ \
         || 3 < __clang_major__ + (5 <= __clang_minor__)))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 45 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __glibc_has_attribute(attr) __has_attribute (attr)
#else
# 47 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __glibc_has_attribute(attr) 0
#endif
# 49 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#ifdef __has_builtin
# define __glibc_has_builtin(name) __has_builtin (name)
#else
# 52 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __glibc_has_builtin(name) 0
#endif
# 54 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#ifdef __has_extension
# define __glibc_has_extension(ext) __has_extension (ext)
#else
# 57 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __glibc_has_extension(ext) 0
#endif
# 59 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined __GNUC__ || defined __clang__
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 61 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* All functions, except those with callbacks or those that
   synchronize memory, are leaf functions.  */
#if 0 /* disabled by -frewrite-includes */
# if __GNUC_PREREQ (4, 6) && !defined _LIBC
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 65 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __LEAF , __leaf__
#  define __LEAF_ATTR __attribute__ ((__leaf__))
# else
# 68 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __LEAF
#  define __LEAF_ATTR
# endif
# 71 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* GCC can always grok prototypes.  For C++ programs we add throw()
   to help it optimize the function calls.  But this only works with
   gcc 2.8.x and egcs.  For gcc 3.4 and up we even mark C functions
   as non-throwing using a function attribute since programs can use
   the -fexceptions options for C code as well.  */
#if 0 /* disabled by -frewrite-includes */
# if !defined __cplusplus \
     && (__GNUC_PREREQ (3, 4) || __glibc_has_attribute (__nothrow__))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 79 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __THROW	__attribute__ ((__nothrow__ __LEAF))
#  define __THROWNL	__attribute__ ((__nothrow__))
#  define __NTH(fct)	__attribute__ ((__nothrow__ __LEAF)) fct
#  define __NTHNL(fct)  __attribute__ ((__nothrow__)) fct
# else
# 84 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#  if defined __cplusplus && (__GNUC_PREREQ (2,8) || __clang_major__ >= 4)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 85 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#   if __cplusplus >= 201103L
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 86 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#    define __THROW	noexcept (true)
#   else
# 88 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#    define __THROW	throw ()
#   endif
# 90 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#   define __THROWNL	__THROW
#   define __NTH(fct)	__LEAF_ATTR fct __THROW
#   define __NTHNL(fct) fct __THROW
#  else
# 94 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#   define __THROW
#   define __THROWNL
#   define __NTH(fct)	fct
#   define __NTHNL(fct) fct
#  endif
# 99 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# endif
# 100 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if __GNUC_PREREQ (4, 3) || __glibc_has_attribute (__cold__)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 102 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __COLD	__attribute__ ((__cold__))
# else
# 104 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __COLD
# endif
# 106 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#else	/* Not GCC or clang.  */
# 108 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if (defined __cplusplus						\
      || (defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L))
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 111 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __inline	inline
# else
# 113 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __inline		/* No inline functions.  */
# endif
# 115 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

# define __THROW
# define __THROWNL
# define __NTH(fct)	fct
# define __COLD

#endif	/* GCC || clang.  */
# 122 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

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
# 143 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __BEGIN_DECLS
# define __END_DECLS
#endif
# 146 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4


/* Fortify support.  */
#define __bos(ptr) __builtin_object_size (ptr, __USE_FORTIFY_LEVEL > 1)
#define __bos0(ptr) __builtin_object_size (ptr, 0)

/* Use __builtin_dynamic_object_size at _FORTIFY_SOURCE=3 when available.  */
#if 0 /* disabled by -frewrite-includes */
#if __USE_FORTIFY_LEVEL == 3 && (__glibc_clang_prereq (9, 0)		      \
				 || __GNUC_PREREQ (12, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 155 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __glibc_objsize0(__o) __builtin_dynamic_object_size (__o, 0)
# define __glibc_objsize(__o) __builtin_dynamic_object_size (__o, 1)
#else
# 158 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __glibc_objsize0(__o) __bos0 (__o)
# define __glibc_objsize(__o) __bos (__o)
#endif
# 161 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __USE_FORTIFY_LEVEL > 0
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 163 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
/* Compile time conditions to choose between the regular, _chk and _chk_warn
   variants.  These conditions should get evaluated to constant and optimized
   away.  */

#define __glibc_safe_len_cond(__l, __s, __osz) ((__l) <= (__osz) / (__s))
#define __glibc_unsigned_or_positive(__l) \
  ((__typeof (__l)) 0 < (__typeof (__l)) -1				      \
   || (__builtin_constant_p (__l) && (__l) > 0))

/* Length is known to be safe at compile time if the __L * __S <= __OBJSZ
   condition can be folded to a constant and if it is true, or unknown (-1) */
#define __glibc_safe_or_unknown_len(__l, __s, __osz) \
  ((__builtin_constant_p (__osz) && (__osz) == (__SIZE_TYPE__) -1)	      \
   || (__glibc_unsigned_or_positive (__l)				      \
       && __builtin_constant_p (__glibc_safe_len_cond ((__SIZE_TYPE__) (__l), \
						       (__s), (__osz)))	      \
       && __glibc_safe_len_cond ((__SIZE_TYPE__) (__l), (__s), (__osz))))

/* Conversely, we know at compile time that the length is unsafe if the
   __L * __S <= __OBJSZ condition can be folded to a constant and if it is
   false.  */
#define __glibc_unsafe_len(__l, __s, __osz) \
  (__glibc_unsigned_or_positive (__l)					      \
   && __builtin_constant_p (__glibc_safe_len_cond ((__SIZE_TYPE__) (__l),     \
						   __s, __osz))		      \
   && !__glibc_safe_len_cond ((__SIZE_TYPE__) (__l), __s, __osz))

/* Fortify function f.  __f_alias, __f_chk and __f_chk_warn must be
   declared.  */

#define __glibc_fortify(f, __l, __s, __osz, ...) \
  (__glibc_safe_or_unknown_len (__l, __s, __osz)			      \
   ? __ ## f ## _alias (__VA_ARGS__)					      \
   : (__glibc_unsafe_len (__l, __s, __osz)				      \
      ? __ ## f ## _chk_warn (__VA_ARGS__, __osz)			      \
      : __ ## f ## _chk (__VA_ARGS__, __osz)))

/* Fortify function f, where object size argument passed to f is the number of
   elements and not total size.  */

#define __glibc_fortify_n(f, __l, __s, __osz, ...) \
  (__glibc_safe_or_unknown_len (__l, __s, __osz)			      \
   ? __ ## f ## _alias (__VA_ARGS__)					      \
   : (__glibc_unsafe_len (__l, __s, __osz)				      \
      ? __ ## f ## _chk_warn (__VA_ARGS__, (__osz) / (__s))		      \
      : __ ## f ## _chk (__VA_ARGS__, (__osz) / (__s))))
#endif
# 210 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (4,3)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 212 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __warnattr(msg) __attribute__((__warning__ (msg)))
# define __errordecl(name, msg) \
  extern void name (void) __attribute__((__error__ (msg)))
#else
# 216 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __warnattr(msg)
# define __errordecl(name, msg) extern void name (void)
#endif
# 219 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* Support for flexible arrays.
   Headers that should use flexible arrays only if they're "real"
   (e.g. only if they won't affect sizeof()) should test
   #if __glibc_c99_flexarr_available.  */
#if 0 /* disabled by -frewrite-includes */
#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L && !defined __HP_cc
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 225 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __flexarr	[]
# define __glibc_c99_flexarr_available 1
#if 0 /* disabled by -frewrite-includes */
#if 0
#elif __GNUC_PREREQ (2,97) || defined __clang__
#endif
#endif /* disabled by -frewrite-includes */
#elif 0 /* evaluated by -frewrite-includes */
# 228 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
/* GCC 2.97 and clang support C99 flexible array members as an extension,
   even when in C89 mode or compiling C++ (any version).  */
# define __flexarr	[]
# define __glibc_c99_flexarr_available 1
#if 0 /* disabled by -frewrite-includes */
#if 0
#elif defined __GNUC__
#endif
#endif /* disabled by -frewrite-includes */
#elif 0 /* evaluated by -frewrite-includes */
# 233 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
/* Pre-2.97 GCC did not support C99 flexible arrays but did have
   an equivalent extension with slightly different notation.  */
# define __flexarr	[0]
# define __glibc_c99_flexarr_available 1
#else
# 238 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
/* Some other non-C99 compiler.  Approximate with [1].  */
# define __flexarr	[1]
# define __glibc_c99_flexarr_available 0
#endif
# 242 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4


/* __asm__ ("xyz") is used throughout the headers to rename functions
   at the assembly language level.  This is wrapped by the __REDIRECT
   macro, in order to support compilers that can do this some other
   way.  When compilers don't support asm-names at all, we have to do
   preprocessor tricks instead (which don't have exactly the right
   semantics, but it's the best we can do).

   Example:
   int __REDIRECT(setpgrp, (__pid_t pid, __pid_t pgrp), setpgid); */

#if 0 /* disabled by -frewrite-includes */
#if (defined __GNUC__ && __GNUC__ >= 2) || (__clang_major__ >= 4)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 255 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

# define __REDIRECT(name, proto, alias) name proto __asm__ (__ASMNAME (#alias))
# ifdef __cplusplus
#  define __REDIRECT_NTH(name, proto, alias) \
     name proto __THROW __asm__ (__ASMNAME (#alias))
#  define __REDIRECT_NTHNL(name, proto, alias) \
     name proto __THROWNL __asm__ (__ASMNAME (#alias))
# else
# 263 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __REDIRECT_NTH(name, proto, alias) \
     name proto __asm__ (__ASMNAME (#alias)) __THROW
#  define __REDIRECT_NTHNL(name, proto, alias) \
     name proto __asm__ (__ASMNAME (#alias)) __THROWNL
# endif
# 268 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __ASMNAME(cname)  __ASMNAME2 (__USER_LABEL_PREFIX__, cname)
# define __ASMNAME2(prefix, cname) __STRING (prefix) cname

#ifndef __REDIRECT_FORTIFY
#define __REDIRECT_FORTIFY __REDIRECT
#endif
# 274 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#ifndef __REDIRECT_FORTIFY_NTH
#define __REDIRECT_FORTIFY_NTH __REDIRECT_NTH
#endif
# 278 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/*
#elif __SOME_OTHER_COMPILER__

# define __REDIRECT(name, proto, alias) name proto; \
	_Pragma("let " #name " = " #alias)
*/
#endif
# 286 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* GCC and clang have various useful declarations that can be made with
   the '__attribute__' syntax.  All of the ways we use this do fine if
   they are omitted for compilers that don't understand it.  */
#if 0 /* disabled by -frewrite-includes */
#if !(defined __GNUC__ || defined __clang__)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 291 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute__(xyz)	/* Ignore */
#endif
# 293 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* At some point during the gcc 2.96 development the `malloc' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (2,96) || __glibc_has_attribute (__malloc__)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 298 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_malloc__ __attribute__ ((__malloc__))
#else
# 300 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_malloc__ /* Ignore */
#endif
# 302 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* Tell the compiler which arguments to an allocation function
   indicate the size of the allocation.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (4, 3)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 306 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_alloc_size__(params) \
  __attribute__ ((__alloc_size__ params))
#else
# 309 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_alloc_size__(params) /* Ignore.  */
#endif
# 311 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* Tell the compiler which argument to an allocation function
   indicates the alignment of the allocation.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (4, 9) || __glibc_has_attribute (__alloc_align__)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 315 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_alloc_align__(param) \
  __attribute__ ((__alloc_align__ param))
#else
# 318 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_alloc_align__(param) /* Ignore.  */
#endif
# 320 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* At some point during the gcc 2.96 development the `pure' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (2,96) || __glibc_has_attribute (__pure__)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 325 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_pure__ __attribute__ ((__pure__))
#else
# 327 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_pure__ /* Ignore */
#endif
# 329 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* This declaration tells the compiler that the value is constant.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (2,5) || __glibc_has_attribute (__const__)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 332 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_const__ __attribute__ ((__const__))
#else
# 334 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_const__ /* Ignore */
#endif
# 336 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (2,7) || __glibc_has_attribute (__unused__)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 338 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_maybe_unused__ __attribute__ ((__unused__))
#else
# 340 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_maybe_unused__ /* Ignore */
#endif
# 342 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* At some point during the gcc 3.1 development the `used' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (3,1) || __glibc_has_attribute (__used__)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 347 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_used__ __attribute__ ((__used__))
# define __attribute_noinline__ __attribute__ ((__noinline__))
#else
# 350 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_used__ __attribute__ ((__unused__))
# define __attribute_noinline__ /* Ignore */
#endif
# 353 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* Since version 3.2, gcc allows marking deprecated functions.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (3,2) || __glibc_has_attribute (__deprecated__)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 356 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_deprecated__ __attribute__ ((__deprecated__))
#else
# 358 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_deprecated__ /* Ignore */
#endif
# 360 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* Since version 4.5, gcc also allows one to specify the message printed
   when a deprecated function is used.  clang claims to be gcc 4.2, but
   may also support this feature.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (4,5) \
    || __glibc_has_extension (__attribute_deprecated_with_message__)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 366 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_deprecated_msg__(msg) \
	 __attribute__ ((__deprecated__ (msg)))
#else
# 369 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_deprecated_msg__(msg) __attribute_deprecated__
#endif
# 371 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* At some point during the gcc 2.8 development the `format_arg' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.
   If several `format_arg' attributes are given for the same function, in
   gcc-3.0 and older, all but the last one are ignored.  In newer gccs,
   all designated arguments are considered.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (2,8) || __glibc_has_attribute (__format_arg__)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 379 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_format_arg__(x) __attribute__ ((__format_arg__ (x)))
#else
# 381 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_format_arg__(x) /* Ignore */
#endif
# 383 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* At some point during the gcc 2.97 development the `strfmon' format
   attribute for functions was introduced.  We don't want to use it
   unconditionally (although this would be possible) since it
   generates warnings.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (2,97) || __glibc_has_attribute (__format__)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 389 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_format_strfmon__(a,b) \
  __attribute__ ((__format__ (__strfmon__, a, b)))
#else
# 392 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_format_strfmon__(a,b) /* Ignore */
#endif
# 394 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* The nonnull function attribute marks pointer parameters that
   must not be NULL.  This has the name __nonnull in glibc,
   and __attribute_nonnull__ in files shared with Gnulib to avoid
   collision with a different __nonnull in DragonFlyBSD 5.9.  */
#ifndef __attribute_nonnull__
#if 0 /* disabled by -frewrite-includes */
# if __GNUC_PREREQ (3,3) || __glibc_has_attribute (__nonnull__)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 401 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __attribute_nonnull__(params) __attribute__ ((__nonnull__ params))
# else
# 403 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __attribute_nonnull__(params)
# endif
# 405 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#endif
# 406 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#ifndef __nonnull
# define __nonnull(params) __attribute_nonnull__ (params)
#endif
# 409 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* The returns_nonnull function attribute marks the return type of the function
   as always being non-null.  */
#ifndef __returns_nonnull
#if 0 /* disabled by -frewrite-includes */
# if __GNUC_PREREQ (4, 9) || __glibc_has_attribute (__returns_nonnull__)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 414 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __returns_nonnull __attribute__ ((__returns_nonnull__))
# else
# 416 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __returns_nonnull
# endif
# 418 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#endif
# 419 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* If fortification mode, we warn about unused results of certain
   function calls which can lead to problems.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (3,4) || __glibc_has_attribute (__warn_unused_result__)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 423 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_warn_unused_result__ \
   __attribute__ ((__warn_unused_result__))
#if 0 /* disabled by -frewrite-includes */
# if defined __USE_FORTIFY_LEVEL && __USE_FORTIFY_LEVEL > 0
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 426 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __wur __attribute_warn_unused_result__
# endif
# 428 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#else
# 429 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_warn_unused_result__ /* empty */
#endif
# 431 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#ifndef __wur
# define __wur /* Ignore */
#endif
# 434 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* Forces a function to be always inlined.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (3,2) || __glibc_has_attribute (__always_inline__)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 437 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
/* The Linux kernel defines __always_inline in stddef.h (283d7573), and
   it conflicts with this definition.  Therefore undefine it first to
   allow either header to be included first.  */
# undef __always_inline
# define __always_inline __inline __attribute__ ((__always_inline__))
#else
# 443 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# undef __always_inline
# define __always_inline __inline
#endif
# 446 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* Associate error messages with the source location of the call site rather
   than with the source location inside the function.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (4,3) || __glibc_has_attribute (__artificial__)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 450 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_artificial__ __attribute__ ((__artificial__))
#else
# 452 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_artificial__ /* Ignore */
#endif
# 454 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

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
# 467 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#if 0 /* disabled by -frewrite-includes */
# if defined __GNUC_STDC_INLINE__ || defined __cplusplus
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 468 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __extern_inline extern __inline __attribute__ ((__gnu_inline__))
#  define __extern_always_inline \
  extern __always_inline __attribute__ ((__gnu_inline__))
# else
# 472 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __extern_inline extern __inline
#  define __extern_always_inline extern __always_inline
# endif
# 475 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#endif
# 476 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#ifdef __extern_always_inline
# define __fortify_function __extern_always_inline __attribute_artificial__
#endif
# 480 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* GCC 4.3 and above allow passing all anonymous arguments of an
   __extern_always_inline function to some other vararg function.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (4,3)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 484 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __va_arg_pack() __builtin_va_arg_pack ()
# define __va_arg_pack_len() __builtin_va_arg_pack_len ()
#endif
# 487 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* It is possible to compile containing GCC extensions even if GCC is
   run in pedantic mode if the uses are carefully marked using the
   `__extension__' keyword.  But this is not generally available before
   version 2.8.  */
#if 0 /* disabled by -frewrite-includes */
#if !(__GNUC_PREREQ (2,8) || defined __clang__)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 493 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __extension__		/* Ignore */
#endif
# 495 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* __restrict is known in EGCS 1.2 and above, and in clang.
   It works also in C++ mode (outside of arrays), but only when spelled
   as '__restrict', not 'restrict'.  */
#if 0 /* disabled by -frewrite-includes */
#if !(__GNUC_PREREQ (2,92) || __clang_major__ >= 3)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 500 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#if 0 /* disabled by -frewrite-includes */
# if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 501 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __restrict	restrict
# else
# 503 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __restrict	/* Ignore */
# endif
# 505 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#endif
# 506 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* ISO C99 also allows to declare arrays as non-overlapping.  The syntax is
     array_name[restrict]
   GCC 3.1 and clang support this.
   This syntax is not usable in C++ mode.  */
#if 0 /* disabled by -frewrite-includes */
#if (__GNUC_PREREQ (3,1) || __clang_major__ >= 3) && !defined __cplusplus
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 512 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __restrict_arr	__restrict
#else
# 514 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# ifdef __GNUC__
#  define __restrict_arr	/* Not supported in old GCC.  */
# else
# 517 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#  if defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 518 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#   define __restrict_arr	restrict
#  else
# 520 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
/* Some other non-C99 compiler.  */
#   define __restrict_arr	/* Not supported.  */
#  endif
# 523 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# endif
# 524 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#endif
# 525 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if (__GNUC__ >= 3) || __glibc_has_builtin (__builtin_expect)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 527 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __glibc_unlikely(cond)	__builtin_expect ((cond), 0)
# define __glibc_likely(cond)	__builtin_expect ((cond), 1)
#else
# 530 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __glibc_unlikely(cond)	(cond)
# define __glibc_likely(cond)	(cond)
#endif
# 533 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if (!defined _Noreturn \
     && (defined __STDC_VERSION__ ? __STDC_VERSION__ : 0) < 201112 \
     &&  !(__GNUC_PREREQ (4,7) \
           || (3 < __clang_major__ + (5 <= __clang_minor__))))
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 538 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#if 0 /* disabled by -frewrite-includes */
# if __GNUC_PREREQ (2,8)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 539 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define _Noreturn __attribute__ ((__noreturn__))
# else
# 541 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define _Noreturn
# endif
# 543 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#endif
# 544 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (8, 0)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 546 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
/* Describes a char array whose address can safely be passed as the first
   argument to strncpy and strncat, as the char array is not necessarily
   a NUL-terminated string.  */
# define __attribute_nonstring__ __attribute__ ((__nonstring__))
#else
# 551 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_nonstring__
#endif
# 553 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* Undefine (also defined in libc-symbols.h).  */
#undef __attribute_copy__
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (9, 0)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 557 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
/* Copies attributes from the declaration or type referenced by
   the argument.  */
# define __attribute_copy__(arg) __attribute__ ((__copy__ (arg)))
#else
# 561 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_copy__(arg)
#endif
# 563 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if (!defined _Static_assert && !defined __cplusplus \
     && (defined __STDC_VERSION__ ? __STDC_VERSION__ : 0) < 201112 \
     && (!(__GNUC_PREREQ (4, 6) || __clang_major__ >= 4) \
         || defined __STRICT_ANSI__))
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 568 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define _Static_assert(expr, diagnostic) \
    extern int (*__Static_assert_function (void)) \
      [!!sizeof (struct { int __error_if_negative: (expr) ? 2 : -1; })]
#endif
# 572 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* Gnulib avoids including these, as they don't work on non-glibc or
   older glibc platforms.  */
#ifndef __GNULIB_CDEFS
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* wordsize.h expanded by -frewrite-includes */
# include <bits/wordsize.h>
#else /* wordsize.h expanded by -frewrite-includes */
# 576 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
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
# 577 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 2 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* long-double.h expanded by -frewrite-includes */
# include <bits/long-double.h>
#else /* long-double.h expanded by -frewrite-includes */
# 577 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/long-double.h" 1 3 4
/* Properties of long double type.  ldbl-96 version.
   Copyright (C) 2016-2024 Free Software Foundation, Inc.
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
#define __LDOUBLE_REDIRECTS_TO_FLOAT128_ABI 0
#endif /* long-double.h expanded by -frewrite-includes */
# 578 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 2 3 4
#endif
# 579 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __LDOUBLE_REDIRECTS_TO_FLOAT128_ABI == 1
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 581 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# ifdef __REDIRECT

/* Alias name defined automatically.  */
#  define __LDBL_REDIR(name, proto) ... unused__ldbl_redir
#  define __LDBL_REDIR_DECL(name) \
  extern __typeof (name) name __asm (__ASMNAME ("__" #name "ieee128"));
#  define __REDIRECT_LDBL(name, proto, alias) \
  name proto __asm (__ASMNAME ("__" #alias "ieee128"))

/* Alias name defined automatically, with leading underscores.  */
#  define __LDBL_REDIR2_DECL(name) \
  extern __typeof (__##name) __##name \
    __asm (__ASMNAME ("__" #name "ieee128"));

/* Alias name defined manually.  */
#  define __LDBL_REDIR1(name, proto, alias) ... unused__ldbl_redir1
#  define __LDBL_REDIR1_DECL(name, alias) \
  extern __typeof (name) name __asm (__ASMNAME (#alias));

#  define __LDBL_REDIR1_NTH(name, proto, alias) \
  __REDIRECT_NTH (name, proto, alias)
#  define __REDIRECT_NTH_LDBL(name, proto, alias) \
  __LDBL_REDIR1_NTH (name, proto, __##alias##ieee128)

/* Unused.  */
#  define __LDBL_REDIR_NTH(name, proto) ... unused__ldbl_redir_nth

# else
# 609 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
_Static_assert (0, "IEEE 128-bits long double requires redirection on this platform");
# endif
# 611 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#if 0
#elif defined __LONG_DOUBLE_MATH_OPTIONAL && defined __NO_LONG_DOUBLE_MATH
#endif
#endif /* disabled by -frewrite-includes */
#elif 0 /* evaluated by -frewrite-includes */
# 612 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __LDBL_COMPAT 1
# ifdef __REDIRECT
#  define __LDBL_REDIR1(name, proto, alias) __REDIRECT (name, proto, alias)
#  define __LDBL_REDIR(name, proto) \
  __LDBL_REDIR1 (name, proto, __nldbl_##name)
#  define __LDBL_REDIR1_NTH(name, proto, alias) __REDIRECT_NTH (name, proto, alias)
#  define __LDBL_REDIR_NTH(name, proto) \
  __LDBL_REDIR1_NTH (name, proto, __nldbl_##name)
#  define __LDBL_REDIR2_DECL(name) \
  extern __typeof (__##name) __##name __asm (__ASMNAME ("__nldbl___" #name));
#  define __LDBL_REDIR1_DECL(name, alias) \
  extern __typeof (name) name __asm (__ASMNAME (#alias));
#  define __LDBL_REDIR_DECL(name) \
  extern __typeof (name) name __asm (__ASMNAME ("__nldbl_" #name));
#  define __REDIRECT_LDBL(name, proto, alias) \
  __LDBL_REDIR1 (name, proto, __nldbl_##alias)
#  define __REDIRECT_NTH_LDBL(name, proto, alias) \
  __LDBL_REDIR1_NTH (name, proto, __nldbl_##alias)
# endif
# 631 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#endif
# 632 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#if (!defined __LDBL_COMPAT && __LDOUBLE_REDIRECTS_TO_FLOAT128_ABI == 0) \
    || !defined __REDIRECT
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 634 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __LDBL_REDIR1(name, proto, alias) name proto
# define __LDBL_REDIR(name, proto) name proto
# define __LDBL_REDIR1_NTH(name, proto, alias) name proto __THROW
# define __LDBL_REDIR_NTH(name, proto) name proto __THROW
# define __LDBL_REDIR2_DECL(name)
# define __LDBL_REDIR_DECL(name)
# ifdef __REDIRECT
#  define __REDIRECT_LDBL(name, proto, alias) __REDIRECT (name, proto, alias)
#  define __REDIRECT_NTH_LDBL(name, proto, alias) \
  __REDIRECT_NTH (name, proto, alias)
# endif
# 645 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#endif
# 646 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* __glibc_macro_warning (MESSAGE) issues warning MESSAGE.  This is
   intended for use in preprocessor macros.

   Note: MESSAGE must be a _single_ string; concatenation of string
   literals is not supported.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (4,8) || __glibc_clang_prereq (3,5)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 653 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __glibc_macro_warning1(message) _Pragma (#message)
# define __glibc_macro_warning(message) \
  __glibc_macro_warning1 (GCC warning message)
#else
# 657 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __glibc_macro_warning(msg)
#endif
# 659 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

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
	|| __glibc_has_extension (c_generic_selections) \
	|| (!defined __GNUC__ && defined __STDC_VERSION__ \
	    && __STDC_VERSION__ >= 201112L))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 672 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __HAVE_GENERIC_SELECTION 1
#else
# 674 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __HAVE_GENERIC_SELECTION 0
#endif
# 676 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (10, 0)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 678 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
/* Designates a 1-based positional argument ref-index of pointer type
   that can be used to access size-index elements of the pointed-to
   array according to access mode, or at least one element when
   size-index is not provided:
     access (access-mode, <ref-index> [, <size-index>])  */
#  define __attr_access(x) __attribute__ ((__access__ x))
/* For _FORTIFY_SOURCE == 3 we use __builtin_dynamic_object_size, which may
   use the access attribute to get object sizes from function definition
   arguments, so we can't use them on functions we fortify.  Drop the object
   size hints for such functions.  */
#if 0 /* disabled by -frewrite-includes */
#  if __USE_FORTIFY_LEVEL == 3
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 689 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#    define __fortified_attr_access(a, o, s) __attribute__ ((__access__ (a, o)))
#  else
# 691 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#    define __fortified_attr_access(a, o, s) __attr_access ((a, o, s))
#  endif
# 693 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#  if __GNUC_PREREQ (11, 0)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 694 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#    define __attr_access_none(argno) __attribute__ ((__access__ (__none__, argno)))
#  else
# 696 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#    define __attr_access_none(argno)
#  endif
# 698 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#else
# 699 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#  define __fortified_attr_access(a, o, s)
#  define __attr_access(x)
#  define __attr_access_none(argno)
#endif
# 703 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (11, 0)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 705 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
/* Designates dealloc as a function to call to deallocate objects
   allocated by the declared function.  */
# define __attr_dealloc(dealloc, argno) \
    __attribute__ ((__malloc__ (dealloc, argno)))
# define __attr_dealloc_free __attr_dealloc (__builtin_free, 1)
#else
# 711 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attr_dealloc(dealloc, argno)
# define __attr_dealloc_free
#endif
# 714 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

/* Specify that a function such as setjmp or vfork may return
   twice.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (4, 1)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 718 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_returns_twice__ __attribute__ ((__returns_twice__))
#else
# 720 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# define __attribute_returns_twice__ /* Ignore.  */
#endif
# 722 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4

#endif	 /* sys/cdefs.h */
# 724 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
#endif /* cdefs.h expanded by -frewrite-includes */
# 503 "/usr/include/features.h" 2 3 4
# endif
# 504 "/usr/include/features.h" 3 4

/* If we don't have __REDIRECT, prototypes will be missing if
   __USE_FILE_OFFSET64 but not __USE_LARGEFILE[64]. */
#if 0 /* disabled by -frewrite-includes */
# if defined __USE_FILE_OFFSET64 && !defined __REDIRECT
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 508 "/usr/include/features.h" 3 4
#  define __USE_LARGEFILE	1
#  define __USE_LARGEFILE64	1
# endif
# 511 "/usr/include/features.h" 3 4

#endif	/* !ASSEMBLER */
# 513 "/usr/include/features.h" 3 4

/* Decide whether we can define 'extern inline' functions in headers.  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (2, 7) && defined __OPTIMIZE__ \
    && !defined __OPTIMIZE_SIZE__ && !defined __NO_INLINE__ \
    && defined __extern_inline
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 518 "/usr/include/features.h" 3 4
# define __USE_EXTERN_INLINES	1
#endif
# 520 "/usr/include/features.h" 3 4


/* This is here only because every header file already includes this one.
   Get the definitions of all the appropriate `__stub_FUNCTION' symbols.
   <gnu/stubs.h> contains `#define __stub_FUNCTION' when FUNCTION is a stub
   that will always return failure (and set errno to ENOSYS).  */
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* stubs.h expanded by -frewrite-includes */
#include <gnu/stubs.h>
#else /* stubs.h expanded by -frewrite-includes */
# 526 "/usr/include/features.h" 3 4
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
#define __stub_revoke
#define __stub_setlogin
#define __stub_sigreturn
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
# 527 "/usr/include/features.h" 2 3 4


#endif	/* features.h  */
# 530 "/usr/include/features.h" 3 4
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
#if 1 /* evaluated by -frewrite-includes */
# 40 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_LIB_EXT2 1
#else
# 42 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_LIB_EXT2 0
#endif
# 44 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4

/* ISO/IEC TS 18661-1:2014 defines the __STDC_WANT_IEC_60559_BFP_EXT__
   macro.  Most but not all symbols enabled by that macro in TS
   18661-1 are enabled unconditionally in C2X.  In C2X, the symbols in
   Annex F still require a new feature test macro
   __STDC_WANT_IEC_60559_EXT__ instead (C2X does not define
   __STDC_WANT_IEC_60559_BFP_EXT__), while a few features from TS
   18661-1 are not included in C2X (and thus should depend on
   __STDC_WANT_IEC_60559_BFP_EXT__ even when C2X features are
   enabled).

   __GLIBC_USE (IEC_60559_BFP_EXT) controls those features from TS
   18661-1 not included in C2X.

   __GLIBC_USE (IEC_60559_BFP_EXT_C2X) controls those features from TS
   18661-1 that are also included in C2X (with no feature test macro
   required in C2X).

   __GLIBC_USE (IEC_60559_EXT) controls those features from TS 18661-1
   that are included in C2X but conditional on
   __STDC_WANT_IEC_60559_EXT__.  (There are currently no features
   conditional on __STDC_WANT_IEC_60559_EXT__ that are not in TS
   18661-1.)  */
#undef __GLIBC_USE_IEC_60559_BFP_EXT
#if 0 /* disabled by -frewrite-includes */
#if defined __USE_GNU || defined __STDC_WANT_IEC_60559_BFP_EXT__
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 69 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_IEC_60559_BFP_EXT 1
#else
# 71 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_IEC_60559_BFP_EXT 0
#endif
# 73 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
#undef __GLIBC_USE_IEC_60559_BFP_EXT_C2X
#if 0 /* disabled by -frewrite-includes */
#if __GLIBC_USE (IEC_60559_BFP_EXT) || __GLIBC_USE (ISOC2X)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 75 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_IEC_60559_BFP_EXT_C2X 1
#else
# 77 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_IEC_60559_BFP_EXT_C2X 0
#endif
# 79 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
#undef __GLIBC_USE_IEC_60559_EXT
#if 0 /* disabled by -frewrite-includes */
#if __GLIBC_USE (IEC_60559_BFP_EXT) || defined __STDC_WANT_IEC_60559_EXT__
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 81 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_IEC_60559_EXT 1
#else
# 83 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_IEC_60559_EXT 0
#endif
# 85 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4

/* ISO/IEC TS 18661-4:2015 defines the
   __STDC_WANT_IEC_60559_FUNCS_EXT__ macro.  Other than the reduction
   functions, the symbols from this TS are enabled unconditionally in
   C2X.  */
#undef __GLIBC_USE_IEC_60559_FUNCS_EXT
#if 0 /* disabled by -frewrite-includes */
#if defined __USE_GNU || defined __STDC_WANT_IEC_60559_FUNCS_EXT__
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 92 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_IEC_60559_FUNCS_EXT 1
#else
# 94 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_IEC_60559_FUNCS_EXT 0
#endif
# 96 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
#undef __GLIBC_USE_IEC_60559_FUNCS_EXT_C2X
#if 0 /* disabled by -frewrite-includes */
#if __GLIBC_USE (IEC_60559_FUNCS_EXT) || __GLIBC_USE (ISOC2X)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 98 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_IEC_60559_FUNCS_EXT_C2X 1
#else
# 100 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_IEC_60559_FUNCS_EXT_C2X 0
#endif
# 102 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4

/* ISO/IEC TS 18661-3:2015 defines the
   __STDC_WANT_IEC_60559_TYPES_EXT__ macro.  */
#undef __GLIBC_USE_IEC_60559_TYPES_EXT
#if 0 /* disabled by -frewrite-includes */
#if defined __USE_GNU || defined __STDC_WANT_IEC_60559_TYPES_EXT__
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 107 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_IEC_60559_TYPES_EXT 1
#else
# 109 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
# define __GLIBC_USE_IEC_60559_TYPES_EXT 0
#endif
# 111 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 3 4
#endif /* libc-header-start.h expanded by -frewrite-includes */
# 27 "/usr/include/stdlib.h" 2 3 4

/* Get size_t, wchar_t and NULL from <stddef.h>.  */
#define __need_size_t
#define __need_wchar_t
#define __need_NULL
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* stddef.h expanded by -frewrite-includes */
#include <stddef.h>
#else /* stddef.h expanded by -frewrite-includes */
# 32 "/usr/include/stdlib.h" 3 4
# 1 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 1 3 4
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
 * When clang modules are enabled, this header is a textual header. It ignores
 * its header guard so that multiple submodules can export its interfaces.
 * Take module SM with submodules A and B, whose headers both include stddef.h
 * When SM.A builds, __STDDEF_H will be defined. When SM.B builds, the
 * definition from SM.A will leak when building without local submodule
 * visibility. stddef.h wouldn't include any of its implementation headers, and
 * SM.B wouldn't import any of the stddef modules, and SM.B's `export *`
 * wouldn't export any stddef interfaces as expected. However, since stddef.h
 * ignores its header guard when building with modules, it all works as
 * expected.
 *
 * When clang modules are not enabled, the header guards can function in the
 * normal simple fashion.
 */
#if 0 /* disabled by -frewrite-includes */
#if !defined(__STDDEF_H) || __has_feature(modules) ||                          \
    (defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ >= 1) ||        \
    defined(__need_ptrdiff_t) || defined(__need_size_t) ||                     \
    defined(__need_rsize_t) || defined(__need_wchar_t) ||                      \
    defined(__need_NULL) || defined(__need_nullptr_t) ||                       \
    defined(__need_unreachable) || defined(__need_max_align_t) ||              \
    defined(__need_offsetof) || defined(__need_wint_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 38 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if !defined(__need_ptrdiff_t) && !defined(__need_size_t) &&                   \
    !defined(__need_rsize_t) && !defined(__need_wchar_t) &&                    \
    !defined(__need_NULL) && !defined(__need_nullptr_t) &&                     \
    !defined(__need_unreachable) && !defined(__need_max_align_t) &&            \
    !defined(__need_offsetof) && !defined(__need_wint_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 44 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#define __STDDEF_H
#define __need_ptrdiff_t
#define __need_size_t
/* ISO9899:2011 7.20 (C11 Annex K): Define rsize_t if __STDC_WANT_LIB_EXT1__ is
 * enabled. */
#if 0 /* disabled by -frewrite-includes */
#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ >= 1
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 50 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#define __need_rsize_t
#endif
# 52 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#define __need_wchar_t
#define __need_NULL
#if 0 /* disabled by -frewrite-includes */
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L) ||              \
    defined(__cplusplus)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 56 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#define __need_nullptr_t
#endif
# 58 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 59 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#define __need_unreachable
#endif
# 61 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L) ||              \
    (defined(__cplusplus) && __cplusplus >= 201103L)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 63 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#define __need_max_align_t
#endif
# 65 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#define __need_offsetof
/* wint_t is provided by <wchar.h> and not <stddef.h>. It's here
 * for compatibility, but must be explicitly requested. Therefore
 * __need_wint_t is intentionally not defined here. */
#endif
# 70 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_ptrdiff_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 72 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_ptrdiff_t.h>
#endif /* expanded by -frewrite-includes */
# 72 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
# 73 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#undef __need_ptrdiff_t
#endif /* defined(__need_ptrdiff_t) */
# 75 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_size_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 77 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* __stddef_size_t.h expanded by -frewrite-includes */
#include <__stddef_size_t.h>
#else /* __stddef_size_t.h expanded by -frewrite-includes */
# 77 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
# 1 "/usr/lib/llvm-18/lib/clang/18/include/__stddef_size_t.h" 1 3 4
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
# 16 "/usr/lib/llvm-18/lib/clang/18/include/__stddef_size_t.h" 3 4
#define _SIZE_T

typedef __SIZE_TYPE__ size_t;

#endif
# 21 "/usr/lib/llvm-18/lib/clang/18/include/__stddef_size_t.h" 3 4
#endif /* __stddef_size_t.h expanded by -frewrite-includes */
# 78 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 2 3 4
#undef __need_size_t
#endif /*defined(__need_size_t) */
# 80 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_rsize_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 82 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_rsize_t.h>
#endif /* expanded by -frewrite-includes */
# 82 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
# 83 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#undef __need_rsize_t
#endif /* defined(__need_rsize_t) */
# 85 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_wchar_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 87 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* __stddef_wchar_t.h expanded by -frewrite-includes */
#include <__stddef_wchar_t.h>
#else /* __stddef_wchar_t.h expanded by -frewrite-includes */
# 87 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
# 1 "/usr/lib/llvm-18/lib/clang/18/include/__stddef_wchar_t.h" 1 3 4
/*===---- __stddef_wchar.h - Definition of wchar_t -------------------------===
 *
 * Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
 * See https://llvm.org/LICENSE.txt for license information.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 *
 *===-----------------------------------------------------------------------===
 */

#if 0 /* disabled by -frewrite-includes */
#if !defined(__cplusplus) || (defined(_MSC_VER) && !_NATIVE_WCHAR_T_DEFINED)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 11 "/usr/lib/llvm-18/lib/clang/18/include/__stddef_wchar_t.h" 3 4

/*
 * When -fbuiltin-headers-in-system-modules is set this is a non-modular header
 * and needs to behave as if it was textual.
 */
#if 0 /* disabled by -frewrite-includes */
#if !defined(_WCHAR_T) ||                                                      \
    (__has_feature(modules) && !__building_module(_Builtin_stddef))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 18 "/usr/lib/llvm-18/lib/clang/18/include/__stddef_wchar_t.h" 3 4
#define _WCHAR_T

#ifdef _MSC_EXTENSIONS
#define _WCHAR_T_DEFINED
#endif
# 23 "/usr/lib/llvm-18/lib/clang/18/include/__stddef_wchar_t.h" 3 4

typedef __WCHAR_TYPE__ wchar_t;

#endif
# 27 "/usr/lib/llvm-18/lib/clang/18/include/__stddef_wchar_t.h" 3 4

#endif
# 29 "/usr/lib/llvm-18/lib/clang/18/include/__stddef_wchar_t.h" 3 4
#endif /* __stddef_wchar_t.h expanded by -frewrite-includes */
# 88 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 2 3 4
#undef __need_wchar_t
#endif /* defined(__need_wchar_t) */
# 90 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_NULL)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 92 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* __stddef_null.h expanded by -frewrite-includes */
#include <__stddef_null.h>
#else /* __stddef_null.h expanded by -frewrite-includes */
# 92 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
# 1 "/usr/lib/llvm-18/lib/clang/18/include/__stddef_null.h" 1 3 4
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
# 11 "/usr/lib/llvm-18/lib/clang/18/include/__stddef_null.h" 3 4

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
# 21 "/usr/lib/llvm-18/lib/clang/18/include/__stddef_null.h" 3 4
#define NULL __null
#else
# 23 "/usr/lib/llvm-18/lib/clang/18/include/__stddef_null.h" 3 4
#define NULL 0
#endif
# 25 "/usr/lib/llvm-18/lib/clang/18/include/__stddef_null.h" 3 4
#else
# 26 "/usr/lib/llvm-18/lib/clang/18/include/__stddef_null.h" 3 4
#define NULL ((void*)0)
#endif
# 28 "/usr/lib/llvm-18/lib/clang/18/include/__stddef_null.h" 3 4

#endif
# 30 "/usr/lib/llvm-18/lib/clang/18/include/__stddef_null.h" 3 4
#endif /* __stddef_null.h expanded by -frewrite-includes */
# 93 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 2 3 4
#undef __need_NULL
#endif /* defined(__need_NULL) */
# 95 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_nullptr_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 97 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_nullptr_t.h>
#endif /* expanded by -frewrite-includes */
# 97 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
# 98 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#undef __need_nullptr_t
#endif /* defined(__need_nullptr_t) */
# 100 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_unreachable)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 102 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_unreachable.h>
#endif /* expanded by -frewrite-includes */
# 102 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
# 103 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#undef __need_unreachable
#endif /* defined(__need_unreachable) */
# 105 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_max_align_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 107 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_max_align_t.h>
#endif /* expanded by -frewrite-includes */
# 107 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
# 108 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#undef __need_max_align_t
#endif /* defined(__need_max_align_t) */
# 110 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_offsetof)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 112 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_offsetof.h>
#endif /* expanded by -frewrite-includes */
# 112 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
# 113 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#undef __need_offsetof
#endif /* defined(__need_offsetof) */
# 115 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

/* Some C libraries expect to see a wint_t here. Others (notably MinGW) will use
__WINT_TYPE__ directly; accommodate both by requiring __need_wint_t */
#if 0 /* disabled by -frewrite-includes */
#if defined(__need_wint_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 119 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_wint_t.h>
#endif /* expanded by -frewrite-includes */
# 119 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
# 120 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#undef __need_wint_t
#endif /* __need_wint_t */
# 122 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

#endif
# 124 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#endif /* stddef.h expanded by -frewrite-includes */
# 33 "/usr/include/stdlib.h" 2 3 4

__BEGIN_DECLS

#define	_STDLIB_H	1

#if 0 /* disabled by -frewrite-includes */
#if (defined __USE_XOPEN || defined __USE_XOPEN2K8) && !defined _SYS_WAIT_H
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 39 "/usr/include/stdlib.h" 3 4
/* XPG requires a few symbols from <sys/wait.h> being defined.  */
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* waitflags.h expanded by -frewrite-includes */
# include <bits/waitflags.h>
#else /* waitflags.h expanded by -frewrite-includes */
# 40 "/usr/include/stdlib.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/waitflags.h" 1 3 4
/* Definitions of flag bits for `waitpid' et al.
   Copyright (C) 1992-2024 Free Software Foundation, Inc.
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
#if !defined _SYS_WAIT_H && !defined _STDLIB_H
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 20 "/usr/include/x86_64-linux-gnu/bits/waitflags.h" 3 4
# error "Never include <bits/waitflags.h> directly; use <sys/wait.h> instead."
#endif
# 22 "/usr/include/x86_64-linux-gnu/bits/waitflags.h" 3 4


/* Bits in the third argument to `waitpid'.  */
#define	WNOHANG		1	/* Don't block waiting.  */
#define	WUNTRACED	2	/* Report status of stopped children.  */

/* Bits in the fourth argument to `waitid'.  */
#if 0 /* disabled by -frewrite-includes */
#if defined __USE_XOPEN_EXTENDED || defined __USE_XOPEN2K8
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 30 "/usr/include/x86_64-linux-gnu/bits/waitflags.h" 3 4
# define WSTOPPED	2	/* Report stopped child (same as WUNTRACED). */
# define WEXITED	4	/* Report dead child.  */
# define WCONTINUED	8	/* Report continued child.  */
# define WNOWAIT	0x01000000 /* Don't reap, just poll status.  */
#endif
# 35 "/usr/include/x86_64-linux-gnu/bits/waitflags.h" 3 4

#define __WNOTHREAD     0x20000000 /* Don't wait on children of other threads
				      in this group */
#define __WALL		0x40000000 /* Wait for any child.  */
#define __WCLONE	0x80000000 /* Wait for cloned process.  */
#endif /* waitflags.h expanded by -frewrite-includes */
# 41 "/usr/include/stdlib.h" 2 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* waitstatus.h expanded by -frewrite-includes */
# include <bits/waitstatus.h>
#else /* waitstatus.h expanded by -frewrite-includes */
# 41 "/usr/include/stdlib.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/waitstatus.h" 1 3 4
/* Definitions of status bits for `wait' et al.
   Copyright (C) 1992-2024 Free Software Foundation, Inc.
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
#if !defined _SYS_WAIT_H && !defined _STDLIB_H
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 20 "/usr/include/x86_64-linux-gnu/bits/waitstatus.h" 3 4
# error "Never include <bits/waitstatus.h> directly; use <sys/wait.h> instead."
#endif
# 22 "/usr/include/x86_64-linux-gnu/bits/waitstatus.h" 3 4


/* Everything extant so far uses these same bits.  */


/* If WIFEXITED(STATUS), the low-order 8 bits of the status.  */
#define	__WEXITSTATUS(status)	(((status) & 0xff00) >> 8)

/* If WIFSIGNALED(STATUS), the terminating signal.  */
#define	__WTERMSIG(status)	((status) & 0x7f)

/* If WIFSTOPPED(STATUS), the signal that stopped the child.  */
#define	__WSTOPSIG(status)	__WEXITSTATUS(status)

/* Nonzero if STATUS indicates normal termination.  */
#define	__WIFEXITED(status)	(__WTERMSIG(status) == 0)

/* Nonzero if STATUS indicates termination by a signal.  */
#define __WIFSIGNALED(status) \
  (((signed char) (((status) & 0x7f) + 1) >> 1) > 0)

/* Nonzero if STATUS indicates the child is stopped.  */
#define	__WIFSTOPPED(status)	(((status) & 0xff) == 0x7f)

/* Nonzero if STATUS indicates the child continued after a stop.  We only
   define this if <bits/waitflags.h> provides the WCONTINUED flag bit.  */
#ifdef WCONTINUED
# define __WIFCONTINUED(status)	((status) == __W_CONTINUED)
#endif
# 51 "/usr/include/x86_64-linux-gnu/bits/waitstatus.h" 3 4

/* Nonzero if STATUS indicates the child dumped core.  */
#define	__WCOREDUMP(status)	((status) & __WCOREFLAG)

/* Macros for constructing status values.  */
#define	__W_EXITCODE(ret, sig)	((ret) << 8 | (sig))
#define	__W_STOPCODE(sig)	((sig) << 8 | 0x7f)
#define __W_CONTINUED		0xffff
#define	__WCOREFLAG		0x80
#endif /* waitstatus.h expanded by -frewrite-includes */
# 42 "/usr/include/stdlib.h" 2 3 4

/* Define the macros <sys/wait.h> also would define this way.  */
# define WEXITSTATUS(status)	__WEXITSTATUS (status)
# define WTERMSIG(status)	__WTERMSIG (status)
# define WSTOPSIG(status)	__WSTOPSIG (status)
# define WIFEXITED(status)	__WIFEXITED (status)
# define WIFSIGNALED(status)	__WIFSIGNALED (status)
# define WIFSTOPPED(status)	__WIFSTOPPED (status)
# ifdef __WIFCONTINUED
#  define WIFCONTINUED(status)	__WIFCONTINUED (status)
# endif
# 53 "/usr/include/stdlib.h" 3 4
#endif	/* X/Open or XPG7 and <sys/wait.h> not included.  */
# 54 "/usr/include/stdlib.h" 3 4

/* _FloatN API tests for enablement.  */
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* floatn.h expanded by -frewrite-includes */
#include <bits/floatn.h>
#else /* floatn.h expanded by -frewrite-includes */
# 56 "/usr/include/stdlib.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 1 3 4
/* Macros to control TS 18661-3 glibc features on x86.
   Copyright (C) 2017-2024 Free Software Foundation, Inc.
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

#ifndef _BITS_FLOATN_H
#define _BITS_FLOATN_H

#if 0 /* expanded by -frewrite-includes */
#include <features.h>
#endif /* expanded by -frewrite-includes */
# 22 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4
# 23 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4

/* Defined to 1 if the current compiler invocation provides a
   floating-point type with the IEEE 754 binary128 format, and this
   glibc includes corresponding *f128 interfaces for it.  The required
   libgcc support was added some time after the basic compiler
   support, for x86_64 and x86.  */
#if 0 /* disabled by -frewrite-includes */
#if (defined __x86_64__							\
     ? __GNUC_PREREQ (4, 3)						\
     : (defined __GNU__ ? __GNUC_PREREQ (4, 5) : __GNUC_PREREQ (4, 4)))
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 32 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4
# define __HAVE_FLOAT128 1
#else
# 34 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4
# define __HAVE_FLOAT128 0
#endif
# 36 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4

/* Defined to 1 if __HAVE_FLOAT128 is 1 and the type is ABI-distinct
   from the default float, double and long double types in this glibc.  */
#if 0 /* disabled by -frewrite-includes */
#if __HAVE_FLOAT128
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 40 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4
# define __HAVE_DISTINCT_FLOAT128 1
#else
# 42 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4
# define __HAVE_DISTINCT_FLOAT128 0
#endif
# 44 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4

/* Defined to 1 if the current compiler invocation provides a
   floating-point type with the right format for _Float64x, and this
   glibc includes corresponding *f64x interfaces for it.  */
#define __HAVE_FLOAT64X 1

/* Defined to 1 if __HAVE_FLOAT64X is 1 and _Float64x has the format
   of long double.  Otherwise, if __HAVE_FLOAT64X is 1, _Float64x has
   the format of _Float128, which must be different from that of long
   double.  */
#define __HAVE_FLOAT64X_LONG_DOUBLE 1

#ifndef __ASSEMBLER__

/* Defined to concatenate the literal suffix to be used with _Float128
   types, if __HAVE_FLOAT128 is 1. */
#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT128
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 61 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0) || (defined __cplusplus && !__GNUC_PREREQ (13, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 62 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4
/* The literal suffix f128 exists only since GCC 7.0.  */
#   define __f128(x) x##q
#  else
# 65 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4
#   define __f128(x) x##f128
#  endif
# 67 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4
# endif
# 68 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4

/* Defined to a complex binary128 type if __HAVE_FLOAT128 is 1.  */
#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT128
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 71 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0) || (defined __cplusplus && !__GNUC_PREREQ (13, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 72 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4
/* Add a typedef for older GCC compilers which don't natively support
   _Complex _Float128.  */
typedef _Complex float __cfloat128 __attribute__ ((__mode__ (__TC__)));
#   define __CFLOAT128 __cfloat128
#  else
# 77 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4
#   define __CFLOAT128 _Complex _Float128
#  endif
# 79 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4
# endif
# 80 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4

/* The remaining of this file provides support for older compilers.  */
#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT128
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 83 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4

/* The type _Float128 exists only since GCC 7.0.  */
#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0) || (defined __cplusplus && !__GNUC_PREREQ (13, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 86 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4
typedef __float128 _Float128;
#  endif
# 88 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4

/* __builtin_huge_valf128 doesn't exist before GCC 7.0.  */
#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 91 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4
#   define __builtin_huge_valf128() ((_Float128) __builtin_huge_val ())
#  endif
# 93 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4

/* Older GCC has only a subset of built-in functions for _Float128 on
   x86, and __builtin_infq is not usable in static initializers.
   Converting a narrower sNaN to _Float128 produces a quiet NaN, so
   attempts to use _Float128 sNaNs will not work properly with older
   compilers.  */
#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 100 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4
#   define __builtin_copysignf128 __builtin_copysignq
#   define __builtin_fabsf128 __builtin_fabsq
#   define __builtin_inff128() ((_Float128) __builtin_inf ())
#   define __builtin_nanf128(x) ((_Float128) __builtin_nan (x))
#   define __builtin_nansf128(x) ((_Float128) __builtin_nans (x))
#  endif
# 106 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4

/* In math/math.h, __MATH_TG will expand signbit to __builtin_signbit*,
   e.g.: __builtin_signbitf128, before GCC 6.  However, there has never
   been a __builtin_signbitf128 in GCC and the type-generic builtin is
   only available since GCC 6.  */
#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (6, 0)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 112 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4
#   define __builtin_signbitf128 __signbitf128
#  endif
# 114 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4

# endif
# 116 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4

#endif /* !__ASSEMBLER__.  */
# 118 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4

#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* floatn-common.h expanded by -frewrite-includes */
#include <bits/floatn-common.h>
#else /* floatn-common.h expanded by -frewrite-includes */
# 119 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 1 3 4
/* Macros to control TS 18661-3 glibc features where the same
   definitions are appropriate for all platforms.
   Copyright (C) 2017-2024 Free Software Foundation, Inc.
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

#ifndef _BITS_FLOATN_COMMON_H
#define _BITS_FLOATN_COMMON_H

#if 0 /* expanded by -frewrite-includes */
#include <features.h>
#endif /* expanded by -frewrite-includes */
# 23 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
# 24 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* long-double.h expanded by -frewrite-includes */
#include <bits/long-double.h>
#else /* long-double.h expanded by -frewrite-includes */
# 24 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/long-double.h" 1 3 4
/* Properties of long double type.  ldbl-96 version.
   Copyright (C) 2016-2024 Free Software Foundation, Inc.
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
#define __LDOUBLE_REDIRECTS_TO_FLOAT128_ABI 0
#endif /* long-double.h expanded by -frewrite-includes */
# 25 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 2 3 4

/* This header should be included at the bottom of each bits/floatn.h.
   It defines the following macros for each _FloatN and _FloatNx type,
   where the same definitions, or definitions based only on the macros
   in bits/floatn.h, are appropriate for all glibc configurations.  */

/* Defined to 1 if the current compiler invocation provides a
   floating-point type with the right format for this type, and this
   glibc includes corresponding *fN or *fNx interfaces for it.  */
#define __HAVE_FLOAT16 0
#define __HAVE_FLOAT32 1
#define __HAVE_FLOAT64 1
#define __HAVE_FLOAT32X 1
#define __HAVE_FLOAT128X 0

/* Defined to 1 if the corresponding __HAVE_<type> macro is 1 and the
   type is the first with its format in the sequence of (the default
   choices for) float, double, long double, _Float16, _Float32,
   _Float64, _Float128, _Float32x, _Float64x, _Float128x for this
   glibc; that is, if functions present once per floating-point format
   rather than once per type are present for this type.

   All configurations supported by glibc have _Float32 the same format
   as float, _Float64 and _Float32x the same format as double, the
   _Float64x the same format as either long double or _Float128.  No
   configurations support _Float128x or, as of GCC 7, have compiler
   support for a type meeting the requirements for _Float128x.  */
#define __HAVE_DISTINCT_FLOAT16 __HAVE_FLOAT16
#define __HAVE_DISTINCT_FLOAT32 0
#define __HAVE_DISTINCT_FLOAT64 0
#define __HAVE_DISTINCT_FLOAT32X 0
#define __HAVE_DISTINCT_FLOAT64X 0
#define __HAVE_DISTINCT_FLOAT128X __HAVE_FLOAT128X

/* Defined to 1 if the corresponding _FloatN type is not binary compatible
   with the corresponding ISO C type in the current compilation unit as
   opposed to __HAVE_DISTINCT_FLOATN, which indicates the default types built
   in glibc.  */
#define __HAVE_FLOAT128_UNLIKE_LDBL (__HAVE_DISTINCT_FLOAT128	\
				     && __LDBL_MANT_DIG__ != 113)

/* Defined to 1 if any _FloatN or _FloatNx types that are not
   ABI-distinct are however distinct types at the C language level (so
   for the purposes of __builtin_types_compatible_p and _Generic).  */
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (7, 0) && !defined __cplusplus
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 70 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
# define __HAVE_FLOATN_NOT_TYPEDEF 1
#else
# 72 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
# define __HAVE_FLOATN_NOT_TYPEDEF 0
#endif
# 74 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#ifndef __ASSEMBLER__

/* Defined to concatenate the literal suffix to be used with _FloatN
   or _FloatNx types, if __HAVE_<type> is 1.  The corresponding
   literal suffixes exist since GCC 7, for C only.  */
#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT16
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 81 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0) || (defined __cplusplus && !__GNUC_PREREQ (13, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 82 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
/* No corresponding suffix available for this type.  */
#   define __f16(x) ((_Float16) x##f)
#  else
# 85 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   define __f16(x) x##f16
#  endif
# 87 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
# endif
# 88 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT32
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 90 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0) || (defined __cplusplus && !__GNUC_PREREQ (13, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 91 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   define __f32(x) x##f
#  else
# 93 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   define __f32(x) x##f32
#  endif
# 95 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
# endif
# 96 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT64
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 98 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0) || (defined __cplusplus && !__GNUC_PREREQ (13, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 99 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   ifdef __NO_LONG_DOUBLE_MATH
#    define __f64(x) x##l
#   else
# 102 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#    define __f64(x) x
#   endif
# 104 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#  else
# 105 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   define __f64(x) x##f64
#  endif
# 107 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
# endif
# 108 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT32X
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 110 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0) || (defined __cplusplus && !__GNUC_PREREQ (13, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 111 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   define __f32x(x) x
#  else
# 113 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   define __f32x(x) x##f32x
#  endif
# 115 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
# endif
# 116 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT64X
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 118 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0) || (defined __cplusplus && !__GNUC_PREREQ (13, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 119 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#   if __HAVE_FLOAT64X_LONG_DOUBLE
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 120 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#    define __f64x(x) x##l
#   else
# 122 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#    define __f64x(x) __f128 (x)
#   endif
# 124 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#  else
# 125 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   define __f64x(x) x##f64x
#  endif
# 127 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
# endif
# 128 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT128X
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 130 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0) || (defined __cplusplus && !__GNUC_PREREQ (13, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 131 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   error "_Float128X supported but no constant suffix"
#  else
# 133 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   define __f128x(x) x##f128x
#  endif
# 135 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
# endif
# 136 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

/* Defined to a complex type if __HAVE_<type> is 1.  */
#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT16
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 139 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0) || (defined __cplusplus && !__GNUC_PREREQ (13, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 140 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
typedef _Complex float __cfloat16 __attribute__ ((__mode__ (__HC__)));
#   define __CFLOAT16 __cfloat16
#  else
# 143 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   define __CFLOAT16 _Complex _Float16
#  endif
# 145 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
# endif
# 146 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT32
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 148 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0) || (defined __cplusplus && !__GNUC_PREREQ (13, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 149 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   define __CFLOAT32 _Complex float
#  else
# 151 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   define __CFLOAT32 _Complex _Float32
#  endif
# 153 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
# endif
# 154 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT64
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 156 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0) || (defined __cplusplus && !__GNUC_PREREQ (13, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 157 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   ifdef __NO_LONG_DOUBLE_MATH
#    define __CFLOAT64 _Complex long double
#   else
# 160 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#    define __CFLOAT64 _Complex double
#   endif
# 162 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#  else
# 163 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   define __CFLOAT64 _Complex _Float64
#  endif
# 165 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
# endif
# 166 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT32X
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 168 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0) || (defined __cplusplus && !__GNUC_PREREQ (13, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 169 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   define __CFLOAT32X _Complex double
#  else
# 171 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   define __CFLOAT32X _Complex _Float32x
#  endif
# 173 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
# endif
# 174 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT64X
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 176 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0) || (defined __cplusplus && !__GNUC_PREREQ (13, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 177 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#   if __HAVE_FLOAT64X_LONG_DOUBLE
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 178 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#    define __CFLOAT64X _Complex long double
#   else
# 180 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#    define __CFLOAT64X __CFLOAT128
#   endif
# 182 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#  else
# 183 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   define __CFLOAT64X _Complex _Float64x
#  endif
# 185 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
# endif
# 186 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT128X
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 188 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0) || (defined __cplusplus && !__GNUC_PREREQ (13, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 189 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   error "_Float128X supported but no complex type"
#  else
# 191 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   define __CFLOAT128X _Complex _Float128x
#  endif
# 193 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
# endif
# 194 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

/* The remaining of this file provides support for older compilers.  */
#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT16
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 197 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0) || (defined __cplusplus && !__GNUC_PREREQ (13, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 199 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
typedef float _Float16 __attribute__ ((__mode__ (__HF__)));
#  endif
# 201 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 203 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   define __builtin_huge_valf16() ((_Float16) __builtin_huge_val ())
#   define __builtin_inff16() ((_Float16) __builtin_inf ())
#   define __builtin_nanf16(x) ((_Float16) __builtin_nan (x))
#   define __builtin_nansf16(x) ((_Float16) __builtin_nans (x))
#  endif
# 208 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

# endif
# 210 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT32
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 212 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0) || (defined __cplusplus && !__GNUC_PREREQ (13, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 214 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
typedef float _Float32;
#  endif
# 216 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 218 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   define __builtin_huge_valf32() (__builtin_huge_valf ())
#   define __builtin_inff32() (__builtin_inff ())
#   define __builtin_nanf32(x) (__builtin_nanf (x))
#   define __builtin_nansf32(x) (__builtin_nansf (x))
#  endif
# 223 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

# endif
# 225 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT64
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 227 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

/* If double, long double and _Float64 all have the same set of
   values, TS 18661-3 requires the usual arithmetic conversions on
   long double and _Float64 to produce _Float64.  For this to be the
   case when building with a compiler without a distinct _Float64
   type, _Float64 must be a typedef for long double, not for
   double.  */

#  ifdef __NO_LONG_DOUBLE_MATH

#if 0 /* disabled by -frewrite-includes */
#   if !__GNUC_PREREQ (7, 0) || (defined __cplusplus && !__GNUC_PREREQ (13, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 238 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
typedef long double _Float64;
#   endif
# 240 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#   if !__GNUC_PREREQ (7, 0)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 242 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#    define __builtin_huge_valf64() (__builtin_huge_vall ())
#    define __builtin_inff64() (__builtin_infl ())
#    define __builtin_nanf64(x) (__builtin_nanl (x))
#    define __builtin_nansf64(x) (__builtin_nansl (x))
#   endif
# 247 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#  else
# 249 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#   if !__GNUC_PREREQ (7, 0) || (defined __cplusplus && !__GNUC_PREREQ (13, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 251 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
typedef double _Float64;
#   endif
# 253 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#   if !__GNUC_PREREQ (7, 0)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 255 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#    define __builtin_huge_valf64() (__builtin_huge_val ())
#    define __builtin_inff64() (__builtin_inf ())
#    define __builtin_nanf64(x) (__builtin_nan (x))
#    define __builtin_nansf64(x) (__builtin_nans (x))
#   endif
# 260 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#  endif
# 262 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

# endif
# 264 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT32X
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 266 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0) || (defined __cplusplus && !__GNUC_PREREQ (13, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 268 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
typedef double _Float32x;
#  endif
# 270 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 272 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   define __builtin_huge_valf32x() (__builtin_huge_val ())
#   define __builtin_inff32x() (__builtin_inf ())
#   define __builtin_nanf32x(x) (__builtin_nan (x))
#   define __builtin_nansf32x(x) (__builtin_nans (x))
#  endif
# 277 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

# endif
# 279 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT64X
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 281 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#  if __HAVE_FLOAT64X_LONG_DOUBLE
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 283 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#   if !__GNUC_PREREQ (7, 0) || (defined __cplusplus && !__GNUC_PREREQ (13, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 285 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
typedef long double _Float64x;
#   endif
# 287 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#   if !__GNUC_PREREQ (7, 0)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 289 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#    define __builtin_huge_valf64x() (__builtin_huge_vall ())
#    define __builtin_inff64x() (__builtin_infl ())
#    define __builtin_nanf64x(x) (__builtin_nanl (x))
#    define __builtin_nansf64x(x) (__builtin_nansl (x))
#   endif
# 294 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#  else
# 296 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#   if !__GNUC_PREREQ (7, 0) || (defined __cplusplus && !__GNUC_PREREQ (13, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 298 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
typedef _Float128 _Float64x;
#   endif
# 300 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#   if !__GNUC_PREREQ (7, 0)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 302 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#    define __builtin_huge_valf64x() (__builtin_huge_valf128 ())
#    define __builtin_inff64x() (__builtin_inff128 ())
#    define __builtin_nanf64x(x) (__builtin_nanf128 (x))
#    define __builtin_nansf64x(x) (__builtin_nansf128 (x))
#   endif
# 307 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#  endif
# 309 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

# endif
# 311 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT128X
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 313 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0) || (defined __cplusplus && !__GNUC_PREREQ (13, 0))
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 315 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   error "_Float128x supported but no type"
#  endif
# 317 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#  if !__GNUC_PREREQ (7, 0)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 319 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#   define __builtin_huge_valf128x() ((_Float128x) __builtin_huge_val ())
#   define __builtin_inff128x() ((_Float128x) __builtin_inf ())
#   define __builtin_nanf128x(x) ((_Float128x) __builtin_nan (x))
#   define __builtin_nansf128x(x) ((_Float128x) __builtin_nans (x))
#  endif
# 324 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

# endif
# 326 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#endif /* !__ASSEMBLER__.  */
# 328 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4

#endif /* _BITS_FLOATN_COMMON_H */
# 330 "/usr/include/x86_64-linux-gnu/bits/floatn-common.h" 3 4
#endif /* floatn-common.h expanded by -frewrite-includes */
# 120 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 2 3 4

#endif /* _BITS_FLOATN_H */
# 122 "/usr/include/x86_64-linux-gnu/bits/floatn.h" 3 4
#endif /* floatn.h expanded by -frewrite-includes */
# 57 "/usr/include/stdlib.h" 2 3 4

/* Returned by `div'.  */
typedef struct
  {
    int quot;			/* Quotient.  */
    int rem;			/* Remainder.  */
  } div_t;

/* Returned by `ldiv'.  */
#ifndef __ldiv_t_defined
typedef struct
  {
    long int quot;		/* Quotient.  */
    long int rem;		/* Remainder.  */
  } ldiv_t;
# define __ldiv_t_defined	1
#endif
# 74 "/usr/include/stdlib.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined __USE_ISOC99 && !defined __lldiv_t_defined
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 76 "/usr/include/stdlib.h" 3 4
/* Returned by `lldiv'.  */
__extension__ typedef struct
  {
    long long int quot;		/* Quotient.  */
    long long int rem;		/* Remainder.  */
  } lldiv_t;
# define __lldiv_t_defined	1
#endif
# 84 "/usr/include/stdlib.h" 3 4


/* The largest number rand will return (same as INT_MAX).  */
#define	RAND_MAX	2147483647


/* We define these the same for all machines.
   Changes from this to the outside world should be done in `_exit'.  */
#define	EXIT_FAILURE	1	/* Failing exit status.  */
#define	EXIT_SUCCESS	0	/* Successful exit status.  */


/* Maximum length of a multibyte character in the current locale.  */
#define	MB_CUR_MAX	(__ctype_get_mb_cur_max ())
extern size_t __ctype_get_mb_cur_max (void) __THROW __wur;


/* Convert a string to a floating-point number.  */
extern double atof (const char *__nptr)
     __THROW __attribute_pure__ __nonnull ((1)) __wur;
/* Convert a string to an integer.  */
extern int atoi (const char *__nptr)
     __THROW __attribute_pure__ __nonnull ((1)) __wur;
/* Convert a string to a long integer.  */
extern long int atol (const char *__nptr)
     __THROW __attribute_pure__ __nonnull ((1)) __wur;

#ifdef __USE_ISOC99
/* Convert a string to a long long integer.  */
__extension__ extern long long int atoll (const char *__nptr)
     __THROW __attribute_pure__ __nonnull ((1)) __wur;
#endif
# 116 "/usr/include/stdlib.h" 3 4

/* Convert a string to a floating-point number.  */
extern double strtod (const char *__restrict __nptr,
		      char **__restrict __endptr)
     __THROW __nonnull ((1));

#ifdef	__USE_ISOC99
/* Likewise for `float' and `long double' sizes of floating-point numbers.  */
extern float strtof (const char *__restrict __nptr,
		     char **__restrict __endptr) __THROW __nonnull ((1));

extern long double strtold (const char *__restrict __nptr,
			    char **__restrict __endptr)
     __THROW __nonnull ((1));
#endif
# 131 "/usr/include/stdlib.h" 3 4

/* Likewise for '_FloatN' and '_FloatNx'.  */

#if 0 /* disabled by -frewrite-includes */
#if __HAVE_FLOAT16 && __GLIBC_USE (IEC_60559_TYPES_EXT)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 135 "/usr/include/stdlib.h" 3 4
extern _Float16 strtof16 (const char *__restrict __nptr,
			  char **__restrict __endptr)
     __THROW __nonnull ((1));
#endif
# 139 "/usr/include/stdlib.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __HAVE_FLOAT32 && __GLIBC_USE (IEC_60559_TYPES_EXT)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 141 "/usr/include/stdlib.h" 3 4
extern _Float32 strtof32 (const char *__restrict __nptr,
			  char **__restrict __endptr)
     __THROW __nonnull ((1));
#endif
# 145 "/usr/include/stdlib.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __HAVE_FLOAT64 && __GLIBC_USE (IEC_60559_TYPES_EXT)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 147 "/usr/include/stdlib.h" 3 4
extern _Float64 strtof64 (const char *__restrict __nptr,
			  char **__restrict __endptr)
     __THROW __nonnull ((1));
#endif
# 151 "/usr/include/stdlib.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __HAVE_FLOAT128 && __GLIBC_USE (IEC_60559_TYPES_EXT)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 153 "/usr/include/stdlib.h" 3 4
extern _Float128 strtof128 (const char *__restrict __nptr,
			    char **__restrict __endptr)
     __THROW __nonnull ((1));
#endif
# 157 "/usr/include/stdlib.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __HAVE_FLOAT32X && __GLIBC_USE (IEC_60559_TYPES_EXT)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 159 "/usr/include/stdlib.h" 3 4
extern _Float32x strtof32x (const char *__restrict __nptr,
			    char **__restrict __endptr)
     __THROW __nonnull ((1));
#endif
# 163 "/usr/include/stdlib.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __HAVE_FLOAT64X && __GLIBC_USE (IEC_60559_TYPES_EXT)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 165 "/usr/include/stdlib.h" 3 4
extern _Float64x strtof64x (const char *__restrict __nptr,
			    char **__restrict __endptr)
     __THROW __nonnull ((1));
#endif
# 169 "/usr/include/stdlib.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __HAVE_FLOAT128X && __GLIBC_USE (IEC_60559_TYPES_EXT)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 171 "/usr/include/stdlib.h" 3 4
extern _Float128x strtof128x (const char *__restrict __nptr,
			      char **__restrict __endptr)
     __THROW __nonnull ((1));
#endif
# 175 "/usr/include/stdlib.h" 3 4

/* Convert a string to a long integer.  */
extern long int strtol (const char *__restrict __nptr,
			char **__restrict __endptr, int __base)
     __THROW __nonnull ((1));
/* Convert a string to an unsigned long integer.  */
extern unsigned long int strtoul (const char *__restrict __nptr,
				  char **__restrict __endptr, int __base)
     __THROW __nonnull ((1));

#ifdef __USE_MISC
/* Convert a string to a quadword integer.  */
__extension__
extern long long int strtoq (const char *__restrict __nptr,
			     char **__restrict __endptr, int __base)
     __THROW __nonnull ((1));
/* Convert a string to an unsigned quadword integer.  */
__extension__
extern unsigned long long int strtouq (const char *__restrict __nptr,
				       char **__restrict __endptr, int __base)
     __THROW __nonnull ((1));
#endif /* Use misc.  */
# 197 "/usr/include/stdlib.h" 3 4

#ifdef __USE_ISOC99
/* Convert a string to a quadword integer.  */
__extension__
extern long long int strtoll (const char *__restrict __nptr,
			      char **__restrict __endptr, int __base)
     __THROW __nonnull ((1));
/* Convert a string to an unsigned quadword integer.  */
__extension__
extern unsigned long long int strtoull (const char *__restrict __nptr,
					char **__restrict __endptr, int __base)
     __THROW __nonnull ((1));
#endif /* ISO C99 or use MISC.  */
# 210 "/usr/include/stdlib.h" 3 4

/* Versions of the above functions that handle '0b' and '0B' prefixes
   in base 0 or 2.  */
#if 0 /* disabled by -frewrite-includes */
#if __GLIBC_USE (C2X_STRTOL)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 214 "/usr/include/stdlib.h" 3 4
# ifdef __REDIRECT
extern long int __REDIRECT_NTH (strtol, (const char *__restrict __nptr,
					 char **__restrict __endptr,
					 int __base), __isoc23_strtol)
     __nonnull ((1));
extern unsigned long int __REDIRECT_NTH (strtoul,
					 (const char *__restrict __nptr,
					  char **__restrict __endptr,
					  int __base), __isoc23_strtoul)
     __nonnull ((1));
#  ifdef __USE_MISC
__extension__
extern long long int __REDIRECT_NTH (strtoq, (const char *__restrict __nptr,
					      char **__restrict __endptr,
					      int __base), __isoc23_strtoll)
     __nonnull ((1));
__extension__
extern unsigned long long int __REDIRECT_NTH (strtouq,
					      (const char *__restrict __nptr,
					       char **__restrict __endptr,
					       int __base), __isoc23_strtoull)
     __nonnull ((1));
#  endif
# 237 "/usr/include/stdlib.h" 3 4
__extension__
extern long long int __REDIRECT_NTH (strtoll, (const char *__restrict __nptr,
					       char **__restrict __endptr,
					       int __base), __isoc23_strtoll)
     __nonnull ((1));
__extension__
extern unsigned long long int __REDIRECT_NTH (strtoull,
					      (const char *__restrict __nptr,
					       char **__restrict __endptr,
					       int __base), __isoc23_strtoull)
     __nonnull ((1));
# else
# 249 "/usr/include/stdlib.h" 3 4
extern long int __isoc23_strtol (const char *__restrict __nptr,
				 char **__restrict __endptr, int __base)
     __THROW __nonnull ((1));
extern unsigned long int __isoc23_strtoul (const char *__restrict __nptr,
					   char **__restrict __endptr,
					   int __base)
     __THROW __nonnull ((1));
__extension__
extern long long int __isoc23_strtoll (const char *__restrict __nptr,
				       char **__restrict __endptr, int __base)
     __THROW __nonnull ((1));
__extension__
extern unsigned long long int __isoc23_strtoull (const char *__restrict __nptr,
						 char **__restrict __endptr,
						 int __base)
     __THROW __nonnull ((1));
#  define strtol __isoc23_strtol
#  define strtoul __isoc23_strtoul
#  ifdef __USE_MISC
#   define strtoq __isoc23_strtoll
#   define strtouq __isoc23_strtoull
#  endif
# 271 "/usr/include/stdlib.h" 3 4
#  define strtoll __isoc23_strtoll
#  define strtoull __isoc23_strtoull
# endif
# 274 "/usr/include/stdlib.h" 3 4
#endif
# 275 "/usr/include/stdlib.h" 3 4

/* Convert a floating-point number to a string.  */
#if 0 /* disabled by -frewrite-includes */
#if __GLIBC_USE (IEC_60559_BFP_EXT_C2X)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 278 "/usr/include/stdlib.h" 3 4
extern int strfromd (char *__dest, size_t __size, const char *__format,
		     double __f)
     __THROW __nonnull ((3));

extern int strfromf (char *__dest, size_t __size, const char *__format,
		     float __f)
     __THROW __nonnull ((3));

extern int strfroml (char *__dest, size_t __size, const char *__format,
		     long double __f)
     __THROW __nonnull ((3));
#endif
# 290 "/usr/include/stdlib.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __HAVE_FLOAT16 && __GLIBC_USE (IEC_60559_TYPES_EXT)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 292 "/usr/include/stdlib.h" 3 4
extern int strfromf16 (char *__dest, size_t __size, const char * __format,
		       _Float16 __f)
     __THROW __nonnull ((3));
#endif
# 296 "/usr/include/stdlib.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __HAVE_FLOAT32 && __GLIBC_USE (IEC_60559_TYPES_EXT)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 298 "/usr/include/stdlib.h" 3 4
extern int strfromf32 (char *__dest, size_t __size, const char * __format,
		       _Float32 __f)
     __THROW __nonnull ((3));
#endif
# 302 "/usr/include/stdlib.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __HAVE_FLOAT64 && __GLIBC_USE (IEC_60559_TYPES_EXT)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 304 "/usr/include/stdlib.h" 3 4
extern int strfromf64 (char *__dest, size_t __size, const char * __format,
		       _Float64 __f)
     __THROW __nonnull ((3));
#endif
# 308 "/usr/include/stdlib.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __HAVE_FLOAT128 && __GLIBC_USE (IEC_60559_TYPES_EXT)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 310 "/usr/include/stdlib.h" 3 4
extern int strfromf128 (char *__dest, size_t __size, const char * __format,
			_Float128 __f)
     __THROW __nonnull ((3));
#endif
# 314 "/usr/include/stdlib.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __HAVE_FLOAT32X && __GLIBC_USE (IEC_60559_TYPES_EXT)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 316 "/usr/include/stdlib.h" 3 4
extern int strfromf32x (char *__dest, size_t __size, const char * __format,
			_Float32x __f)
     __THROW __nonnull ((3));
#endif
# 320 "/usr/include/stdlib.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __HAVE_FLOAT64X && __GLIBC_USE (IEC_60559_TYPES_EXT)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 322 "/usr/include/stdlib.h" 3 4
extern int strfromf64x (char *__dest, size_t __size, const char * __format,
			_Float64x __f)
     __THROW __nonnull ((3));
#endif
# 326 "/usr/include/stdlib.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __HAVE_FLOAT128X && __GLIBC_USE (IEC_60559_TYPES_EXT)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 328 "/usr/include/stdlib.h" 3 4
extern int strfromf128x (char *__dest, size_t __size, const char * __format,
			 _Float128x __f)
     __THROW __nonnull ((3));
#endif
# 332 "/usr/include/stdlib.h" 3 4


#ifdef __USE_GNU
/* Parallel versions of the functions above which take the locale to
   use as an additional parameter.  These are GNU extensions inspired
   by the POSIX.1-2008 extended locale API.  */
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* locale_t.h expanded by -frewrite-includes */
# include <bits/types/locale_t.h>
#else /* locale_t.h expanded by -frewrite-includes */
# 338 "/usr/include/stdlib.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/locale_t.h" 1 3 4
/* Definition of locale_t.
   Copyright (C) 2017-2024 Free Software Foundation, Inc.
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

#ifndef _BITS_TYPES_LOCALE_T_H
#define _BITS_TYPES_LOCALE_T_H 1

#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* __locale_t.h expanded by -frewrite-includes */
#include <bits/types/__locale_t.h>
#else /* __locale_t.h expanded by -frewrite-includes */
# 22 "/usr/include/x86_64-linux-gnu/bits/types/locale_t.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/__locale_t.h" 1 3 4
/* Definition of struct __locale_struct and __locale_t.
   Copyright (C) 1997-2024 Free Software Foundation, Inc.
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

#ifndef _BITS_TYPES___LOCALE_T_H
#define _BITS_TYPES___LOCALE_T_H 1

/* POSIX.1-2008: the locale_t type, representing a locale context
   (implementation-namespace version).  This type should be treated
   as opaque by applications; some details are exposed for the sake of
   efficiency in e.g. ctype functions.  */

struct __locale_struct
{
  /* Note: LC_ALL is not a valid index into this array.  */
  struct __locale_data *__locales[13]; /* 13 = __LC_LAST. */

  /* To increase the speed of this solution we add some special members.  */
  const unsigned short int *__ctype_b;
  const int *__ctype_tolower;
  const int *__ctype_toupper;

  /* Note: LC_ALL is not a valid index into this array.  */
  const char *__names[13];
};

typedef struct __locale_struct *__locale_t;

#endif /* bits/types/__locale_t.h */
# 44 "/usr/include/x86_64-linux-gnu/bits/types/__locale_t.h" 3 4
#endif /* __locale_t.h expanded by -frewrite-includes */
# 23 "/usr/include/x86_64-linux-gnu/bits/types/locale_t.h" 2 3 4

typedef __locale_t locale_t;

#endif /* bits/types/locale_t.h */
# 27 "/usr/include/x86_64-linux-gnu/bits/types/locale_t.h" 3 4
#endif /* locale_t.h expanded by -frewrite-includes */
# 339 "/usr/include/stdlib.h" 2 3 4

extern long int strtol_l (const char *__restrict __nptr,
			  char **__restrict __endptr, int __base,
			  locale_t __loc) __THROW __nonnull ((1, 4));

extern unsigned long int strtoul_l (const char *__restrict __nptr,
				    char **__restrict __endptr,
				    int __base, locale_t __loc)
     __THROW __nonnull ((1, 4));

__extension__
extern long long int strtoll_l (const char *__restrict __nptr,
				char **__restrict __endptr, int __base,
				locale_t __loc)
     __THROW __nonnull ((1, 4));

__extension__
extern unsigned long long int strtoull_l (const char *__restrict __nptr,
					  char **__restrict __endptr,
					  int __base, locale_t __loc)
     __THROW __nonnull ((1, 4));

/* Versions of the above functions that handle '0b' and '0B' prefixes
   in base 0 or 2.  */
#if 0 /* disabled by -frewrite-includes */
# if __GLIBC_USE (C2X_STRTOL)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 364 "/usr/include/stdlib.h" 3 4
#  ifdef __REDIRECT
extern long int __REDIRECT_NTH (strtol_l, (const char *__restrict __nptr,
					   char **__restrict __endptr,
					   int __base, locale_t __loc),
				__isoc23_strtol_l)
     __nonnull ((1, 4));
extern unsigned long int __REDIRECT_NTH (strtoul_l,
					 (const char *__restrict __nptr,
					  char **__restrict __endptr,
					  int __base, locale_t __loc),
					 __isoc23_strtoul_l)
     __nonnull ((1, 4));
__extension__
extern long long int __REDIRECT_NTH (strtoll_l, (const char *__restrict __nptr,
						 char **__restrict __endptr,
						 int __base,
						 locale_t __loc),
				     __isoc23_strtoll_l)
     __nonnull ((1, 4));
__extension__
extern unsigned long long int __REDIRECT_NTH (strtoull_l,
					      (const char *__restrict __nptr,
					       char **__restrict __endptr,
					       int __base, locale_t __loc),
					      __isoc23_strtoull_l)
     __nonnull ((1, 4));
#  else
# 391 "/usr/include/stdlib.h" 3 4
extern long int __isoc23_strtol_l (const char *__restrict __nptr,
				   char **__restrict __endptr, int __base,
				   locale_t __loc) __THROW __nonnull ((1, 4));
extern unsigned long int __isoc23_strtoul_l (const char *__restrict __nptr,
					     char **__restrict __endptr,
					     int __base, locale_t __loc)
     __THROW __nonnull ((1, 4));
__extension__
extern long long int __isoc23_strtoll_l (const char *__restrict __nptr,
					 char **__restrict __endptr,
					 int __base, locale_t __loc)
     __THROW __nonnull ((1, 4));
__extension__
extern unsigned long long int __isoc23_strtoull_l (const char *__restrict __nptr,
						   char **__restrict __endptr,
						   int __base, locale_t __loc)
     __THROW __nonnull ((1, 4));
#   define strtol_l __isoc23_strtol_l
#   define strtoul_l __isoc23_strtoul_l
#   define strtoll_l __isoc23_strtoll_l
#   define strtoull_l __isoc23_strtoull_l
#  endif
# 413 "/usr/include/stdlib.h" 3 4
# endif
# 414 "/usr/include/stdlib.h" 3 4

extern double strtod_l (const char *__restrict __nptr,
			char **__restrict __endptr, locale_t __loc)
     __THROW __nonnull ((1, 3));

extern float strtof_l (const char *__restrict __nptr,
		       char **__restrict __endptr, locale_t __loc)
     __THROW __nonnull ((1, 3));

extern long double strtold_l (const char *__restrict __nptr,
			      char **__restrict __endptr,
			      locale_t __loc)
     __THROW __nonnull ((1, 3));

#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT16
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 429 "/usr/include/stdlib.h" 3 4
extern _Float16 strtof16_l (const char *__restrict __nptr,
			    char **__restrict __endptr,
			    locale_t __loc)
     __THROW __nonnull ((1, 3));
# endif
# 434 "/usr/include/stdlib.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT32
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 436 "/usr/include/stdlib.h" 3 4
extern _Float32 strtof32_l (const char *__restrict __nptr,
			    char **__restrict __endptr,
			    locale_t __loc)
     __THROW __nonnull ((1, 3));
# endif
# 441 "/usr/include/stdlib.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT64
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 443 "/usr/include/stdlib.h" 3 4
extern _Float64 strtof64_l (const char *__restrict __nptr,
			    char **__restrict __endptr,
			    locale_t __loc)
     __THROW __nonnull ((1, 3));
# endif
# 448 "/usr/include/stdlib.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT128
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 450 "/usr/include/stdlib.h" 3 4
extern _Float128 strtof128_l (const char *__restrict __nptr,
			      char **__restrict __endptr,
			      locale_t __loc)
     __THROW __nonnull ((1, 3));
# endif
# 455 "/usr/include/stdlib.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT32X
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 457 "/usr/include/stdlib.h" 3 4
extern _Float32x strtof32x_l (const char *__restrict __nptr,
			      char **__restrict __endptr,
			      locale_t __loc)
     __THROW __nonnull ((1, 3));
# endif
# 462 "/usr/include/stdlib.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT64X
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 464 "/usr/include/stdlib.h" 3 4
extern _Float64x strtof64x_l (const char *__restrict __nptr,
			      char **__restrict __endptr,
			      locale_t __loc)
     __THROW __nonnull ((1, 3));
# endif
# 469 "/usr/include/stdlib.h" 3 4

#if 0 /* disabled by -frewrite-includes */
# if __HAVE_FLOAT128X
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 471 "/usr/include/stdlib.h" 3 4
extern _Float128x strtof128x_l (const char *__restrict __nptr,
				char **__restrict __endptr,
				locale_t __loc)
     __THROW __nonnull ((1, 3));
# endif
# 476 "/usr/include/stdlib.h" 3 4
#endif /* GNU */
# 477 "/usr/include/stdlib.h" 3 4


#ifdef __USE_EXTERN_INLINES
__extern_inline int
__NTH (atoi (const char *__nptr))
{
  return (int) strtol (__nptr, (char **) NULL, 10);
}
__extern_inline long int
__NTH (atol (const char *__nptr))
{
  return strtol (__nptr, (char **) NULL, 10);
}

# ifdef __USE_ISOC99
__extension__ __extern_inline long long int
__NTH (atoll (const char *__nptr))
{
  return strtoll (__nptr, (char **) NULL, 10);
}
# endif
# 498 "/usr/include/stdlib.h" 3 4
#endif /* Optimizing and Inlining.  */
# 499 "/usr/include/stdlib.h" 3 4


#if 0 /* disabled by -frewrite-includes */
#if defined __USE_MISC || defined __USE_XOPEN_EXTENDED
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 502 "/usr/include/stdlib.h" 3 4
/* Convert N to base 64 using the digits "./0-9A-Za-z", least-significant
   digit first.  Returns a pointer to static storage overwritten by the
   next call.  */
extern char *l64a (long int __n) __THROW __wur;

/* Read a number from a string S in base 64 as above.  */
extern long int a64l (const char *__s)
     __THROW __attribute_pure__ __nonnull ((1)) __wur;

#endif	/* Use misc || extended X/Open.  */
# 512 "/usr/include/stdlib.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined __USE_MISC || defined __USE_XOPEN_EXTENDED
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 514 "/usr/include/stdlib.h" 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* types.h expanded by -frewrite-includes */
# include <sys/types.h>	/* we need int32_t... */
#else /* types.h expanded by -frewrite-includes */
# 514 "/usr/include/stdlib.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/sys/types.h" 1 3 4
/* Copyright (C) 1991-2024 Free Software Foundation, Inc.
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
 *	POSIX Standard: 2.6 Primitive System Data Types	<sys/types.h>
 */

#ifndef	_SYS_TYPES_H
#define	_SYS_TYPES_H	1

#if 0 /* expanded by -frewrite-includes */
#include <features.h>
#endif /* expanded by -frewrite-includes */
# 25 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
# 26 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4

__BEGIN_DECLS

#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* types.h expanded by -frewrite-includes */
#include <bits/types.h>
#else /* types.h expanded by -frewrite-includes */
# 29 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types.h" 1 3 4
/* bits/types.h -- definitions of __*_t types underlying *_t types.
   Copyright (C) 2002-2024 Free Software Foundation, Inc.
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
   Copyright (C) 2018-2024 Free Software Foundation, Inc.
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

#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* wordsize.h expanded by -frewrite-includes */
#include <bits/wordsize.h>
#else /* wordsize.h expanded by -frewrite-includes */
# 19 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 3 4
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
# 20 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 2 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined __x86_64__ && defined __ILP32__
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 22 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 3 4
/* For x32, time is 64-bit even though word size is 32-bit.  */
# define __TIMESIZE	64
#else
# 25 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 3 4
/* For others, time size is word size.  */
# define __TIMESIZE	__WORDSIZE
#endif
# 28 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 3 4
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
   Copyright (C) 2012-2024 Free Software Foundation, Inc.
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
#define __SUSECONDS64_T_TYPE	__SQUAD_TYPE
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

/* And for getitimer, setitimer and rusage  */
# define __KERNEL_OLD_TIMEVAL_MATCHES_TIMEVAL64 1
#else
# 95 "/usr/include/x86_64-linux-gnu/bits/typesizes.h" 3 4
# define __RLIM_T_MATCHES_RLIM64_T	0

# define __STATFS_MATCHES_STATFS64  0

# define __KERNEL_OLD_TIMEVAL_MATCHES_TIMEVAL64 0
#endif
# 101 "/usr/include/x86_64-linux-gnu/bits/typesizes.h" 3 4

/* Number of descriptors that can fit in an `fd_set'.  */
#define __FD_SETSIZE		1024


#endif /* bits/typesizes.h */
# 107 "/usr/include/x86_64-linux-gnu/bits/typesizes.h" 3 4
#endif /* typesizes.h expanded by -frewrite-includes */
# 142 "/usr/include/x86_64-linux-gnu/bits/types.h" 2 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* time64.h expanded by -frewrite-includes */
#include <bits/time64.h>	/* Defines __TIME*_T_TYPE macros.  */
#else /* time64.h expanded by -frewrite-includes */
# 142 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/time64.h" 1 3 4
/* bits/time64.h -- underlying types for __time64_t.  Generic version.
   Copyright (C) 2018-2024 Free Software Foundation, Inc.
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
__STD_TYPE __SUSECONDS64_T_TYPE __suseconds64_t;

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
# 221 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
# define __time64_t __time_t
#if 0 /* disabled by -frewrite-includes */
#if 0
#elif __TIMESIZE != 64
#endif
#endif /* disabled by -frewrite-includes */
#elif 0 /* evaluated by -frewrite-includes */
# 223 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
__STD_TYPE __TIME64_T_TYPE __time64_t;
#endif
# 225 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4

#undef __STD_TYPE

#endif /* bits/types.h */
# 229 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
#endif /* types.h expanded by -frewrite-includes */
# 30 "/usr/include/x86_64-linux-gnu/sys/types.h" 2 3 4

#ifdef	__USE_MISC
# ifndef __u_char_defined
typedef __u_char u_char;
typedef __u_short u_short;
typedef __u_int u_int;
typedef __u_long u_long;
typedef __quad_t quad_t;
typedef __u_quad_t u_quad_t;
typedef __fsid_t fsid_t;
#  define __u_char_defined
# endif
# 42 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
typedef __loff_t loff_t;
#endif
# 44 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4

#ifndef __ino_t_defined
# ifndef __USE_FILE_OFFSET64
typedef __ino_t ino_t;
# else
# 49 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
typedef __ino64_t ino_t;
# endif
# 51 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
# define __ino_t_defined
#endif
# 53 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#if defined __USE_LARGEFILE64 && !defined __ino64_t_defined
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 54 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
typedef __ino64_t ino64_t;
# define __ino64_t_defined
#endif
# 57 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4

#ifndef __dev_t_defined
typedef __dev_t dev_t;
# define __dev_t_defined
#endif
# 62 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4

#ifndef __gid_t_defined
typedef __gid_t gid_t;
# define __gid_t_defined
#endif
# 67 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4

#ifndef __mode_t_defined
typedef __mode_t mode_t;
# define __mode_t_defined
#endif
# 72 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4

#ifndef __nlink_t_defined
typedef __nlink_t nlink_t;
# define __nlink_t_defined
#endif
# 77 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4

#ifndef __uid_t_defined
typedef __uid_t uid_t;
# define __uid_t_defined
#endif
# 82 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4

#ifndef __off_t_defined
# ifndef __USE_FILE_OFFSET64
typedef __off_t off_t;
# else
# 87 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
typedef __off64_t off_t;
# endif
# 89 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
# define __off_t_defined
#endif
# 91 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#if defined __USE_LARGEFILE64 && !defined __off64_t_defined
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 92 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
typedef __off64_t off64_t;
# define __off64_t_defined
#endif
# 95 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4

#ifndef __pid_t_defined
typedef __pid_t pid_t;
# define __pid_t_defined
#endif
# 100 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if (defined __USE_XOPEN || defined __USE_XOPEN2K8) \
    && !defined __id_t_defined
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 103 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
typedef __id_t id_t;
# define __id_t_defined
#endif
# 106 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4

#ifndef __ssize_t_defined
typedef __ssize_t ssize_t;
# define __ssize_t_defined
#endif
# 111 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4

#ifdef	__USE_MISC
# ifndef __daddr_t_defined
typedef __daddr_t daddr_t;
typedef __caddr_t caddr_t;
#  define __daddr_t_defined
# endif
# 118 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
#endif
# 119 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if (defined __USE_MISC || defined __USE_XOPEN) && !defined __key_t_defined
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 121 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
typedef __key_t key_t;
# define __key_t_defined
#endif
# 124 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined __USE_XOPEN || defined __USE_XOPEN2K8
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 126 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* clock_t.h expanded by -frewrite-includes */
# include <bits/types/clock_t.h>
#else /* clock_t.h expanded by -frewrite-includes */
# 126 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/clock_t.h" 1 3 4
#ifndef __clock_t_defined
#define __clock_t_defined 1

#if 0 /* expanded by -frewrite-includes */
#include <bits/types.h>
#endif /* expanded by -frewrite-includes */
# 4 "/usr/include/x86_64-linux-gnu/bits/types/clock_t.h" 3 4
# 5 "/usr/include/x86_64-linux-gnu/bits/types/clock_t.h" 3 4

/* Returned by `clock'.  */
typedef __clock_t clock_t;

#endif
# 10 "/usr/include/x86_64-linux-gnu/bits/types/clock_t.h" 3 4
#endif /* clock_t.h expanded by -frewrite-includes */
# 127 "/usr/include/x86_64-linux-gnu/sys/types.h" 2 3 4
#endif
# 128 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* clockid_t.h expanded by -frewrite-includes */
#include <bits/types/clockid_t.h>
#else /* clockid_t.h expanded by -frewrite-includes */
# 128 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/clockid_t.h" 1 3 4
#ifndef __clockid_t_defined
#define __clockid_t_defined 1

#if 0 /* expanded by -frewrite-includes */
#include <bits/types.h>
#endif /* expanded by -frewrite-includes */
# 4 "/usr/include/x86_64-linux-gnu/bits/types/clockid_t.h" 3 4
# 5 "/usr/include/x86_64-linux-gnu/bits/types/clockid_t.h" 3 4

/* Clock ID used in clock and timer functions.  */
typedef __clockid_t clockid_t;

#endif
# 10 "/usr/include/x86_64-linux-gnu/bits/types/clockid_t.h" 3 4
#endif /* clockid_t.h expanded by -frewrite-includes */
# 129 "/usr/include/x86_64-linux-gnu/sys/types.h" 2 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* time_t.h expanded by -frewrite-includes */
#include <bits/types/time_t.h>
#else /* time_t.h expanded by -frewrite-includes */
# 129 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/time_t.h" 1 3 4
#ifndef __time_t_defined
#define __time_t_defined 1

#if 0 /* expanded by -frewrite-includes */
#include <bits/types.h>
#endif /* expanded by -frewrite-includes */
# 4 "/usr/include/x86_64-linux-gnu/bits/types/time_t.h" 3 4
# 5 "/usr/include/x86_64-linux-gnu/bits/types/time_t.h" 3 4

/* Returned by `time'.  */
#ifdef __USE_TIME_BITS64
typedef __time64_t time_t;
#else
# 10 "/usr/include/x86_64-linux-gnu/bits/types/time_t.h" 3 4
typedef __time_t time_t;
#endif
# 12 "/usr/include/x86_64-linux-gnu/bits/types/time_t.h" 3 4

#endif
# 14 "/usr/include/x86_64-linux-gnu/bits/types/time_t.h" 3 4
#endif /* time_t.h expanded by -frewrite-includes */
# 130 "/usr/include/x86_64-linux-gnu/sys/types.h" 2 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* timer_t.h expanded by -frewrite-includes */
#include <bits/types/timer_t.h>
#else /* timer_t.h expanded by -frewrite-includes */
# 130 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types/timer_t.h" 1 3 4
#ifndef __timer_t_defined
#define __timer_t_defined 1

#if 0 /* expanded by -frewrite-includes */
#include <bits/types.h>
#endif /* expanded by -frewrite-includes */
# 4 "/usr/include/x86_64-linux-gnu/bits/types/timer_t.h" 3 4
# 5 "/usr/include/x86_64-linux-gnu/bits/types/timer_t.h" 3 4

/* Timer ID returned by `timer_create'.  */
typedef __timer_t timer_t;

#endif
# 10 "/usr/include/x86_64-linux-gnu/bits/types/timer_t.h" 3 4
#endif /* timer_t.h expanded by -frewrite-includes */
# 131 "/usr/include/x86_64-linux-gnu/sys/types.h" 2 3 4

#ifdef __USE_XOPEN
# ifndef __useconds_t_defined
typedef __useconds_t useconds_t;
#  define __useconds_t_defined
# endif
# 137 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
# ifndef __suseconds_t_defined
typedef __suseconds_t suseconds_t;
#  define __suseconds_t_defined
# endif
# 141 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
#endif
# 142 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4

#define	__need_size_t
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* stddef.h expanded by -frewrite-includes */
#include <stddef.h>
#else /* stddef.h expanded by -frewrite-includes */
# 144 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
# 1 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 1 3 4
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
 * When clang modules are enabled, this header is a textual header. It ignores
 * its header guard so that multiple submodules can export its interfaces.
 * Take module SM with submodules A and B, whose headers both include stddef.h
 * When SM.A builds, __STDDEF_H will be defined. When SM.B builds, the
 * definition from SM.A will leak when building without local submodule
 * visibility. stddef.h wouldn't include any of its implementation headers, and
 * SM.B wouldn't import any of the stddef modules, and SM.B's `export *`
 * wouldn't export any stddef interfaces as expected. However, since stddef.h
 * ignores its header guard when building with modules, it all works as
 * expected.
 *
 * When clang modules are not enabled, the header guards can function in the
 * normal simple fashion.
 */
#if 0 /* disabled by -frewrite-includes */
#if !defined(__STDDEF_H) || __has_feature(modules) ||                          \
    (defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ >= 1) ||        \
    defined(__need_ptrdiff_t) || defined(__need_size_t) ||                     \
    defined(__need_rsize_t) || defined(__need_wchar_t) ||                      \
    defined(__need_NULL) || defined(__need_nullptr_t) ||                       \
    defined(__need_unreachable) || defined(__need_max_align_t) ||              \
    defined(__need_offsetof) || defined(__need_wint_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 38 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if !defined(__need_ptrdiff_t) && !defined(__need_size_t) &&                   \
    !defined(__need_rsize_t) && !defined(__need_wchar_t) &&                    \
    !defined(__need_NULL) && !defined(__need_nullptr_t) &&                     \
    !defined(__need_unreachable) && !defined(__need_max_align_t) &&            \
    !defined(__need_offsetof) && !defined(__need_wint_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 44 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#define __STDDEF_H
#define __need_ptrdiff_t
#define __need_size_t
/* ISO9899:2011 7.20 (C11 Annex K): Define rsize_t if __STDC_WANT_LIB_EXT1__ is
 * enabled. */
#if 0 /* disabled by -frewrite-includes */
#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ >= 1
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 50 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#define __need_rsize_t
#endif
# 52 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#define __need_wchar_t
#define __need_NULL
#if 0 /* disabled by -frewrite-includes */
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L) ||              \
    defined(__cplusplus)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 56 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#define __need_nullptr_t
#endif
# 58 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 59 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#define __need_unreachable
#endif
# 61 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if 0 /* disabled by -frewrite-includes */
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L) ||              \
    (defined(__cplusplus) && __cplusplus >= 201103L)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 63 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#define __need_max_align_t
#endif
# 65 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#define __need_offsetof
/* wint_t is provided by <wchar.h> and not <stddef.h>. It's here
 * for compatibility, but must be explicitly requested. Therefore
 * __need_wint_t is intentionally not defined here. */
#endif
# 70 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_ptrdiff_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 72 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_ptrdiff_t.h>
#endif /* expanded by -frewrite-includes */
# 72 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
# 73 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#undef __need_ptrdiff_t
#endif /* defined(__need_ptrdiff_t) */
# 75 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_size_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 77 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* __stddef_size_t.h expanded by -frewrite-includes */
#include <__stddef_size_t.h>
#else /* __stddef_size_t.h expanded by -frewrite-includes */
# 77 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
# 1 "/usr/lib/llvm-18/lib/clang/18/include/__stddef_size_t.h" 1 3 4
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
#if 0 /* evaluated by -frewrite-includes */
# 16 "/usr/lib/llvm-18/lib/clang/18/include/__stddef_size_t.h" 3 4
#define _SIZE_T

typedef __SIZE_TYPE__ size_t;

#endif
# 21 "/usr/lib/llvm-18/lib/clang/18/include/__stddef_size_t.h" 3 4
#endif /* __stddef_size_t.h expanded by -frewrite-includes */
# 78 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 2 3 4
#undef __need_size_t
#endif /*defined(__need_size_t) */
# 80 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_rsize_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 82 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_rsize_t.h>
#endif /* expanded by -frewrite-includes */
# 82 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
# 83 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#undef __need_rsize_t
#endif /* defined(__need_rsize_t) */
# 85 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_wchar_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 87 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_wchar_t.h>
#endif /* expanded by -frewrite-includes */
# 87 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
# 88 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#undef __need_wchar_t
#endif /* defined(__need_wchar_t) */
# 90 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_NULL)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 92 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_null.h>
#endif /* expanded by -frewrite-includes */
# 92 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
# 93 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#undef __need_NULL
#endif /* defined(__need_NULL) */
# 95 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_nullptr_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 97 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_nullptr_t.h>
#endif /* expanded by -frewrite-includes */
# 97 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
# 98 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#undef __need_nullptr_t
#endif /* defined(__need_nullptr_t) */
# 100 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_unreachable)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 102 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_unreachable.h>
#endif /* expanded by -frewrite-includes */
# 102 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
# 103 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#undef __need_unreachable
#endif /* defined(__need_unreachable) */
# 105 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_max_align_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 107 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_max_align_t.h>
#endif /* expanded by -frewrite-includes */
# 107 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
# 108 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#undef __need_max_align_t
#endif /* defined(__need_max_align_t) */
# 110 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined(__need_offsetof)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 112 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_offsetof.h>
#endif /* expanded by -frewrite-includes */
# 112 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
# 113 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#undef __need_offsetof
#endif /* defined(__need_offsetof) */
# 115 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

/* Some C libraries expect to see a wint_t here. Others (notably MinGW) will use
__WINT_TYPE__ directly; accommodate both by requiring __need_wint_t */
#if 0 /* disabled by -frewrite-includes */
#if defined(__need_wint_t)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 119 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <__stddef_wint_t.h>
#endif /* expanded by -frewrite-includes */
# 119 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
# 120 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#undef __need_wint_t
#endif /* __need_wint_t */
# 122 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4

#endif
# 124 "/usr/lib/llvm-18/lib/clang/18/include/stddef.h" 3 4
#endif /* stddef.h expanded by -frewrite-includes */
# 145 "/usr/include/x86_64-linux-gnu/sys/types.h" 2 3 4

#ifdef __USE_MISC
/* Old compatibility names for C types.  */
typedef unsigned long int ulong;
typedef unsigned short int ushort;
typedef unsigned int uint;
#endif
# 152 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4

/* These size-specific names are used by some of the inet code.  */

#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* stdint-intn.h expanded by -frewrite-includes */
#include <bits/stdint-intn.h>
#else /* stdint-intn.h expanded by -frewrite-includes */
# 155 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/stdint-intn.h" 1 3 4
/* Define intN_t types.
   Copyright (C) 2017-2024 Free Software Foundation, Inc.
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

#ifndef _BITS_STDINT_INTN_H
#define _BITS_STDINT_INTN_H	1

#if 0 /* expanded by -frewrite-includes */
#include <bits/types.h>
#endif /* expanded by -frewrite-includes */
# 22 "/usr/include/x86_64-linux-gnu/bits/stdint-intn.h" 3 4
# 23 "/usr/include/x86_64-linux-gnu/bits/stdint-intn.h" 3 4

typedef __int8_t int8_t;
typedef __int16_t int16_t;
typedef __int32_t int32_t;
typedef __int64_t int64_t;

#endif /* bits/stdint-intn.h */
# 30 "/usr/include/x86_64-linux-gnu/bits/stdint-intn.h" 3 4
#endif /* stdint-intn.h expanded by -frewrite-includes */
# 156 "/usr/include/x86_64-linux-gnu/sys/types.h" 2 3 4

/* These were defined by ISO C without the first `_'.  */
typedef __uint8_t u_int8_t;
typedef __uint16_t u_int16_t;
typedef __uint32_t u_int32_t;
typedef __uint64_t u_int64_t;

#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (2, 7)
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 164 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
typedef int register_t __attribute__ ((__mode__ (__word__)));
#else
# 166 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
typedef int register_t;
#endif
# 168 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4

/* Some code from BIND tests this macro to see if the types above are
   defined.  */
#define __BIT_TYPES_DEFINED__	1


#ifdef	__USE_MISC
/* In BSD <sys/types.h> is expected to define BYTE_ORDER.  */
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* endian.h expanded by -frewrite-includes */
# include <endian.h>
#else /* endian.h expanded by -frewrite-includes */
# 176 "/usr/include/x86_64-linux-gnu/sys/types.h" 3 4
# 1 "/usr/include/endian.h" 1 3 4
/* Copyright (C) 1992-2024 Free Software Foundation, Inc.
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

#ifndef	_ENDIAN_H
#define	_ENDIAN_H	1

#if 0 /* expanded by -frewrite-includes */
#include <features.h>
#endif /* expanded by -frewrite-includes */
# 21 "/usr/include/endian.h" 3 4
# 22 "/usr/include/endian.h" 3 4

/* Get the definitions of __*_ENDIAN, __BYTE_ORDER, and __FLOAT_WORD_ORDER.  */
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* endian.h expanded by -frewrite-includes */
#include <bits/endian.h>
#else /* endian.h expanded by -frewrite-includes */
# 24 "/usr/include/endian.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/endian.h" 1 3 4
/* Endian macros for string.h functions
   Copyright (C) 1992-2024 Free Software Foundation, Inc.
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
   <http://www.gnu.org/licenses/>.  */

#ifndef _BITS_ENDIAN_H
#define _BITS_ENDIAN_H 1

/* Definitions for byte order, according to significance of bytes,
   from low addresses to high addresses.  The value is what you get by
   putting '4' in the most significant byte, '3' in the second most
   significant byte, '2' in the second least significant byte, and '1'
   in the least significant byte, and then writing down one digit for
   each byte, starting with the byte at the lowest address at the left,
   and proceeding to the byte with the highest address at the right.  */

#define	__LITTLE_ENDIAN	1234
#define	__BIG_ENDIAN	4321
#define	__PDP_ENDIAN	3412

/* This file defines `__BYTE_ORDER' for the particular machine.  */
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* endianness.h expanded by -frewrite-includes */
#include <bits/endianness.h>
#else /* endianness.h expanded by -frewrite-includes */
# 35 "/usr/include/x86_64-linux-gnu/bits/endian.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/endianness.h" 1 3 4
#ifndef _BITS_ENDIANNESS_H
#define _BITS_ENDIANNESS_H 1

#ifndef _BITS_ENDIAN_H
# error "Never use <bits/endianness.h> directly; include <endian.h> instead."
#endif
# 7 "/usr/include/x86_64-linux-gnu/bits/endianness.h" 3 4

/* i386/x86_64 are little-endian.  */
#define __BYTE_ORDER __LITTLE_ENDIAN

#endif /* bits/endianness.h */
# 12 "/usr/include/x86_64-linux-gnu/bits/endianness.h" 3 4
#endif /* endianness.h expanded by -frewrite-includes */
# 36 "/usr/include/x86_64-linux-gnu/bits/endian.h" 2 3 4

/* Some machines may need to use a different endianness for floating point
   values.  */
#ifndef __FLOAT_WORD_ORDER
# define __FLOAT_WORD_ORDER __BYTE_ORDER
#endif
# 42 "/usr/include/x86_64-linux-gnu/bits/endian.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if __BYTE_ORDER == __LITTLE_ENDIAN
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 44 "/usr/include/x86_64-linux-gnu/bits/endian.h" 3 4
# define __LONG_LONG_PAIR(HI, LO) LO, HI
#if 0 /* disabled by -frewrite-includes */
#if 0
#elif __BYTE_ORDER == __BIG_ENDIAN
#endif
#endif /* disabled by -frewrite-includes */
#elif 0 /* evaluated by -frewrite-includes */
# 46 "/usr/include/x86_64-linux-gnu/bits/endian.h" 3 4
# define __LONG_LONG_PAIR(HI, LO) HI, LO
#endif
# 48 "/usr/include/x86_64-linux-gnu/bits/endian.h" 3 4

#endif /* bits/endian.h */
# 50 "/usr/include/x86_64-linux-gnu/bits/endian.h" 3 4
#endif /* endian.h expanded by -frewrite-includes */
# 25 "/usr/include/endian.h" 2 3 4

#ifdef __USE_MISC
# define LITTLE_ENDIAN	__LITTLE_ENDIAN
# define BIG_ENDIAN	__BIG_ENDIAN
# define PDP_ENDIAN	__PDP_ENDIAN
# define BYTE_ORDER	__BYTE_ORDER
#endif
# 32 "/usr/include/endian.h" 3 4

#if 0 /* disabled by -frewrite-includes */
#if defined __USE_MISC && !defined __ASSEMBLER__
#endif
#endif /* disabled by -frewrite-includes */
#if 1 /* evaluated by -frewrite-includes */
# 34 "/usr/include/endian.h" 3 4
/* Conversion interfaces.  */
#if defined(__CLANG_REWRITTEN_INCLUDES) || defined(__CLANG_REWRITTEN_SYSTEM_INCLUDES) /* byteswap.h expanded by -frewrite-includes */
# include <bits/byteswap.h>
#else /* byteswap.h expanded by -frewrite-includes */
# 35 "/usr/include/endian.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 1 3 4
/* Macros and inline functions to swap the order of bytes in integer values.
   Copyright (C) 1997-2024 Free Software Foundation, Inc.
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
#if !defined _BYTESWAP_H && !defined _NETINET_IN_H && !defined _ENDIAN_H
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 20 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 3 4
# error "Never use <bits/byteswap.h> directly; include <byteswap.h> instead."
#endif
# 22 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 3 4

#ifndef _BITS_BYTESWAP_H
#define _BITS_BYTESWAP_H 1

#if 0 /* expanded by -frewrite-includes */
#include <features.h>
#endif /* expanded by -frewrite-includes */
# 26 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 3 4
# 27 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 3 4
#if 0 /* expanded by -frewrite-includes */
#include <bits/types.h>
#endif /* expanded by -frewrite-includes */
# 27 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 3 4
# 28 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 3 4

/* Swap bytes in 16-bit value.  */
#define __bswap_constant_16(x)					\
  ((__uint16_t) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)))

static __inline __uint16_t
__bswap_16 (__uint16_t __bsx)
{
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (4, 8)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 37 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 3 4
  return __builtin_bswap16 (__bsx);
#else
# 39 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 3 4
  return __bswap_constant_16 (__bsx);
#endif
# 41 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 3 4
}

/* Swap bytes in 32-bit value.  */
#define __bswap_constant_32(x)					\
  ((((x) & 0xff000000u) >> 24) | (((x) & 0x00ff0000u) >> 8)	\
   | (((x) & 0x0000ff00u) << 8) | (((x) & 0x000000ffu) << 24))

static __inline __uint32_t
__bswap_32 (__uint32_t __bsx)
{
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (4, 3)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 52 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 3 4
  return __builtin_bswap32 (__bsx);
#else
# 54 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 3 4
  return __bswap_constant_32 (__bsx);
#endif
# 56 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 3 4
}

/* Swap bytes in 64-bit value.  */
#define __bswap_constant_64(x)			\
  ((((x) & 0xff00000000000000ull) >> 56)	\
   | (((x) & 0x00ff000000000000ull) >> 40)	\
   | (((x) & 0x0000ff0000000000ull) >> 24)	\
   | (((x) & 0x000000ff00000000ull) >> 8)	\
   | (((x) & 0x00000000ff000000ull) << 8)	\
   | (((x) & 0x0000000000ff0000ull) << 24)	\
   | (((x) & 0x000000000000ff00ull) << 40)	\
   | (((x) & 0x00000000000000ffull) << 56))

__extension__ static __inline __uint64_t
__bswap_64 (__uint64_t __bsx)
{
#if 0 /* disabled by -frewrite-includes */
#if __GNUC_PREREQ (4, 3)
#endif
#endif /* disabled by -frewrite-includes */
#if 0 /* evaluated by -frewrite-includes */
# 73 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 3 4
  return __builtin_bswap64 (__bsx);
#else
# 75 "/usr/include/x86_64-linux-gnu/bits/byteswap.h" 3 4
  return __bswap_constant_64 (__bsx);
#endif
# 77 "/usr/