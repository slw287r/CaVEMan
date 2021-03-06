/**   LICENSE
* Copyright (c) 2014-2018 Genome Research Ltd.
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

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <limits.h>
#include <assert.h>
#include <dbg.h>
#include <alg_bean.h>
#include <config_file_access.h>

static int includeSW = 0;
static int includeSingleEnd = 0;
static int includeDups = 0;
static char *tum_bam_file;
static char *norm_bam_file;
static char *results = "./results";
static char *ref_idx = NULL;
static char *list_loc = "./splitList";
static char *alg_bean_loc = "./alg_bean";
static char *norm_copy_no;
static char *tum_copy_no ;
static char *ignore_regions_file;
static char *CaVEManfg_ini = "./caveman.cfg.ini";


void setup_print_usage (int exit_code){
	printf ("Usage: caveman setup -t tum.bam -n norm.bam -r reference.fa.fai -g ignore_regions.tab -e tum_cn.bed -j norm_cn.bed [-f path] [-l path] [-a path] [-wzu]\n\n");
	printf("-t  --tumour-bam [file]             Location of tumour bam\n");
	printf("-n  --normal-bam [file]             Location of normal bam\n");
	printf("-r  --reference-index [file]        Location of reference fasta index\n");
	printf("-g  --ignore-regions-file [file]    Location of tsv ignore regions file\n\n");
	printf("Optional\n");
	printf("-c  --config-file [file]            File to write caveman run config file [default:'%s']\n",CaVEManfg_ini);
	printf("-f  --results-folder [file]         Folder to write results [default:'%s']\n",results);
	printf("-l  --split-file [file]             File to write list of split sections [default:'%s']\n",list_loc);
	printf("-a  --alg-bean-file [file]          Location to write alg-bean [default:'%s']\n",alg_bean_loc);
	printf("-w  --include-smith-waterman        Include SW mapped reads in the analysis\n");
	printf("-z  --include-single-end            Use single end reads for this analysis\n");
	printf("-u  --include-duplicates            Include reads marked as duplicates in the analysis\n");
		printf("-e  --tumour-copy-no-file [file]    Location of tumour copy number bed file (if the extension is not .bed the file will be treated as 1 based start). If no copy number file is supplied then the default cn of 2 will be used\n");
	printf("-j  --normal-copy-no-file [file]    Location of normal copy number bed file (if the extension is not .bed the file will be treated as 1 based start). If no copy number file is supplied then the default cn of 2 will be used\n");
	printf("-h	--help                          Display this usage information.\n");
  exit(exit_code);
}

void setup_setup_options(int argc, char *argv[]){
	const struct option long_opts[] =
	{
		{"tumour-bam", required_argument, 0, 't'},
		{"normal-bam", required_argument, 0, 'n'},
		{"results-folder", required_argument, 0, 'f'},
		{"split-file", required_argument, 0, 'l'},
		{"alg-bean-file",required_argument, 0, 'a'},
		{"reference-index", required_argument, 0, 'r'},
		{"include-smith-waterman", no_argument, 0, 'w'},
		{"include-single-end", no_argument, 0, 'z'},
		{"include-duplicates", no_argument, 0, 'u'},
		{"config-file",required_argument, 0,'c'},
		{"ignore-regions-file", required_argument, 0, 'g'},
		{"tumour-copy-no-file", required_argument, 0, 'e'},
		{"normal-copy-no-file", required_argument, 0, 'j'},
		{"help", no_argument, 0, 'h'},
		{ NULL, 0, NULL, 0}
   }; //End of declaring opts

   int index = 0;
   int iarg = 0;

   //Iterate through options
   while((iarg = getopt_long(argc, argv, "t:n:r:f:a:l:e:j:g:c:wzuh",
                            								long_opts, &index)) != -1){
   	switch(iarg){
				case 'h':
					setup_print_usage(0);
					break;

				case 'c':
					CaVEManfg_ini = optarg;
					break;

				case 't':
					tum_bam_file = optarg;
					break;

				case 'n':
					norm_bam_file = optarg;
					break;

				case 'g':
					ignore_regions_file = optarg;
					break;

				case 'j':
					norm_copy_no = optarg;
					break;

				case 'e':
					tum_copy_no = optarg;
					break;

				case 'r':
					ref_idx = optarg;
					break;

				case 'f':
					results = optarg;
					break;

				case 'a':
					alg_bean_loc = optarg;
					break;

				case 'l':
					list_loc = optarg;
					break;

				case 'w':
					includeSW = 1;
					break;

				case 'z':
					includeSingleEnd = 1;
					break;

				case 'u':
					includeDups = 1;
					break;

				case '?':
					setup_print_usage (1);
					break;

      	default:
      		setup_print_usage (1);

   	}; // End of args switch statement

   }//End of iteration through options

   //Do some checking to ensure required arguments were passed
   if(tum_bam_file == NULL || norm_bam_file == NULL || ref_idx  == NULL
   			|| ignore_regions_file == NULL){
   	setup_print_usage(1);
   }
   return;
}

int setup_main(int argc, char *argv[]){
	setup_setup_options(argc,argv);

	//Create and write the config file in the current directory.
	FILE *config_read;
	char CaVEMancfg_ini[PATH_MAX+1];
	config_read = fopen(CaVEManfg_ini,"r");
	if(config_read == NULL){
		FILE *config_create;
		config_create = fopen(CaVEManfg_ini,"w");
		check(config_create != NULL,"Error trying to initially create config file location for write: %s.",CaVEManfg_ini);
		int res = fclose(config_create);
		check(res==0,"Error closing config file writer after initially creating config file.");
	}else{
		int res = fclose(config_read);
		check(res==0,"Error closing config file reader when checking for existance.");
		printf("Config file file: '%s' already exists. Overwriting\n",CaVEManfg_ini);
	}
	char *ptr = realpath(CaVEManfg_ini,CaVEMancfg_ini);
	check(ptr!=NULL,"Error assigning real path for caveman config file %s.",CaVEManfg_ini);

	char alg_bean_file[PATH_MAX+1];
	FILE *alg_check_read;
	alg_check_read = fopen(alg_bean_loc,"r");
	if(alg_check_read == NULL){
		FILE *alg_create;
		alg_create = fopen(alg_bean_loc,"w");
		check(alg_create != NULL,"Error trying to initially create alg_bean location for write: %s.",alg_bean_loc);
		int res = fclose(alg_create);
		check(res==0,"Error closing config file writer after initially creating alb_bean file.");
	}else{
		int res = fclose(alg_check_read);
		check(res==0,"Error closing alg_bean reader when checking for existance.");
		printf("Alg bean file: '%s' already exists. Overwriting\n",alg_bean_loc);
	}
	ptr = realpath(alg_bean_loc,alg_bean_file);
	check(ptr!=NULL,"Error assigning real path for alg bean file %s.",alg_bean_loc);

	//Try reading to see if we already have one
	FILE *config_out = fopen(CaVEManfg_ini,"w");
	check(config_out != NULL,"Error trying to open config file location for write: %s.",CaVEMancfg_ini);
	int res = config_file_access_write_config_file(config_out, tum_bam_file, norm_bam_file, ref_idx,
																	ignore_regions_file, alg_bean_file, results, list_loc, includeSW,
																				includeSingleEnd, includeDups, norm_copy_no, tum_copy_no);
	check(res==0,"Problem encountered when writing config file to to %s.",CaVEMancfg_ini);
	res = fclose(config_out);
	check(res==0,"Error closing config file.");

	//Create the alg bean
	FILE *bean_out = fopen(alg_bean_file,"w");
	check(bean_out != NULL,"Error trying to open alg_bean file location for write: %s.",alg_bean_file);
	int res_chk = alg_bean_create_default_file(bean_out,norm_bam_file,tum_bam_file);
	check(res_chk==0,"Problem encountered when writing new alg bean to %s.",alg_bean_file);
	res_chk = fclose(bean_out);
	check(res_chk==0,"Error closing alg bean file.");

	return 0;
error:
	return -1;

}
