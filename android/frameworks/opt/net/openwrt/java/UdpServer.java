package com.rtk.net.openwrt;

import android.util.Log;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import android.os.SystemProperties;

/**
 * Created by archerho on 2017/9/27.
 */

public class UdpServer {
    private static final String TAG = UdpServer.class.getSimpleName();
    private static final int UDP_SERVER_PORT = 59955;
    private static final int MAX_UDP_DATAGRAM_LEN = 1500;
    private MyDatagramReceiver myDatagramReceiver;
    private DatagramSocket mSocket;
    private static UdpServer sInstance;

    public static UdpServer getInstance(){
        if(sInstance == null)
            sInstance = new UdpServer();
        return sInstance;
    }

    private class MyDatagramReceiver extends Thread {
        private boolean bKeepRunning = true;
        private String lastMessage = "";

        public void run() {
            String message;
            byte[] lmessage = new byte[MAX_UDP_DATAGRAM_LEN];
            DatagramPacket packet = new DatagramPacket(lmessage, lmessage.length);

            try {
                mSocket = new DatagramSocket(UDP_SERVER_PORT);
                log("listen port " + mSocket.getLocalPort());
            } catch (SocketException e) {
                e.printStackTrace();
            }
            if(mSocket==null) return;
            while (bKeepRunning) {
                try {
                    mSocket.receive(packet);
                    message = trimZeros(new String(lmessage, 0, packet.getLength()));
                    lastMessage = message;
                    log("lastMsg = " + message);
                    sendEvent(message);
                } catch (SocketTimeoutException e) {
                    log("SocketTimeoutException");
                } catch (SocketException e) {
                    log(e.toString());
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

        String trimZeros(String str) {
            int pos = str.indexOf(0);
            return pos == -1 ? str : str.substring(0, pos);
        }

        boolean isEthDown = false;
        final String IFACE_DOWN_UP_SERVICE = "eth0_down_up";

        private void sendEvent(String message) {
            log("get msg: "+message);
            if("eth_link_down".equals(message)){
                isEthDown = true;
            } else if("eth_link_up".equals(message)){
                if(isEthDown){
                    log("renew eth0");
                    SystemProperties.set("ctl.start", IFACE_DOWN_UP_SERVICE);
                    isEthDown = false;
                }
            } else if("lan_changed".equals(message)){
                log("renew eth0");
                SystemProperties.set("ctl.start", IFACE_DOWN_UP_SERVICE);
            } else if("link_change".equals(message)){
                
            } else {
                log("Unknown message: "+message);
            }
        }

        void kill() {
            log("kill socket");
            bKeepRunning = false;
            if (mSocket != null && !mSocket.isClosed()) {
                mSocket.close();
                mSocket = null;
            }
            try {
                this.join(2000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        public String getLastMessage() {
            return lastMessage;
        }
    }

    private void log(String s) {
        Log.d(TAG, s);
    }

    public void start(){
        if(myDatagramReceiver==null) {
            myDatagramReceiver = new MyDatagramReceiver();
            myDatagramReceiver.start();
        }
    }

    public void stop(){
        if(myDatagramReceiver!=null) {
            myDatagramReceiver.kill();
        }
    }
}
