/* Copyright (C) 2014 Realtek Semiconductor Corp. */

package android.hardware.usb;

import android.app.PendingIntent;
import android.content.Context;
import android.os.Bundle;
import android.os.ParcelFileDescriptor;
import android.os.RemoteException;
import android.os.SystemProperties;
import android.util.Log;

import android.hardware.usb.UsbManager;

/**
 * This class allows you to access the state of USB Camera.
 */
public class UsbCamera {
    private static final String TAG = "UsbCamera";

    /**
     * Broadcast Action:  A broadcast for USB camera connected event.
     *
     * This intent is sent when a USB camera is connected.
     * <ul>
     * <li> {@link #USB_CAMERA_NAME} containing the usb camera device name
     * </ul>
     */
    public static final String ACTION_USB_CAMERA_CONNECTED =
            "android.hardware.usb.action.USB_CAMERA_CONNECTED";

    /**
     * Broadcast Action:  A broadcast for USB camera disconnected event.
     *
     * This intent is sent when a USB camera is disconnected
     * <ul>
     * <li> {@link #USB_CAMERA_NAME} containing the usb camera device name
     * </ul>
     */
    public static final String ACTION_USB_CAMERA_DISCONNECTED =
            "android.hardware.usb.action.USB_CAMERA_DISCONNECTED";

    /**
     * Name of extra for {@link #ACTION_USB_CAMERA_CONNECTED} and {@link #ACTION_USB_CAMERA_DISCONNECTED} broadcasts
     * containing the name of the camera device.
     */

    public static final String USB_CAMERA_NAME = "name";

    public UsbCamera() {
    }
}

