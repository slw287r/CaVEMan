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

#ifndef _genotype_h
#define _genotype_h

#include <List.h>

typedef struct genotype_t{
	int a_count;
	int c_count;
	int g_count;
	int t_count;	
	char var_base;
	int var_base_idx;
	long double var_base_prop;
}genotype_t;

typedef struct combined_genotype_t{
	genotype_t *norm_geno;
	genotype_t *tum_geno;
	long double prob;
}combined_genotype_t;

typedef struct genotype_store_t{
	List *normal_genos;
	List *tumour_genos;	
	combined_genotype_t *ref_genotype;
	long double ref_geno_norm_prob;
	long double ref_geno_tum_prob;
	combined_genotype_t **het_snp_norm_genotypes;
	int het_norm_count;
	combined_genotype_t **het_snp_genotypes;
	int het_count;
	combined_genotype_t **hom_snp_genotypes;
	int hom_count;
	combined_genotype_t **somatic_genotypes;
	int somatic_count;
	int tum_max;
	int norm_max;
	int total_max;
}genotype_store_t;

int genotype_equals(genotype_t *g_a, genotype_t *g_b);
genotype_t *genotype_init_genotype();
void genotype_clear_genotype_cache();
List *genotype_hard_copy_genotype_t_list(List *new_list, List *old);
List *genotype_calculate_genotypes(int copy_num, char *ref_base);
char *genotype_get_genotype_t_as_string(genotype_t *geno);
long double genotype_get_var_base_proportion(genotype_t *gen, char ref_base, int copy_num);
char genotype_get_var_base(genotype_t *geno, char ref_base);
void genotype_add_base_to_count(genotype_t *geno, char base);
int genotype_get_base_count(genotype_t *geno, char base);
void genotype_set_base_count(genotype_t *geno, char base, int count);
int genotype_get_total_base_count(genotype_t *geno);
genotype_t *genotype_copy_genotype(genotype_t *geno);
genotype_store_t *genotype_generate_genotype_list_for_cn_and_ref_base(int norm_cn, int tum_cn, char *ref_base);
void genotype_destroy_genotype_store(genotype_store_t *store);

#endif
