package com.example.hdmirxdemo;

public interface DbListener {

    public abstract void onItemInserted(SchRecItem2 item);
    public abstract void onItemDeleted(SchRecItem2 item);
}
