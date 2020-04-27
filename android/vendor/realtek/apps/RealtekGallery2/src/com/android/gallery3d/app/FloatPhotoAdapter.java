/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.gallery3d.app;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.media.ThumbnailUtils;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;

import com.realtek.bitmapfun.util.ImageWorker;

public class FloatPhotoAdapter extends BaseAdapter
{
    private String[] photoList;
    private ImageWorker mImageWorker;
    private int mWidth;
    private int mHeight;

    public FloatPhotoAdapter(ImageWorker imageWorker, String[] photoList, int width, int height)
    {
        this.mImageWorker = imageWorker;
        this.photoList = photoList;
        this.mWidth = width;
        this.mHeight = height;
    }

    @Override
    public int getCount()
    {
        return photoList.length;
    }

    @Override
    public String getItem(int i)
    {
        return photoList[i];
    }

    public String[] getTotalItem()
    {
        return photoList;
    }

    @Override
    public long getItemId(int i)
    {
        return i;
    }

    @Override
    public final View getView(int i, View reusableView, ViewGroup viewGroup)
    {
        ImageView imageView = null;

        if(reusableView != null) {
            imageView = (ImageView) reusableView;
        }
        else {
            imageView = new ImageView(viewGroup.getContext());
        }
        imageView.setScaleType(ImageView.ScaleType.CENTER_INSIDE);
        imageView.setLayoutParams(new FloatPhotoListView.LayoutParams(mWidth, mHeight));

        if(mImageWorker!=null && photoList!=null && photoList.length>1 && i<photoList.length)
        {
            String fileName = photoList[i];
            if( fileName!=null && fileName.indexOf("file://")>=0 )
            {
                fileName = fileName.substring(fileName.indexOf("file://")+7);
            }
            mImageWorker.loadImage(fileName, imageView);
        }
        return imageView;
    }
}
