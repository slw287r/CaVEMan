/**   LICENSE
* Copyright (c) 2014-2015 Genome Research Ltd.
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
*
*    1. The usage of a range of years within a copyright statement contained within
*    this distribution should be interpreted as being equivalent to a list of years
*    including the first and last year specified and all consecutive years between
*    them. For example, a copyright statement that reads ‘Copyright (c) 2005, 2007-
*    2009, 2011-2012’ should be interpreted as being identical to a statement that
*    reads ‘Copyright (c) 2005, 2007, 2008, 2009, 2011, 2012’ and a copyright
*    statement that reads ‘Copyright (c) 2005-2012’ should be interpreted as being
*    identical to a statement that reads ‘Copyright (c) 2005, 2006, 2007, 2008,
*    2009, 2010, 2011, 2012’."
*
*/

#include "minunit.h"
#include <ignore_reg_access.h>

char *test_ign_file = "testData/ign.test";
//Above file contains:
//1	200	5000
//10	13	5678
//10	5679	5699

char *test_ignore_reg_access_get_ign_reg_count_for_chr(){
	char *chr = "1";
	int res = ignore_reg_access_get_ign_reg_count_for_chr(test_ign_file, chr);
	mu_assert(res==1,"Incorrect count of ignored regions returned.\n");
	chr = "10";
	res = ignore_reg_access_get_ign_reg_count_for_chr(test_ign_file, chr);
	mu_assert(res==2,"Incorrect count of ignored regions returned.\n");
	return NULL;
}

char *test_ignore_reg_access_get_ign_reg_overlap(){
	struct seq_region_t **ignore_regs;
	int ignore_reg_count = 2;
	ignore_regs = malloc(sizeof(struct seq_region_t *) *  ignore_reg_count);
	char *chr = "10";
	ignore_reg_access_get_ign_reg_for_chr(test_ign_file,chr,ignore_reg_count, ignore_regs);
	seq_region_t *reg = ignore_reg_access_get_ign_reg_overlap(5677,ignore_regs, ignore_reg_count);
	mu_assert(reg->beg == 13,"Wrong start retrieved for ignored region 1.\n");
	mu_assert(reg->end == 5678,"Wrong stop retrieved for ignored region 1.\n");
	reg = ignore_reg_access_get_ign_reg_overlap(5678,ignore_regs, ignore_reg_count);
	mu_assert(reg->beg == 13,"Wrong start retrieved for ignored region 2.\n");
	mu_assert(reg->end == 5678,"Wrong stop retrieved for ignored region 2.\n");
	reg = ignore_reg_access_get_ign_reg_overlap(5680,ignore_regs, ignore_reg_count);
	mu_assert(reg->beg == 5680,"Wrong start retrieved for ignored region 2.\n");
	mu_assert(reg->end == 5699,"Wrong stop retrieved for ignored region 2.\n");
	ignore_reg_access_destroy_seq_region_t_arr(ignore_reg_count, ignore_regs);
	return NULL;
}

char *test_ignore_reg_access_get_ign_reg_for_chr(){
    struct seq_region_t **ignore_regs;
    int ignore_reg_count = 2;
    char *chr = "10";
    ignore_regs = malloc(sizeof(struct seq_region_t *) *  ignore_reg_count);
    ignore_reg_access_get_ign_reg_for_chr(test_ign_file,chr,ignore_reg_count, ignore_regs);
    mu_assert(((seq_region_t *)ignore_regs[0])->beg == 13,"Wrong start retrieved for ignored region 1.\n");
    mu_assert(((seq_region_t *)ignore_regs[0])->end == 5678,"Wrong stop retrieved for ignored region 1.\n");
    mu_assert(((seq_region_t *)ignore_regs[1])->beg == 5680,"Wrong start retrieved for ignored region 2.\n");
    mu_assert(((seq_region_t *)ignore_regs[1])->end == 5699,"Wrong stop retrieved for ignored region 2.\n");
    ignore_reg_access_destroy_seq_region_t_arr(ignore_reg_count, ignore_regs);
    return NULL;
}

