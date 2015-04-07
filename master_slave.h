/*
Copyright (C) 2013  
Fabien Gaud <fgaud@sfu.ca>, Baptiste Lepers <baptiste.lepers@inria.fr>,
Fabien Mottet <fabien.mottet@inria.fr>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
version 2 or later, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef MASTER_SLAVE_H_
#define MASTER_SLAVE_H_

#include "debug_tools.h"

#define UNIQUE_FILE_ACCESS_PATTERN      1
#define SPECWEB99_FILE_ACCESS_PATTERN   0
#define SPECWEB05_FILE_ACCESS_PATTERN   0
#define SPECWEB09_BANKING_WORKLOAD      0

// Other protocol
#define CLOSE_AFTER_REQUEST             0


#if SPECWEB99_FILE_ACCESS_PATTERN
// For specweb99 explaination see http://www.spec.org/web99/docs/whitepaper.html
#define SPECWEB99_NB_CLASS                      4
#define SPECWEB99_NB_FILES                      9
#define SPECWEB99_LOAD                          1000


// Frequency of each class
const double class_freq[] =                     {0.35, 0.85, 0.99, 1};

//fancy class frequencies to show increased throughput (DON'T USE FOR BENCH, NOT RELEVANT)
//const double class_freq[] =                     {0.35, 0.55, 0.70, 1};

// File frequency within each class
const double file_freq[] =                     {0.039, 0.098, 0.186, 0.363, 0.716, 0.834, 0.905, 0.955, 1};


#elif SPECWEB05_FILE_ACCESS_PATTERN
// For specweb99 explaination see http://www.spec.org/web2005/docs/SupportDesign.html
#define SPECWEB05_NB_CLASS                      6
#define SPECWEB05_NB_FILES                      16
#define SPECWEB05_MAX_FILE_PER_CLASS            5

#define SPECWEB05_SIMULTANEOUS_SESSIONS         1000
//#define SPECWEB05_DIRSCALING                    0.25  //Legacy value but doesn't fit in memory
#define SPECWEB05_DIRSCALING                    0.00300
#define SPECWEB05_ZIPF_ALPHA                    1.2

// Frequency of each class
const double class_freq[] =                     {0.1366, 0.2627, 0.5467, .7699, .8949, 1.};
// Order of file popularity within each class
const double file_freq[SPECWEB05_NB_CLASS][SPECWEB05_MAX_FILE_PER_CLASS]
                                           =    {
                                                    {0.273, 0.364, 0.529, 0.715,  1.00},
                                                    {0.579, 0.757,  1.00, -1.00, -1.00},
                                                    {0.275, 0.445, 0.615,  1.00, -1.00},
                                                    {0.666,  1.00, -1.00, -1.00, -1.00},
                                                    { 1.00, -1.00, -1.00, -1.00, -1.00},
                                                    { 1.00, -1.00, -1.00, -1.00, -1.00}
                                           };

/** PHP **/
#define PHP_NB_FILES                            6

const char* php_files[PHP_NB_FILES] = {
         "index.php",
         "fileCatalog.php?id=3279&category=4&os=4&lang=9",
         "search.php?q=cdrom+wireless+memory",
         "file.php?id=36043",
         "product.php?id=1812",
         "catalog.php?id=1165"
};

/** Images **/
#define IMAGES_NB_FILES                         31
const char* images_files[IMAGES_NB_FILES] = {
         "template_javascripts",
         "flattab_sl",
         "h_product_selection",
         "blue_arrow_top",
         "print",
         "email",
         "flattab_nr",
         "bar",
         "button-1",
         "nav_q",
         "us",
         "help",
         "aaa",
         "masthead_transparent",
         "button-3",
         "masthead_local_sep",
         "H_Service_Tag_Unkown",
         "content_arrow",
         "masthead_global",
         "global",
         "content_action",
         "H_D",
         "masthead_subnavsep",
         "note",
         "button-2",
         "___",
         "flattab_nl",
         "blue_arrow_right",
         "flattab_sr",
         "ccc",
         "spacer"
};

/** Workload Mix **/
/** images, PHP, downloads **/
const double type_freq[] = {
         0.85,
         0.99,
         1
};

#elif SPECWEB09_BANKING_WORKLOAD
/** Based on
	https://www.spec.org/web2009/docs/pseudocode/banking_scripts_pseudocode.html
	And https://www.spec.org/web2009/docs/design/BankingDesign.html **/

