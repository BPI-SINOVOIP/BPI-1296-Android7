package com.android.music.content;

public class MusicUrlPath {
    private int id;
    private String path;

    public MusicUrlPath(int id, String path) {
        super();
        this.id = id;
        this.path = path;
    }
    public int getId() {
        return id;
    }
    public void setId(int id) {
        this.id = id;
    }
    public String getPath() {
        return path;
    }
    public void setPath(String path) {
        this.path = path;
    }
    @Override
    public String toString() {
        return "MusicUrlPath id=" + id + ", path=" + path + "]";
    }
}