package com.realtek.rtksmb;

/**
 * Created by arpitkh996 on 16-01-2016.
 */

import android.content.Context;
import android.util.Log;
import com.rtk.mediabrowser.Util;
import jcifs.Address;
import jcifs.CIFSContext;
import jcifs.NameServiceClient;
import jcifs.NetbiosAddress;
import jcifs.smb.SmbFile;

import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;

public class SubnetScanner extends Thread {
    private static final String TAG = "SubnetScanner";
    private static final boolean DEBUG = false;
    public static final int RETRY_COUNT = 1;
    private Thread bdThread;
    private final Object mLock;
    private List<Computer> mResults;
    private ScanObserver observer;
    ExecutorService pool;
    private List<Future<Computer>> tasks;
    private NameServiceClient nscl;

    public interface ScanObserver {
        void computerFound(Computer computer);

        void searchFinished();
    }



    class Task implements Callable<Computer> {
        String addr;

        public Task(String str) {
            this.addr = str;
        }

        public Computer call() throws Exception {
//            log("call "+this.addr);
            if(nscl==null) return null;
            try {
                NetbiosAddress[] allByAddress = nscl.getNbtAllByAddress(this.addr);
                if (allByAddress == null || allByAddress.length <= 0) {
                    return new Computer(null, this.addr);
                }
                return new Computer(allByAddress[0].getHostName(), this.addr);
            } catch (UnknownHostException e) {
                return new Computer(null, this.addr);
            }
        }
    }

    static {
        configure();
    }

    public static void configure() {
//        Config.setProperty("jcifs.resolveOrder", "BCAST");
//        Config.setProperty("jcifs.smb.client.responseTimeout", "2500");
//        Config.setProperty("jcifs.netbios.soTimeout", "2500");
    }
    Context context;
    public SubnetScanner(Context context) {
        this.context=context;
        this.mLock = new Object();
        this.tasks = new ArrayList(260);
        this.pool = Executors.newFixedThreadPool(40);
//        this.pool = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors()+2);
        this.mResults = new ArrayList();
    }


    public void run() {
        CIFSContext cifsContext = SmbUtils.getContext().withDefaultCredentials();
        nscl = cifsContext.getNameServiceClient();
        String ipStr = Util.getIPAddress(true);
        log("ipStr="+ipStr);
        if (ipStr.length()>0) {
            tryWithBroadcast();
            String formatIpAddress = ipStr;
            String substring = formatIpAddress.substring(0, formatIpAddress.lastIndexOf('.') + 1);
            if (!isInterrupted()) {
                int ipAddress = 0;
                for (ipAddress = 0; ipAddress < 100; ipAddress++) {
                    if(isInterrupted() || this.pool.isShutdown()) break;
                    if(ipAddress>0)
                        this.tasks.add(this.pool.submit(new Task(substring + ipAddress)));
                    if(isInterrupted() || this.pool.isShutdown()) break;
                    this.tasks.add(this.pool.submit(new Task(substring + (ipAddress + 100))));
                    if(isInterrupted() || this.pool.isShutdown()) break;
                    if (ipAddress < 55) {
                        this.tasks.add(this.pool.submit(new Task(substring + (ipAddress + 200))));
                    }
                }
                while (!this.tasks.isEmpty()) {
                    int size = this.tasks.size();
                    int i = 0;
                    while (i < size) {
                        if (!isInterrupted()) {
                            try {
                                Computer computer = (Computer) ((Future) this.tasks.get(i)).get(1, TimeUnit.MILLISECONDS);
                                this.tasks.remove(i);
                                size--;
                                if (computer.name != null) {
                                    //SELog.d("SMB host found at ", computer.addr);
                                    onFound(computer);
                                } else {
                                    //SELog.d("No SMB host found at ", computer.addr);
                                }
                                ipAddress = size;
                            } catch (InterruptedException e) {
                                return;
                            } catch (ExecutionException e2) {
                                Throwable th = e2;
                                ipAddress = size;
                              //  SELog.w(th);
                            } catch (TimeoutException e3) {
                                ipAddress = size;
                            }
                            i++;
                            size = ipAddress;
                        } else {
                            log("task interrupt");
                            return;
                        }
                    }
                }
                if(this.bdThread!=null) {
                    try {
                        this.bdThread.join();
                    } catch (InterruptedException e4) {
                    }
                }
            } else {
                return;
            }
        }
        synchronized (this.mLock) {
            if (this.observer != null) {
                this.observer.searchFinished();
            }
        }
        this.pool.shutdown();
    }

    private void log(String s) {
        if(DEBUG)
            Log.d(TAG, s);
    }

    private void tryWithBroadcast() {
        this.bdThread = new Thread() {
            public void run() {
                for (int i = 0; i < SubnetScanner.RETRY_COUNT; i++) {
                    try {
                        CIFSContext cifsContext = SmbUtils.getContext().withDefaultCredentials();
                        SmbFile smbFile = new SmbFile("smb://", cifsContext);
                        smbFile.setConnectTimeout(5000);
                        SmbFile[] domains = smbFile.listFiles();
                        for (SmbFile d : domains) {
                            log("listDomains d="+d.getName());
                            SmbFile[] machines = d.listFiles();
                            for (SmbFile m : machines) {
                                log("listMachines m="+m.getName());
                                try {
                                    String substring = m.getName().substring(0, m.getName().length()-1);
                                    Address addr = nscl.getByName(substring);
//                                    UniAddress byName = UniAddress.getByName(substring);
                                    if (addr != null) {
                                        SubnetScanner.this.onFound(new Computer(substring, addr.getHostAddress()));
                                    }
                                } catch (Throwable e) {
//                                    e.printStackTrace();
                                }
                            }
                        }
                    } catch (Throwable e2) {
//                        e2.printStackTrace();
                    }
                }
            }
        };
        this.bdThread.start();
    }

    void onFound(Computer computer) {
        log("onFound "+computer);
        this.mResults.add(computer);
        synchronized (this.mLock) {
            if (this.observer != null) {
                this.observer.computerFound(computer);
            }
        }
    }

    public void setObserver(ScanObserver scanObserver) {
        synchronized (this.mLock) {
            this.observer = scanObserver;
        }
    }

    public void interrupt() {
        log("interrupt");
        super.interrupt();
        try {
            this.pool.shutdownNow();
        } catch (Throwable th) {

        }
    }

    public List<Computer> getResults() {
        return new ArrayList(this.mResults);
    }

}

