package com.realtek.Utils;

import jcifs.smb.SmbFile;

import java.io.File;

public class FileInfo {
	SmbFile mSmbFile;
	public int mFileType;
	File mFile;
	public String fileName;
	String time ="";
	public String path ="";
    long   Size = 0;
	public FileInfo(int mFileType, File mFile) {
		this.mFileType = mFileType;
		this.mFile = mFile;
	}
	public FileInfo(int fileType, SmbFile smbFile){
		this.mFileType = fileType;
		this.mSmbFile = smbFile;
	}
	public FileInfo(int mFileType, File mFile, String fileName) {
		this.mFileType = mFileType;
		this.mFile = mFile;
		this.fileName = fileName;
	}
	public FileInfo(int mFileType, File mFile, String fileName, String path) {
		this.mFileType = mFileType;
		this.mFile = mFile;
		this.fileName = fileName;
		this.path = path;
	}

	@Override
	public String toString() {
		return String.format("%s,%s,%s,%s,%s,%s", mFileType, mFile.getName(), fileName, time, path, Size);
	}

	public int getmFileType() {
		return mFileType;
	}
    public long getSize()
    {
        return Size;
    }
    public void setSize(long Size)
    {
        this.Size = Size;
    }
	public void setmFileType(int mFileType) {
		this.mFileType = mFileType;
	}
	public File getmFile() {
		return mFile;
	}
	public SmbFile getSmbFile(){return mSmbFile;}
	public void setmFile(File mFile) {
		this.mFile = mFile;
	}
	public String getFileName() {
		return fileName;
	}
	public void setFileName(String fileName) {
		this.fileName = fileName;
	}
	public String getTime() {
		return time;
	}
	public void setTime(String time) {
		this.time = time;
	}
}
