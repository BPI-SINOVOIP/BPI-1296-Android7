package com.mediacodec;

import java.util.ArrayList;
import java.util.Map;
import java.util.Scanner;
import java.io.IOException;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.File;
import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.ContextWrapper;
import android.content.SharedPreferences;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.webkit.MimeTypeMap;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.CheckBox;
import android.widget.LinearLayout;
import android.widget.Spinner;
import android.widget.Toast;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.BroadcastReceiver;
import android.os.UserHandle;

public class SmbUtils {
    private static final String TAG = "SmbUtils";//debug tag
    private static final boolean DEBUG = true;
    public static final String SMB_MOUNT_ACTION  = "android.intent.action.SMB_MOUNT";
    public static final String SMB_UMOUNT_ACTION = "android.intent.action.SMB_UMOUNT";
    public static final int SmbMountOkay = 933;
    public static final int SmbMountFailed = 934;
    public static final int SmbUmountOkay  = 935;
    public static final int SmbUmountFailed = 936;
    public static final String MOUNT_ROOT = "/tmp/ramfs/mnt";
    private static SmbUtils objSmbUtils = null;
    private Context context;
    private String Username;
    private String Server;
    private String Password;
    private String Share;
    private String Domain;
    private String key;
    private StringBuffer mountPath = new StringBuffer();
    private Spinner spinner = null;
    private ArrayList<String> userInfo = new ArrayList<String>();
    private ArrayAdapter<String> UserList = null;
    private ArrayList<String> filelist = new ArrayList<String>();	
    private SharedPreferences preferences;
    private SharedPreferences.Editor editor;
    private IntentFilter userFilter = new IntentFilter();
    
    private final BroadcastReceiver SmbStatusReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            Log.d(TAG, "Action = " + action);
            int code = intent.getIntExtra("code" , 0);
            if (code == SmbMountOkay) {
                Log.d(TAG,"Receive SmbMountSuccess");
                mountPath.replace(0, mountPath.length(), intent.getStringExtra("mountPath"));
            }
            else if (code == SmbMountFailed) {
                Log.d(TAG,"Receive SmbMountFailed");
                mountPath.replace(0, mountPath.length(),"");
                Toast.makeText(context, "Samba mount failed!", Toast.LENGTH_SHORT).show();
            }
            else if (code == SmbUmountOkay) {
                Log.d(TAG, "Receive SmbUmountOkay");
            }
            else if (code == SmbUmountFailed) {
                Log.d(TAG, "Receive SmbUmountFailed");
            }
        }
    };
    public SmbUtils(Context context) {
        this.context = context;
        preferences = new ContextWrapper(context).getSharedPreferences("smblogin", Context.MODE_PRIVATE);
        editor = preferences.edit();
        Username = Server = Password = Share = Domain = key = "";
        userFilter.addAction("android.rtksmb.resp_code");
        context.registerReceiver(SmbStatusReceiver, userFilter);
    }
