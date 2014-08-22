/**   LICENSE
* Copyright (c) 2014 Genome Research Ltd.
*
* Author: Cancer Genome Project cgpit@sanger.ac.uk
*
* This file is part of CaVEMan.
*
* CaVEMan is free software: you can redistribute it and/or modify it under
* the terms of the GNU Affero General Public License as published by the Free
* Software Foundation; either version 3 of the License, or (at your option) any
* later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
* details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _cn_access_h
#define _cn_access_h

#include <stdio.h>

int cn_access_get_copy_number_for_location(char *file_loc,char *chr,int pos,int is_normal);
void clear_copy_number_store();
void cn_access_set_max_cn(int max_copy_number);
int cn_access_get_mean_cn_for_range(char *file_loc,char *chr,int start,int stop,int is_normal);

#endif