char *test_ignore_reg_access_get_ign_regs_covered(){
    //List *ignore_reg_access_get_ign_reg_contained(int from, int to, struct seq_region_t **regions, int entry_count)
    struct seq_region_t **ignore_regs;
    int ignore_reg_count = 2;
    //10	13	5678
    //10	5680	5699
    ignore_regs = malloc(sizeof(struct seq_region_t *) *  ignore_reg_count);
    char *chr = "10";
    ignore_reg_access_get_ign_reg_for_chr(test_ign_file,chr,ignore_reg_count, ignore_regs);
    List *contained = ignore_reg_access_get_ign_regs_covered(10,5679,ignore_regs,ignore_reg_count);
    mu_assert(List_count(contained)==1,"Incorrect number of regions found.\n")
    List_clear_destroy(contained);
    contained = ignore_reg_access_get_ign_regs_covered(10,5700,ignore_regs,ignore_reg_count);
    mu_assert(List_count(contained)==2,"Incorrect number of regions found.\n")
    List_clear_destroy(contained);
    //Try with a beginning overlap - covered
    contained = ignore_reg_access_get_ign_regs_covered(14,5700,ignore_regs,ignore_reg_count);
    mu_assert(List_count(contained)==2,"Incorrect number of regions found.\n")
    List_clear_destroy(contained);
    //Try with a end overlap - covered
    contained = ignore_reg_access_get_ign_regs_covered(5697,5700,ignore_regs,ignore_reg_count);
    fprintf(stderr,"REGIONS4: %d\n",List_count(contained));
    mu_assert(List_count(contained)==1,"Incorrect number of regions found.\n")
    List_clear_destroy(contained);
    ignore_reg_access_destroy_seq_region_t_arr(ignore_reg_count, ignore_regs);
    return NULL;
}

char *test_ignore_reg_access_resolve_ignores_to_analysis_sections(){
    //List *ignore_reg_access_resolve_ignores_to_analysis_sections(int start, int end, struct seq_region_t **regions, int entry_count)
    struct seq_region_t **ignore_regs;
    int ignore_reg_count = 2;
    ignore_regs = malloc(sizeof(struct seq_region_t *) *  ignore_reg_count);
    char *chr = "10";
    ignore_reg_access_get_ign_reg_for_chr(test_ign_file,chr,ignore_reg_count, ignore_regs);
    //10	13	5678
    //10	5680	5699
    List *sects = ignore_reg_access_resolve_ignores_to_analysis_sections(11,5679,ignore_regs,ignore_reg_count);
    mu_assert(List_count(sects)==2,"Incorrect number of sections resolved.\n");
    mu_assert(((seq_region_t *)sects->first->value)->beg == 11,"Incorrect first section start.\n");
    mu_assert(((seq_region_t *)sects->first->value)->end == 12,"Incorrect first section stop.\n");
    mu_assert(((seq_region_t *)sects->last->value)->beg == 5679,"Incorrect first section start.\n");
    mu_assert(((seq_region_t *)sects->last->value)->end == 5679,"Incorrect first section stop.\n");
    List_clear_destroy(sects);

    //Try with a start overlap (1 inside the region)
    sects = ignore_reg_access_resolve_ignores_to_analysis_sections(14,5800,ignore_regs,ignore_reg_count);
    LIST_FOREACH(sects, first, next, cur){
        fprintf(stderr,"REGION FOUND: %d-%d\n",((seq_region_t *) cur->value)->beg,((seq_region_t *) cur->value)->end);
    }
    mu_assert(List_count(sects)==2,"Incorrect number of sections resolved.\n");
    mu_assert(((seq_region_t *)sects->first->value)->beg == 5679,"Incorrect first section start.\n");
    mu_assert(((seq_region_t *)sects->first->value)->end == 5679,"Incorrect first section stop.\n");
    mu_assert(((seq_region_t *)sects->last->value)->beg == 5700,"Incorrect first section start.\n");
    mu_assert(((seq_region_t *)sects->last->value)->end == 5800,"Incorrect first section stop.\n");
    List_clear_destroy(sects);

    // Try with a start overlap exactly one before the start of the region as this is possible with the split logic
    sects = ignore_reg_access_resolve_ignores_to_analysis_sections(12,5800,ignore_regs,ignore_reg_count);
    mu_assert(List_count(sects)==3,"Incorrect number of sections resolved.\n");
    mu_assert(((seq_region_t *)sects->first->value)->beg == 12,"Incorrect first section start.\n");
    mu_assert(((seq_region_t *)sects->first->value)->end == 12,"Incorrect first section stop.\n");
    mu_assert(((seq_region_t *)sects->last->value)->beg == 5700,"Incorrect first section start.\n");
    mu_assert(((seq_region_t *)sects->last->value)->end == 5800,"Incorrect first section stop.\n");
    List_clear_destroy(sects);
    ignore_reg_access_destroy_seq_region_t_arr(ignore_reg_count, ignore_regs);


    return NULL;
}

char *all_tests() {
   mu_suite_start();
   mu_run_test(test_ignore_reg_access_get_ign_reg_count_for_chr);
   mu_run_test(test_ignore_reg_access_get_ign_reg_for_chr);
   mu_run_test(test_ignore_reg_access_get_ign_reg_overlap);
   mu_run_test(test_ignore_reg_access_get_ign_regs_covered);
   mu_run_test(test_ignore_reg_access_resolve_ignores_to_analysis_sections);

   return NULL;
}

RUN_TESTS(all_tests);
