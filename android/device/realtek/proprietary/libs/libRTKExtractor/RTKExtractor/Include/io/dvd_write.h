#ifndef DVD_WRITE_H_INCLUDED
#define DVD_WRITE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

int32_t RTDVDFileWriteSeek( dvd_file_t *dvd_file, __off64_t offset );
int RTDVDFileWriteBytes( dvd_file_t *dvd_file,  __off64_t offset,
			      unsigned char *data, size_t bytes_count);
int VRWriteBytes( dvd_file_t *dvd_file,  __off64_t offset,
			      unsigned char *data, size_t bytes_count);
ssize_t RTDVDFileWriteBlocks( dvd_file_t *dvd_file, void *data, size_t block_size );
ssize_t VRWriteBlocks( dvd_file_t *dvd_file, void *data, size_t block_size );

dvd_file_t *RTDVDOpenNewFile( dvd_info_t *dvd, int titlenum, ENUM_DVD_FILETYPE domain );
void RTDVDOpenNewVOBFile( dvd_info_t *dvd, dvd_file_t **dvd_file, int titlenum, int part);

ssize_t RTDVDVOBWriteBlocks( dvd_file_t *dvd_file,
                       size_t block_count, unsigned char *data );
#if DVR_UDF_RW  
ssize_t VRWriteBlocks_2( dvd_file_t *dvd_file, void *data, size_t block_size );
int VRWriteBytes_2( dvd_file_t *dvd_file, __off64_t offset, unsigned char *data, size_t bytes_count);
#endif

int OpenNewDirectory(dvd_info_t *dvd, char *directory);
int DeleteVTSFile(dvd_info_t *dvd, int vtsn);
int LinkVOBFile(dvd_info_t *dvd);
int RTDVDFlushFile(dvd_file_t* dvd_file);
int AdjustVobFileHandle(dvd_file_t* dvd_file, unsigned int NWA);

#ifdef __cplusplus
};
#endif
#endif  //DVD_WRITE_H_INCLUDED
