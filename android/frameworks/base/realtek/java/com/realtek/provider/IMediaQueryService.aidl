package com.realtek.provider;

interface IMediaQueryService {
	String  queryMusic(int nOffset, int nMax, int nTarget, int nId);
	String  queryAlbum(int nOffset, int nMax, int nTarget, int nId);
	byte[]  queryAlbumArt(int nId, int nWidth, int nHeight);
	String  queryArtist(int nOffset, int nMax);
	String  queryGenre(int nOffset, int nMax);
	
	String  queryPhoto(int nOffset, int nMax);
	String  queryVideo(int nOffset, int nMax);

	int     play(String url);

	int     playMusic(int nId);
	int     playPhoto(int nId);
	int     playVideo(int nId);

	String  getContentProperty(int contentType, int nId);
	
}