#define SPECWEB09_BANKING_WORKLOAD_PAGE_COUNT 17

// Mapping ID => PHP page
// Note: don't support multiple pages per ID
const char * page_mapping[SPECWEB09_BANKING_WORKLOAD_PAGE_COUNT] = {
	"/bank/login.php",
	"/bank/account_summary.php",
	"/bank/check_detail_html.php?check_no=6",
	"/bank/bill_pay.php?userid=39",
	"/bank/add_payee.php?userid=0000000079",
	"/bank/post_payee.php",
	"/bank/quick_pay.php",
	"/bank/bill_pay_status_output.php?userid=39&start=2005-01-9&end=2005-02-09",
	"/bank/profile.php?userid=90",
	"/bank/change_profile.php",
	"/bank/order_check.php?userid=76",
	"/bank/place_check_order.php",
	"/bank/transfer.php",
	"/bank/post_transfer.php",
	"/bank/logout.php?userid=6",
	"/bank/check_detail_image.php?side=front&check_no=6",
	"/bank/check_detail_image.php?side=back&check_no=6",
};

// Markov table for transitions between dynamic pages

/** The probabilities on most rows (except from states 2, 14, and 16) add up to
 * 0.8. This is because there is a 20% probability associated with the user
 * leaving the system from any state. States 15 and 16 are not real states, they
 * are part of state 2 and were created to ease implementation.
 * ==>
 * 20% of probability to fetch login.php again
 */
const float page_transitions[SPECWEB09_BANKING_WORKLOAD_PAGE_COUNT][SPECWEB09_BANKING_WORKLOAD_PAGE_COUNT] = {
	// 0     1     2     3     4     5     6     7     8     9    10    11    12    13    14    15    16
	{0.20, 0.32, 0.  , 0.32, 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.08, 0.  , 0.08, 0.  , 0.  },
	{0.20, 0.  , 0.56, 0.  , 0.  , 0.  , 0.  , 0.  , 0.08, 0.  , 0.08, 0.  , 0.  , 0.  , 0.08, 0.  , 0.  },
	{0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 1.00, 0.  },
	{0.20, 0.  , 0.  , 0.  , 0.08, 0.  , 0.48, 0.16, 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.08, 0.  , 0.  },
	{0.20, 0.  , 0.  , 0.  , 0.  , 0.72, 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.08, 0.  , 0.  },
	{0.20, 0.  , 0.  , 0.72, 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.08, 0.  , 0.  },
	{0.20, 0.  , 0.  , 0.72, 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.08, 0.  , 0.  },
	{0.20, 0.  , 0.  , 0.72, 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.08, 0.  , 0.  },
	{0.20, 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.72, 0.  , 0.  , 0.  , 0.  , 0.08, 0.  , 0.  },
	{0.20, 0.72, 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.08, 0.  , 0.  },
	{0.20, 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.72, 0.  , 0.  , 0.08, 0.  , 0.  },
	{0.20, 0.72, 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.08, 0.  , 0.  },
	{0.20, 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.72, 0.08, 0.  , 0.  },
	{0.20, 0.72, 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.08, 0.  , 0.  },
	{1.00, 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  },
	{0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 1.00},
	{0.20, 0.72, 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.  , 0.08, 0.  , 0.  },
};

#endif

/** Internal for communication **/
// Master orders buffer size
#define MASTER_ORDER_BUFFER_SIZE                256

// Response max size
#define RESPONSE_SIZE	                        1024

#if UNIQUE_FILE_ACCESS_PATTERN + SPECWEB99_FILE_ACCESS_PATTERN + SPECWEB05_FILE_ACCESS_PATTERN + SPECWEB09_BANKING_WORKLOAD!= 1
#error 'You must choose a (unique) file workload'
#endif


#ifdef __x86_64__
#define rdtscll(val) { \
    unsigned int __a,__d;                                        \
    asm volatile("rdtsc" : "=a" (__a), "=d" (__d));              \
    (val) = ((unsigned long)__a) | (((unsigned long)__d)<<32);   \
}

#else
   #define rdtscll(val) __asm__ __volatile__("rdtsc" : "=A" (val))
#endif

#endif /*MASTER_SLAVE_H_*/