/*
    @SuppressLint("InflateParams")
    public SmbUtils(Context context, StringBuffer mountPath) {
        //initial something
        this.context = context;
        this.mountPath = mountPath;
        preferences = new ContextWrapper(context).getSharedPreferences("smblogin", Context.MODE_PRIVATE);
        editor = preferences.edit();
        Username = Server = Password = Share = Domain = key = "";
    
        userFilter.addAction("android.rtksmb.resp_code");
            context.registerReceiver(SmbStatusReceiver, userFilter);	
    }
    
    public static SmbUtils getInstance(Context context, StringBuffer mountPath) {
        if(objSmbUtils == null) {
            objSmbUtils = new SmbUtils(context, mountPath);
        }
        objSmbUtils.context = context;
        objSmbUtils.mountPath = mountPath;
        return objSmbUtils;
    }
*/
    public static SmbUtils getInstance(Context context) {
        if(objSmbUtils == null) {
            objSmbUtils = new SmbUtils(context);
        }
        objSmbUtils.context = context;
        return objSmbUtils;
    }
    public static void deleteInstance() {
        Log.d(TAG, "DelInstance");
        if (objSmbUtils != null) {
            //objSmbUtils.context.unregisterReceiver(objSmbUtils.SmbStatusReceiver);
            if (objSmbUtils.CheckSmbMount(null)) {
                Intent intent = new Intent(SMB_UMOUNT_ACTION)
                                .putExtra(Intent.EXTRA_USER_HANDLE, UserHandle.USER_SYSTEM)
                                .putExtra("smb_mount_cmd", true);
                objSmbUtils.context.sendBroadcast(intent);
            }
        }
        objSmbUtils = null;
        //System.gc();
    }
    @SuppressLint("InflateParams")
    public void createSmbLoginDialog() {
        final LinearLayout SmbLoginLayout = (LinearLayout)LayoutInflater.from(context).inflate(R.layout.smb_service, null);
        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setTitle("Samba");
        builder.setIcon(R.drawable.ic_launcher);
        builder.setView(SmbLoginLayout);
        builder.create();
        
        Button addButton = (Button)SmbLoginLayout.findViewById(R.id.bt_add);
        Button editButton = (Button)SmbLoginLayout.findViewById(R.id.bt_edit);
        Button deleteButton = (Button)SmbLoginLayout.findViewById(R.id.bt_delete);
        Button loginButton = (Button)SmbLoginLayout.findViewById(R.id.bt_login);
        spinner = (Spinner)SmbLoginLayout.findViewById(R.id.spinner);
        
        UserList = new ArrayAdapter<String>(context, android.R.layout.simple_list_item_1,userInfo);
        spinner.setAdapter(UserList);
        getAllUserInfo();
        
        final AlertDialog dialog = builder.show();
        addButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                //Toast.makeText(context, "add", Toast.LENGTH_SHORT).show();
                Username = Server = Password = Share = Domain = key = "";
                createSmbUserInfoDialog(dialog);
            }
            
        });
        editButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                //Toast.makeText(context, "edit", Toast.LENGTH_SHORT).show();
                if (spinner.getSelectedItem() != null) {
                    key = spinner.getSelectedItem().toString();
                    getUserInfoFromKey(key);
                    createSmbUserInfoDialog(dialog);
                } else {
                    Toast.makeText(context, "No item to edit...", Toast.LENGTH_SHORT).show();
                }
            }	
        });
        deleteButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                //Toast.makeText(context, "delete", Toast.LENGTH_SHORT).show();
                if (spinner.getSelectedItem() != null) {
                    key = spinner.getSelectedItem().toString();
                    editor.remove(key);
                    editor.commit();
                    getAllUserInfo();
                } else {
                    Toast.makeText(context, "No item to delete...", Toast.LENGTH_SHORT).show();
                }
            }	
        });
        loginButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                if(spinner.getSelectedItem() == null) {
                    Toast.makeText(context, "Please type login information!", Toast.LENGTH_SHORT).show();
                    return;
                }
                key = spinner.getSelectedItem().toString();
                getUserInfoFromKey(key);
                Domain = Domain.equals("null")?null:Domain;
                String mntPath = MOUNT_ROOT+"/"+Domain+"_"+Server+"_"+Username;
                CreateMountPath(mntPath);
                String smbPath = "//"+Server+"/"+Share;
                if(!CheckSmbMount(mntPath)) {
                    Log.d(TAG, "//"+Server+"/"+Share);
                    Intent intent = new Intent(SMB_MOUNT_ACTION)
                                            .putExtra(Intent.EXTRA_USER_HANDLE, UserHandle.USER_SYSTEM)
                                            .putExtra("smb_mount_cmd", true)
                                            .putExtra("smbPath", smbPath)
                                            .putExtra("mountPath", mntPath)
                                            .putExtra("domain", Domain)
                                            .putExtra("user", Username)
                                            .putExtra("passwd", Password);
                    context.sendBroadcast(intent);
                } else {
                    mountPath.replace(0, mountPath.length(), mntPath);
                }
                dialog.dismiss();
            }
        });
    }
    
    private Dialog createSmbUserInfoDialog(final Dialog mDialog) {
        
        final LinearLayout SmbUserInfoLayout = (LinearLayout)LayoutInflater.from(context).inflate(R.layout.smbinfo, null);
        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setTitle("User Information");
        builder.setIcon(R.drawable.ic_launcher);
        builder.setView(SmbUserInfoLayout);
        builder.create();
                
        Button okButton = (Button)SmbUserInfoLayout.findViewById(R.id.bt_ok);
        Button cancelButton = (Button)SmbUserInfoLayout.findViewById(R.id.bt_cancel);
        final CheckBox chbnodomain = (CheckBox)SmbUserInfoLayout.findViewById(R.id.chb_nodomain);		
        final CheckBox chbrememberme = (CheckBox)SmbUserInfoLayout.findViewById(R.id.chb_rememberme);		
    
        ((EditText)SmbUserInfoLayout.findViewById(R.id.server)).setText(Server);
        ((EditText)SmbUserInfoLayout.findViewById(R.id.username)).setText(Username);
        ((EditText)SmbUserInfoLayout.findViewById(R.id.password)).setText(Password);
        ((EditText)SmbUserInfoLayout.findViewById(R.id.share)).setText(Share);
        if(Domain.equals("null"))
            chbnodomain.setChecked(true);
        else
            ((EditText)SmbUserInfoLayout.findViewById(R.id.domain)).setText(Domain);
    
    
        final AlertDialog dialog = builder.show();
        
        okButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                //Toast.makeText(context, "OK", Toast.LENGTH_SHORT).show();
    
                Server = ((EditText)SmbUserInfoLayout.findViewById(R.id.server)).getText().toString();
                Username = ((EditText)SmbUserInfoLayout.findViewById(R.id.username)).getText().toString();
                Password = ((EditText)SmbUserInfoLayout.findViewById(R.id.password)).getText().toString();
                Share = ((EditText)SmbUserInfoLayout.findViewById(R.id.share)).getText().toString();
                if (chbnodomain.isChecked())
                    Domain = "null";
                else
                    Domain = ((EditText)SmbUserInfoLayout.findViewById(R.id.domain)).getText().toString();
    
                if(Server.equals("") || Username.equals("") || Password.equals("") || Share.equals("") || Domain.equals("")) {
                    Toast.makeText(context, "Items can't be blank...", Toast.LENGTH_SHORT).show();
                } else if(chbrememberme.isChecked()){
                    createSmbNameDialog();
                    dialog.dismiss();
                } else if(!chbrememberme.isChecked()) {
                    String mntPath = MOUNT_ROOT+"/"+Domain+"_"+Server+"_"+Username;
                    CreateMountPath(mntPath);
                    Domain = Domain.equals("null")?null:Domain;
                    String smbPath = "//"+Server+"/"+Share;
                    if(!CheckSmbMount(mntPath)) {
                        Log.d(TAG, "//"+Server+"/"+Share);
                        Intent intent = new Intent(SMB_MOUNT_ACTION)
                                                .putExtra(Intent.EXTRA_USER_HANDLE, UserHandle.USER_SYSTEM)
                                                .putExtra("smb_mount_cmd", true)
                                                .putExtra("smbPath", smbPath)
                                                .putExtra("mountPath", mntPath)
                                                .putExtra("domain", Domain)
                                                .putExtra("user", Username)
                                                .putExtra("passwd", Password);
                        context.sendBroadcast(intent);
                    } else {
                        mountPath.replace(0, mountPath.length(), mntPath);
                    }
                    mDialog.dismiss();
                    dialog.dismiss();
                }
            }
        });
        cancelButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                dialog.dismiss();
            }
        });
        return dialog;
    }
    
    private Dialog createSmbNameDialog() {
        final LinearLayout SmbNameLayout = (LinearLayout)LayoutInflater.from(context).inflate(R.layout.smbname, null);
        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setTitle("Samba name");
        builder.setIcon(R.drawable.ic_launcher);
        builder.setView(SmbNameLayout);
        builder.create();
        
        ((EditText)SmbNameLayout.findViewById(R.id.smbname)).setText(key);
        final AlertDialog dialog = builder.show();
        
        Button saveButton = (Button)SmbNameLayout.findViewById(R.id.save);
        Button cancelButton = (Button)SmbNameLayout.findViewById(R.id.cancel);
        saveButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                String smbname = ((EditText)SmbNameLayout.findViewById(R.id.smbname)).getText().toString();
                if(!smbname.equals("")) {
                    if(preferences.contains(smbname)) {
                        if(key.equals("")) {
                            Toast.makeText(context, smbname+" already exits...", Toast.LENGTH_SHORT).show();
                        } else {
                            editor.putString(smbname, Server+"^"+Username+"^"+Password+"^"+Share+"^"+Domain);
                            editor.commit();
                            getAllUserInfo();
                            dialog.dismiss();
                        }
                    } else {
                        if(key=="") {
                            editor.putString(smbname, Server+"^"+Username+"^"+Password+"^"+Share+"^"+Domain);
                            editor.commit();
                            getAllUserInfo();
                            dialog.dismiss();
                        } else {
                            editor.remove(key);
                            editor.putString(smbname, Server+"^"+Username+"^"+Password+"^"+Share+"^"+Domain);
                            editor.commit();
                            getAllUserInfo();
                            dialog.dismiss();
                        }
                    }
                    
                } else {
                    Toast.makeText(context, "please type a name...", Toast.LENGTH_SHORT).show();				
                }			
            }
        });
        cancelButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                dialog.dismiss();
            }
        });
        return dialog;
    }
    //Get all samba name(alias) information of users
    private void getAllUserInfo() {
        UserList.clear();
        Map<String, ?> allContent = preferences.getAll();
        for(Map.Entry<String, ?>  entry : allContent.entrySet()) {  
            UserList.add(entry.getKey());
        }
    }
    //Get information of user according to key.
    private void getUserInfoFromKey(String key) {
        String value = preferences.getString(key, null);
        if(value != null && !value.isEmpty()) {
            log(key+"--"+value);
            String[] segs = value.split("\\^");
            Server = segs[0];
            Username = segs[1];
            Password = segs[2];
            Share = segs[3];
            Domain = segs[4];
        }	
    }
    //Create new directory used to mount samba server
    private boolean CreateMountPath(String mountPath) {
        File file = new File(mountPath);
        if (!file.exists()) {
        log("mkdir success");
            return file.mkdirs();
        }
        log("mkdir failed");
        return true;
    }
    
    //Check to see if the Samba server has mounted 
    public static boolean CheckSmbMount(String mountPath) {
        String CheckPath = mountPath!=null? mountPath:(MOUNT_ROOT+"/");
        File file = new File("/proc/mounts");
        if (file.canRead()) {   
            BufferedReader reader = null;
            try {
                reader = new BufferedReader(new FileReader(file));
                String text = null;
                while ((text = reader.readLine()) != null) { 
                    String[] segs = text.split(" ");
                    if (segs.length > 1) {
                        if (segs[1].indexOf(CheckPath) == 0) {
                            reader.close();
                            log("the Samba server has mounted");
                            return true;
                        }
                    }
                }
            } catch (IOException e) {
                Log.d(TAG, "IOException", e);
            } finally {
                try {
                    if (reader != null) {
                        reader.close();
                    }
                } catch (IOException e) {
                    Log.d(TAG, "IOException", e);
                }
            }
        }
        log("the Samba server has not mounted");
        return false;
    }
    //Unmount samba server
    public void UnmountSmb() {
        context.unregisterReceiver(SmbStatusReceiver);
        Log.d(TAG, "UnmountSmb");
        if (CheckSmbMount(null)) {
            Intent intent = new Intent(SMB_UMOUNT_ACTION)
                                .putExtra(Intent.EXTRA_USER_HANDLE, UserHandle.USER_SYSTEM)
                                .putExtra("smb_mount_cmd", true);
            context.sendBroadcast(intent);
        }		
    }
    //Debug method
    private static void log(String s) {
        if(DEBUG)
            Log.d(TAG, s);
    }
    
}
