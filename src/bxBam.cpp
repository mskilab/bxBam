#include <inttypes.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> // for optparse
#include <Rcpp.h>
#include <iostream>
#include <stdio.h>

#include "htslib/hts.h"
#include "htslib/bgzf.h"
#include "htslib/hts.h"

#include "include/bamdb.h"
#include "include/bam_sqlite.h"
#include "include/bam_lmdb.h"
#include "include/bam_api.h"
#include "include/bamdb_c.h"

using namespace Rcpp;
using namespace std;

/* Command to do successfully run.
barcodedReads('/gpfs/commons/groups/imielinski_lab/data/10X/bam3/HCC1143_BL_phased_possorted.bam','/gpfs/commons/groups/imielinski_lab/data/10X/bam3/HCC1143_BL_phased_possorted.db','GTGGTCGCAACGCTTA-1')
*/

#define WORK_BUFFER_SIZE 65536

#define get_int_chars(i) ((i == 0) ? 1 : floor(log10(abs(i))) + 1)

// [[Rcpp::export]]
void barcodedReads(std::string& bamFile, std::string& indexFile, std::string& barcode){

  printf("KHAGAY\n");
  print_bam_sqlite();
  int rc = 0;
  samFile *input_file = 0;
  bam_args_t bam_args;
  int max_rows = 0;
  offset_list_t *offset_list = NULL;

  bam_args.index_file_name = NULL;
  bam_args.bx = NULL;
  bam_args.convert_to = BAMDB_CONVERT_TO_TEXT;
  int c;

  // change the values of the struct bam_args_t to those of the input values.

  strcpy(bam_args.input_file_name, bamFile.c_str());
  bam_args.index_file_name = strdup(indexFile.c_str()); // returns a pointer to a new string which is a duplicate of the input c-style string.
  bam_args.bx = strdup(barcode.c_str());

  input_file = sam_open(bam_args.input_file_name, "r");

  //  printf("%p\n", input_file);

  printf("SDASDSADSSSSSSSSSSSSSSSSSSSSS\n");
  // open file and write to it.
  //  ofstream outfile;
  //  outfile.open(outputFile);

  //  ofstream outfile;

  //  std::ifstream in(outputFile.c_str());

  // open file for writing results of querying a bam file for a given barcode.

  //FILE *ifp; // this worked.
  //ifp = fopen(outputFile.c_str(), "w"); //this worked.

  //if (ifp == NULL){
  // fprintf(stderr, "Can't open the input file\n");
  // exit(1);
  //}

  if (bam_args.bx != NULL && bam_args.index_file_name != NULL) {
    // allocate memory for an object of offset_list_t that has 1 element.
    offset_list = (offset_list_t *)calloc(1, sizeof(offset_list_t));
    int intermediate = get_offsets(offset_list, bam_args.index_file_name, bam_args.bx);
    //printf("%d\n", intermediate);
    // write bam records that share barcode to file.

    //fprintf(ifp, "%d\n", intermediate);

    // writing to file.
    // outfile << intermediate << std::endl;
  }
  //outfile.close();
  //fclose(ifp);


  rc = read_file(input_file, offset_list);
}

// [[Rcpp::export]]
void
generate_bxi(const std::string& bamFile){
  int c = 0;
  int rc = 0;
  samFile *input_file = 0;
  bam_args_t bam_args;
  int max_rows = 0;
  offset_list_t *offset_list = NULL;

  bam_args.index_file_name = NULL;
  bam_args.bx = NULL;
  bam_args.convert_to = BAMDB_CONVERT_TO_TEXT;
  // the following command is for the -t parameter and the "sqlite" string that is passed to it.
  bam_args.convert_to = BAMDB_CONVERT_TO_SQLITE;
  // the following command is for the -f parameter.
  strcpy(bam_args.input_file_name, bamFile.c_str());

  input_file = sam_open(bam_args.input_file_name, "r");

  if (bam_args.convert_to == BAMDB_CONVERT_TO_SQLITE) {
    rc = convert_to_sqlite(input_file, NULL, max_rows);
  }
}

int main()
{
}
