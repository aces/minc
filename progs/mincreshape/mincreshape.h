/* ----------------------------- MNI Header -----------------------------------
@NAME       : mincreshape.h
@DESCRIPTION: Header file for mincreshape.c
@METHOD     : 
@GLOBALS    : 
@CALLS      : 
@CREATED    : March 11, 1994 (Peter Neelin)
@MODIFIED   : $Log: mincreshape.h,v $
@MODIFIED   : Revision 1.1  1994-11-02 16:22:00  neelin
@MODIFIED   : Initial revision
@MODIFIED   :
@COPYRIGHT  :
              Copyright 1993 Peter Neelin, McConnell Brain Imaging Centre, 
              Montreal Neurological Institute, McGill University.
              Permission to use, copy, modify, and distribute this
              software and its documentation for any purpose and without
              fee is hereby granted, provided that the above copyright
              notice appear in all copies.  The author and McGill University
              make no representations about the suitability of this
              software for any purpose.  It is provided "as is" without
              express or implied warranty.
---------------------------------------------------------------------------- */

/* Constants used in program */
#define NOFILL DBL_MAX   /* Fillvalue indicating -nofill */
#define FILL -DBL_MAX    /* Fillvalue for -fill */
#define NCOPTS_DEFAULT NC_VERBOSE | NC_FATAL
#define DEFAULT_MAX_CHUNK_SIZE_IN_KB (1024*4)
#define DIM_WIDTH_SUFFIX "-width"
#define VECTOR_SEPARATOR ','
#ifndef TRUE
#  define TRUE 1
#  define FALSE 0
#endif

/* Types used in program */

typedef struct {
   int verbose;
   int icvid, inmincid, outmincid, outimgid;
   nc_type output_datatype;
   int output_is_signed;
   int input_ndims;                  /* Number of input dimensions */
   int output_ndims;                 /* Number of output dimensions */
   long input_size[MAX_VAR_DIMS];    /* Size of input volume */
   long input_start[MAX_VAR_DIMS];   /* Start of desired hyperslab */
   long input_count[MAX_VAR_DIMS];   /* Size of desired hyperslab 
                                        (<0 means 1 and remove dimension) */ 
   int map_out_to_in[MAX_VAR_DIMS];  /* Map output dimension index to input */
   int map_in_to_out[MAX_VAR_DIMS];  /* Map input dimension index to output
                                        (-1 means no mapping) */
   int dim_used_in_block[MAX_VAR_DIMS]; /* TRUE if output dim used in block */
   int chunk_count[MAX_VAR_DIMS];    /* Specifies count for chunk hyperslab */
   int need_fillvalue;               /* TRUE if we will need a fill value */
   double fillvalue;                 /* Value to fill with (FILL_DEFAULT
                                        means fill with real value zero) */
   int do_block_normalization;       /* Normalize slices to block max/min */

   /* Note that a block is a hyperslab of the output volume in which all
      values are normalized the same way. A chunk is a hyperslab that is
      copied in one piece (smaller than or equal to a block). */

} Reshape_info;

typedef struct {
   int nentries;
   char *name[MAX_VAR_DIMS];
   long size[MAX_VAR_DIMS];
} Dimsize_list;

typedef struct {
   int nentries;
   char *name[MAX_VAR_DIMS];
   long start[MAX_VAR_DIMS];
   long count[MAX_VAR_DIMS];
} Axis_ranges;

/* Macros used in program */
#define ISSPACE(ch) (isspace((int)ch))
#define ABS(x) (((x) >= 0) ? (x) : (-(x)))
#define  MAX( x, y )  ( ((x) >= (y)) ? (x) : (y) )
#define  MIN( x, y )  ( ((x) <= (y)) ? (x) : (y) )

/* Function prototypes */
public int main(int argc, char *argv[]);
public void get_arginfo(int argc, char *argv[],
                        Reshape_info *reshape_info);
public int get_fillvalue(char *dst, char *key, char *nextArg);
public int get_dimsize(char *dst, char *key, char *nextArg);
public int get_axis_order(char *dst, char *key, char *nextArg);
public int get_axis_range(char *dst, char *key, char *nextArg);
public int get_arg_vector(char *dst, char *key, char *nextArg);
public void get_default_datatype(int mincid, nc_type *datatype, int *is_signed,
                                 double valid_range[2]);
public void setup_dim_sizes(int icvid, int mincid, Dimsize_list *dimsize_list);
public void setup_reshaping_info(int icvid, int mincid, 
                                 int do_norm, double fillvalue, int do_scalar,
                                 char *axis_order[], Axis_ranges *axis_ranges,
                                 long hs_start[], long hs_count[],
                                 int max_chunk_size_in_kb,
                                 Reshape_info *reshape_info);
public void setup_output_file(int mincid, char *history, 
                              Reshape_info *reshape_info);
public void create_dim_var(int outmincid, int outdimid,
                           int inicvid, int cur_image_dim, int inmincid, 
                           long input_start, long input_count);
public void copy_dimension_values(int outmincid, int outdimid, int inmincid,
                                  long input_start, long input_count);
public void copy_dim_var_values(int outmincid, char *dimname, char *varname,
                                int inmincid,
                                long input_start, long input_count);
public void copy_data(Reshape_info *reshape_info);
public void get_num_minmax_values(Reshape_info *reshape_info,
                                  long *block_start, long *block_count,
                                  long *num_min_values, long *num_max_values);
public void handle_normalization(Reshape_info *reshape_info,
                                 long *block_start,
                                 long *block_count,
                                 double *minmax_buffer,
                                 double *fillvalue);
public void truncate_input_vectors(Reshape_info *reshape_info,
                                   long *input_start,
                                   long *input_count);
public void translate_output_to_input(Reshape_info *reshape_info,
                                      long *output_start,
                                      long *output_count,
                                      long *input_start,
                                      long *input_count);
public void translate_input_to_output(Reshape_info *reshape_info,
                                      long *input_start,
                                      long *input_count,
                                      long *output_start,
                                      long *output_count);
public void copy_the_chunk(Reshape_info *reshape_info,
                           long chunk_start[],
                           long chunk_count[],
                           void *chunk_data,
                           double fillvalue);
public void convert_value_from_double(double dvalue, 
                                      nc_type datatype, int is_signed,
                                      void *ptr);
public void nd_begin_looping(long start[], long current[], int ndims);
public int nd_end_of_loop(long start[], long end[], int ndims);
public void nd_update_current_count(long current[], 
                                    long increment[], long end[],
                                    long current_count[],
                                    int ndims);
public void nd_increment_loop(long current[], 
                              long start[], long increment[], long end[],
                              int ndims);