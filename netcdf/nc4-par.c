/* Copyright 2019 University Corporation for Atmospheric
    Research/Unidata.  See COPYRIGHT file for conditions of use. */
 #include <stdlib.h>
 #include <stdio.h>
 #include <netcdf_par.h>
 #include <netcdf.h>
  
 /* This is the name of the data file we will create. */
 #define FILE_NAME "simple_xy_nc4.nc"
  
 /* We are writing 2D data, a 6 x 12 grid. */
 #define NDIMS 2
 #define NX 6
 #define NY 12
  
 /* Handle errors by printing an error message and exiting with a
  * non-zero status. */
 #define ERRCODE 2
 #define ERR(e) {printf("Error: %s\n", nc_strerror(e)); exit(ERRCODE);}
  
 int main(int argc, char ** argv) {
    MPI_Init(& argc, & argv);
    int ncid, x_dimid, y_dimid, varid;
    int dimids[NDIMS];
    size_t chunks[NDIMS];
    int shuffle, deflate, deflate_level;
    int data_out[NX][NY];
    int x, y, retval;
  
    /* Set chunking, shuffle, and deflate. */
    shuffle = NC_SHUFFLE;
    deflate = 1;
    deflate_level = 1;
  
    /* Create some pretend data. If this wasn't an example program, we
     * would have some real data to write, for example, model output. */
    for (x = 0; x < NX; x++)
       for (y = 0; y < NY; y++)
          data_out[x][y] = x * NY + y;
  
    /* Create the file. The NC_NETCDF4 parameter tells netCDF to create
     * a file in netCDF-4/HDF5 standard. */
    if ((retval = nc_create_par(FILE_NAME, NC_CLOBBER | NC_MPIIO | NC_NETCDF4, MPI_COMM_WORLD, MPI_INFO_NULL, &ncid)))
       ERR(retval);
  
    /* Define the dimensions. */
    if ((retval = nc_def_dim(ncid, "x", NX, &x_dimid)))
       ERR(retval);
    if ((retval = nc_def_dim(ncid, "y", NY, &y_dimid)))
       ERR(retval);
  
    /* Set up variable data. */
    dimids[0] = x_dimid;
    dimids[1] = y_dimid;
    chunks[0] = NX/4;
    chunks[1] = NY/4;
  
    /* Define the variable. */
    if ((retval = nc_def_var(ncid, "data", NC_INT, NDIMS,
                             dimids, &varid)))
       ERR(retval);
    if ((retval = nc_def_var_chunking(ncid, varid, 0, &chunks[0])))
       ERR(retval);
    /*if ((retval = nc_def_var_deflate(ncid, varid, shuffle, deflate,
                                     deflate_level)))
       ERR(retval); */
    if((retval = nc_var_par_access(ncid, varid, NC_COLLECTIVE))) ERR(retval);
    if ((retval = nc_enddef(ncid))) ERR(retval);


  
    /* No need to explicitly end define mode for netCDF-4 files. Write
     * the pretend data to the file. */
    if ((retval = nc_put_var_int(ncid, varid, &data_out[0][0])))
       ERR(retval);
  
    /* Close the file. */
    if ((retval = nc_close(ncid)))
       ERR(retval);
  
    printf("*** SUCCESS writing example file simple_xy_nc4.nc!\n");
    MPI_Finalize();
    return 0;
 }
